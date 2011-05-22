/*=========================================================================

  Module    : MeshLib
  File      : $RCSfile: $
  Copyright : (C)opyright (Pompeu Fabra University) 2007++
              See COPYRIGHT statement in top level directory.
  Authors   : Nacho Larrabide
  Modified  : $Author: $
  Purpose   : 
  Date      : $Date: $
  Version   : $Revision: $
  Changes   : $Locker: $
              $Log: $

=========================================================================*/

#include "meVTKPolyLineTopologyAnalysis.h"

// VTK
#include <vtkPoints.h>
#include <vtkPolyLine.h>
#include <vtkGenericCell.h>
#include <vtkCharArray.h>
#include <vtkCellData.h>

//STD
#include <list>
//--------------------------------------------------
meVTKPolyLineTopologyAnalysis::meVTKPolyLineTopologyAnalysis()
//--------------------------------------------------
{
	// Input initialized to do DeepCopy at SetInput function
	this->input = NULL;
	this->output = NULL;
	this->intermediateSkeleton = NULL;
}

//--------------------------------------------------
meVTKPolyLineTopologyAnalysis::~meVTKPolyLineTopologyAnalysis()
//--------------------------------------------------
{
}

//--------------------------------------------------
void meVTKPolyLineTopologyAnalysis::Update()
//--------------------------------------------------
{
	if (this->input!=NULL){
		if (output!=NULL){
			output->Delete();
		}
		this->intermediateSkeleton = vtkPolyData::New();
		this->intermediateSkeleton->DeepCopy(this->input);
		this->intermediateSkeleton->BuildCells();

		this->AnalyzePolyline();

		this->output = vtkPolyData::New();
		this->output->DeepCopy(this->intermediateSkeleton);
	}	
}

//--------------------------------------------------
void meVTKPolyLineTopologyAnalysis::AnalyzePolyline()
//--------------------------------------------------
{
	if (this->intermediateSkeleton!=NULL){
		vtkIdList *neighCellsList = vtkIdList::New();
		vtkGenericCell *auxCell = vtkGenericCell::New();
		char term;
		this->intermediateSkeleton->GetCellData()->RemoveArray( "Neighbors Count" );
		this->intermediateSkeleton->GetCellData()->RemoveArray( "Terminal Cells" );

		vtkCharArray *neighborsCount = vtkCharArray::New();
		neighborsCount->SetNumberOfComponents(2);
		neighborsCount->SetNumberOfTuples(this->intermediateSkeleton->GetNumberOfCells());
		neighborsCount->SetName("Neighbors Count");
		neighborsCount->Allocate(this->intermediateSkeleton->GetNumberOfCells());

		vtkCharArray *terminalCells = vtkCharArray::New();
		terminalCells->SetNumberOfComponents(1);
		terminalCells->SetNumberOfTuples(this->intermediateSkeleton->GetNumberOfCells());
		terminalCells->SetName("Terminal Cells");
		terminalCells->Allocate(this->intermediateSkeleton->GetNumberOfCells());

		for (vtkIdType i=0; i < this->intermediateSkeleton->GetNumberOfCells() ; i++)	{
			this->intermediateSkeleton->GetCell(i, auxCell);
			char neighCells[2];

			this->intermediateSkeleton->GetPointCells( auxCell->GetPointId(0), neighCellsList);
			neighCells[0] = neighCellsList->GetNumberOfIds() - 1;

			this->intermediateSkeleton->GetPointCells( auxCell->GetPointId(auxCell->GetNumberOfPoints()-1), neighCellsList);
			neighCells[1] = neighCellsList->GetNumberOfIds() - 1;

			neighborsCount->InsertTupleValue(i, neighCells);

			if (neighCells[0]==0 || neighCells[1]==0){
				term = 1;
				terminalCells->InsertTupleValue(i, &term);
			}else{
				term = 0;
				terminalCells->InsertTupleValue(i, &term);
			}
		}
		this->intermediateSkeleton->GetCellData()->AddArray(neighborsCount);
		this->intermediateSkeleton->GetCellData()->AddArray(terminalCells);
		this->intermediateSkeleton->Modified();
		neighborsCount->Delete();
		terminalCells->Delete();
	}
}
