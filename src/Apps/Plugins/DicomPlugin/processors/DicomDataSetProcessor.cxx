/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "DicomDataSetProcessor.h"

#include <string>
#include <iostream>

#include "coreReportExceptionMacros.h"
#include "coreDataEntity.h"
#include "coreDataEntityHelper.h"
#include "coreDataEntityHelper.txx"
#include "coreKernel.h"

#include "dcmDataSet.h"
#include "dcmDataSetReader.h"
#include "coreDataEntityReader.h"
#include "coreDICOMFileReader.h"


DicomPlugin::DataSetProcessor::DataSetProcessor( )
{
	BaseProcessor::SetNumberOfInputs( 0 );
	BaseProcessor::SetNumberOfOutputs( 0 );

	m_DataSetHolder = DataSetHolderType::New( );
	m_DataSetHolder->SetSubject( dcmAPI::DataSet::New( ) );

	m_EnableObservers = true;
}

DicomPlugin::DataSetProcessor::~DataSetProcessor()
{
}

DicomPlugin::DataSetHolderType::Pointer 
DicomPlugin::DataSetProcessor::GetDataSetHolder() const
{
	return m_DataSetHolder;
}

dcmAPI::DataSet::Pointer 
DicomPlugin::DataSetProcessor::GetDataSet() const
{
	return m_DataSetHolder->GetSubject( );
}

void DicomPlugin::DataSetProcessor::SetPath( const std::string& path )
{
	m_Path = path;
}

void DicomPlugin::DataSetProcessor::Update()
{
	InternalUpdate();

	if ( m_EnableObservers )
	{
		GetDataSetHolder( )->NotifyObservers( );
	}
}

DicomPlugin::INPUT_PATH_TYPE DicomPlugin::DataSetProcessor::GetInputPathType() const
{
	return m_InputPathType;
}

std::string DicomPlugin::DataSetProcessor::GetPath() const
{
	return m_Path;
}

bool DicomPlugin::DataSetProcessor::SearchTag( dcmAPI::TagId tag )
{
	if ( m_DataSetHolder->GetSubject() == NULL )
	{
		return false;
	}

	bool found = false;
	dcmAPI::PatientIdVectorPtr patiendIdVector = GetDataSet( )->GetPatientIds();
	for(unsigned i=0; i < patiendIdVector->size(); i++)
	{
		dcmAPI::Patient::Pointer patient = GetDataSet( )->GetPatient( patiendIdVector->at(i) );

		if ( SearchTag( tag, patient->GetTagsMap( ) ) )
		{
			found = true;
			return found;
		}

		dcmAPI::StudyIdVectorPtr studiesIdVector = patient->StudyIds();
		for(unsigned i=0; i < studiesIdVector->size(); i++)
		{
			dcmAPI::Study::Pointer study = patient->Study(studiesIdVector->at(i));

			if ( SearchTag( tag, study->GetTagsMap( ) ) )
			{
				found = true;
				return found;
			}

			dcmAPI::SeriesIdVectorPtr seriesIdVector = study->SeriesIds();
			for(unsigned i=0; i < seriesIdVector->size(); i++)
			{
				dcmAPI::Series::Pointer series = study->Series(seriesIdVector->at(i));

				if ( SearchTag( tag, series->GetTagsMap( ) ) )
				{
					found = true;
					return found;
				}

				dcmAPI::TimePointIdVectorPtr timePointIdVector = series->TimePointIds();
				for(unsigned i=0; i < timePointIdVector->size(); i++)
				{
					dcmAPI::TimePoint::Pointer timePoint = series->TimePoint(timePointIdVector->at(i));

					if ( SearchTag( tag, timePoint->GetTagsMap( ) ) )
					{
						found = true;
						return found;
					}

					dcmAPI::SliceIdVectorPtr sliceIdVector = timePoint->SliceIds();
					for(unsigned i=0; i < sliceIdVector->size(); i++)
					{
						dcmAPI::Slice::Pointer slice = timePoint->Slice( sliceIdVector->at(i) );

						if ( SearchTag( tag, slice->GetTagsMap( ) ) )
						{
							found = true;
							return found;
						}
					}
				}
			}
		}
	}

	return found;
}

bool DicomPlugin::DataSetProcessor::SearchTag( dcmAPI::TagId tag, dcmAPI::TagsMap *tagMap )
{
	bool found = false;
	if ( tagMap->find( tag ) != tagMap->end() )
	{
		found = true;
	}

	return found;
}

void DicomPlugin::DataSetProcessor::SetTimeTag( const dcmAPI::TagId& timeTag )
{
	m_TimeTag = timeTag;
}

void DicomPlugin::DataSetProcessor::InternalUpdate()
{
	dcmAPI::DataSetReader::Pointer reader = dcmAPI::DataSetReader::New( );

	// Register readers
	Core::IO::BaseDataEntityReader::Pointer baseReader;
	baseReader = Core::IO::DataEntityReader::GetRegisteredReader( "DICOMFileReader" );
	if ( baseReader.IsNotNull() )
	{
		Core::IO::DICOMFileReader::Pointer dICOMReader;
		dICOMReader = dynamic_cast<Core::IO::DICOMFileReader*>( baseReader.GetPointer() );
		std::list<dcmAPI::AbstractImageReader::Pointer> readers;
		readers = dICOMReader->GetRegisteredReaders();
		std::list<dcmAPI::AbstractImageReader::Pointer>::iterator it;
		for ( it = readers.begin() ; it != readers.end() ; it++ )
		{
			reader->AddReader( *it );
		}
	}

	reader->SetDataSet( GetDataSet( ) );
	if ( m_TimeTag.IsSet() )
	{
		reader->SetTimeTag( m_TimeTag );
		if ( m_TimeTag == dcmAPI::tags::SOPInstanceUID )
		{
			reader->SetTimeTagTruncate('.', 1);
		}
		else if ( m_TimeTag == dcmAPI::tags::PhaseNumberMR )
		{
			reader->SetTimeTagLocation(true);
		}
		else if ( m_TimeTag == dcmAPI::tags::AcquisitionTime )
		{
			reader->SetTimeTagLocation(true);
		}
	}

	bool isDirectory = itksys::SystemTools::FileIsDirectory( m_Path.c_str() );
	if ( isDirectory )
	{
		reader->ReadDirectory( m_Path.c_str() );
		m_InputPathType = INPUT_PATH_DIRECTORY;
	}
	else
	{
		// Try DICOMDIR else try DCM File
		try
		{
			reader->ReadDicomDir( m_Path.c_str() );
			m_InputPathType = INPUT_PATH_DICOMDIR;
		}
		catch(...)
		{
			reader->ReadDcmFile( m_Path.c_str() );
			m_InputPathType = INPUT_PATH_DCMFILE;
		}
	}

}

void DicomPlugin::DataSetProcessor::EnableObservers( bool enable )
{
	m_EnableObservers = enable;
}
