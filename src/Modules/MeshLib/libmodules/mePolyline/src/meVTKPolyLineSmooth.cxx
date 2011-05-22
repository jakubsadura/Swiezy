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

#include "meVTKPolyLineSmooth.h"

// VTK
#include <vtkPoints.h>
#include <vtkPolyLine.h>
#include <vtkGenericCell.h>
#include <vtkMergePoints.h>

//STD
#include <map>
//--------------------------------------------------
meVTKPolyLineSmooth::meVTKPolyLineSmooth()
//--------------------------------------------------
{
	// Input initialized to do DeepCopy at SetInput function
	this->input = NULL;
	this->output = NULL;
	this->cellId = -1;
	this->iters = -1;
}

//--------------------------------------------------
meVTKPolyLineSmooth::~meVTKPolyLineSmooth()
//--------------------------------------------------
{
}

void meVTKPolyLineSmooth::SmoothCell(vtkIdType cId){
	vtkIdType Npts;
	vtkGenericCell *cell = vtkGenericCell::New();
	this->output->GetCell(cId,cell);
	Npts = cell->GetNumberOfPoints();
	vtkIdType j;
	double avgPos[3], auxPos[3];
	for (unsigned int it = 0; it<this->iters; it++){
		avgPos[0]=0;avgPos[1]=0;avgPos[2]=0;
		for(j=1; j<Npts-1; j++){
			output->GetPoint(cell->GetPointId(j-1),auxPos);
			avgPos[0]=auxPos[0];avgPos[1]=auxPos[1];avgPos[2]=auxPos[2];
			output->GetPoint(cell->GetPointId(j),auxPos);
			avgPos[0]+=(2*auxPos[0]);avgPos[1]+=(2*auxPos[1]);avgPos[2]+=(2*auxPos[2]);
			output->GetPoint(cell->GetPointId(j+1),auxPos);
			avgPos[0]+=auxPos[0];avgPos[1]+=auxPos[1];avgPos[2]+=auxPos[2];
			avgPos[0]/=4;avgPos[1]/=4;avgPos[2]/=4;
			output->GetPoints()->SetPoint(cell->GetPointId(j),avgPos);
		}
	}
	return;
}

//--------------------------------------------------
void meVTKPolyLineSmooth::Update()
//--------------------------------------------------
{
	if (this->input!=NULL){
		if (output!=NULL){
			output->Delete();
		}
		output = vtkPolyData::New();
		output->DeepCopy(this->input);
		if (this->iters!=-1){
			if (this->cellId==-1) {
				vtkIdType totalCells = output->GetNumberOfCells();
				for (vtkIdType  i=0; i<totalCells; i++){
					SmoothCell(i);
				}
			}else{
				SmoothCell(this->cellId);
			}
		}else{
			std::cout << "Number of iteration must be set." << std::endl;
		}
	}	
}
