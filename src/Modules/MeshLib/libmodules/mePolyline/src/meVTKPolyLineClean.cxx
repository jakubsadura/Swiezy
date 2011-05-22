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

#include "meVTKPolyLineClean.h"
#include "meVTKPolyLineDelete.h"
#include "meVTKPolyLineCombine.h"
#include "meVTKPolyLineSmooth.h"
#include "meVTKPolyLineTopologyAnalysis.h"

// VTK
#include <vtkPoints.h>
#include <vtkPolyLine.h>
#include <vtkGenericCell.h>
#include <vtkMergePoints.h>
#include <vtkCharArray.h>
#include <vtkCellData.h>

//STD
#include <list>
//--------------------------------------------------
meVTKPolyLineClean::meVTKPolyLineClean()
//--------------------------------------------------
{
	// Input initialized to do DeepCopy at SetInput function
	this->input = NULL;
	this->output = NULL;
	this->intermediateSkeleton = NULL;
	this->terminalSize = 2;
	this->smoothingIerations = 5;
}

//--------------------------------------------------
meVTKPolyLineClean::~meVTKPolyLineClean()
//--------------------------------------------------
{
}

//--------------------------------------------------
void meVTKPolyLineClean::Update()
//--------------------------------------------------
{
	if (this->input!=NULL){
		if (output!=NULL){
			output->Delete();
		}
		this->intermediateSkeleton = vtkPolyData::New();
		this->intermediateSkeleton->DeepCopy(this->input);
		this->intermediateSkeleton->BuildCells();

		while ( this->RemoveTerminals(this->terminalSize) > 0 );
		
		//this->AnalyzePolyline();

		meVTKPolyLineTopologyAnalysis::Pointer analyze = meVTKPolyLineTopologyAnalysis::New();
		analyze->SetInput( this->intermediateSkeleton );
		analyze->Update();
		this->intermediateSkeleton->DeepCopy( analyze->GetOutput() );

		this->MergeCells();

		this->BeautifyPolyline();

		this->output = vtkPolyData::New();
		this->output->DeepCopy(this->intermediateSkeleton);
	}	
}

//--------------------------------------------------
int meVTKPolyLineClean::RemoveTerminals(int nPts)
//--------------------------------------------------
{
	int removedTerminals = 0;
	if (this->intermediateSkeleton!=NULL){
		vtkIdList *neighCellsList0 = vtkIdList::New();
		vtkIdList *neighCellsListN = vtkIdList::New();
		vtkIdList *cellsToDelete = vtkIdList::New();
		vtkGenericCell *auxCell = vtkGenericCell::New();

		for (vtkIdType i = 0 ; i < this->intermediateSkeleton->GetNumberOfCells(); i++)	{
			neighCellsList0 = vtkIdList::New();
			neighCellsListN = vtkIdList::New();
			this->intermediateSkeleton->GetCell(i, auxCell);
			unsigned int neighCells0, neighCellsN;

			this->intermediateSkeleton->GetPointCells( auxCell->GetPointId(0), neighCellsList0);
			neighCells0 = neighCellsList0->GetNumberOfIds() - 1;

			this->intermediateSkeleton->GetPointCells( auxCell->GetPointId(auxCell->GetNumberOfPoints()-1), neighCellsListN);
			neighCellsN = neighCellsListN->GetNumberOfIds() - 1;

			if ((neighCells0==0 || neighCellsN==0) && (auxCell->GetNumberOfPoints()<=nPts))	{
				cellsToDelete->InsertNextId(i);
				removedTerminals++;
			}
			neighCellsList0->Delete();
			neighCellsListN->Delete();
		}

		meVTKPolyLineDelete::Pointer del = meVTKPolyLineDelete::New();
		del->SetInput( this->intermediateSkeleton );
		del->DeleteCells( cellsToDelete );
		del->Update();
		this->intermediateSkeleton->DeepCopy( del->GetOutput() );
		this->intermediateSkeleton->BuildCells();
	}
	return removedTerminals;
}

//--------------------------------------------------
void meVTKPolyLineClean::AnalyzePolyline()
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

//--------------------------------------------------
void meVTKPolyLineClean::MergeCells()
//--------------------------------------------------
{
	if (this->intermediateSkeleton!=NULL){
		vtkCharArray *neighborsCount;
		vtkGenericCell *auxCell = vtkGenericCell::New();
		vtkGenericCell *auxCell2 = vtkGenericCell::New();
		vtkIdList *neighCellsList = vtkIdList::New();
		vtkIdType nextStartingCell = 0;
		bool firstRun = true;
		bool cellsToMergeExist = true;
		vtkIdType mergeCellId1, mergeCellId2; 
		char cellNeighborsCount[2];
		while (cellsToMergeExist){
			neighborsCount = dynamic_cast<vtkCharArray *>(this->intermediateSkeleton->GetCellData()->GetArray("Neighbors Count"));
			vtkIdType i = nextStartingCell;
			bool foundCellsToMerge = false;
			while ( (i < this->intermediateSkeleton->GetNumberOfCells()) && (!foundCellsToMerge) ){
				//std::cout << "Searching for a cell to merge, currently at : " << i << std::endl;
				neighborsCount->GetTupleValue(i, cellNeighborsCount);
				// true if one of the terminals of the cell has exactly one neighbor
				// -> cell need to be merged
				foundCellsToMerge =((cellNeighborsCount[0] == 1) || (cellNeighborsCount[1] == 1));	
				i++;
			}
			//cellsToMergeExist = false;
			i--;
			nextStartingCell = i;
			//std::cout << "Stopped at: " << i << std::endl;
			if ( foundCellsToMerge )	{
				mergeCellId1 = i;
				mergeCellId2 = -1;
				this->intermediateSkeleton->GetCell(i, auxCell);
				if ((cellNeighborsCount[0] == 1)){
					this->intermediateSkeleton->GetPointCells( auxCell->GetPointId(0), neighCellsList);
					(neighCellsList->GetId(0) != mergeCellId1)?	mergeCellId2 = neighCellsList->GetId(0):mergeCellId2 = neighCellsList->GetId(1);
					//std::cout << "num of points of the cell : " << auxCell->GetNumberOfPoints()  << ", using id : "<< mergeCellId2 << std::endl;
				} else if (cellNeighborsCount[1] == 1) {
					this->intermediateSkeleton->GetPointCells( auxCell->GetPointId(auxCell->GetNumberOfPoints()-1), neighCellsList);
					(neighCellsList->GetId(0) != mergeCellId1)?	mergeCellId2 = neighCellsList->GetId(0):mergeCellId2 = neighCellsList->GetId(1);
					//std::cout << "num of points of the cell : " << auxCell->GetNumberOfPoints()-1  << ", using id : "<< mergeCellId2 << std::endl;
				}
				this->intermediateSkeleton->GetCell(mergeCellId2, auxCell2);
				//std::cout << "auxCell points 0 and n : " << auxCell->GetPointId(0) << " and " << auxCell->GetPointId(auxCell->GetNumberOfPoints()-1) << std::endl;
				//std::cout << "auxCell2 points 0 and n : " << auxCell2->GetPointId(0) << " and " << auxCell2->GetPointId(auxCell2->GetNumberOfPoints()-1) << std::endl;
				if ((auxCell2->GetPointId(0) == auxCell->GetPointId(0)) || 
					(auxCell2->GetPointId(0) == auxCell->GetPointId(auxCell->GetNumberOfPoints()-1)) ||
					(auxCell2->GetPointId(auxCell2->GetNumberOfPoints()-1) == auxCell->GetPointId(0)) ||
					(auxCell2->GetPointId(auxCell2->GetNumberOfPoints()-1) == auxCell->GetPointId(auxCell->GetNumberOfPoints()-1)) ){
						//std::cout << "Merging " << mergeCellId1 << " and " << mergeCellId2 << std::endl;
						meVTKPolyLineCombine::Pointer comb = meVTKPolyLineCombine::New();
						comb->SetInput( this->intermediateSkeleton );
						comb->CombineByCellId(mergeCellId1, mergeCellId2);
						comb->Update();
						this->intermediateSkeleton->DeepCopy( comb->GetOutput() );
						this->intermediateSkeleton->BuildCells();
						this->AnalyzePolyline();
				}
			} else {
				if (firstRun){
					nextStartingCell = 0;
					firstRun = false;
					cellsToMergeExist = true;
				}else{
					cellsToMergeExist = false;
				}
			}
		}		
	}
}

//--------------------------------------------------
void meVTKPolyLineClean::BeautifyPolyline()
//--------------------------------------------------
{
	meVTKPolyLineSmooth::Pointer smooth = meVTKPolyLineSmooth::New();
	smooth->SetInput( this->intermediateSkeleton );
	smooth->SetCellId( -1 );
	smooth->SetIterations( this->smoothingIerations );
	smooth->Update();
	this->intermediateSkeleton->DeepCopy( smooth->GetOutput() );
}
