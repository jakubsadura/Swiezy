/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreContourRenDataBuilder.h"
#include "blVTKHelperTools.h"
#include <vtkPolyData.h>
#include <mitkSurface.h>
#include <mitkContour.h>
#include "coreVTKPolyDataHolder.h"
#include "coreDataEntity.h"

using namespace Core;

//!
ContourRenDataBuilder::ContourRenDataBuilder(void)
{
}

//!
ContourRenDataBuilder::~ContourRenDataBuilder(void)
{
}

//!
void ContourRenDataBuilder::Update( )
{
	// Get input
	Core::DataEntity::Pointer dataEntity = GetInputDataEntity( 0 );

	if ( !CheckInput( ) )
	{
		return;
	}

	//! Check processing data
	if( !CheckProcessingDataIsOk( dataEntity->GetTimeStep( 0 ) ) )
	{
		return;
	}

	mitk::BaseData::Pointer mitkData;
	boost::any anyData = dataEntity->GetRenderingData( typeid( mitkData ) );
	Core::CastAnyProcessingData( anyData, mitkData );
	mitk::Contour::Pointer mitkContour = dynamic_cast<mitk::Contour*> ( mitkData.GetPointer() );

	// Create rendering data
	if ( mitkContour.IsNull() )
	{
		mitkContour = mitk::Contour::New( );
	}
	mitkContour->Expand( dataEntity->GetNumberOfTimeSteps() );

	// For all time steps
	for ( unsigned iTimeStep = 0 ; iTimeStep < dataEntity->GetNumberOfTimeSteps() ; iTimeStep++ )
	{
		// Be careful with this code. If you create a new rendering data
		// The observers to the data entity needs to update the node in the
		// rendering tree with SetData function each time this function is called
		vtkPolyDataPtr meshPtr;
		dataEntity->GetProcessingData( meshPtr, iTimeStep );

		// Add all the points
		vtkPoints *points = meshPtr->GetPoints( );
		if ( points != NULL )
		{
			for( int i = 0; i < points->GetNumberOfPoints(); i++ )
			{
				mitk::Point3D mitkPoint;
				mitkPoint[ 0 ] = mitk::ScalarType( points->GetPoint( i )[ 0 ] );
				mitkPoint[ 1 ] = mitk::ScalarType( points->GetPoint( i )[ 1 ] );
				mitkPoint[ 2 ] = mitk::ScalarType( points->GetPoint( i )[ 2 ] );
				mitkContour->AddVertex( mitkPoint );
			}
		}

	}

	dataEntity->SetRenderingData( mitk::BaseData::Pointer(mitkContour) );
}

bool Core::ContourRenDataBuilder::CheckProcessingDataIsOk( 
	Core::DataEntityImpl::Pointer timeStep )
{
	vtkPolyDataPtr meshPtr;
	bool worked = Core::CastAnyProcessingData( timeStep->GetDataPtr(), meshPtr ); 
	return worked;
}
