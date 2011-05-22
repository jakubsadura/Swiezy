/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "dcmPACSQueryFileReader.h"

#include "dcmTypes.h"

using namespace dcmAPI;

PACSQueryFileReader::PACSQueryFileReader()
{
	m_DataSet = dcmAPI::DataSet::New( );
}

PACSQueryFileReader::~PACSQueryFileReader()
{
}

void PACSQueryFileReader::Update()
{
	// Open query file
	std::ifstream queryFile;
	queryFile.open ( m_Path.c_str() );
	if ( queryFile.bad() )
	{
		return;
	}

	// For each patient -> Add to the vector
	std::string line;

	// Set info to dcmData
	m_DataSet->Clear();

	bool fileisOk = true;
	while ( fileisOk )
	{
		// Collect all TAGS for each query result
		dcmAPI::TagsMap tagMap;
		for ( unsigned i = 0 ; i < m_Query.size() ; i++ )
		{
			fileisOk = std::getline(queryFile,line);
			dcmAPI::Tag dcmTAG( line );
			tagMap[ dcmTAG.m_tagId ] = dcmTAG.m_value;
		}


		if ( fileisOk )
		{

			// Find if the patient already exists
			bool bNewPatient=false;
			dcmAPI::Patient::Pointer patient;
			patient = m_DataSet->GetPatient( tagMap[ dcmAPI::tags::PatientRealID ] );
			
			if(patient==NULL)
			{
				patient = dcmAPI::Patient::New();
				patient->AddTag(Tag(tags::PatientId,tagMap[ dcmAPI::tags::PatientRealID ]));
				patient->AddTag( dcmAPI::Tag( dcmAPI::tags::PatientRealID, tagMap[ dcmAPI::tags::PatientRealID ] ) );
				patient->AddTag( dcmAPI::Tag( dcmAPI::tags::PatientName, tagMap[ dcmAPI::tags::PatientName ] ) );
				patient->AddTag( dcmAPI::Tag( dcmAPI::tags::PatientDate, tagMap[ dcmAPI::tags::PatientDate ] ) );
				bNewPatient=true;
			}
			
			if ( tagMap[ dcmAPI::tags::PatientRealID ].empty() )
			{
				continue;
			}


			// Fill study data
			dcmAPI::Study::Pointer study;
			if ( !tagMap[ dcmAPI::tags::StudyInstanceUID ].empty() )
			{
				study = patient->Study( tagMap[ dcmAPI::tags::StudyInstanceUID ] );
				if(study==NULL)
				{
					study=dcmAPI::Study::New();
					study->AddTag( dcmAPI::Tag( dcmAPI::tags::StudyInstanceUID, tagMap[ dcmAPI::tags::StudyInstanceUID ] ) );
					study->AddTag( dcmAPI::Tag( dcmAPI::tags::StudyRealID, tagMap[ dcmAPI::tags::StudyRealID ] ) );
					study->AddTag( dcmAPI::Tag( dcmAPI::tags::StudyId, tagMap[ dcmAPI::tags::StudyInstanceUID ] ) );
					study->AddTag( dcmAPI::Tag( dcmAPI::tags::StudyDate, tagMap[ dcmAPI::tags::StudyDate ] ) );
					study->AddTag( dcmAPI::Tag( dcmAPI::tags::StudyDescription, tagMap[ dcmAPI::tags::StudyDescription ] ) );
					patient->AddStudy( study );
				}
			}

			// Fill series data
			if ( !tagMap[ dcmAPI::tags::SeriesInstanceUID ].empty() )
			{
				dcmAPI::Series::Pointer series = study->Series( tagMap[ dcmAPI::tags::SeriesInstanceUID ] );
				if(series==NULL)
				{
					series=dcmAPI::Series::New();
					series->AddTag( dcmAPI::Tag( dcmAPI::tags::SeriesInstanceUID, tagMap[ dcmAPI::tags::SeriesInstanceUID ] ) );
					series->AddTag( dcmAPI::Tag( dcmAPI::tags::SeriesId, tagMap[ dcmAPI::tags::SeriesInstanceUID ] ) );
					series->AddTag( dcmAPI::Tag( dcmAPI::tags::SeriesNumber, tagMap[ dcmAPI::tags::SeriesNumber ] ) );
					series->AddTag( dcmAPI::Tag( dcmAPI::tags::SeriesDescription, tagMap[ dcmAPI::tags::SeriesDescription ] ) );
					series->AddTag( dcmAPI::Tag( dcmAPI::tags::Modality, tagMap[ dcmAPI::tags::Modality ] ) );
					study->AddSeries( series );
				}
			}

			if(bNewPatient)
				m_DataSet->AddPatient( patient );
		}
	}

	// Close file
	queryFile.close();
}

dcmAPI::DataSet::Pointer PACSQueryFileReader::GetDataSet() const
{
	return m_DataSet;
}

void PACSQueryFileReader::SetDataSet( dcmAPI::DataSet::Pointer val )
{
	m_DataSet = val;
}

std::string dcmAPI::PACSQueryFileReader::GetPath() const
{
	return m_Path;
}

void dcmAPI::PACSQueryFileReader::SetPath( std::string val )
{
	m_Path = val;
}

std::vector< dcmAPI::Tag > dcmAPI::PACSQueryFileReader::GetQuery() const
{
	return m_Query;
}

void dcmAPI::PACSQueryFileReader::SetQuery( 
	const std::vector< dcmAPI::Tag > &val )
{
	m_Query = val;
}
