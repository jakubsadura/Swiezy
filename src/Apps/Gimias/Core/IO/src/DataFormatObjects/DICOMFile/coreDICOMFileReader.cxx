/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreDICOMFileReader.h"
#include "coreVTKImageDataHolder.h"

#include "blImageUtils.h"
#include "blSignalCollective.h"

#include "dcmDataSetReader.h"
#include "dcmImageUtilities.h"
#include "dcmIOUtils.h"
#include "blSliceImage.h"

#include <itksys/SystemTools.hxx>

using namespace Core::IO;


enum INPUT_PATH_TYPE{
	INPUT_PATH_DICOMDIR,
	INPUT_PATH_DIRECTORY,
	INPUT_PATH_DCMFILE
};



DICOMFileReader::DICOMFileReader(void)
{
	m_ValidExtensionsList.push_back( "" );
	m_ValidExtensionsList.push_back( ".dcm" );
	m_ValidTypesList.push_back( ImageTypeId );
}

DICOMFileReader::~DICOMFileReader(void)
{
}

void DICOMFileReader::Update()
{

	if ( m_Filenames.size( ) == 0 )
	{
		return;
	}

	// Read single DICOM dile
	std::vector<Core::DataEntity::Pointer> dataEntities;
	dataEntities = ImportSingleDICOMFile( m_Filenames[ 0 ] );

	if ( dataEntities.size() == 0 )
	{
		dataEntities = ImportDICOM( m_Filenames[ 0 ] );
	}


	if ( dataEntities.size() )
	{
		SetNumberOfOutputs(dataEntities.size());
		for( size_t i = 0;  i<dataEntities.size(); i++)
		{
			if (dataEntities[i].IsNotNull() )
				SetOutputDataEntity(i, dataEntities[i] );
		}
	}

}

std::vector<Core::DataEntity::Pointer> DICOMFileReader::ImportSingleDICOMFile( std::string filename )
{
	std::vector<Core::DataEntity::Pointer> dataEntities;
	Core::DataEntity::Pointer imageDataEntity;
	Core::DataEntity::Pointer signalDataEntity;
	std::vector< float> cachedECGData;
	std::vector< float > timestamps;
	dcmAPI::EcgData ecgData;

	// USDataGEReader
	std::vector< Core::vtkImageDataPtr > processingDataVector;
	std::list<dcmAPI::AbstractImageReader::Pointer>::iterator it;
	for ( it = m_ReaderList.begin() ; it != m_ReaderList.end() ; it++ )
	{
		try
		{
			(*it)->SetFilename(filename);
			(*it)->Update();
			(*it)->GetVolumes(processingDataVector,timestamps);
			(*it)->GetEcgData(ecgData);
		}
		catch(...)
		{}

	}

	// Create an image data entity
	if ( processingDataVector.size() )
	{
		imageDataEntity = Core::DataEntity::New( ImageTypeId );
		imageDataEntity->AddTimeSteps( processingDataVector );
		imageDataEntity->GetMetadata( )->SetModality( Core::US );
		dataEntities.push_back(imageDataEntity);
	}

	if( timestamps.size() > 0 )
		imageDataEntity->SetTimeForAllTimeSteps( timestamps );

	// Create the ECG data entity
	ecgData.GetData(cachedECGData);
	if ( cachedECGData.size() )
	{
		blSignalCollective::Pointer signalCollectiveECG = blSignalCollective::New();

		blSignal::Pointer signal = blSignal::BuildSignalFromEcgData( 
			cachedECGData, 
			ecgData.GetStartTime(), 
			ecgData.GetTimeIncrement(), 
			100 );

		signalCollectiveECG->SetNumberOfSignals(1);
		signalCollectiveECG->SetSignal( 0 , signal );

		signalDataEntity = Core::DataEntity::New( SignalTypeId );
		signalDataEntity->AddTimeStep( signalCollectiveECG );
		signalDataEntity->GetMetadata( )->SetName( "ECG" );
		signalDataEntity->SetFather(imageDataEntity);
		dataEntities.push_back(signalDataEntity);
	}

	
	return dataEntities;
}

std::vector<Core::DataEntity::Pointer> 
Core::IO::DICOMFileReader::ImportDICOM( 
	std::string filename )
{
	INPUT_PATH_TYPE inputPath;
	dcmAPI::DataSet::Pointer	dataSet = dcmAPI::DataSet::New( );
	dcmAPI::DataSetReader::Pointer reader = dcmAPI::DataSetReader::New( );
	std::list<dcmAPI::AbstractImageReader::Pointer>::iterator it;
	for ( it = m_ReaderList.begin() ; it != m_ReaderList.end() ; it++ )
	{
		reader->AddReader( *it );
	}
	reader->SetDataSet( dataSet );

	bool isDirectory = itksys::SystemTools::FileIsDirectory( filename.c_str() );
	if ( isDirectory )
	{
		reader->ReadDirectory( filename.c_str() );
		inputPath = INPUT_PATH_DIRECTORY;
	}
	else
	{
		// Try DICOMDIR else try DCM File
		try
		{
			reader->ReadDicomDir( filename.c_str() );
			inputPath = INPUT_PATH_DICOMDIR;
		}
		catch(...)
		{
			reader->ReadDcmFile( filename.c_str() );
			inputPath = INPUT_PATH_DCMFILE;
		}
	}

	bool nonParalelSlices = false;
	std::vector<std::vector<dcmAPI::Slice::Pointer> > totalSlices;

	// Build Data Entity
	Core::DataEntity::Pointer dataEntity = Core::DataEntity::New( );

	// Read data from DataSet
	dcmAPI::PatientIdVectorPtr patiendIdVector = dataSet->GetPatientIds();
	for(unsigned i=0; i < patiendIdVector->size(); i++)
	{
		std::string currentDcmPatientId = patiendIdVector->at(i);
		dcmAPI::Patient::Pointer patientData = dataSet->GetPatient(currentDcmPatientId);

		AddPatientData( patientData, dataEntity );

		//reading studies
		dcmAPI::StudyIdVectorPtr studiesIdVector = patientData->StudyIds();
		for(unsigned i=0; i < studiesIdVector->size(); i++)
		{
			std::string currentDcmStudyId = studiesIdVector->at(i);
			dcmAPI::Study::Pointer studyData = patientData->Study(currentDcmStudyId);
			//reading series
			dcmAPI::SeriesIdVectorPtr seriesIdVector = studyData->SeriesIds();
			for(unsigned i=0; i < seriesIdVector->size(); i++)
			{
				std::string currentDcmSeriesId = seriesIdVector->at(i);
				dcmAPI::Series::Pointer seriesData = studyData->Series(currentDcmSeriesId);

				AddSeriesData( seriesData, dataEntity );

				//reading timepoints
				dcmAPI::TimePointIdVectorPtr timePointIdVector = seriesData->TimePointIds();
				for(unsigned i=0; i < timePointIdVector->size(); i++)
				{
					std::string currentDcmTimepointId = timePointIdVector->at(i);
					dcmAPI::TimePoint::Pointer timePointData = seriesData->TimePoint(currentDcmTimepointId);

					nonParalelSlices = nonParalelSlices || dcmAPI::IOUtils::TimepointContainsNonParallelSlices(timePointData);

					//reading slices
					std::vector< dcmAPI::Slice::Pointer > slices;
					dcmAPI::SliceIdVectorPtr sliceIdVector = timePointData->SliceIds();
					for(unsigned i=0; i < sliceIdVector->size(); i++)
					{
						std::string currentDcmSliceId = sliceIdVector->at(i);
						dcmAPI::Slice::Pointer sliceData = timePointData->Slice(currentDcmSliceId);
						slices.push_back( sliceData );
					}

					totalSlices.push_back( slices );

				}
			}
		}
	}

	// Create DataEntity time steps
	for ( unsigned i = 0 ; i < totalSlices.size() ; i++ )
	{
		std::vector< dcmAPI::Slice::Pointer > slices = totalSlices [ i ];

		itk::ImageIOBase::IOComponentType pixelType;
		std::vector< std::string > sliceFilePaths;
		std::vector< std::string > slicePositions;
		std::vector< std::string > sliceOrientations;
		for ( unsigned j = 0 ; j < slices.size() ; j++ )
		{
			dcmAPI::Slice::Pointer sliceData = slices[ j ];
			pixelType = dcmAPI::ImageUtilities::ParsePixelType( sliceData->GetTagAsText( dcmAPI::tags::PxType ) );
			sliceFilePaths.push_back( sliceData->GetTagAsText( dcmAPI::tags::SliceFilePath ) );
			slicePositions.push_back(sliceData->GetTagAsText(dcmAPI::tags::ImagePositionPatient));
			sliceOrientations.push_back(sliceData->GetTagAsText(dcmAPI::tags::ImageOrientationPatient));
		}

		if ( nonParalelSlices )
		{
			dataEntity->SetType( Core::SliceImageTypeId );
			blSliceImage::Pointer sliceImage = blSliceImage::New();
			switch ( pixelType )
			{
				blVtkTemplateMacro( 
					( sliceImage = dcmAPI::ImageUtilities::ReadMultiSliceImageSliceFromFiles< PixelT >( sliceFilePaths ) ) 
					);
			}

			for (size_t id = 0; id < sliceFilePaths.size(); id++)
			{
				double orientation[6] = {0,0,0,0,0,0};
				double position[3] = {0,0,0};
				dcmAPI::ImageUtilities::StringToDouble(sliceOrientations.at(id).c_str(),orientation, 6);
				dcmAPI::ImageUtilities::StringToDouble(slicePositions.at(id).c_str(),position, 3);
				//fill sliceImage with the orientation and the position
				sliceImage->SetSliceOrientation( orientation, id);
				sliceImage->SetSlicePosition( position, id);
			}

			dataEntity->AddTimeStep( sliceImage );
		}
		else
		{
			dataEntity->SetType( Core::ImageTypeId );
			// Create VTK image
			Core::vtkImageDataPtr vtkImage;
			switch ( pixelType )
			{
				blVtkTemplateMacro( 
					( vtkImage = dcmAPI::ImageUtilities::ReadMultiSliceVtkImageFromFiles< PixelT, 3 >( sliceFilePaths, true ) ) 
					);
			}
			dataEntity->AddTimeStep( vtkImage );
		}

	}


	std::vector<Core::DataEntity::Pointer> dataEntities;
	if ( dataEntity->GetNumberOfTimeSteps() == 0 )
	{
		return dataEntities;
	}

	dataEntities.push_back( dataEntity );

	return dataEntities;
}

void Core::IO::DICOMFileReader::AddPatientData( 
	dcmAPI::Patient::Pointer patientData, 
	Core::DataEntity::Pointer dataEntity )
{
	// Get patient information
	std::string patientName = patientData->GetTagAsText(dcmAPI::tags::PatientName);
	std::string patientBirthDate = patientData->GetTagAsText(dcmAPI::tags::PatientDate);
	std::string patientSex = patientData->GetTagAsText(dcmAPI::tags::PatientSex);

	dataEntity->GetMetadata()->AddTag(
		Core::DataEntityTag::New( Core::DE_TAG_PATIENT_NAME, "Patient name", patientName ) );

	dataEntity->GetMetadata()->AddTag(
		Core::DataEntityTag::New( Core::DE_TAG_PATIENT_SEX, "Patient sex", patientSex ) );

	std::string formatedData = dcmAPI::CreateDateFromRawDcmTagDate( patientBirthDate );
	dataEntity->GetMetadata()->AddTag(
		Core::DataEntityTag::New( Core::DE_TAG_PATIENT_BIRTH_DATE, "Patient birth date", formatedData ) );

}

void Core::IO::DICOMFileReader::AddSeriesData( 
	dcmAPI::Series::Pointer seriesData, 
	Core::DataEntity::Pointer dataEntity )
{

	std::string modality = seriesData->GetTagAsText(dcmAPI::tags::Modality);
	if(modality == "MR")
		dataEntity->GetMetadata()->SetModality(Core::MRI);
	else if(modality == "CT")
		dataEntity->GetMetadata()->SetModality(Core::CT);
	else if(modality == "NM")
		dataEntity->GetMetadata()->SetModality(Core::NM);
	else if(modality == "US")
		dataEntity->GetMetadata()->SetModality(Core::US);
	else if(modality == "XA")
		dataEntity->GetMetadata()->SetModality(Core::XA);
	else
		dataEntity->GetMetadata()->SetModality(Core::UnknownModality);
}

boost::any Core::IO::DICOMFileReader::ReadSingleTimeStep( int iTimeStep, const std::string &filename )
{
	return NULL;
}

::itk::LightObject::Pointer Core::IO::DICOMFileReader::CreateAnother( void ) const
{
	::itk::LightObject::Pointer smartPtr;
	smartPtr = DICOMFileReader::New().GetPointer();

	// Pass the registered readers
	DICOMFileReader* reader = dynamic_cast<DICOMFileReader*> ( smartPtr.GetPointer() );
	std::list<dcmAPI::AbstractImageReader::Pointer>::const_iterator it;
	for ( it = m_ReaderList.begin() ; it != m_ReaderList.end() ; it++ )
	{
		reader->RegisterReader( *it );
	}

	return smartPtr;
}

Core::IO::DICOMFileReader::Pointer Core::IO::DICOMFileReader::New( void )
{
	Pointer smartPtr;
	DICOMFileReader *rawPtr = new DICOMFileReader;
	smartPtr = rawPtr;
	rawPtr->UnRegister();
	return smartPtr;
}

void Core::IO::DICOMFileReader::RegisterReader( dcmAPI::AbstractImageReader::Pointer reader )
{
	m_ReaderList.push_back( reader );
}

void Core::IO::DICOMFileReader::UnRegisterReader( dcmAPI::AbstractImageReader::Pointer reader )
{
	std::list<dcmAPI::AbstractImageReader::Pointer>::iterator it;
	it = std::find( m_ReaderList.begin(), m_ReaderList.end(), reader );
	if ( it != m_ReaderList.end() )
	{
		m_ReaderList.erase( it );
	}
}

std::list<dcmAPI::AbstractImageReader::Pointer> Core::IO::DICOMFileReader::GetRegisteredReaders()
{
	return m_ReaderList;
}
