/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include <blLogger.h>

#include "dcmDataSetReader.h"
#include "dcmDicomDirReader.h"
#include "dcmDirectoryReader.h"
#include "dcmFileReader.h"
#include "dcmImageUtilities.h"

using namespace dcmAPI;


DataSetReader::DataSetReader()
{
    m_DataSet = DataSet::New();
    m_logger = blLogger::getInstance("dcmapi.DataSetReader");

    m_TruncateSeparator = '\0';
    m_TruncateLevel = 0;
    m_IsTimeTagInFile = false;
}

DataSetReader::~DataSetReader()
{
}

DataSetReader::Pointer DataSetReader::New()
{
    return Pointer(new DataSetReader());
}

TagId DataSetReader::GetTimeTag() const
{
    return m_TimeTag;
}

void DataSetReader::SetTimeTag(const TagId& timeTag)
{
    m_TimeTag = timeTag;
}

void DataSetReader::SetTimeTagTruncate(char separator, int level)
{
    m_TruncateSeparator = separator;
    m_TruncateLevel = level;
}

void DataSetReader::SetTimeTagLocation(bool isInFile)
{
    m_IsTimeTagInFile = isInFile;
}

void DataSetReader::ReadDicomDir(const std::string& path)
{
    LOG4CPLUS_TRACE_METHOD(m_logger, "ReadDicomDir");

    m_DataSet->Clear();
    DicomDirReader::Pointer reader = DicomDirReader::New();
    reader->SetPath( path );
    reader->SetDataSet( m_DataSet );
    reader->Update();

    unsigned int nSliceCount = m_DataSet->GetNumberOfSlices();
    if(nSliceCount < 1)
	    throw DcmDataSetReaderException( "Retrieved dataset is empty" );

    RecognizeData();

    // dataset must be recreated because the files are not 2D images
    if (m_DataSet->GetStorageType()  == DataSet::MULTI_SLICE_PER_FILE)
    {
	    // call read multi slice from files
	    std::vector<std::string> filePaths = GetFilepathsOfSlices();
	    ReadMultiSliceDcmFiles(filePaths);
	    RecognizeData();
    }
}

void DataSetReader::ReadDirectory(const std::string& path)
{
    LOG4CPLUS_TRACE_METHOD(m_logger, "ReadDirectory");

    m_DataSet->Clear();

    DirectoryReader::Pointer reader = DirectoryReader::New();
    reader->SetPath( path );
    reader->SetDataSet( m_DataSet );
    if( m_TimeTag.IsSet() )
    {
        reader->SetTimeTag( m_TimeTag );
    }
    if( m_TruncateSeparator != '\0' )
    {
        reader->SetTimeTagTruncate(m_TruncateSeparator, m_TruncateLevel);
    }
    if( m_IsTimeTagInFile )
    {
        reader->SetTimeTagLocation(m_IsTimeTagInFile);
    }
    reader->Update();

    unsigned int nSliceCount = m_DataSet->GetNumberOfSlices();
    if(nSliceCount < 1)
	    throw DcmDataSetReaderException( "Retrieved dataset is empty" );

    RecognizeData();

    // dataset must be recreated because the files are not 2D images
    if (m_DataSet->GetStorageType() == DataSet::MULTI_SLICE_PER_FILE)
    {
	    // call read multi slice from files
	    std::vector<std::string> filePaths = GetFilepathsOfSlices();
	    ReadMultiSliceDcmFiles(filePaths);
	    RecognizeData();
    }
}

void DataSetReader::ReadDcmFile(const std::string& path)
{
    LOG4CPLUS_TRACE_METHOD(m_logger, "ReadDcmFile");

    m_DataSet->Clear();

    FileReader::Pointer reader = FileReader::New();
    for( unsigned int i = 0; i < m_Readers.size(); ++i )
    {
        reader->AddReader( m_Readers[i] );
    }
    reader->SetPath( path );
    reader->SetDataSet( m_DataSet );
    reader->Update();

    unsigned int nSliceCount = m_DataSet->GetNumberOfSlices();
    if(nSliceCount < 1)
	    throw DcmDataSetReaderException( "Retrieved dataset is empty" );

    RecognizeData();
}


std::vector<std::string> DataSetReader::GetFilepathsOfSlices() const
{
	LOG4CPLUS_TRACE_METHOD(m_logger, "GetFilepathsOfSlices");

	std::vector<std::string> filepaths;
	PatientIdVectorPtr patiendIdVector = m_DataSet->GetPatientIds();
	for(size_t i = 0; i < patiendIdVector->size(); i++)
	{
		Patient::Pointer patient = m_DataSet->GetPatient(patiendIdVector->at(i));
		//reading studies
		StudyIdVectorPtr studyIdVector = patient->StudyIds();
		for(size_t j=0; j < studyIdVector->size(); j++)
		{
			Study::Pointer study = patient->Study(studyIdVector->at(j));
			//reading series
			SeriesIdVectorPtr seriesIdVector = study->SeriesIds();
			for(size_t k=0; k < seriesIdVector->size(); k++)
			{
				Series::Pointer series = study->Series(seriesIdVector->at(k));
				//reading timepoints
				TimePointIdVectorPtr timepointIdVector = series->TimePointIds();
				for(size_t l=0; l < timepointIdVector->size(); l++)
				{
					TimePoint::Pointer timepoint = series->TimePoint(timepointIdVector->at(l));
					//reading slices
					SliceIdVectorPtr sliceIdVector = timepoint->SliceIds();
					for(size_t m=0; m < sliceIdVector->size(); m++)
					{
						Slice::Pointer slice = Slice::New();
						slice= m_DataSet->GetSlice(patiendIdVector->at(i),
												   studyIdVector->at(j),
												   seriesIdVector->at(k),
												   timepointIdVector->at(l),
												   sliceIdVector->at(m));
						filepaths.push_back(slice->GetTagAsText(tags::SliceFilePath));
					}
				}

			}

		}
	}
	return filepaths;
}

DataSet::Pointer DataSetReader::GetDataSet() const
{
    return m_DataSet;
}

void DataSetReader::SetDataSet( DataSet::Pointer val )
{
    m_DataSet = val;
}

/**
most of the data will is complaint with DICOM STANDARD, but there are also some non standard data - 
based on the modality and manufacturer we can set the non standard dicom data type 
*/

void DataSetReader::RecognizeData()
{
	LOG4CPLUS_TRACE_METHOD(m_logger, "RecognizeData");

	std::string modality, manufacturer;
	PatientIdVectorPtr patiendIdVector = m_DataSet->GetPatientIds();
	if(patiendIdVector->size() > 0)
	{
		Patient::Pointer patient = m_DataSet->GetPatient(patiendIdVector->at(0));
		//reading studies
		StudyIdVectorPtr studyIdVector = patient->StudyIds();
		if(studyIdVector->size() > 0 )
		{
			Study::Pointer study = patient->Study(studyIdVector->at(0));
			//reading series
			SeriesIdVectorPtr seriesIdVector = study->SeriesIds();
			if(seriesIdVector->size() > 0)
			{
				Series::Pointer series = study->Series(seriesIdVector->at(0));
				modality = series->GetTagAsText(tags::Modality);
				manufacturer = series->GetTagAsText(tags::Manufacturer);

	            LOG4CPLUS_DEBUG(m_logger, "Modality: " << modality);
	            LOG4CPLUS_DEBUG(m_logger, "Manufacturer: " << manufacturer);
	            
				//check if this type is non standard Dicom data
				if(modality == "US" && manufacturer == "Philips Medical Systems")
				{
	                LOG4CPLUS_DEBUG(m_logger, "Setting data type to: PHILIPS_US");
					m_DataSet->SetDataType(DataSet::PHILIPS_US);
				}
				else if(modality == "US" && manufacturer == "GE Vingmed Ultrasound")
				{
	                LOG4CPLUS_DEBUG(m_logger, "Setting data type to: GE_US");
					m_DataSet->SetDataType(DataSet::GE_US);
				}
				else
				{
	                LOG4CPLUS_DEBUG(m_logger, "Setting data type to: STANDARD");
					m_DataSet->SetDataType(DataSet::STANDARD);
				}


				//check the first slice if the file contain single slice or multislice
				TimePointIdVectorPtr timepointsIdVector = series->TimePointIds();
				if(timepointsIdVector->size() > 0)
				{
					TimePoint::Pointer timepoint = series->TimePoint(timepointsIdVector->at(0));
					SliceIdVectorPtr sliceIdVector = timepoint->SliceIds();
					if(sliceIdVector->size() > 0)
					{
						Slice::Pointer slice = timepoint->Slice(sliceIdVector->at(0));
						std::string filePath = slice->GetTagAsText(tags::SliceFilePath);
						std::string firstPosition = slice->GetTagAsText(tags::ImagePositionPatient);
						std::string firstOrientation = slice->GetTagAsText(tags::ImageOrientationPatient);
						double ori[6];
						double refOr[6];
						for (size_t it= 0; it <6; it++)
								refOr[it]= ori[it] = 0;
						File::Pointer file = File::New();
						if( file->Open(filePath) )
						{
							std::string numberOfFrames = file->ReadTagValue(tags::NumberOfFrames);
							if(numberOfFrames.size() == 0 || numberOfFrames == "gdcm::Unfound")
							{
                                LOG4CPLUS_DEBUG(m_logger, "Setting storage type to: SINGLE_SLICE_PER_FILE");
								m_DataSet->SetStorageType(DataSet::SINGLE_SLICE_PER_FILE);
							}
							else
							{
								int n = atoi(numberOfFrames.c_str());
								if(n > 1)
								{
	                                LOG4CPLUS_DEBUG(m_logger, "Setting storage type to: MULTI_SLICE_PER_FILE");
									m_DataSet->SetStorageType(DataSet::MULTI_SLICE_PER_FILE);
								}
								else
								{
	                                LOG4CPLUS_DEBUG(m_logger, "Setting storage type to: SINGLE_SLICE_PER_FILE");
									m_DataSet->SetStorageType(DataSet::SINGLE_SLICE_PER_FILE);
								}
							}
						}
					}
				}
			}
		}
	}
}

void DataSetReader::ReadMultiSliceDcmFiles( const std::vector< std::string>& filePaths )
{
	LOG4CPLUS_TRACE_METHOD(m_logger, "ReadMultiSliceDcmFiles");

	std::vector<DataSet::Pointer> datasetVector;
	for (unsigned i=0; i<filePaths.size(); i++)
	{
		FileReader::Pointer reader = FileReader::New();
	    for( unsigned int r = 0; r < m_Readers.size(); ++r )
	    {
	        reader->AddReader( m_Readers[r] );
	    }
		reader->SetPath( filePaths.at(i) );
		reader->SetDataSet(  DataSet::New());
		reader->Update();
		DataSet::Pointer dataset = reader->GetDataSet();
		if(dataset->GetPatientIds()->size() == 0)
			continue;
		
		/*
		a patch: for some reason the UID of series is the same for all files
		for this reason we have to set a unique id for a series 
		*/
		Series::Pointer series( boost::dynamic_pointer_cast<Series,Base>(dataset->GetFirstSpecifiedElement(SERIES)) );
		if(series.get() != NULL)
		{
			//create string id
			std::strstream t;
			t<<i+1<<std::ends;
			series->AddTag(tags::SeriesId,t.str());
		}
		
		datasetVector.push_back(dataset);
	}

	//merge dataSets into one
	MergeDataSets(datasetVector);
}

void dcmAPI::DataSetReader::MergeDataSets(std::vector<dcmAPI::DataSet::Pointer> dataSetVector )
{
	LOG4CPLUS_TRACE_METHOD(m_logger, "MergeDataSets");

	m_DataSet->Clear();

	unsigned numOfSets = dataSetVector.size();
	
	PatientIdVectorPtr patientId;
	StudyIdVectorPtr studyId;
	SeriesIdVectorPtr seriesId;
	TimePointIdVectorPtr timepointId;
	SliceIdVectorPtr sliceId;
	dcmAPI::Slice::Pointer slice;

	//initialize patientIds of the 
	for (size_t i=0; i < numOfSets; i++)
	{
		dcmAPI::Patient::Pointer patient( boost::dynamic_pointer_cast<Patient,Base>(
			dataSetVector.at(i)->GetFirstSpecifiedElement(dcmAPI::PATIENT)));
		if(patient != NULL)
			m_DataSet->AddPatient(patient);
	}

	// go through the patientIds
	patientId = m_DataSet->GetPatientIds();
	for (size_t j=0; j < patientId->size(); j++)
	{
		//initialize studyIds for all the Patients
		for (size_t i=0; i < numOfSets; i++)
		{
			if (dataSetVector.at(i)->FindPatient(patientId->at(j)))
			{
				dcmAPI::Study::Pointer study(boost::dynamic_pointer_cast<Study,Base>(
					dataSetVector.at(i)->GetFirstSpecifiedElement(dcmAPI::STUDY)));
				if(study != NULL)
					m_DataSet->GetPatient(patientId->at(j))->AddStudy(study);
			}
		}	
		
		// go through the studyIds
		studyId = m_DataSet->GetPatient(patientId->at(j))->StudyIds();
		for (size_t l=0; l< studyId->size(); l++)
		{
			for (size_t i =0; i < numOfSets; i++)
			{
				dcmAPI::Patient::Pointer patient = dataSetVector.at(i)->GetPatient(patientId->at(j));
				if(patient != NULL)
				{
					//if (dataSetVector.at(i)->GetPatient(patientId->at(j))->FindStudy(studyId->at(l)))
					dcmAPI::Study::Pointer study = patient->Study(studyId->at(l));
					//if (patient->FindStudy(studyId->at(l)))
					if(study != NULL)
					{
						Series::Pointer series( boost::dynamic_pointer_cast<Series,Base>(
							dataSetVector.at(i)->GetFirstSpecifiedElement(SERIES)) );
						if(series != NULL)
							m_DataSet->GetPatient(patientId->at(j))->Study(studyId->at(l))->AddSeries(series);
					}
				}
			}
		}
		
	}
}
