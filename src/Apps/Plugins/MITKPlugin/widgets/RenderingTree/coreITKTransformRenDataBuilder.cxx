/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreITKTransformRenDataBuilder.h"
#include "mitkTransform.h"
#include "mitkTransformObjectFactory.h"
#include "coreITKTransformImpl.h"

using namespace Core;

ITKTransformRenDataBuilder::ITKTransformRenDataBuilder()
{
	RegisterTransformObjectFactory( );
}

ITKTransformRenDataBuilder::~ITKTransformRenDataBuilder()
{
}

void ITKTransformRenDataBuilder::Update( )
{
	// Get input
	Core::DataEntity::Pointer dataEntity = GetInputDataEntity( 0 );

	if ( !CheckInput( ) )
	{
		return;
	}

	// Try casting to the expected format. The first image is used to initialization
	Core::ITKTransformImpl::TransformPointer transform; 
	bool worked = dataEntity->GetProcessingData( transform, 0 );
	if(!worked || transform.GetPointer() == NULL)
	{
		return;
	}

	mitk::BaseData::Pointer mitkData;
	boost::any anyData = dataEntity->GetRenderingData( typeid( mitkData ) );
	Core::CastAnyProcessingData( anyData, mitkData );
	mitk::Transform::Pointer mitkTransform = dynamic_cast<mitk::Transform*> ( mitkData.GetPointer() );

	//! Distinguish in between a 3D+t and a 3D image for initialization
	if ( mitkTransform.IsNull() )
	{
		mitkTransform = mitk::Transform::New();
	}

	mitkTransform->Initialize( dataEntity->GetNumberOfTimeSteps() );

	for ( int pos = 0 ; pos < dataEntity->GetNumberOfTimeSteps() ; pos++ )
	{
		dataEntity->GetProcessingData( transform, pos );

		mitkTransform->SetTransform( pos, transform );
	}

	dataEntity->SetRenderingData( mitk::BaseData::Pointer(mitkTransform) );
}
