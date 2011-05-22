/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "corePointSetRenDataBuilder.h"
#include "blVTKHelperTools.h"
#include <vtkPolyData.h>
#include <mitkSurface.h>
#include <mitkPointSet.h>
#include "mitkPointOperation.h"
#include "mitkInteractionConst.h"

#include "coreVTKPolyDataHolder.h"
#include "coreDataEntity.h"

using namespace Core;

//!
PointSetRenDataBuilder::PointSetRenDataBuilder(void)
{
}

//!
PointSetRenDataBuilder::~PointSetRenDataBuilder(void)
{
}

//!
void PointSetRenDataBuilder::Update()
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
	mitk::PointSet::Pointer mitkPointSet = dynamic_cast<mitk::PointSet*> ( mitkData.GetPointer() );

	// Create rendering data
	if ( mitkPointSet.IsNull() )
	{
		mitkPointSet = mitk::PointSet::New( );
	}
	mitkPointSet->Expand( dataEntity->GetNumberOfTimeSteps() );

	// For all time steps
	for ( unsigned iTimeStep = 0 ; iTimeStep < dataEntity->GetNumberOfTimeSteps() ; iTimeStep++ )
	{
		// Be careful with this code. If you create a new rendering data
		// The observers to the data entity needs to update the node in the
		// rendering tree with SetData function each time this function is called
		vtkPolyDataPtr meshPtr;
		dataEntity->GetProcessingData( meshPtr, iTimeStep );

		mitk::PointSet::PointsContainer *pointsItk;
		mitk::PointSet::PointsContainer::iterator it;
		pointsItk = mitkPointSet->GetPointSet( iTimeStep )->GetPoints();
		vtkPoints *pointsVtk = meshPtr->GetPoints( );

		// When the user moves a point, we need to keep the same points reference
		// and change only the position, otherwise, it crashes
		if ( pointsVtk && pointsVtk->GetNumberOfPoints() == mitkPointSet->GetSize( ) )
		{
			for( int i = 0; i < mitkPointSet->GetSize(  ); i++ )
			{
				mitk::PointSet::PointType mitkPoint;
				mitkPoint = mitkPointSet->GetPoint( i, iTimeStep );
				mitk::PointSet::PointType mitkPointVtk;
				mitkPointVtk[ 0 ] = mitk::PointSet::CoordinateType( pointsVtk->GetPoint( i )[ 0 ] );
				mitkPointVtk[ 1 ] = mitk::PointSet::CoordinateType( pointsVtk->GetPoint( i )[ 1 ] );
				mitkPointVtk[ 2 ] = mitk::PointSet::CoordinateType( pointsVtk->GetPoint( i )[ 2 ] );
				if ( mitkPointVtk != mitkPoint )
				{
					mitkPointSet->SetPoint( i, mitkPoint, iTimeStep );
				}
			}
		}
		else
		{

			// Remove all points
			while ( pointsItk->size( ) )
			{
				mitk::ScalarType timeInMS = 0.0;
				mitk::Point3D point;
				timeInMS = mitkPointSet->GetTimeSlicedGeometry()->TimeStepToMS( iTimeStep );
				mitk::PointOperation* doOp = new mitk::PointOperation(
					mitk::OpREMOVE, timeInMS, point, pointsItk->begin( )->first );
				mitkData->ExecuteOperation( doOp );
			}

			// Add all the points
			if ( pointsVtk != NULL )
			{
				for( int i = 0; i < pointsVtk->GetNumberOfPoints(); i++ )
				{
					mitk::PointSet::PointType mitkPoint;
					mitkPoint[ 0 ] = mitk::PointSet::CoordinateType( pointsVtk->GetPoint( i )[ 0 ] );
					mitkPoint[ 1 ] = mitk::PointSet::CoordinateType( pointsVtk->GetPoint( i )[ 1 ] );
					mitkPoint[ 2 ] = mitk::PointSet::CoordinateType( pointsVtk->GetPoint( i )[ 2 ] );
					mitkPointSet->InsertPoint( i, mitkPoint, iTimeStep );
				}
			}
		}

	}
		
	dataEntity->SetRenderingData( mitk::BaseData::Pointer(mitkPointSet) );
}

bool Core::PointSetRenDataBuilder::CheckProcessingDataIsOk( 
	const Core::DataEntityImpl::Pointer timeStep )
{
	vtkPolyDataPtr meshPtr;
	bool worked = CastAnyProcessingData( timeStep->GetDataPtr(), meshPtr ); 
	return worked;
}
