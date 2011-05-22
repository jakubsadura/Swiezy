/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreDynDataTransferCLPShared.h"


Core::DynDataTransferCLPShared::DynDataTransferCLPShared()
{
}

Core::DynDataTransferCLPShared::~DynDataTransferCLPShared()
{
}


void Core::DynDataTransferCLPShared::UpdateModuleParameter( 
	ModuleParameter* param, 
	int num )
{
	// Share data entity pointer with images and use itk::DataEntityIO
	// to get and set the memory pointer
	if ( m_DynProcessor->GetModuleExecution()->GetModuleType( ) == "SharedObjectModule"
		 && param->GetTag() == "image" 
		 && param->GetFileExtensions().size( ) == 0 )
	{
		if ( param->GetChannel() == "input" )
		{
			std::ostringstream fnameString;
			fnameString << "DataEntity:"
				<< m_DynProcessor->GetInputPort( num )->GetDataEntity().GetPointer();
			param->SetDefault( fnameString.str() );
		}
		else if ( param->GetChannel() == "output" )
		{
			Core::DataEntity::Pointer dataEntity;
			dataEntity = Core::DataEntity::New( ImageTypeId );
			dataEntity->GetMetadata()->SetName( param->GetName() );
			m_OutputDataEntities[ num ] = dataEntity;
			std::ostringstream fnameString;
			fnameString << "DataEntity:" << dataEntity.GetPointer();
			param->SetDefault( fnameString.str() );
		}
	}
	else
	{
		DynDataTransferCLP::UpdateModuleParameter( param, num );
	}
}

void Core::DynDataTransferCLPShared::UpdateProcessorOutput( ModuleParameter* param, int num )
{
	if ( param->GetTag() == "image" && 
		 m_DynProcessor->GetModuleExecution()->GetModuleType( ) == "SharedObjectModule" )
	{
		Core::DataEntity::Pointer parentData = FindInputReferenceData( param );

		m_DynProcessor->UpdateOutput( num, 
			m_OutputDataEntities[ num ]->GetProcessingData( ), 
			param->GetName(), true, 1, parentData );
	}
	else
	{
		DynDataTransferCLP::UpdateProcessorOutput( param, num );
	}

}

void Core::DynDataTransferCLPShared::CleanTemporaryFiles()
{
	DynDataTransferCLP::CleanTemporaryFiles();

	m_OutputDataEntities.clear();
}

