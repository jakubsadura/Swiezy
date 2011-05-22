/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreDynDataTransferBase.h"
#include "dynModuleHelper.h"

Core::DynDataTransferBase::DataTransferMapType Core::DynDataTransferBase::m_DataTransferMap;

Core::DynDataTransferBase::DynDataTransferBase()
{
	m_TemporaryDirectory = "";
}

Core::DynDataTransferBase::~DynDataTransferBase()
{
}


void Core::DynDataTransferBase::UpdateModuleParameter( 
	ModuleParameter* param, 
	int num )
{

}

void Core::DynDataTransferBase::UpdateProcessorOutput( ModuleParameter* param, int num )
{

}

void Core::DynDataTransferBase::CleanTemporaryFiles()
{
}

Core::DynProcessor::Pointer Core::DynDataTransferBase::GetDynProcessor() const
{
	return m_DynProcessor;
}

void Core::DynDataTransferBase::SetDynProcessor( DynProcessor::Pointer val )
{
	m_DynProcessor = val;
}

void Core::DynDataTransferBase::UpdateAllModuleParameters()
{
	for ( size_t i = 0 ; i < m_DynProcessor->GetNumberOfInputs() ; i++ )
	{
		ModuleParameter* param = dynModuleHelper::GetInput( m_DynProcessor->GetModule(), i );
		bool isOptional = m_DynProcessor->GetInputPort( i )->GetOptional();
		bool isActive = m_DynProcessor->GetInputPort( i )->GetActive();
		if ( isOptional && !isActive )
		{
			param->SetDefault( "" );
		}
		else
		{
			UpdateModuleParameter( param, i );
		}
	}

	for ( size_t i = 0 ; i < m_DynProcessor->GetNumberOfOutputs() ; i++ )
	{
		ModuleParameter* param = dynModuleHelper::GetOutput( m_DynProcessor->GetModule( ), i );
		bool isOptional = m_DynProcessor->GetOutputPort( i )->GetOptional();
		// Outputs have the checkbox set to true or false indicating if are active or not
		if ( isOptional && param->GetDefault() == "false" )
		{
			param->SetDefault( "" );
		}
		else
		{
			UpdateModuleParameter( param, i );
		}
	}

}

void Core::DynDataTransferBase::UpdateAllOutputs()
{
	if ( m_DynProcessor->GetModuleExecution()->GetSaveScript( ) )
	{
		return;
	}

	for ( size_t i = 0 ; i < m_DynProcessor->GetNumberOfOutputs() ; i++ )
	{
		ModuleParameter* param = dynModuleHelper::GetOutput( m_DynProcessor->GetModule(), i );
		bool isOptional = m_DynProcessor->GetOutputPort( i )->GetOptional();
		if ( isOptional && param->GetDefault() == "" )
		{
		}
		else
		{
			UpdateProcessorOutput( param, i );
		}
	}

}

std::string Core::DynDataTransferBase::GetTemporaryDirectory() const
{
	return m_TemporaryDirectory;
}

void Core::DynDataTransferBase::SetTemporaryDirectory( std::string val )
{
	m_TemporaryDirectory = val;
}

Core::DataEntity::Pointer Core::DynDataTransferBase::FindInputReferenceData( 
	ModuleParameter* param )
{
	// Find input parent data
	Core::DataEntity::Pointer parentData;
	int inputIndex = 0;
	if ( !param->GetReference().empty() )
	{
		for ( size_t i = 0 ; i < m_DynProcessor->GetNumberOfInputs() ; i++ )
		{
			ModuleParameter* inputParam;
			inputParam = dynModuleHelper::GetInput( m_DynProcessor->GetModule( ), i );
			if ( inputParam->GetName() == param->GetReference() )
			{
				inputIndex = i;
			}
		}
	}
	if ( m_DynProcessor->GetNumberOfInputs() > 0 )
	{
		parentData = m_DynProcessor->GetInputDataEntity( inputIndex );
	}

	return parentData;

}

void Core::DynDataTransferBase::RegisterDataTransfer( const std::string &name, DynDataTransferBase::Pointer p )
{
	DataTransferMapType::iterator it = m_DataTransferMap.find( name );
	if ( it == m_DataTransferMap.end() )
	{
		m_DataTransferMap[ name ] = p;
	}
}

void Core::DynDataTransferBase::UnRegisterDataTransfer( const std::string &name )
{
	DataTransferMapType::iterator it = m_DataTransferMap.find( name );
	if ( it == m_DataTransferMap.end() )
	{
		return;
	}

	m_DataTransferMap.erase( it );
}

Core::DynDataTransferBase::Pointer Core::DynDataTransferBase::NewDataTransfer( const std::string &name )
{
	DataTransferMapType::iterator it = m_DataTransferMap.find( name );
	if ( it == m_DataTransferMap.end() )
	{
		throw Core::Exceptions::Exception( "DynDataTransferBase", "Data Trasnfer not registered for this name" );
	}

	Core::DynDataTransferBase::Pointer newInstance;
	newInstance = static_cast<DynDataTransferBase*>( it->second->CreateAnother().GetPointer() );
	return newInstance;
}
