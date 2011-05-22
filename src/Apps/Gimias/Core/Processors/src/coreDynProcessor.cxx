/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreDynProcessor.h"
#include "coreDynDataTransferBase.h"
#include "coreDynDataTransferCLP.h"
#include "coreDynDataTransferCLPShared.h"
#include "coreDynDataTransferDLL.h"
#include "dynModuleHelper.h"

Core::DynProcessor::DynProcessor()
{
	m_ModuleExecution = dynModuleExecution::New();

	DynDataTransferBase::RegisterDataTransfer( "CommandLineModule", DynDataTransferBase::Pointer( DynDataTransferCLP::New() ) );
	DynDataTransferBase::RegisterDataTransfer( "SharedObjectModule", DynDataTransferBase::Pointer( DynDataTransferCLPShared::New() ) );
	DynDataTransferBase::RegisterDataTransfer( "RawDynLibModule", DynDataTransferBase::Pointer( DynDataTransferDLL::New() ) );
}

Core::DynProcessor::~DynProcessor()
{
}

void Core::DynProcessor::SetModule( ModuleDescription* module )
{
	m_ModuleExecution->SetModule( module );
	
	SetName( module->GetTitle() );

	// Set inputs, names and optional configuration parameter
	SetNumberOfInputs( dynModuleHelper::GetNumberOfInputs( module ) );
	for ( size_t i = 0 ; i < GetNumberOfInputs() ; i++ )
	{
		ModuleParameter* param;
		param = dynModuleHelper::GetInput( module, i );
		GetInputPort( i )->SetName( param->GetLabel() );

		// Is a optional parameter
		bool isOptional = param->GetIndex() == "" && 
			( param->GetTag() == "image" 
			|| param->GetTag() == "geometry"
			|| param->GetTag() == "transform" );
		GetInputPort( i )->SetOptional( isOptional );
		if ( param->GetTag() == "image" )
		{
			GetInputPort( i )->SetDataEntityType( Core::ImageTypeId );
		}
		else if ( param->GetTag() == "geometry" )
		{
			GetInputPort( i )->SetDataEntityType( Core::SurfaceMeshTypeId );
		}
		else if ( param->GetTag() == "transform" )
		{
			GetInputPort( i )->SetDataEntityType( Core::TransformId );
		}
	}

	// Set outputs
	SetNumberOfOutputs( dynModuleHelper::GetNumberOfOutputs( module ) );
	for ( size_t i = 0 ; i < GetNumberOfOutputs() ; i++ )
	{
		ModuleParameter* param;
		param = dynModuleHelper::GetOutput( module, i );
		bool isOptional = param->GetIndex() == "" && 
			( param->GetTag() == "image" 
			|| param->GetTag() == "geometry"
			|| param->GetTag() == "transform" );
		GetOutputPort( i )->SetName( param->GetName() );
		GetOutputPort( i )->SetDataEntityName( param->GetLabel() );
		GetOutputPort( i )->SetOptional( isOptional );
		if ( param->GetTag() == "image" )
		{
			GetOutputPort( i )->SetDataEntityType( Core::ImageTypeId );
		}
		else if ( param->GetTag() == "geometry" )
		{
			GetOutputPort( i )->SetDataEntityType( Core::SurfaceMeshTypeId );
		}
		else if ( param->GetTag() == "transform" )
		{
			GetOutputPort( i )->SetDataEntityType( Core::TransformId );
		}
	}
}

ModuleDescription * Core::DynProcessor::GetModule()
{
	return m_ModuleExecution->GetModule();
}

void Core::DynProcessor::Update()
{
	DynDataTransferBase::Pointer dataTransfer;
	dataTransfer = DynDataTransferBase::NewDataTransfer( m_ModuleExecution->GetModuleType() );
	dataTransfer->SetDynProcessor( this );
	dataTransfer->SetTemporaryDirectory( m_TemporaryDirectory );
	dataTransfer->UpdateAllModuleParameters();

	try
	{
		GetModuleExecution()->Update();
		dataTransfer->UpdateAllOutputs();
	}
	catch ( ... )
	{
		dataTransfer->CleanTemporaryFiles( );
		GetModuleExecution()->Clean();
		throw;
	}

	dataTransfer->CleanTemporaryFiles( );
	GetModuleExecution()->Clean();
}

dynModuleExecution::Pointer Core::DynProcessor::GetModuleExecution() const
{
	return m_ModuleExecution;
}

std::string Core::DynProcessor::GetTemporaryDirectory() const
{
	return m_TemporaryDirectory;
}

void Core::DynProcessor::SetTemporaryDirectory( std::string val )
{
	m_TemporaryDirectory = val;
}

void Core::DynProcessor::SetWorkingDirectory( std::string val )
{
	m_ModuleExecution->SetWorkingDirectory( val );
}
