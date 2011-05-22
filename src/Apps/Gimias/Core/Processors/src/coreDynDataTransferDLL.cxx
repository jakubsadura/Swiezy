/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreDynDataTransferDLL.h"
#include "coreDataEntityWriter.h"
#include "dynModuleHelper.h"

Core::DynDataTransferDLL::DynDataTransferDLL()
{
}

Core::DynDataTransferDLL::~DynDataTransferDLL()
{
}


void Core::DynDataTransferDLL::UpdateModuleParameter( 
	ModuleParameter* param, int num )
{
	if ( param->GetChannel() != "input" )
	{
		return;
	}

	Core::DataEntity::Pointer dataEntity;
	dataEntity = m_DynProcessor->GetInputPort( num )->GetDataEntityHolder( )->GetSubject();
	if ( dataEntity.IsNull( ) )
	{
		throw Core::Exceptions::Exception("DynDataTransferDLL::UpdateModuleParameter", 
			"You must select an input data from the Processing Browser and set it as input" 
			);
	}

	DataEntityImplFactory::Pointer factory;
	factory = DataEntityImplFactory::FindFactory( param->GetDataType() );
	if ( factory.IsNull() )
	{
		throw Core::Exceptions::Exception("DynDataTransferDLL::UpdateModuleParameter", 
			"Is not possible to convert selected input data to the input data type" 
			);
	}

	bool success = dataEntity->SwitchImplementation( factory->GetDataType( ) );
	if ( !success )
	{
		throw Core::Exceptions::Exception("DynDataTransferDLL::UpdateModuleParameter", 
			"Is not possible to convert selected input data to the input data type" 
			);
	}

	void* ptr = NULL;
	if ( factory->GetProperties()->FindTagByName( "single" ) )
	{
		ptr = dataEntity->GetTimeStep( 0 )->GetVoidPtr( );
	}
	else if ( factory->GetProperties()->FindTagByName( "multiple" ) )
	{
		ptr = dataEntity->GetTimeStep( -1 )->GetVoidPtr( );
	}
	param->SetDefault( dynModuleHelper::PointerToString( ptr ) );

}

void Core::DynDataTransferDLL::UpdateProcessorOutput( ModuleParameter* param, int num )
{
	void *ptr = dynModuleHelper::StringToPointer( param->GetDefault( ) );

	Core::DataEntity::Pointer parentData = FindInputReferenceData( param );

	DataEntityImplFactory::Pointer factory;
	factory = DataEntityImplFactory::FindFactory( param->GetDataType() );
	if ( factory.IsNotNull() )
	{
		DataEntityImpl::Pointer data = factory->NewData();
		data->SetVoidPtr( ptr );
		m_DynProcessor->UpdateOutput( num, data->GetDataPtr(), "Output", true, 1, parentData );
	}
	else
	{
		std::ostringstream strError;
		strError <<"Failed get output of the filter. You should specify a valid output <dataType> tag";
		throw Core::Exceptions::Exception("DynDataTransferDLL::Update", 
			strError.str().c_str() );
	}

}
