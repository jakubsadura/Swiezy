/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreBaseProcessor.h"
#include "coreWxMitkGraphicalInterface.h"
#include "coreKernel.h"
#include "coreDataEntityHelper.h"
#include "coreLoggerHelper.h"
#include "blShapeUtils.h"

Core::BaseProcessor::BaseProcessor()
{
}

Core::BaseProcessor::~BaseProcessor()
{
}

void Core::BaseProcessor::UpdateOutput( 
	int iIndex, 
	boost::any data,
	std::string strDataEntityName,
	bool bReuseOutput,
	int iTotalTimeSteps,
	Core::DataEntity::Pointer fatherDataEntity )
{
	Core::DataEntity::Pointer dataEntityOutput;
	dataEntityOutput = GetOutputDataEntityHolder( iIndex )->GetSubject( );

	DataEntityImplFactory::Pointer factory;
	factory = DataEntityImplFactory::FindFactory( data.type() );
	if ( factory.IsNull() )
	{
		std::ostringstream strError;
		strError << "Cannot manage " << data.type().name( ) << std::endl;
		throw Core::Exceptions::Exception( 
			"BaseWindowFactories::CreateWindow",
			strError.str().c_str() );
	}


	// Create a new data entity
	bool buildOutput = CheckBuildOutput( 
			bReuseOutput, fatherDataEntity, 
			dataEntityOutput, iTotalTimeSteps );
	if ( buildOutput )
	{
		dataEntityOutput = DataEntity::New( );
		dataEntityOutput->GetMetadata()->SetName( strDataEntityName );
		Core::DataEntityType type = GetOutputPort( iIndex )->GetDataEntityType();
		if ( type == UnknownTypeId )
		{
			blTag::Pointer tag;
			tag = factory->GetProperties()->FindTagByName( "DefaultDataEntityType" );
			if ( tag.IsNotNull() )
			{
				type = tag->GetValueCasted<Core::DataEntityType>();
			}
		}
		dataEntityOutput->SetType( type );
		dataEntityOutput->Resize( iTotalTimeSteps, data.type() );
	}

	if ( factory->GetProperties()->FindTagByName( "single" ) )
	{
		// Copy the time step
		DataEntityImpl::Pointer impl = dataEntityOutput->GetTimeStep( GetTimeStep() );
		impl->DeepCopy( data );
	}
	else if ( factory->GetProperties()->FindTagByName( "multiple" ) )
	{
		// Copy all time steps
		dataEntityOutput->GetTimeStep( -1 )->DeepCopy( data );
	}

	// Set Father
	if ( dataEntityOutput->GetFather( ).IsNull( ) )
	{
		dataEntityOutput->SetFather( fatherDataEntity );
	}

	dataEntityOutput->Modified();

	// Update the output subject
	GetOutputDataEntityHolder( iIndex )->SetSubject( dataEntityOutput );
}

void Core::BaseProcessor::SetState( 
	Core::Runtime::APP_STATE val, 
	Core::ProgressTicketHolderType::Pointer progressTicket /*= NULL */ )
{
	Core::Runtime::Kernel::ApplicationRuntimePointer app;
	app = Core::Runtime::Kernel::GetApplicationRuntime( );
	if ( app )
	{
		app->SetAppState( val, progressTicket );
	}

}

bool Core::BaseProcessor::CheckBuildOutput( 
	bool bReuseOutput,
	Core::DataEntity::Pointer fatherDataEntity,
	Core::DataEntity::Pointer dataEntityOutput,
	unsigned totalTimeSteps )
{
	return ( !bReuseOutput || 
		dataEntityOutput.IsNull() || 
		dataEntityOutput->GetNumberOfTimeSteps( ) != totalTimeSteps ||
		( dataEntityOutput->GetFather( ).IsNotNull( ) && 
		  fatherDataEntity.IsNotNull( ) &&
		  fatherDataEntity->GetId( ) != dataEntityOutput->GetFather( )->GetId( ) && 
		  fatherDataEntity->GetId( ) != dataEntityOutput->GetId( ) ) );

}