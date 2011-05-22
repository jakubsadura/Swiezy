/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "dynModuleExecution.h"
#include "dynModuleExecutionDLL.h"
#include "dynModuleExecutionCLPShared.h"
#include "dynModuleExecutionCLPExe.h"
#include "ModuleDescriptionUtilities.h"
#include <sstream>
#include <stdio.h>

dynModuleExecution::FactoryMap dynModuleExecution::m_Factories;

dynModuleExecution::dynModuleExecution( )
{
	m_ForceExecutionMode = "UnknownModule";
	m_SaveScript = false;
	m_Module = NULL;

	RegisterImpl( "CommandLineModule", dynModuleExecutionCLPExe::Factory::New( ) );
	RegisterImpl( "SharedObjectModule", dynModuleExecutionCLPShared::Factory::New( ) );
	RegisterImpl( "RawDynLibModule", dynModuleExecutionDLL::Factory::New( ) );
}

dynModuleExecution::~dynModuleExecution()
{
}

ModuleDescription* dynModuleExecution::GetModule() const
{
	return m_Module;
}

void dynModuleExecution::SetModule( ModuleDescription *val )
{
	m_Module = val;
	m_ModuleType = GetModuleType( val );
}

void dynModuleExecution::Update()
{

	FactoryMap::iterator it = m_Factories.find( GetModuleType() );
	if ( it == m_Factories.end() )
	{
		return;
	}

	m_Impl = it->second->NewModuleExecution( );
	m_Impl->SetSaveScript(m_SaveScript);
	m_Impl->SetWorkingDirectory( GetWorkingDirectory() );
	m_Impl->SetTemporaryDirectory( GetWorkingDirectory( ) );
	m_Impl->SetModule( m_Module );
	m_Impl->Update();
}


std::string dynModuleExecution::GetModuleType() const
{
	if ( GetForceExecutionMode( ) != "UnknownModule" )
	{
		return GetForceExecutionMode();
	}

	return m_ModuleType;
}

std::string dynModuleExecution::GetModuleType( ModuleDescription *module )
{
	std::string moduleType;

	if ( module->GetType() != "UnknownModule" )
	{
		moduleType = module->GetType();
	}
	else if ( module->GetAlternativeType() != "UnknownModule" )
	{
		moduleType = module->GetAlternativeType();
	}
	else
	{
		moduleType = "UnknownModule";
	}

	return moduleType;
}

std::string dynModuleExecution::GetWorkingDirectory() const
{
	return m_WorkingDirectory;
}

void dynModuleExecution::SetWorkingDirectory( std::string val )
{
	m_WorkingDirectory = val;
}

std::string dynModuleExecution::GetForceExecutionMode() const
{
	return m_ForceExecutionMode;
}

void dynModuleExecution::SetForceExecutionMode( const std::string &val )
{
	m_ForceExecutionMode = val;
}

void dynModuleExecution::SetSaveScript( bool val )
{
	m_SaveScript = val;
}

bool dynModuleExecution::GetSaveScript()
{
	return m_SaveScript;
}

void dynModuleExecution::Clean()
{
	m_Impl = NULL;
}

void dynModuleExecution::RegisterImpl( 
	const std::string &name, 
	dynModuleExecutionImpl::Factory::Pointer factory )
{
	FactoryMap::iterator it = m_Factories.find( name );
	if ( it == m_Factories.end() )
	{
		m_Factories[ name ] = factory;
	}
}

void dynModuleExecution::UnRegisterImpl( const std::string &name )
{
	FactoryMap::iterator it = m_Factories.find( name );
	if ( it == m_Factories.end() )
	{
		return;
	}

	m_Factories.erase( it );
}
