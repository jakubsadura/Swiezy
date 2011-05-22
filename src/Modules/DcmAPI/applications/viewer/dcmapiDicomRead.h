/*
* Copyright (c) 2010,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
/**
* @file dcmapiDicomRead.h
* @brief DICOM reading methods using the dcmapi.
*/
#ifndef DCMAPIDICOMREAD_H
#define DCMAPIDICOMREAD_H

// boost
#include <boost/filesystem.hpp>

// VTK
#include <vtkSmartPointer.h>
#include <vtkImageData.h>

// dcmapi
#include <dcmImageUtilities.h>
#include <dcmDataSetReader.h>
#include <dcmTypes.h>


/**
* \defgroup dcmapiread DCMAPI Readers
* \ingroup viewer
*/

//@{ 
namespace dcmapi
{

/**
* Option for the dcmapi reader (mostly changes of the time separator tag).
*/
enum ReadOption
{
    DEFAULT,
    QUIRON_CT,
    QUIRON_MR,
    QUIRON_MR2
};


/**
* Get the list of files to read from a DICOM folder. Simple: reads everything.
* @param path The path to the DICOM folder.
* @return The file list.
*/
std::vector<std::string> GetDicomFolderFileListSimple(const std::string& path)
{
    // create list of files
    std::vector< std::string > dcmFileNames;
    boost::filesystem::directory_iterator end_itr; // default construction yields past-the-end
    for( boost::filesystem::directory_iterator itr( path );
        itr != end_itr; ++itr )
    {
        if( is_regular_file(itr->status()) && itr->leaf() != "DICOMDIR" && itr->leaf()[0] != '.')
        {
            dcmFileNames.push_back( path + "/" + itr->leaf() );
        }
    }
    // sort the files alphabetically
    std::sort(dcmFileNames.begin(), dcmFileNames.end());
    
    // output
    std::cout << "[dcmapi] " << dcmFileNames.size() << " file(s)." << std::endl;

    // return
    return dcmFileNames;
}

/**
* Get the list of files to read from a series.
* @param series The series representing the structure.
* @return The file list.
*/
std::vector< std::vector<std::string> > GetSeriesFileList(const dcmAPI::Series::Pointer& series)
{
    // file name list
    std::vector< std::vector< std::string > > dcmFileNames;
    
	// vars
	std::string timePointId;
    dcmAPI::TimePoint::Pointer timePoint;
    dcmAPI::SliceIdVectorPtr sliceIdVector;
	std::string sliceId;
    dcmAPI::Slice::Pointer slice;
    std::string filePath;

    // TimePoints
    const dcmAPI::TimePointIdVectorPtr timePointIdVector = series->TimePointIds();
    for( unsigned int i=0; i < timePointIdVector->size(); ++i)
    {
	    timePointId = timePointIdVector->at(i);
	    timePoint = series->TimePoint(timePointId);
	    // sub level list
	    std::vector< std::string > list;
        // Slice
        sliceIdVector = timePoint->SliceIds();
        for( unsigned int j=0; j < sliceIdVector->size(); ++j)
        {
            sliceId = sliceIdVector->at(j);
            slice = timePoint->Slice(sliceId);
            filePath = slice->GetTagAsText(dcmAPI::tags::SliceFilePath);
            // only add if not allready present (could be multislice single file)
            if( list.size() == 0 || find( list.begin(), list.end(), filePath ) == list.end() )
            {
                list.push_back( filePath );
            }
        }
	    // add a level to the file list
	    dcmFileNames.push_back(list);
    }
    
    // return
    return dcmFileNames;
}

/**
* Utility method to print a std::map.
* @input map The map from which to print the content.
*/
void printMap( const std::map< dcmAPI::TagId, std::string >& map )
{
    std::map< dcmAPI::TagId, std::string >::const_iterator it;
    for( it=map.begin(); it != map.end(); it++ )
    {
        std::cout << (*it).first << " => " << (*it).second << std::endl;
    }
}

/**
* Get the list of files to read from a data set.
* @param dataSet The data set representing the structure.
* @return The file list.
*/
std::vector< std::vector<std::string> > GetDataSetFileList(const dcmAPI::DataSet::Pointer& dataSet)
{
    // file name list
    std::vector< std::vector< std::string > > dcmFileNames;
    
    // Patient: take the first one
    if( dataSet->GetPatientIds()->size() > 1 )
    {
        std::cerr << "[dcmapi][WARNING] More than one patient, loading the first one." << std::endl;
    }
    const dcmAPI::Patient::Pointer patient = dataSet->GetPatient( dataSet->GetPatientIds()->at(0) );
    // Study: take the first one
    if( patient->StudyIds()->size() > 1 )
    {
        std::cerr << "[dcmapi][WARNING] More than one study, loading the first one." << std::endl;
    }
    const dcmAPI::Study::Pointer study = patient->Study( patient->StudyIds()->at(0) );
    
    // vars
	std::string seriesId;
    dcmAPI::Series::Pointer series;
    std::string tmp;
    dcmAPI::TimePointIdVectorPtr timePointIdVector;
    unsigned int timePointSize;
    
	// Series of the study
    const dcmAPI::SeriesIdVectorPtr seriesIdVector = study->SeriesIds();
	const unsigned int seriesSize = seriesIdVector->size();
    // seriesNumber starts at 1
    unsigned int seriesNumber = 1;
    bool modeAll = false;
	if( seriesSize > 1 )
	{
	    std::cout << "[dcmapi] Found multiple series:" << std::endl;
	    for(unsigned int k=0; k < seriesSize; ++k)
	    {
	        seriesId = seriesIdVector->at(k);
	        series = study->Series(seriesId);
	        
	        // tag info
	        std::cout << "[dcmapi] [" << seriesId << "]";
	        tmp = series->GetTagAsText(dcmAPI::tags::SeriesNumber);
	        if( tmp.size() > 0 ) std::cout << " SeriesNumber: " << tmp;
	        tmp = series->GetTagAsText(dcmAPI::tags::SeriesInstanceUID);
	        if( tmp.size() > 0 ) std::cout << " SeriesInstanceUID: " << tmp;
	        tmp = series->GetTagAsText(dcmAPI::tags::SeriesDescription);
	        if( tmp.size() > 0 ) std::cout << " SeriesDescription: " << tmp;
	        
            // size info
            timePointIdVector = series->TimePointIds();
	        timePointSize = timePointIdVector->size();
	        
	        std::cout << " (";
	        std::cout << timePointSize << " timepoint";
            if( timePointSize > 1 ) std::cout << "s";
            
            int min = 1e6;
            int max = 0;
            int sliceSize = 0;
	        std::string timePointId;
            dcmAPI::TimePoint::Pointer timePoint;
            for( unsigned int i=0; i < timePointSize; ++i)
            {
	            timePointId = timePointIdVector->at(i);
	            timePoint = series->TimePoint(timePointId);
                sliceSize = timePoint->SliceIds()->size();
                if( i == 0 ) 
                {
                    max = sliceSize; 
                    min = sliceSize;
                }
                else if( sliceSize > max ) max = sliceSize;
                else if( sliceSize < min ) min = sliceSize;
            }
            if( timePointSize > 1 ) std::cout << ", each";
	        if( min == max )
	        {
	            std::cout << " with " << min << " slice(s)";
            }
	        else
	        {   
	            std::cout << " with " << min << " to " << max << " slice(s)";
            }

	        std::cout << ")";
	        std::cout << std::endl;
	    }
	
	    std::cout << "[dcmapi] Enter the series number to load (0 for all): ";
	    std::cin >> seriesNumber;
	
	    if( seriesNumber > seriesSize )
	    {
	        throw std::runtime_error("[dcmapi][ERROR] Series number is not in allowed range.");
	    }
	    else if( seriesNumber == 0 )
	    {
	        modeAll = true;
	    }
    }
    
    if( modeAll )
    {
		// Series of the study
		std::vector< std::vector<std::string> > subVector;
		for(unsigned int k=0; k < seriesIdVector->size(); ++k)
		{
            seriesId = seriesIdVector->at(k);
            series = study->Series(seriesId);
            subVector = GetSeriesFileList( series );
            for( unsigned int l=0; l < subVector.size(); ++l)
            {
                dcmFileNames.push_back( subVector[l] );
            }
		}
    }
    else
    {
        // seriesNumber starts at 1
        seriesId = seriesIdVector->at(seriesNumber-1);
        series = study->Series(seriesId);
        dcmFileNames = GetSeriesFileList( series );
    }
    
    // return
    return dcmFileNames;
}

/**
* Print the data set structure on screen.
* @param dataSet The data set representing the structure.
* @return True if all went well.
*/
bool ShowDataSetStructure(const dcmAPI::DataSet::Pointer& dataSet)
{
	std::string patientId;
	std::string studyId;
	std::string seriesId;
	std::string timePointId;
	std::string sliceId;
	std::string text; 
	
    std::cerr << "[dcmapi] structure:" << std::endl;

	// Get the vector of patients Ids
	dcmAPI::PatientIdVectorPtr patientIdVector = dataSet->GetPatientIds();
	// Patients in the Dataset
	for(unsigned int i=0; i < patientIdVector->size(); ++i)
	{
		patientId = patientIdVector->at(i);
		dcmAPI::Patient::Pointer patient = dataSet->GetPatient(patientId);
		text = patient->GetTagAsText(dcmAPI::tags::PatientName);
		std::cout << "|_ patient name: " << text << std::endl;
		
		// Studies of the patient
		dcmAPI::StudyIdVectorPtr studiesIdVector = patient->StudyIds();
		for(unsigned int j=0; j < studiesIdVector->size(); ++j)
		{
			studyId = studiesIdVector->at(j);
			dcmAPI::Study::Pointer study = patient->Study(studyId);
			text = study->GetTagAsText(dcmAPI::tags::StudyInstanceUID);
		    std::cout << "  |_ study UID: " << studyId << std::endl;
			
			// Series of the study
			dcmAPI::SeriesIdVectorPtr seriesIdVector = study->SeriesIds();
			for(unsigned int k=0; k < seriesIdVector->size(); ++k)
			{
				seriesId = seriesIdVector->at(k);
				dcmAPI::Series::Pointer series = study->Series(seriesId);
			    text = series->GetTagAsText(dcmAPI::tags::SeriesInstanceUID);
		        std::cout << "    |_  series UID: " << seriesId << std::endl;
				
				// TimePoints in the series
				dcmAPI::TimePointIdVectorPtr timePointIdVector = series->TimePointIds();
				for(unsigned int p=0; p < timePointIdVector->size(); ++p)
				{
					timePointId = timePointIdVector->at(p);
					dcmAPI::TimePoint::Pointer timePoint = series->TimePoint(timePointId);
		            std::cout << "      |_ time point: " << timePointId << std::endl;
					
					// Slices in the timepoint
					dcmAPI::SliceIdVectorPtr sliceIdVector = timePoint->SliceIds();
					for(unsigned int q=0; q < sliceIdVector->size(); ++q)
					{
						sliceId = sliceIdVector->at(q);
						dcmAPI::Slice::Pointer slice = timePoint->Slice(sliceId);
						text = slice->GetTagAsText(dcmAPI::tags::SliceFileName);
		                std::cout << "        |_ slice: " << text << std::endl;
					}
				}
			}
		}
	}
	return true;
}

/**
* Get the list of files to read from an input path.
* @param path The path to input path.
* @param option Option for specific data reading.
*   [0] Default.
*   [1] Use a truncate of the SOPInstanceUID as time sorter.
* @return The file list.
*/
std::vector< std::vector<std::string> > GetDicomFilesList(const boost::filesystem::path& inputPath, const ReadOption& option)
{
	// DICOM data: representation of the data structure
	dcmAPI::DataSet::Pointer dcmData = dcmAPI::DataSet::New();

	// scan the current working folder for dicom files
	dcmAPI::DataSetReader::Pointer reader = dcmAPI::DataSetReader::New( );
	reader->SetDataSet( dcmData );
    
	// Special time tag
	
	// Use a truncate of the SOPInstanceUID
	if( option == QUIRON_CT )
	{
	    std::cout << "[dcmapi] Option: QUIRON_CT." << std::endl;
	    reader->SetTimeTag(dcmAPI::tags::SOPInstanceUID);
	    reader->SetTimeTagTruncate('.', 1);
    }
    // Use the phase number from the dicom file
    else if( option == QUIRON_MR )
    {
	    std::cout << "[dcmapi] Option: QUIRON_MR." << std::endl;
	    reader->SetTimeTag(dcmAPI::tags::PhaseNumberMR);
	    reader->SetTimeTagLocation(true);
    }
    // Use the acquisition time from the dicom file
    else if( option == QUIRON_MR2 )
    {
	    std::cout << "[dcmapi] Option: QUIRON_MR2." << std::endl;
	    reader->SetTimeTag(dcmAPI::tags::AcquisitionTime);
	    reader->SetTimeTagLocation(true);
    }
	
    // file list to load
    std::vector< std::vector<std::string> > fileList;
    // single file case
    if( boost::filesystem::is_regular_file(inputPath) )
    {
        if( inputPath.file_string() == "DICOMDIR" )
        {
            std::cout << "[dcmapi] Reading DICOMDIR structure." << std::endl;
            // set the data set from the DICOMDIR
            reader->ReadDicomDir( inputPath.file_string() ); 
            // get the file list
            fileList = dcmapi::GetDataSetFileList(dcmData);
            // output
            unsigned int nFiles = 0;
            for( unsigned int i = 0; i < fileList.size(); ++i )
            {
                nFiles += fileList[i].size();
            }
            std::cout << "[dcmapi] " << nFiles << " file(s)." << std::endl;
        }
        else
        {
            std::cout << "[dcmapi] Reading DICOM file structure." << std::endl;
            // set the data set from the file
            reader->ReadDcmFile( inputPath.file_string() ); 
            // get the file list
            std::vector<std::string> list;
            list.push_back(inputPath.file_string());
            fileList.push_back( list );
        }
    }
    // folder case
    else if( boost::filesystem::is_directory(inputPath))
    {
        std::cout << "[dcmapi] Reading DICOM folder structure." << std::endl;
        // set the data set from the directory
        reader->ReadDirectory( inputPath.file_string() ); 
        // get the file list
        fileList = dcmapi::GetDataSetFileList(dcmData);
        // output
        unsigned int nFiles = 0;
        for( unsigned int i = 0; i < fileList.size(); ++i )
        {
            nFiles += fileList[i].size();
        }
        std::cout << "[dcmapi] " << nFiles << " file(s)." << std::endl;
    }
    else
    {
        std::cerr << "[dcmapi][ERROR] Neither file, nor folder, this is a problem..." << std::endl;
    }
    
    // show the data set structure
    //ShowDataSetStructure(dcmData);
    
    // return
    return fileList;
}

/**
* Read a list of DICOM files with a dcmapi reader.
* @param paths The list of DICOM files paths.
* @param reorientData Flag to reorient the data or not.
* @param isTaggedMR Flag to load as tagged MR or not.
* @return The read data.
*/
template< class T, unsigned int N >
vtkSmartPointer<vtkImageData> ReadDicomFiles(const std::vector<std::string>& paths, bool reorientData, bool isTaggedMR)
{
    // Read using dcmAPI::ImageUtilities
    return dcmAPI::ImageUtilities::ReadMultiSliceVtkImageFromFiles< T, N >( 
        paths, reorientData, isTaggedMR );
}

/**
* Read a list of DICOM files with a dcmapi reader.
* @param paths The list of DICOM files paths.
* @param reorientData Flag to reorient the data or not.
* @param isTaggedMR Flag to load as tagged MR or not.
* @param pixelType The pixel type.
* @return The read data.
*/
std::vector< vtkSmartPointer<vtkImageData> > ReadDicomFiles(
    const std::vector< std::vector<std::string> >& paths,
    bool reorientData,
    bool isTaggedMR,
    const itk::ImageIOBase::IOComponentType& pixelType)
{
    std::vector< vtkSmartPointer<vtkImageData> > data;
    
    for( unsigned int i=0; i < paths.size(); ++i )
    {
        std::cout << "[dcmapi] Loading time point " << i+1 << "/" << paths.size() << std::endl;
        std::cout << "[dcmapi] files:";
        for( unsigned int j=0; j < paths[i].size(); ++j )
        {
            std::string path = paths[i][j];
            std::cout << " " << path.substr( path.find_last_of('/')+1, path.size() );
        }
        std::cout << std::endl;
        
        vtkSmartPointer<vtkImageData> imageData;
        // load the files
        switch(pixelType)
        {
            case itk::ImageIOBase::DOUBLE:
                std::cout << "[dcmapi] Data type: ImageIOBase::DOUBLE" << std::endl;
                imageData = dcmapi::ReadDicomFiles< double, 3 >(paths[i], reorientData, isTaggedMR);
                break;
            case itk::ImageIOBase::FLOAT:
                std::cout << "[dcmapi] Data type: ImageIOBase::FLOAT" << std::endl;
                imageData = dcmapi::ReadDicomFiles< float, 3 >(paths[i], reorientData, isTaggedMR);
                break;
            case itk::ImageIOBase::LONG:
                std::cout << "[dcmapi] Data type: ImageIOBase::LONG" << std::endl;
                imageData = dcmapi::ReadDicomFiles< long, 3 >(paths[i], reorientData, isTaggedMR);
                break;
            case itk::ImageIOBase::ULONG:
                std::cout << "[dcmapi] Data type: ImageIOBase::ULONG" << std::endl;
                imageData = dcmapi::ReadDicomFiles< unsigned long, 3 >(paths[i], reorientData, isTaggedMR);
                break;
            case itk::ImageIOBase::INT:
                std::cout << "[dcmapi] Data type: ImageIOBase::INT" << std::endl;
                imageData = dcmapi::ReadDicomFiles< int, 3 >(paths[i], reorientData, isTaggedMR);
                break;
            case itk::ImageIOBase::UINT:
                std::cout << "[dcmapi] Data type: ImageIOBase::UINT" << std::endl;
                imageData = dcmapi::ReadDicomFiles< unsigned int, 3 >(paths[i], reorientData, isTaggedMR);
                break;
            case itk::ImageIOBase::SHORT:
                std::cout << "[dcmapi] Data type: ImageIOBase::SHORT" << std::endl;
                imageData = dcmapi::ReadDicomFiles< short, 3 >(paths[i], reorientData, isTaggedMR);
                break;
            case itk::ImageIOBase::USHORT:
                std::cout << "[dcmapi] Data type: ImageIOBase::USHORT" << std::endl;
                imageData = dcmapi::ReadDicomFiles< unsigned short, 3 >(paths[i], reorientData, isTaggedMR);
                break;
            case itk::ImageIOBase::CHAR:
                std::cout << "[dcmapi] Data type: ImageIOBase::CHAR" << std::endl;
                imageData = dcmapi::ReadDicomFiles< char, 3 >(paths[i], reorientData, isTaggedMR);
                break;
            case itk::ImageIOBase::UCHAR:
                std::cout << "[dcmapi] Data type: ImageIOBase::UCHAR" << std::endl;
                imageData = dcmapi::ReadDicomFiles< unsigned char, 3 >(paths[i], reorientData, isTaggedMR);
                break;
            default:
                throw std::runtime_error("[dcmapi] Unkown pixel type.");
        }
        data.push_back( imageData );
    }
    // return
    return data;
}

} // namepsace dcmapi
//@}

#endif // DCMAPIDICOMREAD_H

