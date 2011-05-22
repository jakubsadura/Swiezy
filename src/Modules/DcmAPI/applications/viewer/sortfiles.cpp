/*
* Copyright (c) 2010,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
/**
* @file sortUID.cpp 
* @brief Sort a folder containing DICOM data and puts all different series in specific folders.
*/
// boost
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/trim.hpp>
// dcmAPI
#include "dcmapiDicomRead.h"

/**
* Converts a number to a string.
* @param value The number to convert.
* @return The string representation of the input.
*/
template< class T >
std::string toString( const T& value )
{
    std::ostringstream oss;
    oss << value;
    return oss.str(); 
}

/**
* Clean a string: trim, replace folder separator.
*/
std::string cleanStr( const std::string& input )
{
    return boost::algorithm::replace_all_copy( boost::algorithm::trim_copy(input), "/", "_" );
}

/**
* Sort DICOM data using UID.
* @param argv[1] The path to the DICOM data.
* @param argv[2] The DICOM reader.
*
* @ingroup sort
*/
int main(int argc, char* argv[])
{
    // process command line
    if( argc != 3 && argc != 4 )
    {
        std::cerr << "Usage: " << argv[0] << " PATH OUTPUT_PATH CREATE_TIME_FOLDER" << std::endl;
        std::cerr << "  PATH: the path to the DICOM data." << std::endl;
        std::cerr << "  OUTPUT_PATH: the path to sorted DICOM data (it should exist)." << std::endl;
        std::cerr << "  CREATE_TIME_FOLDER: bool to create time point folder or not (true/false, default = false)." << std::endl;
        std::cerr << "Sortfiles will return the following structure:" << std::endl;
        std::cerr << "  OUTPUT_PATH/" << std::endl;
        std::cerr << "      |_ patientName#1" << std::endl;
        std::cerr << "             |_ studyUID#1_Description" << std::endl;
        std::cerr << "                     |_ seriesUID#1_Description" << std::endl;
        std::cerr << "                     |_ seriesUID#2_Description" << std::endl;
        std::cerr << "                     |_ ..." << std::endl;
        std::cerr << "              |_ studyUID#2_Description" << std::endl;
        std::cerr << "                     |_ ..." << std::endl;
        std::cerr << "      |_ patientName#2" << std::endl;
        std::cerr << "             |_ ..." << std::endl;
        return EXIT_FAILURE;
    }
    const std::string inputStr = argv[1];
    const boost::filesystem::path inputPath(inputStr);

    const std::string outputStr = argv[2];
    const boost::filesystem::path outputPath(outputStr);
    
    bool createTimeFolder = false;
    if( argc == 4 )
    {
        const std::string timeFolderStr = argv[3];
        createTimeFolder = (timeFolderStr == "true" ? true : false);        
    }

    // exit if input path does not exist
    if( !boost::filesystem::exists(inputPath) )
    { 
        std::cerr << "[ERROR] Input path does not exist: " << inputStr << std::endl;
        return EXIT_FAILURE;
    }
    // exit if input path does is not a directory
    if( !boost::filesystem::is_directory(inputPath) )
    { 
        std::cerr << "[ERROR] Input path is not a directory: " << inputStr << std::endl;
        return EXIT_FAILURE;
    }

    // exit if output path does not exist
    if( !boost::filesystem::exists(outputPath) )
    { 
        std::cerr << "[ERROR] Output path does not exist: " << outputStr << std::endl;
        return EXIT_FAILURE;
    }
    // exit if output path does is not a directory
    if( !boost::filesystem::is_directory(outputPath) )
    { 
        std::cerr << "[ERROR] Output path is not a directory: " << outputStr << std::endl;
        return EXIT_FAILURE;
    }

    // DICOM data: representation of the data structure
    dcmAPI::DataSet::Pointer dcmData = dcmAPI::DataSet::New();
    dcmAPI::DataSetReader::Pointer reader = dcmAPI::DataSetReader::New( );
    reader->SetDataSet( dcmData );
    std::cout << "Reading dicom data from folder..." << std::endl;
    reader->ReadDirectory( inputPath.file_string() ); 
    
    // local vars
    std::string patientId;
    dcmAPI::Patient::Pointer patient;
    dcmAPI::PatientIdVectorPtr patientIdVector;
    std::string studyId;
    dcmAPI::Study::Pointer study;
    dcmAPI::StudyIdVectorPtr studiesIdVector;
    std::string seriesId;
    dcmAPI::Series::Pointer series;
    dcmAPI::SeriesIdVectorPtr seriesIdVector;
    std::string timePointId;
    dcmAPI::TimePoint::Pointer timePoint;
    dcmAPI::TimePointIdVectorPtr timePointIdVector;
    std::string sliceId;
    dcmAPI::Slice::Pointer slice;
    dcmAPI::SliceIdVectorPtr sliceIdVector;
    
    boost::filesystem::path patientPath;
    boost::filesystem::path studyPath;
    boost::filesystem::path seriesPath;
    boost::filesystem::path newSliceFilePath;

    std::string patientStr; 
    std::string studyStr; 
    std::string seriesStr;
    std::string timePointName;
    std::string sliceFileName;
    std::string sliceFilePath;
    std::string tmp;
    
    unsigned int count = 0;

    std::cout << "Creating folders..." << std::endl;

    // Patients in the Dataset
    patientIdVector = dcmData->GetPatientIds();
    for(unsigned int i=0; i < patientIdVector->size(); ++i)
    {
        patientId = patientIdVector->at(i);
        patient = dcmData->GetPatient(patientId);
        
        patientStr = patient->GetTagAsText(dcmAPI::tags::PatientName);
        if( patientStr.size() == 0 ) patientStr = "patient_" + patientId;
        patientStr = cleanStr(patientStr);
        
        // handle patient folder allready exists
        if( boost::filesystem::exists(patientStr) )
        {
            patientStr += " (2)";
            while( boost::filesystem::exists(patientStr) )
            {
                const size_t index = patientStr.find_last_of("(");
                const unsigned int number = atoi( patientStr.substr(index+1, patientStr.size()-1 ).c_str() );
                patientStr = patientStr.substr(0, index+1) + toString(number+1) + ")";
            }
        }
        
        // create folder
        patientPath = outputPath / cleanStr(patientStr).c_str();
        std::cout << "Creating patient folder: " << patientPath << std::endl;
        boost::filesystem::create_directory( patientPath );
        
        // Studies of the patient
        studiesIdVector = patient->StudyIds();
        for(unsigned int j=0; j < studiesIdVector->size(); ++j)
        {
            studyId = studiesIdVector->at(j);
            study = patient->Study(studyId);
            
            studyStr = study->GetTagAsText(dcmAPI::tags::StudyInstanceUID);
            if( studyStr.size() == 0 ) studyStr = "study_" + studyId;
            tmp = study->GetTagAsText(dcmAPI::tags::StudyDescription);
            if( tmp.size() != 0 ) studyStr += "-" + tmp;
            
            // create folder
            studyPath = patientPath / cleanStr(studyStr).c_str();
            std::cout << "Creating study folder:   " << studyPath << std::endl;
            boost::filesystem::create_directory( studyPath );
            
            // Series of the study
            seriesIdVector = study->SeriesIds();
            for(unsigned int k=0; k < seriesIdVector->size(); ++k)
            {
                seriesId = seriesIdVector->at(k);
                series = study->Series(seriesId);
                
                seriesStr = series->GetTagAsText(dcmAPI::tags::SeriesInstanceUID);
                if( seriesStr.size() == 0 ) seriesStr = "series_" + seriesId;
                tmp = series->GetTagAsText(dcmAPI::tags::SeriesDescription);
                if( tmp.size() != 0 ) seriesStr += "-" + tmp;
                
                // create folder
                seriesPath = studyPath / cleanStr(seriesStr);
                std::cout << "Creating series folder:  " << seriesPath << std::endl;
                boost::filesystem::create_directory( seriesPath );
                
                // TimePoints in the series
                timePointIdVector = series->TimePointIds();
                for(unsigned int p=0; p < timePointIdVector->size(); ++p)
                {
                    timePointId = timePointIdVector->at(p);
                    timePoint = series->TimePoint(timePointId);
                    
                    std::string timepointStr = "time_" + timePointId;
                    boost::filesystem::path timepointPath = seriesPath / cleanStr(timepointStr);
                    
                    // create folder
                    if( createTimeFolder )
                    {
                        std::cout << "Creating timepooint folder:  " << timepointPath << std::endl;
                        boost::filesystem::create_directory( timepointPath );
                    }

                    // Slices in the timepoint
                    sliceIdVector = timePoint->SliceIds();
                    for(unsigned int q=0; q < sliceIdVector->size(); ++q)
                    {
                        sliceId = sliceIdVector->at(q);
                        slice = timePoint->Slice(sliceId);
                        
                        sliceFilePath = slice->GetTagAsText(dcmAPI::tags::SliceFilePath);
                        sliceFileName = slice->GetTagAsText(dcmAPI::tags::SliceFileName);
                        
                        // copy file
                        if( createTimeFolder ) newSliceFilePath = timepointPath / sliceFileName;
                        else newSliceFilePath = seriesPath / sliceFileName;
                        boost::filesystem::copy_file( sliceFilePath, newSliceFilePath );
                        ++count;
                    }
                }
            }
        }
    }
    
    std::cout << "Copied " << count << " file";
    if( count > 1 ) std::cout << "s";
    std::cout << "." << std::endl;

    return EXIT_SUCCESS;
}

