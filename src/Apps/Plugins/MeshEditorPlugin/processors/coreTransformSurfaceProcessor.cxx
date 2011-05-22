// Copyright 2006 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "coreTransformSurfaceProcessor.h"

#include "vtkTransform.h"
#include "vtkTransformPolyDataFilter.h"
#include "coreDataEntityHelper.h"

Core::TransformSurfaceProcessor::TransformSurfaceProcessor( )
{
	BaseProcessor::SetNumberOfInputs( 2 );
	GetInputPort( 0 )->SetName( "Input shape" );
	GetInputPort( 0 )->SetDataEntityType( Core::SurfaceMeshTypeId );
	GetInputPort( 1 )->SetName( "Input point" );
	GetInputPort( 1 )->SetDataEntityType( Core::PointSetTypeId );
	BaseProcessor::SetNumberOfOutputs( 1 );
	GetOutputPort( 0 )->SetDataEntityType( Core::SurfaceMeshTypeId );
	GetOutputPort( 0 )->SetReuseOutput( true );
	GetOutputPort( 0 )->SetDataEntityName( "Transformed surface" );

	m_StartingPoint[ 0 ] = 0;
	m_StartingPoint[ 1 ] = 0;
	m_StartingPoint[ 2 ] = 0;
}

Core::TransformSurfaceProcessor::~TransformSurfaceProcessor()
{
}

void Core::TransformSurfaceProcessor::Update()
{
	try
	{
		Core::vtkPolyDataPtr polyData;
		GetProcessingData( 
			INPUT_SURFACE_MESH,
			polyData,
			GetTimeStep( ) );

		Core::vtkPolyDataPtr landmarkPoint;
		GetProcessingData( 
			INPUT_POINT,
			landmarkPoint,
			GetTimeStep( ) );

		if ( landmarkPoint->GetNumberOfPoints() != 1 )
		{
			throw Core::Exceptions::Exception( 
				"TransformSurfaceProcessor::Update",
				"You should set one point as input" );
		}

		double diffPoints[ 3 ];
		//double *startingPoint = landmarkPoint->GetPoints( )->GetPoint( 0 );
		double *endPoint = landmarkPoint->GetPoints( )->GetPoint( 0 );
		diffPoints[ 0 ] = endPoint[ 0 ] - m_StartingPoint[ 0 ];
		diffPoints[ 1 ] = endPoint[ 1 ] - m_StartingPoint[ 1 ];
		diffPoints[ 2 ] = endPoint[ 2 ] - m_StartingPoint[ 2 ];

		// Use translation filter
		vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
		transform->Translate( diffPoints );

		vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter;
		transformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
		transformFilter->SetInput( polyData );
		transformFilter->SetTransform( transform );
		transformFilter->Update();

		// Notify Views to render it
		GetOutputPort( 0 )->UpdateOutput( 
			transformFilter->GetOutput(), 0, 
			GetInputDataEntity( 0 )->GetFather() );

	}
	catch( ... )
	{
		throw;
	}

}
