/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreWorkflowStep.h"

Core::WorkflowStep::WorkflowStep( const char* name )
{
	m_Name = name;
	m_Properties = blTagMap::New();
}

std::string Core::WorkflowStep::GetName() const
{
	return m_Name;
}

void Core::WorkflowStep::SetName( std::string val )
{
	m_Name = val;
}

Core::WorkflowStep::SubStepListType& Core::WorkflowStep::GetSubStepVector()
{
	return m_SubStepVector;
}

Core::WorkflowSubStep::Pointer Core::WorkflowStep::Get( int currentStep )
{
	bool found = false;
	int count = 0;
	SubStepListType::iterator it = m_SubStepVector.begin();
	while ( !found && it != m_SubStepVector.end() )
	{
		if ( count == currentStep )
		{
			found = true;
		}
		else 
		{
			it++;
			count++;
		}
	}

	if ( found )
	{
		return (*it);
	}

	return NULL;
}

void Core::WorkflowStep::AddSubStep( Core::WorkflowSubStep::Pointer subStep )
{
	m_SubStepVector.push_back( subStep );
}

void Core::WorkflowStep::AddWindow( std::string factoryName )
{
	m_WindowList.push_back( factoryName );
}

Core::WorkflowStep::WindowListType& Core::WorkflowStep::GetWindowList()
{
	return m_WindowList;
}

blTagMap::Pointer Core::WorkflowStep::GetProperties() const
{
	return m_Properties;
}

