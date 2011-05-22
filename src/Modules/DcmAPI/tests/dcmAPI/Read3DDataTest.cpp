/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#include "Read3DDataTest.h"

// ITK
#include <itkImageFileWriter.h>
#include <itkImageFileReader.h>
// VTK
#include <vtkSmartPointer.h>
// DcmAPI
#include <dcmDataSet.h>
#include <dcmTypes.h>
#include <dcmDataSetReader.h>
#include <dcmFile.h>
#include <dcmImageUtilities.h>
#include <dcmMultiSliceReader.h>
// Predefined paths
#include <CISTIBToolkit.h>
// boost
#include <boost/filesystem.hpp>
// Other
#include <blVTKHelperTools.h>
#include <blImageUtils.h>
#include <algorithm>

bool Read3DDataTest::ReadAndCompare( 
    const std::string& dcmInputNameEnd, 
    const std::string& vtkFileNameEnd,
    const TYPE& inputType ) const
{
	try
	{
        // Working directory of the test
        const std::string dcmInputName = std::string(CISTIB_TOOLKIT_FOLDER) + dcmInputNameEnd;
        const std::string vtkFileName = std::string(CISTIB_TOOLKIT_FOLDER) + vtkFileNameEnd;

         // Check if the vtk file exists
        std::ifstream ifs2( vtkFileName.c_str() );
        TSM_ASSERT( "The input VTK file does not exist.", !ifs2.fail() );
        if( ifs2.fail() ) return false;
        ifs2.close();

        // read input
        vtkSmartPointer<vtkImageData> vtkDicomImage;
        switch( inputType )
        {
            case FILE:
            {
                // Check if the dicom file exists
                std::ifstream ifs1( dcmInputName.c_str() );
                TSM_ASSERT( "The input DICOM file does not exist.", !ifs1.fail() );
                if( ifs1.fail() ) return false;
                ifs1.close();
                // Read using dcmAPI::ImageUtilities
                vtkDicomImage = dcmAPI::ImageUtilities::ReadVtkImageFromFile< unsigned short, 3 >( 
                    dcmInputName.c_str(), true );
            }
            break;
            
            case FOLDER:
            {
                // Check if the folder exists
                if ( !boost::filesystem::exists( dcmInputName ) ) return false;
                // create list of files
                std::vector< std::string > dcmFileNames;
                boost::filesystem::directory_iterator end_itr; // default construction yields past-the-end
                for( boost::filesystem::directory_iterator itr( dcmInputName );
                    itr != end_itr; ++itr )
                {
                    if( is_regular_file(itr->status()) && itr->leaf() != "DICOMDIR")
                    {
                        dcmFileNames.push_back( dcmInputName + "/" + itr->leaf() );
                    }
                }
                // sort the files alphabetically
                std::sort(dcmFileNames.begin(), dcmFileNames.end());
                // Read using dcmAPI::ImageUtilities
                vtkDicomImage = dcmAPI::ImageUtilities::ReadMultiSliceVtkImageFromFiles< unsigned short, 3 >( 
                    dcmFileNames, true );
           }
            break;
            
            case DICOMDIR:
            {
		        // Data set
		        dcmAPI::DataSet::Pointer dcmData = dcmAPI::DataSet::New();
		        
		        // scan the current working folder for dicom files
		        dcmAPI::DataSetReader::Pointer reader = dcmAPI::DataSetReader::New( );
		        reader->SetDataSet( dcmData );
		        reader->ReadDicomDir( dcmInputName ); 
		        
		        // Patient
			    const dcmAPI::Patient::Pointer patient = dcmData->GetPatient( dcmData->GetPatientIds()->at(0) );
		        // Study
		        const dcmAPI::Study::Pointer study = patient->Study( patient->StudyIds()->at(0) );
		        // Serie
		        const dcmAPI::Series::Pointer series = study->Series( study->SeriesIds()->at(0) );
		        // TimePoint
		        const dcmAPI::TimePoint::Pointer timePoint = series->TimePoint( series->TimePointIds()->at(0) );
		        // Slice
		        const dcmAPI::SliceIdVectorPtr sliceIdVector = timePoint->SliceIds();
		        std::vector< std::string > dcmFileNames;
		        for( unsigned int i=0; i < sliceIdVector->size(); ++i)
		        {
			        const dcmAPI::Slice::Pointer slice = timePoint->Slice(sliceIdVector->at(i));
			        const std::string filePath = slice->GetTagAsText(dcmAPI::tags::SliceFilePath);
			        dcmFileNames.push_back( filePath );
			    }
                // Read using dcmAPI::ImageUtilities
                vtkDicomImage = dcmAPI::ImageUtilities::ReadMultiSliceVtkImageFromFiles< unsigned short, 3 >( 
                    dcmFileNames, true );
            }
            break;
            
            default:
            {
                TS_FAIL("Read3DDataTest::ReadAndCompare failed: unknwown case.");
		        return false;
            }
        }
        
        // Read using BaseLib
        vtkSmartPointer<vtkImageData> vtkImage = blImageUtils::LoadImageFromFileAsVTK( vtkFileName.c_str() );

        // Compare image content
        return blImageUtils::CompareImages( vtkDicomImage, vtkImage, 1e-5 );
	}
	catch(...)
	{
		TS_FAIL("Read3DDataTest::ReadAndCompare failed: thrown exception.");
		return false;
	}
}

void Read3DDataTest::TestReadSingleSliceFile()
{
    TSM_ASSERT( "Images (from single slice file) are not equal.", ReadAndCompare( 
        "/Data/Tests/DcmAPI/synthetic/colors/axial/files/color-a1_000.dcm",
        "/Data/Tests/DcmAPI/synthetic/colors/axial/color-a1_000.vtk",
        FILE ) );
}

void Read3DDataTest::TestReadMultiSliceFile()
{
    TSM_ASSERT( "Images (from multi slice file) are not equal.", ReadAndCompare( 
        "/Data/Tests/DcmAPI/synthetic/colors/axial/color-a1.dcm",
        "/Data/Tests/DcmAPI/synthetic/colors/axial/color-a1.vtk",
        FILE ) );
}

void Read3DDataTest::TestReadDirectory()
{
    TSM_ASSERT( "Images (from folder) are not equal.", ReadAndCompare( 
        "/Data/Tests/DcmAPI/synthetic/colors/axial/files",
        "/Data/Tests/DcmAPI/synthetic/colors/axial/color-a1.vtk",
        FOLDER ) );
}

void Read3DDataTest::TestReadDicomDir()
{
    TSM_ASSERT( "Images (from DICOMDIR) are not equal.", ReadAndCompare( 
        "/Data/Tests/DcmAPI/synthetic/colors/axial/files/DICOMDIR",
        "/Data/Tests/DcmAPI/synthetic/colors/axial/color-a1.vtk",
        DICOMDIR ) );
}

void Read3DDataTest::TestReadDirectoryDataSet()
{
	try
	{
		//directory with dicom data
		std::string dirPath = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/DcmAPI/DICOMDIR_1/";

		//Data to compare in the tests
		unsigned int patientsNumber = 1;
		unsigned int studiesNumber = 1;
		unsigned int seriesNumber = 1;
		unsigned int timePointsNumber = 1;
		//int slicesNumber = 14;
		/*static std::string const patientIds[]={"161006"};
		static std::string const studiesIds[]={"1.2.250.1.46.30.60526.20061013113008.849668687"};
		static std::string const seriesIds[]={"1.2.840.113619.2.134.1762890972.2671.1160720118.659"};*/
		static std::string const patientIds[]={"1"};
		static std::string const studiesIds[]={"1"};
		static std::string const seriesIds[]={"1"};
		static std::string const timePointsIds[]={"1"};
		static std::string const sliceIds[] = {"1"};
		static std::string const sliceFileName[] = {"IM000001"};
		static std::string const sliceFilePath[] = {dirPath + "IM000001"};

		dcmAPI::DataSet::Pointer dcmData = dcmAPI::DataSet::New();

		// scan the current working folder for dicom files
		dcmAPI::DataSetReader::Pointer reader = dcmAPI::DataSetReader::New( );
		reader->SetDataSet( dcmData );
		reader->ReadDirectory(dirPath); 

		dcmAPI::Slice::Pointer Myslice = dcmData->GetSlice(
			patientIds[0],
			studiesIds[0],
			seriesIds[0],
			timePointsIds[0],
			sliceIds[0]
		);
		
		TS_ASSERT(Myslice);
		//TS_ASSERT_EQUALS( Myslice->GetTagAsText(dcmAPI::tags::SliceFileName), sliceFileName[0]);

		//Get the vector of patients Ids
		dcmAPI::PatientIdVectorPtr patiendIdVector = dcmData->GetPatientIds();

		//Check if the number of patients is correct
		TS_ASSERT_EQUALS(patientsNumber,patiendIdVector->size());

		//Patients in the Dataset;
		for(unsigned int i=0; i < patiendIdVector->size(); i++)
		{
			std::string patientId = patiendIdVector->at(i);
			dcmAPI::Patient::Pointer patient = dcmData->GetPatient(patientId);

			//Check if the number of patients is correct
			TS_ASSERT_EQUALS(patientIds[i], patient->GetTagAsText(dcmAPI::tags::PatientId));

			dcmAPI::StudyIdVectorPtr studiesIdVector = patient->StudyIds();

			//Check if the number of studies is correct
			TS_ASSERT_EQUALS(studiesNumber, studiesIdVector->size());

			//Studies of the patient
			for(unsigned int j=0; j < studiesIdVector->size(); j++)
			{
				std::string studyId = studiesIdVector->at(j);
				dcmAPI::Study::Pointer study = patient->Study(studyId);

				TS_ASSERT_EQUALS(studiesIds[j], study->GetTagAsText(dcmAPI::tags::StudyId));			

				dcmAPI::SeriesIdVectorPtr seriesIdVector = study->SeriesIds();

				//Check if the number of Series is correct
				TS_ASSERT_EQUALS(seriesNumber, seriesIdVector->size());

				//Prints the all the Series of the Study
				for(unsigned int k=0; k < seriesIdVector->size(); k++)
				{
					std::string seriesId = seriesIdVector->at(k);
					dcmAPI::Series::Pointer series = study->Series(seriesId);

					TS_ASSERT_EQUALS(seriesIds[k], series->GetTagAsText(dcmAPI::tags::SeriesId));

					//Prints the all the TimePoints
					dcmAPI::TimePointIdVectorPtr timePointIdVector = series->TimePointIds();

					//Check if the number of TimePoints is correct
					TS_ASSERT_EQUALS(timePointsNumber, timePointIdVector->size());

					for(unsigned int p=0; p < timePointIdVector->size(); p++)
					{
						std::string timePointId = timePointIdVector->at(p);
						dcmAPI::TimePoint::Pointer timePoint = series->TimePoint(timePointId);

						TS_ASSERT_EQUALS(timePointsIds[p], timePoint->GetTagAsText(dcmAPI::tags::TimePointId));

						dcmAPI::SliceIdVectorPtr sliceIdVector = timePoint->SliceIds();

						//Check if the number of Slices is correct
						//TS_ASSERT_EQUALS(slicesNumber, sliceIdVector->size());

						//iterate over the Slices
						for(unsigned int q=0; q < sliceIdVector->size(); q++)
						{
							//Checking tags
							dcmAPI::Slice::Pointer slice = timePoint->Slice(sliceIds[0]);
							//TS_ASSERT_EQUALS(sliceFilePath[j],  slice->GetTagAsText(dcmAPI::tags::SliceFilePath));
							//TS_ASSERT_EQUALS(sliceFileName[j], slice->GetTagAsText(dcmAPI::tags::SliceFileName));
							break;
						}
						break;
					}
					break;
				}
				break;
			}
			break;
		}
	}
	catch(...)
	{
		TS_FAIL("TestReadDirectory failed: exception was thrown");
	}
}

void Read3DDataTest::TestReadDicomDirDataSet()
{
	try
	{
		//Working directory of the application
		std::string wd = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/DcmAPI/DICOMDIR_1/";

		std::string dicomDir = wd+"DICOMDIR";
		
		//Data to compare in the tests
		unsigned int patientsNumber = 1;
		unsigned int studiesNumber = 1;
		unsigned int seriesNumber = 1;
		unsigned int timePointsNumber = 1;
		unsigned int slicesNumber = 14;
		//static std::string const patientIds[]={"161006"};
		//static std::string const studiesIds[]={"1.2.250.1.46.30.60526.20061013113008.849668687"};
		//static std::string const seriesIds[]={"1.2.840.113619.2.134.1762890972.2671.1160720118.659"};
		const std::string patientIds[]={"1"};
		const std::string studiesIds[]={"1"};
		const std::string seriesIds[]={"1"};
		const std::string timePointsIds[]={"1"};
		const std::string sliceIds[] = {"1"};
		const std::string sliceFileName[] = {"IM000001"};
		const std::string sliceFilePath[] = {wd + "IM000001"};
		
		dcmAPI::DataSet::Pointer dcmData = dcmAPI::DataSet::New();

		// scan the current working folder for dicom files
		dcmAPI::DataSetReader::Pointer reader = dcmAPI::DataSetReader::New( );
		reader->SetDataSet( dcmData );
		reader->ReadDicomDir(dicomDir); 

		dcmAPI::Slice::Pointer Myslice = dcmData->GetSlice(
			patientIds[0],
			studiesIds[0],
			seriesIds[0],
			timePointsIds[0],
			sliceIds[0]
			);

		TS_ASSERT(Myslice);
		TS_ASSERT_EQUALS(sliceFileName[0], Myslice->GetTagAsText(dcmAPI::tags::SliceFileName));

		//Get the vector of patients Ids
		dcmAPI::PatientIdVectorPtr patiendIdVector = dcmData->GetPatientIds();

		//Check if the number of patients is correct
		TS_ASSERT_EQUALS(patientsNumber,patiendIdVector->size());

		//Patients in the Dataset;
		for(unsigned int i=0; i < patiendIdVector->size(); i++)
		{
			std::string patientId = patiendIdVector->at(i);
			dcmAPI::Patient::Pointer patient = dcmData->GetPatient(patientId);
			
			//Check if the number of patients is correct
			TS_ASSERT_EQUALS(patientIds[i], patient->GetTagAsText(dcmAPI::tags::PatientId));

			dcmAPI::StudyIdVectorPtr studiesIdVector = patient->StudyIds();

			//Check if the number of studies is correct
			TS_ASSERT_EQUALS(studiesNumber, studiesIdVector->size());

			//Studies of the patient
			for(unsigned int j=0; j < studiesIdVector->size(); j++)
			{
				std::string studyId = studiesIdVector->at(j);
				dcmAPI::Study::Pointer study = patient->Study(studyId);

				TS_ASSERT_EQUALS(studiesIds[j], study->GetTagAsText(dcmAPI::tags::StudyId));			

				dcmAPI::SeriesIdVectorPtr seriesIdVector = study->SeriesIds();

				//Check if the number of Series is correct
				TS_ASSERT_EQUALS(seriesNumber, seriesIdVector->size());

				//Prints the all the Series of the Study
				for(unsigned int k=0; k < seriesIdVector->size(); k++)
				{
					std::string seriesId = seriesIdVector->at(k);
					dcmAPI::Series::Pointer series = study->Series(seriesId);
					
					TS_ASSERT_EQUALS(seriesIds[k], series->GetTagAsText(dcmAPI::tags::SeriesId));

					//Prints the all the TimePoints
					dcmAPI::TimePointIdVectorPtr timePointIdVector = series->TimePointIds();

					//Check if the number of TimePoints is correct
					TS_ASSERT_EQUALS(timePointsNumber, timePointIdVector->size());

					for(unsigned int p=0; p < timePointIdVector->size(); p++)
					{
						std::string timePointId = timePointIdVector->at(p);
						dcmAPI::TimePoint::Pointer timePoint = series->TimePoint(timePointId);

						TS_ASSERT_EQUALS(timePointsIds[p], timePoint->GetTagAsText(dcmAPI::tags::TimePointId));

						dcmAPI::SliceIdVectorPtr sliceIdVector = timePoint->SliceIds();
						
						//Check if the number of Slices is correct
						TS_ASSERT_EQUALS(slicesNumber, sliceIdVector->size());

						//iterate over the Slices
						for(unsigned int q=0; q < sliceIdVector->size(); q++)
						{
							//std::string sliceId = sliceIdVector->at(q);
							dcmAPI::Slice::Pointer slice = timePoint->Slice(sliceIds[0]);
							TS_ASSERT_EQUALS(sliceFilePath[q],  slice->GetTagAsText(dcmAPI::tags::SliceFilePath));
							TS_ASSERT_EQUALS(sliceFileName[q], slice->GetTagAsText(dcmAPI::tags::SliceFileName));
							break;
						}
						break;
					}
					break;
				}
				break;
			}
			break;
		}
	}
	catch(...)
	{
		TS_FAIL("TestReadDicomDir failed: exception was thrown");
	}
}

