/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _BaseProcessor_TXX
#define _BaseProcessor_TXX

#include "coreBaseProcessor.h"

/**
*/
template <typename T>
void Core::BaseProcessor::UpdateOutput( 
	int iIndex, 
	std::vector<T> dataVector,
	std::string dataEntityName,
	bool bReuseOutput /*= false*/,
	Core::DataEntity::Pointer fatherDataEntity /*= NULL */){

	if ( dataVector.size() == 0 )
	{
		return;
	}

	Core::DataEntity::Pointer dataEntityOutput;
	dataEntityOutput = GetOutputDataEntityHolder( iIndex )->GetSubject( );

	DataEntityImplFactory::Pointer factory;
	factory = DataEntityImplFactory::FindFactory( typeid( T ) );
	if ( factory.IsNull() )
	{
		std::ostringstream strError;
		strError << "Cannot manage " << typeid( T ).name( ) << std::endl;
		throw Core::Exceptions::Exception( 
			"BaseWindowFactories::CreateWindow",
			strError.str().c_str() );
	}

	bool buildOutput = CheckBuildOutput( 
			bReuseOutput, fatherDataEntity, 
			dataEntityOutput, dataVector.size() );
	if ( buildOutput )
	{
		dataEntityOutput = DataEntity::New( );
		dataEntityOutput->GetMetadata()->SetName( dataEntityName );
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
		dataEntityOutput->Resize( dataVector.size(), typeid( T ) );
	}

	// Copy processing data
	for ( unsigned i = 0 ; i < dataVector.size() ; i++ )
	{
		DataEntityImpl::Pointer impl = dataEntityOutput->GetTimeStep( i );
		impl->DeepCopy( dataVector[ i ] );
	}

	// Update rendering
	if ( dataEntityOutput->GetFather( ).IsNull( ) )
	{
		dataEntityOutput->SetFather( fatherDataEntity );
	}

	// Update the output subject
	dataEntityOutput->Modified( );
	GetOutputDataEntityHolder( iIndex )->SetSubject( dataEntityOutput );
}


#endif // _BaseProcessor_TXX


