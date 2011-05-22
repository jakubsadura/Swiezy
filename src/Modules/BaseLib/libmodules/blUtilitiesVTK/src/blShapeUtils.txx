/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _BLSHAPEUTILS_txx
#define _BLSHAPEUTILS_txx

	//---------------------------------------------------
	// HEADERS
	//---------------------------------------------------

	#include "vtkCellArray.h"
	#include "blPDShape2D.h"
	#include "blPDShape3D.h"

	//---------------------------------------------------
	// IMPLEMENTATION
	//---------------------------------------------------


namespace blShapeUtils
{


	/**
	 */
	template <unsigned int Dim>
	void ShapeUtils::Convert( 
						vtkPolyData* vtkShape, 
						typename blPDShape<Dim>::Pointer &pdShape )
	{	
		
		if ( Dim == 2 )
		{
			blPDShape2D::Pointer	shape2D = blPDShape2D::New( );
			pdShape = (blPDShape<Dim>*) ( shape2D.GetPointer() );
		}
		else if ( Dim == 3 )
		{
			blPDShape3D::Pointer	shape3D = blPDShape3D::New( );
			pdShape = static_cast<blPDShape<Dim>*> ( shape3D.GetPointer() );
		}



		//-------------------------------------------
		// Init points
		//-------------------------------------------

		std::vector<double>	pointsBuffer;
		pointsBuffer.resize( vtkShape->GetNumberOfPoints() * Dim );
		
		double point[ 3 ];
		int pointPosition = 0;
		for (	vtkIdType pointId = 0; 
			pointId < vtkShape->GetNumberOfPoints(); 
			pointId++ )
		{
			vtkShape->GetPoint( pointId, point );

			int d;
			for (d = 0; d < Dim; d++)
			{
				pointsBuffer[ pointPosition + d ] = point[ d ];
			}
			pointPosition += Dim; 
		}

		pdShape->InitializeFromPoints( &pointsBuffer[ 0 ], vtkShape->GetNumberOfPoints() );



		//-------------------------------------------
		// Build polygons and polylines
		//-------------------------------------------
		blPDShapeInterface::ShapeCellsContainer polygons, polylines;

		Convert( vtkShape->GetLines( ), polylines );
		Convert( vtkShape->GetPolys( ), polygons );

		pdShape->InitializeCells(polygons, polylines);	
	}



	/**
	 */
	template <unsigned int Dim>
	void ShapeUtils::Convert( 
						typename blPDShape<Dim>::Pointer &pdShape,
						vtkPolyData* vtkShape )
	{	
		

		//-------------------------------------------
		// Init points
		//-------------------------------------------

		vtkPoints		*pointsVtk = vtkPoints::New( VTK_DOUBLE );

		pointsVtk->SetNumberOfPoints( pdShape->GetNumberOfLandmarks( ) );

		double point[ 3 ] = { 0, 0, 0};
		for ( unsigned iNumPoint = 0 ; iNumPoint < pdShape->GetNumberOfLandmarks( ) ;iNumPoint++ )
		{
			pdShape->GetPoint( iNumPoint, point );
			pointsVtk->SetPoint( iNumPoint, point[ 0 ], point[ 1 ], point[ 2 ] );
		}
		vtkShape->SetPoints( pointsVtk );

		pointsVtk->Delete();



		//-------------------------------------------
		// Build polygons and polylines
		//-------------------------------------------
		blPDShapeInterface::ShapeCellsContainer polygons, polylines;
		vtkCellArray	*cellPolys = vtkCellArray::New( );
		vtkCellArray	*cellLines = vtkCellArray::New( );

		pdShape->GetPolylines( polylines );
		pdShape->GetPolygons( polygons );

		Convert( polylines, cellLines );
		Convert( polygons, cellPolys );

		vtkShape->SetPolys( cellPolys );
		vtkShape->SetLines( cellLines );

		cellPolys->Delete( );
		cellLines->Delete( );
	}

}

#endif // _SHAPEUTILS_h
