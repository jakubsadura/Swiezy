/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "meNGtoVtkPolydataFilter.h"

#include "vtkPoints.h"
#include "vtkCellArray.h"

using namespace nglib;

meNGToVtkPolydataFilter::meNGToVtkPolydataFilter()
{
	m_poly = vtkSmartPointer<vtkPolyData>::New();
}


meNGToVtkPolydataFilter::~meNGToVtkPolydataFilter() 
{
}

void meNGToVtkPolydataFilter::Update()
{
	try
	{
		if ( Ng_GetNP( GetInput()->GetNGMesh() ) == 0 )
			return;

		// Points
		double tempPoint[3];
		vtkSmartPointer<vtkPoints>	pointsVtk = vtkSmartPointer<vtkPoints>::New( );
		pointsVtk->SetDataTypeToDouble( );
		pointsVtk->SetNumberOfPoints( Ng_GetNP( GetInput()->GetNGMesh()));

		// Add the points
		for (	unsigned iNumPoint = 1; 
			iNumPoint < Ng_GetNP( GetInput()->GetNGMesh())+1; 
			iNumPoint++ )
		{
			Ng_GetPoint( GetInput()->GetNGMesh( ), iNumPoint, tempPoint );
			pointsVtk->SetPoint(iNumPoint-1,tempPoint[ 0 ], tempPoint[ 1 ], tempPoint[ 2 ] );
		}

		m_poly->SetPoints( pointsVtk );

		 
		//Surface Elements
		int surfEl[3];
		vtkSmartPointer<vtkCellArray>	cellArray = vtkSmartPointer<vtkCellArray>::New( );
		vtkIdType		pointsId[ 3 ];

		// Adds surface elements
		for (	unsigned i = 1; 
			i < Ng_GetNSE( GetInput()->GetNGMesh( ) )+1 ; 
			i++ )
		{
			Ng_GetSurfaceElement ( GetInput()->GetNGMesh( ), i ,surfEl );
			pointsId[0] = surfEl[0]-1;
			pointsId[1] = surfEl[1]-1;
			pointsId[2] = surfEl[2]-1;
			cellArray->InsertNextCell( 3, pointsId );
		}

		m_poly->SetPolys( cellArray );

	}
	catch (...)
	{
		throw;
	}	
}

vtkSmartPointer<vtkPolyData> meNGToVtkPolydataFilter::GetPolyData()
{
	return m_poly;
}

