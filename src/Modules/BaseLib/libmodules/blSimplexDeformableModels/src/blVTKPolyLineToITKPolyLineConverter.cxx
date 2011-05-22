/*
* Copyright (c) 2010,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#include "blVTKPolyLineToITKPolyLineConverter.h"

// VTK
#include <vtkPoints.h>
#include <vtkPolyLine.h>


//--------------------------------------------------
blVTKPolyLineToITKPolyLineConverter::blVTKPolyLineToITKPolyLineConverter()
//--------------------------------------------------
{
	// Input initialized to do DeepCopy at SetInput function
	this->vtkCenterline = vtkPolyData::New();
	this->vtkPDCenterline = vtkPolyData::New();
	this->centerlineSmoothingRadius = 0;
}

//--------------------------------------------------
blVTKPolyLineToITKPolyLineConverter::~blVTKPolyLineToITKPolyLineConverter()
//--------------------------------------------------
{
	this->vtkCenterline->Delete();
	this->vtkPDCenterline->Delete();
}

//--------------------------------------------------
void blVTKPolyLineToITKPolyLineConverter::Update()
//--------------------------------------------------
{
	PolyLineMeshType::PointType pt;
	double vtkPtAvg[3];
	double vtkPt[3];
	vtkPoints *newPoints = vtkPoints::New();
	unsigned long pointIds[2];
	vtkPolyLine *plCell;
	vtkPolyLine *newPlCell = vtkPolyLine::New();

	this->polyLineMesh = PolyLineMeshType::New();

	plCell = (vtkPolyLine *) this->vtkCenterline->GetCell(0);
	PolyLineMeshType::CellAutoPointer c;
	PolyLineCellType *tcell;
	vtkIdType cellId = 0;
	vtkIdType i;
	this->vtkPDCenterline->Delete();
	this->vtkPDCenterline = vtkPolyData::New();

		
	for ( i = this->centerlineSmoothingRadius; i < plCell->GetNumberOfPoints() - this->centerlineSmoothingRadius; i++ )
	{
		vtkIdType count = 0; 
		vtkPtAvg[0] = 0.0; vtkPtAvg[1] = 0.0; vtkPtAvg[2] = 0.0;
		for( vtkIdType j = i - this->centerlineSmoothingRadius ; j <= i + this->centerlineSmoothingRadius; j++ )
		{
			plCell->GetPoints()->GetPoint(j,vtkPt);
			vtkPtAvg[0] += vtkPt[0]; vtkPtAvg[1] += vtkPt[1]; vtkPtAvg[2] += vtkPt[2];
			count++;
		}
		vtkPtAvg[0] /= count; vtkPtAvg[1] /= count; vtkPtAvg[2] /= count;
		pt[0] = vtkPtAvg[0]; pt[1] = vtkPtAvg[1]; pt[2] = vtkPtAvg[2];
		this->polyLineMesh->SetPoint(i-this->centerlineSmoothingRadius, pt);
		newPoints->InsertNextPoint(vtkPtAvg);
	}

	this->vtkPDCenterline->SetPoints(newPoints);
	this->vtkPDCenterline->Allocate();
	newPlCell->GetPointIds()->SetNumberOfIds(this->vtkPDCenterline->GetNumberOfPoints());

	for ( i = 0; i < this->vtkPDCenterline->GetNumberOfPoints(); i++ )
	{
		newPlCell->GetPointIds()->SetId(i,i);
	}

	this->vtkPDCenterline->InsertNextCell(newPlCell->GetCellType(),newPlCell->GetPointIds());

	for ( i = 1; i < plCell->GetNumberOfPoints() - (2*this->centerlineSmoothingRadius); i++ )
	{
		tcell = new PolyLineCellType;
		pointIds[0] = i-1; pointIds[1] = i;
		tcell->SetPointIds( pointIds );
		c.TakeOwnership( tcell );
		this->polyLineMesh->SetCell( cellId, c );
		cellId++;
	}

	newPlCell->Delete();

}
