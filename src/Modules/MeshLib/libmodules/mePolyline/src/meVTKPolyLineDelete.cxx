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

#include "meVTKPolyLineDelete.h"

// VTK
#include <vtkPoints.h>
#include <vtkPolyLine.h>
#include <vtkGenericCell.h>
#include <vtkMergePoints.h>

//STD
#include <map>
//--------------------------------------------------
meVTKPolyLineDelete::meVTKPolyLineDelete()
//--------------------------------------------------
{
	// Input initialized to do DeepCopy at SetInput function
	this->input = NULL;
	this->output = NULL;
	this->cellIds = NULL;
	this->pointId = -1;
}

//--------------------------------------------------
meVTKPolyLineDelete::~meVTKPolyLineDelete()
//--------------------------------------------------
{
}

void meVTKPolyLineDelete::DeleteCellWithId(vtkIdType cellId){
	if (this->cellIds!=NULL){
		cellIds->Delete();
	}
	this->cellIds=vtkIdList::New();
	cellIds->InsertUniqueId(cellId);
	this->pointId=-1;
}

void meVTKPolyLineDelete::DeleteCellsWithPoint(vtkIdType pointId){
	this->input->BuildLinks();
	if (this->cellIds!=NULL){
		cellIds->Delete();
	}
	this->cellIds = vtkIdList::New();
	this->input->GetPointCells(pointId,cellIds);
	this->pointId = pointId;
}

void meVTKPolyLineDelete::DeleteCells(vtkIdList *cellIds){
	this->cellIds = vtkIdList::New();
	this->cellIds->DeepCopy( cellIds );
}

//--------------------------------------------------
void meVTKPolyLineDelete::Update()
//--------------------------------------------------
{
	vtkPolyData *auxPD = vtkPolyData::New();
	typedef std::pair<vtkIdType, vtkIdType> PointPair;
	typedef std::map<vtkIdType, vtkIdType> PointMapType;
	vtkMergePoints *mergePoints = vtkMergePoints::New();
	vtkPoints *points = vtkPoints::New();
	PointMapType pointMap;
	mergePoints->InitPointInsertion(points, this->input->GetBounds(), this->input->GetNumberOfPoints());
	if (this->input!=NULL){
		//std::cout << "input polydata : " << *(this->input) << std::endl;
		auxPD->DeepCopy(this->input);
		//std::cout << "copied polydata : " << *auxPD << std::endl;
		auxPD->BuildCells();
		for (vtkIdType j = 0; j<cellIds->GetNumberOfIds(); j++){
			auxPD->DeleteCell(cellIds->GetId(j));
		}
		this->output = vtkPolyData::New();
		// for all the points in each cell that is not empty
		vtkGenericCell *auxCell = vtkGenericCell::New();
		vtkIdType oldId, newId;
		double pt[3];
		for (vtkIdType cellNum = 0;cellNum<auxPD->GetNumberOfCells();cellNum++){
			auxPD->GetCell(cellNum,auxCell);
			// copy the points to an intermediate point container
			if (auxCell->GetCellType()!=VTK_EMPTY_CELL){
				for (vtkIdType j = 0;j<auxCell->GetNumberOfPoints();j++){
					oldId = auxCell->GetPointId(j);
					auxPD->GetPoint(oldId,pt);
					mergePoints->InsertUniquePoint(pt,newId);
					pointMap.insert(PointPair(oldId,newId));
				}
			}
		}
		output->Allocate(auxPD,auxPD->GetNumberOfCells()-1,points->GetNumberOfPoints());
		output->SetPoints(points);
		vtkIdList *cellIds;
		for (vtkIdType cellNum = 0;cellNum<auxPD->GetNumberOfCells();cellNum++){
			auxPD->GetCell(cellNum,auxCell);
			cellIds = vtkIdList::New();
			cellIds->SetNumberOfIds(auxCell->GetNumberOfPoints());
			for (vtkIdType i = 0;i<auxCell->GetNumberOfPoints();i++){
				cellIds->SetId(i,pointMap[auxCell->GetPointId(i)]);
			}
			if (auxCell->GetCellType()!=VTK_EMPTY_CELL){
                output->InsertNextCell(auxCell->GetCellType(),cellIds);
			}
			cellIds->Delete();
		}
	}
}
