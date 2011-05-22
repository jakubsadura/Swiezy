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

#include "meVTKPolyLineCombine.h"

// VTK
#include <vtkPoints.h>
#include <vtkPolyLine.h>
#include <vtkGenericCell.h>

//--------------------------------------------------
meVTKPolyLineCombine::meVTKPolyLineCombine()
//--------------------------------------------------
{
	// Input initialized to do DeepCopy at SetInput function
	this->input = NULL;
	this->output = NULL;
	this->cellId1 = -1;
	this->cellId2 = -1;
}

//--------------------------------------------------
meVTKPolyLineCombine::~meVTKPolyLineCombine()
//--------------------------------------------------
{
}

//--------------------------------------------------
void meVTKPolyLineCombine::Update()
//--------------------------------------------------
{
	//std::cout << "meVTKPolyLineCombine::Update()" << std::endl;
	if (this->input!=NULL && this->cellId1!=-1 && this->cellId2!=-1){
		this->output = vtkPolyData::New();

		//copy the points from the input
		vtkPoints *points = vtkPoints::New();
		points->DeepCopy(this->input->GetPoints());

		output->Allocate(points->GetNumberOfPoints());
		output->SetPoints(points);

		vtkGenericCell *cell1 = vtkGenericCell::New();
		vtkGenericCell *cell2 = vtkGenericCell::New();
		vtkPolyLine *cellNew = vtkPolyLine::New();
		vtkCell *cellAux;
		//cell1 = this->input->GetCell(this->cellId1);
		//cell2 = this->input->GetCell(this->cellId2);
		this->input->GetCell(this->cellId1, cell1);
		this->input->GetCell(this->cellId2, cell2);
		if (cell1!=NULL && cell2!=NULL){
			if ((cell1->GetCellType()!=VTK_POLY_LINE)||(cell1->GetCellType()!=VTK_LINE) && 
				(cell2->GetCellType()!=VTK_POLY_LINE)||(cell2->GetCellType()!=VTK_LINE)){
				vtkIdType NPCell1 = cell1->GetNumberOfPoints();
				vtkIdType NPCell2 = cell2->GetNumberOfPoints();
				vtkIdType cell1Id0, cell1IdN, cell2Id0,cell2IdN; 
				cell1Id0 = cell1->GetPointId(0);
				cell1IdN = cell1->GetPointId(NPCell1-1);
				cell2Id0 = cell2->GetPointId(0);
				cell2IdN = cell2->GetPointId(NPCell2-1);
				cellNew->GetPointIds()->SetNumberOfIds(NPCell1+NPCell2-1);
				vtkIdType i, newCellPtId, auxId;
				newCellPtId = 0;
                if (cell1Id0 == cell2Id0){
					for (i=NPCell1-1;i>0;i--){
						auxId = cell1->GetPointId(i);
						cellNew->GetPointIds()->SetId(newCellPtId++,auxId);
					}
					for (i=0;i<NPCell2;i++){
						auxId = cell2->GetPointId(i);
						cellNew->GetPointIds()->SetId(newCellPtId++,auxId);
					}
				}else if (cell1Id0 == cell2IdN){
					for (i=NPCell1-1;i>0;i--){
						auxId = cell1->GetPointId(i);
						cellNew->GetPointIds()->SetId(newCellPtId++,auxId);
					}
					for (i=NPCell2-1;i>=0;i--){
						auxId = cell2->GetPointId(i);
						cellNew->GetPointIds()->SetId(newCellPtId++,auxId);
					}
				}else if (cell1IdN == cell2Id0){
					for (i=0;i<NPCell1-1;i++){
						auxId = cell1->GetPointId(i);
						cellNew->GetPointIds()->SetId(newCellPtId++,auxId);
					}
					for (i=0;i<NPCell2;i++){
						auxId = cell2->GetPointId(i);
						cellNew->GetPointIds()->SetId(newCellPtId++,auxId);
					}
				}else if (cell1IdN == cell2IdN){
					for (i=0;i<NPCell1-1;i++){
						auxId = cell1->GetPointId(i);
						cellNew->GetPointIds()->SetId(newCellPtId++,auxId);
					}
					for (i=NPCell2-1;i>=0;i--){
						auxId = cell2->GetPointId(i);
						cellNew->GetPointIds()->SetId(newCellPtId++,auxId);
					}
				}else{
					std::cout << "Polylines can only be combined from their beginning or end points" << std::endl;
					std::cout << "Cell 1 first and last point Ids : " << cell1Id0 << ", " << cell1IdN << std::endl;
					std::cout << "Cell 2 first and last point Ids : " << cell2Id0 << ", " << cell2IdN << std::endl;
					return;
				}
				for (vtkIdType auxCellId = 0;auxCellId < this->input->GetNumberOfCells();auxCellId++){ 
					cellAux = vtkPolyLine::New();
					cellAux->DeepCopy(this->input->GetCell(auxCellId));
					if (auxCellId!=this->cellId1 && auxCellId!=this->cellId2){ 
						output->InsertNextCell(cellAux->GetCellType(),cellAux->GetPointIds());
					}else if (auxCellId==this->cellId1){
						output->InsertNextCell(cellNew->GetCellType(),cellNew->GetPointIds());
					}
				}
			}else{
				std::cout << "Cell type not line(3) or polyline(4)" << std::endl;
				std::cout << "Cell 1 type : " << this->input->GetCellType(cellId1) << std::endl;
				std::cout << "Cell 2 type : " << this->input->GetCellType(cellId2) << std::endl;
			}
		}else{
			std::cout << "Specified cell does not exist" << std::endl;
			std::cout << "Cell Id 1 : " << cellId1 << ", cell pointer : " << cell1 << std::endl;
			std::cout << "Cell Id 2 : " << cellId2 << ", cell pointer : " << cell2 << std::endl;
		}
	}else{
		std::cout << "Cell ids not defined." << std::endl;
	}
}
