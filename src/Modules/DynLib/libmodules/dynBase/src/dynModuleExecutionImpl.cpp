/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "dynModuleExecutionImpl.h"
#include <sstream>
#include <stdio.h>

dynModuleExecutionImpl::dynModuleExecutionImpl( )
{
	m_SaveScript = false;
}

dynModuleExecutionImpl::~dynModuleExecutionImpl()
{
}

ModuleDescription* dynModuleExecutionImpl::GetModule() const
{
	return m_Module;
}

void dynModuleExecutionImpl::SetModule( ModuleDescription *val )
{
	m_Module = val;
}

void dynModuleExecutionImpl::Update()
{

}


void dynModuleExecutionImpl::SetAllParameters()
{
	std::vector<ModuleParameterGroup> parameterGroups = m_Module->GetParameterGroups();
	std::vector<ModuleParameterGroup>::iterator itGroup;
	itGroup = m_Module->GetParameterGroups().begin();
	while (itGroup != m_Module->GetParameterGroups().end())
	{
		std::vector<ModuleParameter>::iterator itParam;
		itParam = itGroup->GetParameters().begin();
		while (itParam != itGroup->GetParameters().end())
		{
			SetParameterValue( &(*itParam) );

			++itParam;
		}

		++itGroup;
	}
}

void dynModuleExecutionImpl::GetAllParameters()
{
	std::vector<ModuleParameterGroup>::iterator itGroup;
	itGroup = m_Module->GetParameterGroups().begin();
	while (itGroup != m_Module->GetParameterGroups().end())
	{
		std::vector<ModuleParameter>::iterator itParam;
		itParam = itGroup->GetParameters().begin();
		while (itParam != itGroup->GetParameters().end())
		{
			GetParameterValue( &(*itParam) );
			++itParam;
		}

		++itGroup;
	}
}

std::string dynModuleExecutionImpl::GetWorkingDirectory() const
{
	return m_WorkingDirectory;
}

void dynModuleExecutionImpl::SetWorkingDirectory( std::string val )
{
	m_WorkingDirectory = val;
}

std::string dynModuleExecutionImpl::GetTemporaryDirectory() const
{
	return m_TemporaryDirectory;
}

void dynModuleExecutionImpl::SetTemporaryDirectory( std::string val )
{
	m_TemporaryDirectory = val;
}

void dynModuleExecutionImpl::SetSaveScript( bool val )
{
	m_SaveScript = val;
}

bool dynModuleExecutionImpl::GetSaveScript() const
{
	return m_SaveScript;
}

