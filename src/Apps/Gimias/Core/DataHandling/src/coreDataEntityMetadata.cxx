/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreDataEntityMetadata.h"


Core::DataEntityMetadata::DataEntityMetadata( unsigned int dataEntityId )
{
	m_DataEntityId = dataEntityId;

	AddTag( DataEntityTag::New( DE_TAG_MODALITY, "Modality", int( Core::UnknownModality ) ) );
	AddTag( DataEntityTag::New( DE_TAG_NAME, "Name", std::string("") ) );

	blTagMap::Pointer patientData = blTagMap::New( );
	patientData->AddTag( DataEntityTag::New( DE_TAG_PATIENT_NAME, "Patient name", std::string("") ) );
	patientData->AddTag( DataEntityTag::New( DE_TAG_PATIENT_SEX, "Patient sex", std::string("") ) );
	patientData->AddTag( DataEntityTag::New( DE_TAG_PATIENT_BIRTH_DATE, "Patient birth date", std::string("") ) );
	AddTag( "Patient", patientData );

	AddTag( DataEntityTag::New( DE_TAG_ED, "ED flag", std::string("") ) );
	AddTag( DataEntityTag::New( DE_TAG_ES, "ES flag", std::string("") ) );
}

Core::DataEntityMetadata::~DataEntityMetadata(void)
{
}

void Core::DataEntityMetadata::SetName( const std::string& name )
{
	AddTag( DataEntityTag::New( DE_TAG_NAME, "Name", name ) );
}

std::string Core::DataEntityMetadata::GetName()
{
	std::string name;
	DataEntityTag::Pointer tag = GetTag( DE_TAG_NAME );
	if ( tag.IsNotNull( ) )
	{
		tag->GetValue<std::string>(name);
	}
	return name;
}

void Core::DataEntityMetadata::SetModality( ModalityType modality )
{
	AddTag( DataEntityTag::New( DE_TAG_MODALITY, "Modality", int( modality ) ) );
}

Core::ModalityType Core::DataEntityMetadata::GetModality()
{
	Core::ModalityType modality = UnknownModality;
	DataEntityTag::Pointer tag = GetTag( DE_TAG_MODALITY );
	if ( tag.IsNotNull( ) )
	{
		int temp;
		tag->GetValue<int>(temp);
		modality = Core::ModalityType( temp );
	}
	return modality;
}

unsigned int Core::DataEntityMetadata::GetDataEntityId() const
{
	return m_DataEntityId;
}

std::string Core::DataEntityMetadata::GetEDTimeStep() 
{
	std::string  ts;
	DataEntityTag::Pointer tag = GetTag( DE_TAG_ED );
	if ( tag.IsNotNull( ) )
	{
		tag->GetValue<std::string>(ts);
	}
	return ts;
}


void Core::DataEntityMetadata::SetEDTimeStep(size_t timeStep) 
{
	char edTimeStep[10] = "";
	if ( timeStep != -1 )
	{
		sprintf (edTimeStep, "%i", timeStep);
	}
	AddTag( DataEntityTag::New( DE_TAG_ED, "ED flag", std::string( edTimeStep ) ) );
}


std::string Core::DataEntityMetadata::GetESTimeStep() 
{
	std::string ts;
	DataEntityTag::Pointer tag = GetTag( DE_TAG_ES );
	if ( tag.IsNotNull( ) )
	{
		tag->GetValue<std::string>(ts);
	}
	return ts;
}


void Core::DataEntityMetadata::SetESTimeStep(size_t timeStep) 
{

	char esTimeStep[10] = "";
	if ( timeStep != -1 )
	{
		sprintf (esTimeStep, "%i", timeStep);
	}
	AddTag( DataEntityTag::New( DE_TAG_ES, "ES flag", std::string( esTimeStep ) ) );
}
