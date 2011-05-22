/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "blMitkInteractorHelper.h"

#include "mitkEventMapper.h"
#include "mitkInteractionConst.h"
#include "mitkVector.h"
#include "mitkPositionEvent.h"
#include "mitkBaseRenderer.h"

void blMitkInteractorHelper::RemoveSelectedPoint( mitk::BaseRenderer* render )
{

	// Press key Delete
	mitk::Event event( render, 
		mitk::Type_KeyPress, 
		mitk::BS_NoButton, 
		mitk::BS_NoButton, 
		mitk::Key_Delete);
	mitk::EventMapper::MapEvent(&event);
}

void blMitkInteractorHelper::AddPoint( mitk::BaseRenderer* render, double x[3] )
{
	mitk::Point3D point;
	point[ 0 ] = mitk::ScalarType( x[ 0 ] );
	point[ 1 ] = mitk::ScalarType( x[ 1 ] );
	point[ 2 ] = mitk::ScalarType( x[ 2 ] );

	SendEventLeftMouseButton( render, point );

}

void blMitkInteractorHelper::SelectPoint( 
						mitk::BaseRenderer* render,
						mitk::PointSet::Pointer pointSet, 
						int iPoint )
{
	int timeStep = 0;
	if ( render != NULL )
	{
		timeStep = render->GetTimeStep();
	}

	// Get point position
	if ( pointSet->GetSize( timeStep ) == 0 )
	{
		return;
	}

	mitk::PointSet::DataType *itkPointSet = pointSet->GetPointSet( timeStep );
	mitk::PointSet::PointsContainer *points = itkPointSet->GetPoints();
	if ( !points->IndexExists(iPoint) )
	{
		return;
	}

	mitk::Point3D point;
	point = pointSet->GetPoint( iPoint, timeStep );

	SendEventLeftMouseButton( render, point );
}

void blMitkInteractorHelper::SendEventLeftMouseButton( 
	mitk::BaseRenderer* render,
	mitk::Point3D point )
{

	mitk::Point2D pt;
	pt[0] = 0;
	pt[1]= 0;

	// Add the point with SHIFT + mouse left button
	mitk::PositionEvent eventPressLeftMouseButton(
		render, 
		mitk::Type_MouseButtonPress, 
		mitk::BS_LeftButton, 
		mitk::BS_ShiftButton, 
		mitk::Key_unknown, 
		pt, 
		point);
	mitk::EventMapper::MapEvent(&eventPressLeftMouseButton);

	// Release the mouse left button
	mitk::PositionEvent eventReleaseLeftMouseButton(
		render, 
		mitk::Type_MouseButtonRelease, 
		mitk::BS_LeftButton, 
		mitk::BS_LeftButton | mitk::BS_ShiftButton, 
		mitk::Key_unknown, 
		pt, 
		point);
	mitk::EventMapper::MapEvent(&eventReleaseLeftMouseButton);
}


