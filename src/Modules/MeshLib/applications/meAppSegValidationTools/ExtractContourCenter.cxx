/*=========================================================================
Library   : GeoLib
Module    : geoMedial
File      : ExtractContourCenter.cxx
Authors   : hbogunovic (Hrvoje Bogunovic)
jpozo (José María Pozo Soler);
(C)opyright 2008
See COPYRIGHT statement in top level directory.
Purpose   : Calculating the center of the closed contour.
Date      : $Date: 2008/04/08 11:45:09 $
Version   : $Revision: 1.0 $
Changes   : $Locker:  $
$Log: ExtractContourDiamFromCutPlane.cxx,v $
=========================================================================*/


//VTK lib
#include <vtkErrorCode.h>
#include <vtkPolyData.h>
#include <vtkPolyDataReader.h>
#include <vtkSmartPointer.h>
#include <vtkFeatureEdges.h>
#include <vtkIdList.h>

//Standard
#include <iostream>


void ComputeCenterOfMassAndNormal
(
 vtkPolyData* inputContour,
 double &outputSurface,
 double outputCM[3],
 double outputNormal[3]
 )
 {
	 outputSurface = 0;
	 for(int i=0; i<3; i++)
	 {
		 outputCM[i]=0;
		 outputNormal[i]=0;
	 }
	 vtkIdList * NeighbourCells = vtkIdList::New();
	 vtkIdList * FirstCell = vtkIdList::New();
	 vtkIdList * SecondCell = vtkIdList::New();
	 int 	numPoints,
		 FirstPointId,
		 SecondPointId,
		 FirstCellId,
		 SecondCellId;
	 double 	length,
		 area,
		 totalLength = 0,
		 origin[3],
		 FirstPoint[3],
		 SecondPoint[3];
	 for(int i=0; i<3; i++)
	 {
		 origin[i] = inputContour->GetPoint(0)[i];
	 }
	 inputContour->GetPointCells(0, NeighbourCells);
	 if( NeighbourCells->GetNumberOfIds() != 2 ) 
	 {
		 cout << "ERROR: The countour is not closed or is not simple" << endl;
	 }
	 FirstCellId = NeighbourCells->GetId(0);
	 inputContour->GetCellPoints(FirstCellId, FirstCell);
	 if( FirstCell->GetId(0) == 0 )
	 {
		 FirstPointId = FirstCell->GetId(1);
	 }
	 else
	 {
		 FirstPointId = FirstCell->GetId(0);
	 }
	 numPoints = inputContour->GetNumberOfPoints();
	 for(int p=2; p<numPoints; p++)  //
	 {
		 inputContour->GetPointCells(FirstPointId, NeighbourCells);
		 if( NeighbourCells->GetNumberOfIds() != 2 ) 
		 {
			 cout << "ERROR: The contour is not closed or is not simple" << endl;
		 }
		 SecondCellId = NeighbourCells->GetId(0);
		 if( SecondCellId == FirstCellId )
		 {
			 SecondCellId = NeighbourCells->GetId(1);
		 }
		 inputContour->GetCellPoints(SecondCellId, SecondCell);
		 if( SecondCell->GetId(0) == FirstPointId )
		 {
			 SecondPointId = SecondCell->GetId(1);
		 }
		 else
		 {
			 SecondPointId = SecondCell->GetId(0);
		 }
		 for(int i=0; i<3; i++)
		 {
			 FirstPoint[i] = inputContour->GetPoint(FirstPointId)[i]-origin[i];
			 SecondPoint[i] = inputContour->GetPoint(SecondPointId)[i]-origin[i];
		 }
		 outputNormal[0] += FirstPoint[1]*SecondPoint[2] - FirstPoint[2]*SecondPoint[1];   //
		 outputNormal[1] += FirstPoint[2]*SecondPoint[0] - FirstPoint[0]*SecondPoint[2];   //
		 outputNormal[2] += FirstPoint[0]*SecondPoint[1] - FirstPoint[1]*SecondPoint[0];	//

		 length = 0;
		 for(int i=0; i<3; i++) length+=pow(FirstPoint[i]-SecondPoint[i],2);
		 length = sqrt(length);
		 totalLength += length;
		 for(int i=0; i<3; i++) outputCM[i]+=length*(FirstPoint[i]+SecondPoint[i]);

		 FirstCellId = SecondCellId;
		 FirstPointId = SecondPointId;		
	 }
	 area = sqrt(pow(outputNormal[0],2)+pow(outputNormal[1],2)+pow(outputNormal[2],2));   //
	 for(int i=0; i<3; i++)
	 {
		 outputNormal[i]=outputNormal[i]/area;
		 outputCM[i]=outputCM[i]/(2*totalLength)+origin[i];
	 }
	 outputSurface = area/2;
 }

void usage() // TODO: Do.
{
	exit(1); 
}

std::string itoa(int value, unsigned int base);

int main(int argc, char **argv)
{
	if (argc < 2){
		usage();
	}
	
	char *contourFilename=NULL;
//	vtkPolyData *contour = vtkPolyData::New();
	double surfaceArea, centerOfMass[3], normal[3];
	
	contourFilename = argv[1];
	argc--; argv++;
	
	vtkPolyDataReader * polyDataReader = vtkPolyDataReader::New();
	polyDataReader->SetFileName(contourFilename);
	polyDataReader->Update();

	ComputeCenterOfMassAndNormal(polyDataReader->GetOutput(), surfaceArea, centerOfMass, normal);

	std::cout << "Contour's CenterOfMass: " << centerOfMass[0] << ", " << centerOfMass[1] << ", " << centerOfMass[2] << std::endl;
	std::cout << "Contour's Normal: " << normal[0] << ", " << normal[1] << ", " << normal[2] << std::endl;
		
	return 0;
}