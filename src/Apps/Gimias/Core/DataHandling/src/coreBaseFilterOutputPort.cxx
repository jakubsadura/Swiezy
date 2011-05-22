/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreBaseFilterOutputPort.h"

Core::BaseFilterOutputPort::BaseFilterOutputPort()
{
	m_ReuseOutput = true;
	m_DataEntityName = "";
	m_TotalTimeSteps = 1;
}

Core::BaseFilterOutputPort::~BaseFilterOutputPort()
{

}

bool Core::BaseFilterOutputPort::CheckDataEntityRestrictions( Core::DataEntity::Pointer val )
{
	return true;
}

bool Core::BaseFilterOutputPort::GetReuseOutput() const
{
	return m_ReuseOutput;
}

void Core::BaseFilterOutputPort::SetReuseOutput( bool val )
{
	m_ReuseOutput = val;
}

std::string Core::BaseFilterOutputPort::GetDataEntityName() const
{
	return m_DataEntityName;
}

void Core::BaseFilterOutputPort::SetDataEntityName( const std::string &val )
{
	m_DataEntityName = val;
}

int Core::BaseFilterOutputPort::GetTotalTimeSteps() const
{
	return m_TotalTimeSteps;
}

void Core::BaseFilterOutputPort::SetTotalTimeSteps( int val )
{
	m_TotalTimeSteps = val;
}


void Core::BaseFilterOutputPort::UpdateOutput( 
	boost::any any, 
	int timeStepNumber,
	Core::DataEntity::Pointer fatherDataEntity )
{
	// Get old data entity
	Core::DataEntity::Pointer dataEntityOutput;
	dataEntityOutput = GetDataEntityHolder( )->GetSubject( );

	// Create a new data entity if needed
	if ( !GetReuseOutput() || 
		dataEntityOutput.IsNull() || 
		dataEntityOutput->GetNumberOfTimeSteps( ) != unsigned( m_TotalTimeSteps ) ||
		( dataEntityOutput->GetFather( ).IsNotNull( ) && 
		  fatherDataEntity.IsNotNull( ) &&
		  fatherDataEntity->GetId( ) != dataEntityOutput->GetFather( )->GetId( ) && 
		  fatherDataEntity->GetId( ) != dataEntityOutput->GetId( ) ) )
	{
		dataEntityOutput = Core::DataEntity::New( GetDataEntityType() );
		dataEntityOutput->AddTimeStep( any );
		dataEntityOutput->GetMetadata()->SetName( m_DataEntityName );
	}
	else
	{
		// Copy the time step
		Core::DataEntityImpl::Pointer timeStep;
		timeStep = dataEntityOutput->GetTimeStep( timeStepNumber );
		timeStep->DeepCopy( any );
	}

	// Set father
	if ( dataEntityOutput->GetFather( ).IsNull( ) && 
		 fatherDataEntity.IsNotNull() &&
		 dataEntityOutput->GetId() != fatherDataEntity->GetId() )
	{
		dataEntityOutput->SetFather( fatherDataEntity );
	}

	// Update the output subject
	GetDataEntityHolder( )->SetSubject( dataEntityOutput, true );
}
