/*=========================================================================
Library   : GeoLib
Module    : geoMedial
File      : ExtractContourDiamFromContour.cxx
Authors   : hbogunovic (Hrvoje Bogunovic)
jpozo (José María Pozo Soler);
(C)opyright 2008
See COPYRIGHT statement in top level directory.
Purpose   : Scaled down version of ExtractImagesNormal2Centerline2. Which takes
as an input just a sequence of points and normals (planes) and calculates 
min and max diameters.
Date      : $Date: 2008/04/08 11:45:09 $
Version   : $Revision: 1.0 $
Changes   : $Locker:  $
$Log: ExtractContourDiamFromCutPlane.cxx,v $
=========================================================================*/


//VTK lib
#include <vtkErrorCode.h>
#include <vtkPolyData.h>
#include <vtkPolyDataReader.h>
#include <vtkPolyDataWriter.h>
#include <vtkClipPolyData.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkCellArray.h>
#include <vtkCellLocator.h>
#include <vtkPlane.h>
#include <vtkLine.h>
#include <vtkCutter.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>

//Standard
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

//MeshLib
#include <meNormalPlaneToVessel.h>
//#include <meCloseHoles.h>

typedef struct {
	double center[3];
	double normal[3];
	double e1[3];
	double e2[3];
	double boundaryDisplacement;
} ResliceDataType;

void usage() 
{
	std::cerr << "Usage: ExtractContourDiamFromContour inputList inputContour type inputResliceData outputRootName [options]" << std::endl;
	std::cerr << "options: -nocdump [avoid dumping contour]"<< std::endl;
	exit(1);
}

std::string itoa(int value, unsigned int base);
int centerInsideContour(vtkPolyData * contour, double pointCoordinates[3]);
void saveAlignedContour(vtkPolyData * contour, ResliceDataType resliceData, const char * filename);

int main(int argc, char **argv)
{
	if (argc < 6){
		usage();
	}

	bool ok;
	bool cdump = true;	// contour dump flag... enabled by default

	double maxD, minD;

	std::string outputRootFilename;	// pat10-3dra-1
	char *inputFilename=NULL;
	char *contourFilename=NULL;
	char *resliceFilename=NULL;

	double neck_normal[3], neck_center[3];	// for neck plane
	double normal[3], center[3];			// for dome plane
	double normal2[3], center2[3];			// for dome plane
	double normal3[3], center3[3];			// for dome plane
	double e1[3];
	int firstLabel=0;
	int contType = 1;	// 1 - neck, 2,3 - dome

	vtkPlane * plane = vtkPlane::New();
	vtkCutter * cutPlane = vtkCutter::New();
	vtkPolyData * aux = vtkPolyData::New();
	vtkPolyData * contour = vtkPolyData::New();
	vtkPolyData * contourMeasure = vtkPolyData::New();

	for (int i=0; i<3; i++)
		e1[i] = 0;	// initialization

	inputFilename = argv[1];
	argc--; argv++;

	contourFilename = argv[1];
	argc--; argv++;

	contType = atoi(argv[1]);
	argc--; argv++;

	// file where the parameters used in reslicing are stored
	resliceFilename = argv[1];
	argc--; argv++;

	outputRootFilename = argv[1];
	argc--; argv++;


	while (argc > 1)
	{
		ok = false;
		//Options:
		if ((ok == false) && (strcmp(argv[1], "-nocdump")==0))
		{
			argc--;
			argv++;
			cdump = false;
			ok = true;
		}
		if (ok == false){
			cerr << "Can not parse argument " << argv[1] << endl;
			usage();
		}
	}

	ifstream inputPlanes(inputFilename);
	if (!inputPlanes) {
		std::cerr << "Problem opening file with centers and normals"<< std::endl;
		exit(1);
	}

	ResliceDataType resliceData1,resliceData2,resliceData3;
	ifstream resliceDataFile(resliceFilename,ios::binary);
	if (!resliceDataFile) {
		std::cerr << "Problem opening file with resliceData"<< std::endl;
		exit(1);
	}
	resliceDataFile.read( (char*) &resliceData1, sizeof (resliceData1) );
	resliceDataFile.read( (char*) &resliceData2, sizeof (resliceData2) );
	resliceDataFile.read( (char*) &resliceData3, sizeof (resliceData3) );
	std::cout << "Reslice Data" << std::endl;
	std::cout << resliceData2.center[0] << " " << resliceData2.center[1] << " " << resliceData2.center[2] << std::endl; 
	std::cout << resliceData2.e1[0] << " " << resliceData2.e1[1] << " " << resliceData2.e1[2] << std::endl; 

	vtkPolyDataWriter * writerC = vtkPolyDataWriter::New();


	vtkPolyDataReader * polyDataReader = vtkPolyDataReader::New();
	polyDataReader->SetFileName(contourFilename);
	polyDataReader->Update();

	// this part caused crashes on my mesh
	//	meCloseHoles::Pointer closer = meCloseHoles::New();
	//	closer->SetInput(polyDataReader->GetOutput());
	//	closer->Update();
	//	mesh->DeepCopy(closer->GetOutput());
	contour->DeepCopy(polyDataReader->GetOutput());

	polyDataReader->Delete();


	char buffer[256];
	int p=0; // main counter

	// First read the neck plane
	inputPlanes.getline(buffer,256);
	sscanf(buffer,"Point 	= ( %lf , %lf , %lf )", &(neck_center[0]), &(neck_center[1]), &(neck_center[2]));
	inputPlanes.getline(buffer,256);
	sscanf(buffer,"Normal = ( %lf , %lf , %lf )", &(neck_normal[0]), &(neck_normal[1]), &(neck_normal[2]));
	std::cout << neck_center[0] << neck_center[1] << neck_center[2] << std::endl;
	std::cout << neck_normal[0] << neck_normal[1] << neck_normal[2] << std::endl;

	inputPlanes.getline(buffer,256);
	sscanf(buffer,"Point 	= ( %lf , %lf , %lf )", &(center2[0]), &(center2[1]), &(center2[2]));
	inputPlanes.getline(buffer,256);
	sscanf(buffer,"Normal = ( %lf , %lf , %lf )", &(normal2[0]), &(normal2[1]), &(normal2[2]));
	std::cout << center2[0] << center2[1] << center2[2] << std::endl;
	std::cout << normal2[0] << normal2[1] << normal2[2] << std::endl;

	inputPlanes.getline(buffer,256);
	sscanf(buffer,"Point 	= ( %lf , %lf , %lf )", &(center3[0]), &(center3[1]), &(center3[2]));
	inputPlanes.getline(buffer,256);
	sscanf(buffer,"Normal = ( %lf , %lf , %lf )", &(normal3[0]), &(normal3[1]), &(normal3[2]));
	std::cout << center3[0] << center3[1] << center3[2] << std::endl;
	std::cout << normal3[0] << normal3[1] << normal3[2] << std::endl;

	meNormalPlaneToVessel::Pointer normalCorrector = meNormalPlaneToVessel::New();

	if ( contType == 1 ) {
		// EXTRACT NECK MEASURES

		vtkTransformPolyDataFilter * filter = vtkTransformPolyDataFilter::New();
		vtkTransform * transf = vtkTransform::New();

		normalCorrector->SetConnectedCutPlane(contour);
		normalCorrector->ComputeMaxAndMinDiameters();
		normalCorrector->GetMaxAndMinDiameters(maxD,minD);
		//contour->DeepCopy(normalCorrector->GetContourCrossSection());
		std::cout << minD <<std::endl;

		double *points1;
		double *points2;
		points1 = normalCorrector->GetMinDiameterPoints1();
		points2 = normalCorrector->GetMinDiameterPoints2();
		double sum = 0;
		for (int i=0;i<3;i++)
			sum += pow(points1[i]-points2[i],2);
		minD = sqrt(sum);
		
		if (centerInsideContour(contour,neck_center))
			std::cout << maxD << " " << minD << std::endl;
		else
			std::cout << -1 << " " << -1 << std::endl;

		filter->SetInput(contour);
		filter->SetTransform(transf);
		filter->Update();
		contour->DeepCopy(filter->GetOutput());		

		if (cdump) {
			saveAlignedContour(contour,resliceData1,(outputRootFilename+"-Contour_1-Aligned.vtk").c_str());

			vtkCellArray* line = vtkCellArray::New();
			vtkPoints *points = vtkPoints::New();
	
			int currPointID = 0;
			points->InsertNextPoint(normalCorrector->GetMaxDiameterPoints1());
			points->InsertNextPoint(normalCorrector->GetMaxDiameterPoints2());
			points->InsertNextPoint(normalCorrector->GetMinDiameterPoints1());
			points->InsertNextPoint(normalCorrector->GetMinDiameterPoints2());
		
			contourMeasure->SetPoints(points);

			// Add the measurements to the contours
			line->InsertNextCell(2);
			line->InsertCellPoint(currPointID);
			line->InsertCellPoint(currPointID+1);
			line->InsertNextCell(2);
			line->InsertCellPoint(currPointID+2);
			line->InsertCellPoint(currPointID+3);
			contourMeasure->SetLines(line);

			saveAlignedContour(contourMeasure,resliceData1,(outputRootFilename + "-Contour_1-AlignedMeasurements.vtk").c_str());
	
			line->Delete();
			points->Delete();
	
		}
	} else {
		if (contType == 2 )
			for (int i=0;i<3;i++) {
				center[i] = center2[i];
				normal[i] = normal2[i];
			}
		else
			for (int i=0;i<3;i++) {
				center[i] = center3[i];
				normal[i] = normal3[i];
			}

		// EXTRACT MAX DEPTH
		double tmpPoint[3], maxDistPoint[3];
		double distMax = 0, distAux = 0;
		int numPoints = contour->GetNumberOfPoints();
		//now traverse through all the points and find the one farthest away from center point
		for(int i=0; i< numPoints; i++)
		{
			contour->GetPoint(i, tmpPoint);
			distAux = sqrt( pow(tmpPoint[0]-neck_center[0],2) + pow(tmpPoint[1]-neck_center[1],2) + pow(tmpPoint[2]-neck_center[2],2) );
			if (distAux > distMax) {
				distMax = distAux;
				maxDistPoint[0] = tmpPoint[0];
				maxDistPoint[1] = tmpPoint[1];
				maxDistPoint[2] = tmpPoint[2];
			}
		}
		std::cout << distMax << "  ";
		//diametersFile << neck_center[0] << " " << neck_center[1] << " " << neck_center[2] << " " << maxDistPoint[0] << " " << maxDistPoint[1] << " " << maxDistPoint[2] << std::endl;
		// ------	

		// EXTRACT MAX WIDTH (parallel to the neck plane)

		int j = 0;
		int intersect_pointNbr=0;
		int intervalNbr = 0;
		int point1Id = 0;
		int point2Id = 0;
		double plane_center[3];
		double step = 0.1; // [mm]
		double tmpPoint1[3], tmpPoint2[3], maxWidthPoint1[3], maxWidthPoint2[3];
		double distWidth = 0;

		while (j<5000) {// 5000 is a safe boundary in case the loop does not end

			j++;

			// Progress in the direction of neck normal from the neck and cut the contour with the surface
			for(int i=0; i<3; i++) {
				plane_center[i] = neck_center[i] + j*step*neck_normal[i];
			}

			plane->SetOrigin(plane_center);
			plane->SetNormal(neck_normal);

			cutPlane->CreateDefaultLocator();
			cutPlane->SetInput(contour);
			cutPlane->SetCutFunction(plane);
			cutPlane->Update();
			aux->DeepCopy(cutPlane->GetOutput());

			intersect_pointNbr = aux->GetNumberOfPoints();
			if (intersect_pointNbr == 0)
				break;	// we finish, we are out of the contour

			// number of intersections has to be even -> because contour is closed
			intervalNbr = intersect_pointNbr/2;
			// take the points in pairs 0-1, 2-3, 4-5 ...
			for ( int k=0; k < intervalNbr; k++ ) {

				point1Id = 2*k;
				point2Id = 2*k+1;
				aux->GetPoint( point1Id, tmpPoint1 );
				aux->GetPoint( point2Id, tmpPoint2 );
				distAux = sqrt( pow(tmpPoint1[0]-tmpPoint2[0],2) + pow(tmpPoint1[1]-tmpPoint2[1],2) + pow(tmpPoint1[2]-tmpPoint2[2],2) );

				if (distAux > distWidth) {

					distWidth = distAux;
					maxWidthPoint1[0] = tmpPoint1[0];
					maxWidthPoint1[1] = tmpPoint1[1];
					maxWidthPoint1[2] = tmpPoint1[2];
					maxWidthPoint2[0] = tmpPoint2[0];
					maxWidthPoint2[1] = tmpPoint2[1];
					maxWidthPoint2[2] = tmpPoint2[2];
					//if (cdump) {
					//	writerC->SetInput(aux);
					//	writerC->SetFileName((outputRootFilename + "-MaxWidthCut_" + itoa(p+firstLabel,10) + ".vtk").c_str());
					//	writerC->Update();
					//}

				}
			}
		}
		std::cout << distWidth << std::endl;
		//diametersFile << maxWidthPoint1[0] << " " << maxWidthPoint1[1] << " " << maxWidthPoint1[2] << " " << maxWidthPoint2[0] << " " << maxWidthPoint2[1] << " " << maxWidthPoint2[2] << std::endl;
		// ------

		if (cdump) {
			if (contType == 2) 
				saveAlignedContour(contour,resliceData2,(outputRootFilename + "-Contour_" + itoa(contType,10) + "-Aligned.vtk").c_str());		
			else
				saveAlignedContour(contour,resliceData3,(outputRootFilename + "-Contour_" + itoa(contType,10) + "-Aligned.vtk").c_str());		

			vtkCellArray* line = vtkCellArray::New();
			vtkPoints *points = vtkPoints::New();

			int currPointID = 0;
			points->InsertNextPoint(maxWidthPoint1);
			points->InsertNextPoint(maxWidthPoint2);
			points->InsertNextPoint(neck_center);
			points->InsertNextPoint(maxDistPoint);

			contourMeasure->SetPoints(points);

			// Add the measurements to the contours
			line->InsertNextCell(2);
			line->InsertCellPoint(currPointID);
			line->InsertCellPoint(currPointID+1);
			line->InsertNextCell(2);
			line->InsertCellPoint(currPointID+2);
			line->InsertCellPoint(currPointID+3);
			contourMeasure->SetLines(line);

			if (contType == 2)
				saveAlignedContour(contourMeasure,resliceData2,(outputRootFilename + "-Contour_" + itoa(contType,10) + "-AlignedMeasurements.vtk").c_str());
			else
				saveAlignedContour(contourMeasure,resliceData3,(outputRootFilename + "-Contour_" + itoa(contType,10) + "-AlignedMeasurements.vtk").c_str());

			line->Delete();
			points->Delete();
		}

	}

	// --------
	writerC->Delete();
	aux->Delete();
	contour->Delete();
	contourMeasure->Delete();
	plane->Delete();
	cutPlane->Delete();

	resliceDataFile.close();
	inputPlanes.close();

	return 0;
}



/**
* C++ version std::string style "itoa":
*/
std::string itoa(int value, unsigned int base)
{
	const char digitMap[] = "0123456789abcdef";
	std::string buf;

	// Guard:
	if (base == 0 || base > 16)
	{
		// Error: may add more trace/log output here
		return buf;
	}

	// Take care of negative int:
	std::string sign;
	int _value = value;

	// Check for case when input is zero:
	if (_value == 0)
		return "0";
	if (value < 0)
	{
		_value = -value;
		sign = "-";
	}

	// Translating number to string with base:
	for (int i = 30; _value && i ; --i)
	{
		buf = digitMap[ _value % base ] + buf;
		_value /= base;
	}

	return sign.append(buf);
}

int centerInsideContour(vtkPolyData * contour, double pointCoordinates[3]) {


	double contourPoint[3],
		contourNormal[3],
		firstPoint[3],
		secondPoint[3],
		thirdPoint[3],
		contourNormalNorm = 0,
		angle;

	contour->GetPoint(contour->GetCell(0)->GetPointId(0),contourPoint);
	contour->GetPoint(contour->GetCell(0)->GetPointId(1),secondPoint);
	if(contour->GetCell(1)->GetPointId(0) == contour->GetCell(0)->GetPointId(1))
	{
		contour->GetPoint(contour->GetCell(1)->GetPointId(1),thirdPoint);
	}
	else
	{
		contour->GetPoint(contour->GetCell(1)->GetPointId(0),thirdPoint);
	}

	double contourCellNumber = contour->GetNumberOfCells();

	for(int i=0; i<3; i++)
	{
		secondPoint[i] -= contourPoint[i];
		thirdPoint[i] -= contourPoint[i];
	}

	contourNormal[0] = secondPoint[1]*thirdPoint[2] - secondPoint[2]*thirdPoint[1];
	contourNormal[1] = secondPoint[2]*thirdPoint[0] - secondPoint[0]*thirdPoint[2];
	contourNormal[2] = secondPoint[0]*thirdPoint[1] - secondPoint[1]*thirdPoint[0];

	for(int i=0; i<3; i++) contourNormalNorm += pow(contourNormal[i],2);
	contourNormalNorm = sqrt(contourNormalNorm);
	for(int i=0; i<3; i++) contourNormal[i] = contourNormal[i]/contourNormalNorm;





	double bivectorAngle[3], squareRadius;

	for(int i=0; i<3; i++)
	{
		thirdPoint[i] = pointCoordinates[i]; //- currentSign*contourNormal[i];
		bivectorAngle[i] = 0;
	}
	//cout << "El numero de cells es " << contourCellNumber << endl;
	for(int c=0; c<contourCellNumber; c++)
	{
		contour->GetPoint(contour->GetCell(c)->GetPointId(0),firstPoint);
		contour->GetPoint(contour->GetCell(c)->GetPointId(1),secondPoint);
		for(int i=0; i<3; i++)
		{
			secondPoint[i] -= firstPoint[i];
			firstPoint[i] -= thirdPoint[i];
		}
		squareRadius = 0;
		for(int i=0; i<3; i++) squareRadius += pow(firstPoint[i],2);
		bivectorAngle[0] += (firstPoint[1]*secondPoint[2]-firstPoint[2]*secondPoint[1])/squareRadius;
		bivectorAngle[1] += (firstPoint[2]*secondPoint[0]-firstPoint[0]*secondPoint[2])/squareRadius;
		bivectorAngle[2] += (firstPoint[0]*secondPoint[1]-firstPoint[1]*secondPoint[0])/squareRadius;
	}
	cout << "square " << squareRadius << endl;


	angle = 0;
	for (int i=0; i<3; i++) angle += pow(bivectorAngle[i],2);
	//cout << "angle = " << angle << endl;
	if (angle > 9)//39) // 39.478 is (2pi)2.
	{
		return 1;
	} else {
		return 0;
	}
}

void saveAlignedContour(vtkPolyData * contour, ResliceDataType resliceData, const char * filename) {
	int numPointsContour = contour->GetNumberOfPoints();
	double pointCoord[3];
	double pointAligned[3];

	for(int i=0; i<numPointsContour; i++)
	{
		contour->GetPoint(i,pointCoord);

		for(int j=0; j<3; j++) 
			pointAligned[j] = 0;

		for(int j=0; j<3; j++)
		{
			pointAligned[0] += (pointCoord[j]-resliceData.center[j])*resliceData.e1[j];
			pointAligned[1] += (pointCoord[j]-resliceData.center[j])*resliceData.e2[j];
			pointAligned[2] += (pointCoord[j]-resliceData.center[j])*resliceData.normal[j];
		}
		pointAligned[0] += resliceData.boundaryDisplacement;
		pointAligned[1] += resliceData.boundaryDisplacement;
		contour->GetPoints()->SetPoint(i,pointAligned);
	}

	vtkPolyDataWriter * writerC = vtkPolyDataWriter::New();	
	writerC->SetInput(contour);
	writerC->SetFileName(filename);
	writerC->Update();
	writerC->Delete();

}