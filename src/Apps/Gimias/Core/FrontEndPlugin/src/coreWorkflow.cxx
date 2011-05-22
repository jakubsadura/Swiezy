/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreWorkflow.h"

Core::Workflow::Workflow( const char* name )
{
	m_Name = name;
	m_Version = "0.0.0";
}

std::string Core::Workflow::GetName() const
{
	return m_Name;
}

void Core::Workflow::SetName( std::string val )
{
	m_Name = val;
}

Core::Workflow::StepListType& Core::Workflow::GetStepVector()
{
	return m_StepVector;
}

void Core::Workflow::AddStep( WorkflowStep::Pointer step )
{
	m_StepVector.push_back( step );
}

Core::Workflow::PluginNamesListType Core::Workflow::GetPluginNamesList() const
{
	return m_PluginNamesList;
}

void Core::Workflow::SetPluginNamesList( PluginNamesListType val )
{
	m_PluginNamesList = val;
}

void Core::Workflow::Clear()
{
	m_Name = "";
	m_StepVector.clear();
	m_PluginNamesList.clear();
}

Core::WorkflowStep::Pointer Core::Workflow::GetStep( const std::string &name )
{
	StepListType::iterator it;
	for ( it  = m_StepVector.begin() ; it != m_StepVector.end() ; it++ )
	{
		if ( (*it)->GetName() == name )
		{
			return (*it);
		}
	}

	return NULL;
}

std::string Core::Workflow::GetVersion() const
{
	return m_Version;
}

void Core::Workflow::SetVersion( std::string val )
{
	m_Version = val;
}

std::string Core::Workflow::GetFullName()
{
	return GetName( ) + " (" + GetVersion( ) + ")";
}
