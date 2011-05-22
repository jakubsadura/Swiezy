/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreExtractScalarProcessor.h"
#include "coreDataEntityHelper.h"
#include "coreEnvironment.h"

#include "vtkPointData.h"
#include "coreVTKUnstructuredGridHolder.h"


Core::ExtractScalarProcessor::ExtractScalarProcessor( )
{
	SetNumberOfInputs( 2 );
	GetInputPort( 0 )->SetName( "Input mesh" );
	Core::DataEntityType type = Core::DataEntityType( Core::VolumeMeshTypeId | Core::SurfaceMeshTypeId );
	GetInputPort( 0 )->SetDataEntityType( type );
	GetInputPort( 1 )->SetName( "Input point" );
	GetInputPort( 1 )->SetDataEntityType( Core::PointSetTypeId );
	SetNumberOfOutputs( 1 );
	GetOutputPort( 0 )->SetDataEntityType( Core::SignalTypeId );
	GetOutputPort( 0 )->SetReuseOutput( true );
	GetOutputPort( 0 )->SetName( "Output signal" );
	GetOutputPort( 0 )->SetDataEntityName( "Scalar" );

	SetName( "ExtractScalar" );

	m_ScalarName = "activation";
}

void Core::ExtractScalarProcessor::Update()
{
	try
	{
		if ( GetInputDataEntity( 0 ).IsNull() )
		{
			return;
		}

		// Get input mesh
		vtkPointSet* inputMesh;
		switch( GetInputDataEntity( 0 )->GetType() )
		{
		case Core::VolumeMeshTypeId: 
			{
				vtkUnstructuredGridPtr volumeMesh;
				GetProcessingData( 0, volumeMesh );
				inputMesh = volumeMesh;
			}
			break;
		case Core::SurfaceMeshTypeId:
			{
				vtkPolyDataPtr surfaceMesh;
				GetProcessingData( 0, surfaceMesh );
				inputMesh = surfaceMesh;
			}
			break;
		default:
			return;
			break;
		}

		// Get input landmarks
		vtkPolyDataPtr pointSet;
		GetProcessingData( 1, pointSet );

		if ( pointSet->GetNumberOfPoints( ) == 0 )
		{
			return;
		}

		// Get first landmark point
		double selectedPoint[ 3 ];
		pointSet->GetPoint( 0, selectedPoint );

		if ( !inputMesh->GetPointData( ) )
		{
			throw Exceptions::Exception( 
				"ExtractScalarProcessor",
				"Input mesh doesn't have PointData");
		}

		// Extract scalar
		vtkIdType pointID = inputMesh->FindPoint( selectedPoint );

		blSignalCollective::Pointer signalCollective;
		signalCollective = blSignalCollective::New();

		std::vector<blSignal::SampleType> values;
		for ( int i = 0 ; i < inputMesh->GetPointData( )->GetNumberOfArrays( ) ; i++ )
		{
			vtkDataArray *dataArray;
			dataArray = inputMesh->GetPointData( )->GetArray( i );

			size_t found;
			std::string arrayName( dataArray->GetName() );
			found = arrayName.find( m_ScalarName.c_str() );
			if ( found != std::string::npos )
			{
				//std::cout << arrayName << ": " << dataArray->GetTuple1( pointID ) << std::endl;
				values.push_back( dataArray->GetTuple1( pointID ) );
			}
		}

		signalCollective->AddSignal( blSignal::Build( m_ScalarName.c_str(), "", values ) );

		UpdateOutput( 0, signalCollective, "Scalars", true, 1, GetInputDataEntity( 0 ) );

	}
	coreCatchExceptionsAddTraceAndThrowMacro( ExtractScalarProcessor::Update )
}

std::string Core::ExtractScalarProcessor::GetScalarName() const
{
	return m_ScalarName;
}

void Core::ExtractScalarProcessor::SetScalarName( std::string val )
{
	m_ScalarName = val;
}
