/*=========================================================================
  Library   : GeoLib
  Module    : geoMedial
  File      : ExtractContourDiamFromAneurCutPlane.cxx
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
              $Log: ExtractContourDiamFromAneurCutPlane.cxx,v $
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

//GeoLib
#include <meNormalPlaneToVessel.h>
#include <meCloseHoles.h>
#include <geoRemoveInnerContourFilter.h>

typedef struct {
	double center[3];
	double normal[3];
	double e1[3];
	double e2[3];
	double boundaryDisplacement;
} ResliceDataType;

void usage() 
{
	std::cerr << "Usage: ExtractContourDiamFromAneurCutPlane inputList inputMesh inputResliceData outputRootName [options]" << std::endl;
	std::cerr << "options: -nocdump [avoid dumping contour]" << std::endl;
	std::cerr << "options: -norm_inside [set that surface normals are pointing inside]" << std::endl;
	exit(1);
}

std::string itoa(int value, unsigned int base);
int centerInsideContour(vtkPolyData * contour, double pointCoordinates[3]);
void saveAlignedContour(vtkPolyData * contour, ResliceDataType resliceData, const char * filename);

int main(int argc, char **argv)
{
	if (argc < 5){
		usage();
	}

	bool ok;
	bool cdump = true;	// contour dump flag... enabled by default
	bool normInside = false;
	
	double maxD, minD;

	std::string outputRootFilename;	// pat10-3dra-1
	char *inputFilename=NULL;
	char *meshFilename=NULL;
	char *resliceFilename=NULL;

	double neck_normal[3], neck_center[3];	// for neck plane
	double normal[3], center[3];			// for dome plane
	double e1[3];
	int firstLabel=0;

	vtkPlane * plane = vtkPlane::New();
	vtkPlane * clipPlane = vtkPlane::New();
	vtkCutter * cutPlane = vtkCutter::New();
	vtkClipPolyData * clip = vtkClipPolyData::New();
	vtkPolyData * aux = vtkPolyData::New();
	vtkPolyData * mesh = vtkPolyData::New();
	vtkPolyData * meshClipped = vtkPolyData::New();
	vtkPolyData * contour = vtkPolyData::New();
	vtkPolyData * contourMeasure = vtkPolyData::New();

	
    ofstream diametersFile;

	for (int i=0; i<3; i++)
		e1[i] = 0;	// initialization

	inputFilename = argv[1];
	argc--; argv++;

	meshFilename = argv[1];
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
		if ((ok == false) && (strcmp(argv[1], "-norm_inside")==0))
		{
			argc--;
			argv++;
			normInside = true;
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

	ResliceDataType resliceData;
	ifstream resliceDataFile(resliceFilename,ios::binary);
	if (!resliceDataFile) {
		std::cerr << "Problem opening file with resliceData"<< std::endl;
		exit(1);
	}

	vtkPolyDataWriter * writerC = vtkPolyDataWriter::New();


	vtkPolyDataReader * polyDataReader = vtkPolyDataReader::New();
	polyDataReader->SetFileName(meshFilename);
	polyDataReader->Update();

	// this part caused crashes on my mesh
//	meCloseHoles::Pointer closer = meCloseHoles::New();
//	closer->SetInput(polyDataReader->GetOutput());
//	closer->Update();
//	mesh->DeepCopy(closer->GetOutput());
	mesh->DeepCopy(polyDataReader->GetOutput());

	polyDataReader->Delete();

	
	diametersFile.open( (outputRootFilename+"-Diameters.txt").c_str());
	if (!diametersFile) {
		std::cerr << "Problem opening output diameters file"<< std::endl;
		exit(1);
	}

	char buffer[256];
	int p=0; // main counter

	// First read the neck plane
	inputPlanes.getline(buffer,256);
	sscanf(buffer,"Point 	= ( %lf , %lf , %lf )", &(neck_center[0]), &(neck_center[1]), &(neck_center[2]));
	inputPlanes.getline(buffer,256);
	sscanf(buffer,"Normal = ( %lf , %lf , %lf )", &(neck_normal[0]), &(neck_normal[1]), &(neck_normal[2]));
	std::cout << neck_center[0] << neck_center[1] << neck_center[2] << std::endl;
	std::cout << neck_normal[0] << neck_normal[1] << neck_normal[2] << std::endl;

	resliceDataFile.read( (char*) &resliceData, sizeof (resliceData) );


/*
	// cut the plane
	plane->SetOrigin(neck_center);
	plane->SetNormal(neck_normal);

	cutPlane->CreateDefaultLocator();
	cutPlane->SetInput(mesh);
	cutPlane->SetCutFunction(plane);
	cutPlane->Update();

	writerC->SetInput(cutPlane->GetOutput());
	writerC->SetFileName((outputRootFilename+"-InitContour_0"+".vtk").c_str());
	writerC->Update();
*/

	// EXTRACT NECK MEASURES
	meNormalPlaneToVessel::Pointer normalCorrector = meNormalPlaneToVessel::New();
	vtkTransformPolyDataFilter * filter = vtkTransformPolyDataFilter::New();
	vtkTransform * transf = vtkTransform::New();

	normalCorrector->SetSurfaceMesh(mesh);
	normalCorrector->SetClosedSurfaceMesh(mesh);
	normalCorrector->SetNormal(neck_normal);
	normalCorrector->SetCenter(neck_center);
	normalCorrector->GetConnectedCutPlane(neck_normal);		
	normalCorrector->ComputeMaxAndMinDiameters();
	normalCorrector->GetMaxAndMinDiameters(maxD,minD);
	contour->DeepCopy(normalCorrector->GetContourCrossSection());
	if (centerInsideContour(contour,neck_center))
		diametersFile << maxD << " " << minD << std::endl;
	else
		diametersFile << -1 << " " << -1 << std::endl;



	filter->SetInput(contour);
	filter->SetTransform(transf);
	filter->Update();
	contour->DeepCopy(filter->GetOutput());		

	if (cdump) {
		writerC->SetInput(contour);
		writerC->SetFileName((outputRootFilename+"-Contour_1.vtk").c_str());
		writerC->Update();
		saveAlignedContour(contour,resliceData,(outputRootFilename+"-Contour_1-Aligned.vtk").c_str());

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

		saveAlignedContour(contourMeasure,resliceData,(outputRootFilename + "-Contour_1-AlignedMeasurements.vtk").c_str());

		line->Delete();
		points->Delete();

	}
	/*
*/
	// --------


	// Clip the mesh using the neck plane
	clipPlane->SetOrigin(neck_center);
	clipPlane->SetNormal(neck_normal);
	clip->SetInput(mesh);
	clip->SetClipFunction(clipPlane);
	clip->Update();
	meshClipped->DeepCopy(clip->GetOutput());
	clip->Delete();
	clipPlane->Delete();

	
	firstLabel=1;

	// Go through the dome cutting planes
	while (!inputPlanes.eof()) {

		p++;
		
		inputPlanes.getline(buffer,256);
		sscanf(buffer,"Point 	= ( %lf , %lf , %lf )", &(center[0]), &(center[1]), &(center[2]));
		inputPlanes.getline(buffer,256);
		sscanf(buffer,"Normal = ( %lf , %lf , %lf )", &(normal[0]), &(normal[1]), &(normal[2]));
		
		resliceDataFile.read( (char*) &resliceData, sizeof (resliceData) );
		
		std::cout << center[0] << center[1] << center[2] << std::endl;
		std::cout << normal[0] << normal[1] << normal[2] << std::endl;

		// cut the plane
		plane->SetOrigin(center);
		plane->SetNormal(normal);

		cutPlane->CreateDefaultLocator();
		cutPlane->SetInput(meshClipped);
		cutPlane->SetCutFunction(plane);
		cutPlane->Update();

		//writerC->SetInput(cutPlane->GetOutput());
		//writerC->SetFileName((outputRootFilename+"-InitContour_"+itoa(p+firstLabel,10) + ".vtk").c_str());
		//writerC->Update();

		geoAPI::geoRemoveInnerContourFilter *removeInnerContourFilter = geoAPI::geoRemoveInnerContourFilter::New();
		removeInnerContourFilter->SetInput(cutPlane->GetOutput());
		removeInnerContourFilter->SetCenter(center);
		removeInnerContourFilter->SetDirection(resliceData.e1);
		if (normInside)
			removeInnerContourFilter->SetNormPointInside();
		removeInnerContourFilter->Update();

		// extract the contour which is closest to the center point (neck point)
		vtkPolyDataConnectivityFilter *connec = vtkPolyDataConnectivityFilter::New();
		connec->SetInput(removeInnerContourFilter->GetOutput());
		//connec->SetInput(cutPlane->GetOutput());
		connec->SetExtractionModeToClosestPointRegion();
		connec->SetClosestPoint(center);
		connec->Update();
		contour->DeepCopy(connec->GetOutput());
		connec->Delete();	
		removeInnerContourFilter->Delete();
		
		
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
		diametersFile << distMax << "  ";
		//diametersFile << neck_center[0] << " " << neck_center[1] << " " << neck_center[2] << " " << maxDistPoint[0] << " " << maxDistPoint[1] << " " << maxDistPoint[2] << std::endl;
		// ------	

		// EXTRACT MAX WIDTH (paralel to the neck plane)
		
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
		diametersFile << distWidth << std::endl;
		//diametersFile << maxWidthPoint1[0] << " " << maxWidthPoint1[1] << " " << maxWidthPoint1[2] << " " << maxWidthPoint2[0] << " " << maxWidthPoint2[1] << " " << maxWidthPoint2[2] << std::endl;
		// ------

		if (cdump) {
			writerC->SetInput(contour);
			writerC->SetFileName((outputRootFilename+"-Contour_"+itoa(p+firstLabel,10) + ".vtk").c_str());
			writerC->Update();
			saveAlignedContour(contour,resliceData,(outputRootFilename + "-Contour_" + itoa(p+firstLabel,10) + "-Aligned.vtk").c_str());		
		
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

			saveAlignedContour(contourMeasure,resliceData,(outputRootFilename + "-Contour_" + itoa(p+firstLabel,10) + "-AlignedMeasurements.vtk").c_str());
		
			line->Delete();
			points->Delete();
		}

	}
	
	writerC->Delete();
	mesh->Delete();
	meshClipped->Delete();
	aux->Delete();
	contour->Delete();
	contourMeasure->Delete();
	plane->Delete();
	cutPlane->Delete();

	resliceDataFile.close();
	diametersFile.close();
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


	double     contourPoint[3],
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
	for(int i=0; i<3; i++) angle += pow(bivectorAngle[i],2);
	//cout << "angle = " << angle << endl;
	if(angle > 9)//39) // 39.478 is (2pi)2.
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