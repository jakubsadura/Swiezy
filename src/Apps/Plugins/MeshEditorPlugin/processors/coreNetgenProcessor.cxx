/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreNetgenProcessor.h"
#include "coreDataEntityHelper.h"
#include "coreNetgenTimeStepImpl.h"

Core::NetgenProcessor::NetgenProcessor( )
{
	SetNumberOfInputs( 1 );
	GetInputPort( 0 )->SetName( "Input surface" );
	GetInputPort( 0 )->SetDataEntityType( Core::SurfaceMeshTypeId );
	SetNumberOfOutputs( 1 );
	SetOutputDataName( 0, "Output surface" );
	GetOutputPort( 0 )->SetDataEntityType( Core::SurfaceMeshTypeId );
	GetOutputPort( 0 )->SetReuseOutput( true );
	GetOutputPort( 0 )->SetDataEntityName( "Netgen optimized" );

	SetName( "NetgenProcessor" );

	m_Filter = meNGOptimizeFilter::New();
}

void Core::NetgenProcessor::Update()
{
	meNetgenMesh::Pointer netgenMesh;
	GetProcessingData( 0, netgenMesh);

	m_Filter->SetInput( netgenMesh );

	m_Filter->Update();

	// Update output
	GetOutputPort( 0 )->UpdateOutput( netgenMesh, 0, GetInputDataEntity(0) );
}

void Core::NetgenProcessor::SetParams(meMeshParamsOptimizePtr param)
{
	m_Filter->SetParams( param );
}

meMeshParamsOptimizePtr Core::NetgenProcessor::GetParams()
{
	return m_Filter->GetParams( );
}
