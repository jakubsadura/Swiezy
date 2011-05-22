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

#include "meVTKPolyLineDecimate.h"

// VTK
#include <vtkPoints.h>
#include <vtkPolyLine.h>
#include <vtkGenericCell.h>
#include <vtkMergePoints.h>

//STD
#include <map>
//--------------------------------------------------
meVTKPolyLineDecimate::meVTKPolyLineDecimate()
//--------------------------------------------------
{
	// Input initialized to do DeepCopy at SetInput function
	this->input = NULL;
	this->output = NULL;
	this->cellId = -1;
	this->perc = -1.0;
}

//--------------------------------------------------
meVTKPolyLineDecimate::~meVTKPolyLineDecimate()
//--------------------------------------------------
{
}

double meVTKPolyLineDecimate::Dist(double *p1, double *p2){
	double dst[3], auxLen;
	dst[0] = p1[0]- p2[0];dst[1] = p1[1]- p2[1];dst[2] = p1[2]- p2[2];
	auxLen = (dst[0]*dst[0])+(dst[1]*dst[1])+(dst[2]*dst[2]);
	return sqrt(auxLen);
}

//--------------------------------------------------
void meVTKPolyLineDecimate::Update()
//--------------------------------------------------
{
	if (this->input!=NULL){
		if (output!=NULL){
			output->Delete();
		}
		output = vtkPolyData::New();
		output->DeepCopy(this->input);
		if ((this->cellId!=-1) && (this->perc!=-1)){
			if ((this->perc>0.0) && (this->perc<=1.0)){
				vtkIdType Npts, newNpts;
				//vtkPoints *newPoints = vtkPoints::New();
				vtkGenericCell *cell = vtkGenericCell::New();
				output->GetCell(this->cellId,cell);
				Npts = cell->GetNumberOfPoints();
				double cellLen, auxLen, newPtDist;
				double pos[3], posm1[3], dst[3];
				vtkIdType j, jm1;
				output->GetPoint(cell->GetPointId(0),pos);
				cellLen = 0.0;
				for(j=1; j<Npts; j++){
					output->GetPoint(cell->GetPointId(j),posm1);
					auxLen = this->Dist(pos, posm1);
                    cellLen += auxLen;
				}
				//because the first and last points in the centerline will always be fixed
				newNpts = ceil((Npts-2)*perc)+2;

				// esto lo deje medio colgado, todavia lo tengo que trabajar un poco, 
				// que hago coon las celdas que estan conectadas a esta y les remuevo los puntos?
				// como distribuyo los nuevos puntos?

				//// new distance between points
				//newPtDist = cellLen/newNpts;
				//newPoints->Allocate(newNpts);
				//double walkedOverCell = 0.0;
				//j = 0;
				//jm1 = 1;
				//while (walkedOverCell<cellLen){
				//	output->GetPoint(cell->GetPointId(j),pos);
				//	output->GetPoint(cell->GetPointId(jm1),posm1);
				//}
			}else{
				std::cout << "Percentage of decimation should be between 0.0 and 1.0." << std::endl;
			}
		}else{
			std::cout << "Cell Id and decimation percentage must be set." << std::endl;
		}
	}	
}
