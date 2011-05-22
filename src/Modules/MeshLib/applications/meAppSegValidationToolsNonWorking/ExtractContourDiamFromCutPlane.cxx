/*=========================================================================
  Library   : GeoLib
  Module    : geoMedial
  File      : ExtractContourDiamFromCutPlane.cxx
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
#include <vtkImageData.h>
#include <vtkStructuredPoints.h>
#include <vtkImageReader.h>
#include <vtkStructuredPointsReader.h>
#include <vtkStructuredPointsWriter.h>
#include <vtkPolyData.h>
#include <vtkPolyDataReader.h>
#include <vtkPolyDataWriter.h>
#include <vtkClipPolyData.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkCellLocator.h>
#include <vtkPlane.h>
#include <vtkCutter.h>
#include <vtkClipPolyData.h>
#include <vtkCellArray.h>
#include <vtkPoints.h>
#include <vtkImageReslice.h>
#include <vtkImageWriter.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>

//Standard
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

//MeshLib
#include <meNormalPlaneToVessel.h>
#include <meCloseHoles.h>


void usage() // TODO: Do.
{
	std::cerr << "Usage: ExtractContourDiamFromCutPlane inputList inputMesh outputRootName [options]" << std::endl;
	std::cerr << "options: -nocdump [avoid dumping contour]"<< std::endl;
	exit(1); 
}

std::string itoa(int value, unsigned int base);
int centerInsideContour(vtkPolyData * contour, double pointCoordinates[3]);

int main(int argc, char **argv)
{
	if (argc < 4){
		usage();
	}

	bool ok;
	bool cdump = true;	// contour dump flag
	
	

	std::string outputRootFilename;	// pat10-3dra-1
	char *inputFilename=NULL;
	char *meshFilename=NULL;

	double normal[3], center[3], maxD, minD;
	double e1[3], e2[3], e1Aux[3];
	double continuitySign;
	int firstLabel=0;

	vtkPolyData * mesh = vtkPolyData::New();
	vtkPolyData * contour = vtkPolyData::New();

    ofstream diametersFile;

	for (int i=0; i<3; i++)
		e1[i] = 0;	// initialization

	inputFilename = argv[1];
	argc--; argv++;

	meshFilename = argv[1];
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

	double boundaryDisplacement = 7.5;

	meNormalPlaneToVessel::Pointer normalCorrector = meNormalPlaneToVessel::New();
	vtkTransformPolyDataFilter * filter = vtkTransformPolyDataFilter::New();
	vtkTransform * transf = vtkTransform::New();
	vtkPolyDataWriter * writerC = vtkPolyDataWriter::New();
	
	diametersFile.open( (outputRootFilename+"-Diameters.txt").c_str());
	if (!diametersFile) {
		std::cerr << "Problem opening output diameters file"<< std::endl;
		exit(1);
	}

	char buffer[256];
	int p=0; // main counter

	while (!inputPlanes.eof()) {

		p++;
		
		inputPlanes.getline(buffer,256);
		sscanf(buffer,"Point 	= ( %lf , %lf , %lf )", &(center[0]), &(center[1]), &(center[2]));
		inputPlanes.getline(buffer,256);
		sscanf(buffer,"Normal = ( %lf , %lf , %lf )", &(normal[0]), &(normal[1]), &(normal[2]));
		
		
		std::cout << center[0] << center[1] << center[2] << std::endl;
		std::cout << normal[0] << normal[1] << normal[2] << std::endl;
	
	
		normalCorrector->SetSurfaceMesh(mesh);
		normalCorrector->SetClosedSurfaceMesh(mesh);
		normalCorrector->SetNormal(normal);
		normalCorrector->SetCenter(center);
		normalCorrector->GetConnectedCutPlane(normal);		
		normalCorrector->ComputeMaxAndMinDiameters();

		normalCorrector->GetMaxAndMinDiameters(maxD,minD);
		contour->DeepCopy(normalCorrector->GetContourCrossSection());
		if (centerInsideContour(contour,center))
			diametersFile << maxD << " " << minD << std::endl;
		else
			diametersFile << -1 << " " << -1 << std::endl;
		

		
		filter->SetInput(contour);
		filter->SetTransform(transf);
		filter->Update();
		contour->DeepCopy(filter->GetOutput());		
	
		if (cdump) {
			writerC->SetInput(contour);
			writerC->SetFileName((outputRootFilename+"-Contour_"+itoa(p+firstLabel,10) + ".vtk").c_str());
			writerC->Update();
		}

		// save results from the previous iteration
		for(int i=0; i<3; i++)
			e1Aux[i] = e1[i];

		normalCorrector->GetMaxAndMinDiameterDirections(e1,e2);
		
		continuitySign = 0;
		for(int i=0; i<3; i++)
		{
			continuitySign += e1[i]*e1Aux[i];
		}
		if( continuitySign < 0 )
		{
			for(int i=0; i<3; i++)
			{
				e1[i] = -e1[i];
				e2[i] = -e2[i];
			}
		}

		int numPointsContour = contour->GetNumberOfPoints();
		double pointCoord[3];
		double pointAligned[3];

		for(int i=0; i<numPointsContour; i++)
		{
			contour->GetPoint(i,pointCoord);
			for(int j=0; j<3; j++) pointAligned[j] = 0;
			for(int j=0; j<3; j++)
			{
				pointAligned[0] += (pointCoord[j]-center[j])*e1[j];
				pointAligned[1] += (pointCoord[j]-center[j])*e2[j];
				pointAligned[2] += (pointCoord[j]-center[j])*normal[j];
			}
			pointAligned[0] += boundaryDisplacement;
			pointAligned[1] += boundaryDisplacement;
			contour->GetPoints()->SetPoint(i,pointAligned);
		}

		if (cdump) {
			writerC->SetInput(contour);
			writerC->SetFileName((outputRootFilename+"-Contour-Aligned_"+itoa(p+firstLabel,10) + ".vtk").c_str());
			writerC->Update();

			vtkCellArray* line = vtkCellArray::New();
			vtkPoints *points = vtkPoints::New();
			vtkPolyData * contourMeasure = vtkPolyData::New();

			int currPointID = 0;
			points->InsertNextPoint(normalCorrector->GetMaxDiameterPoints1());
			points->InsertNextPoint(normalCorrector->GetMaxDiameterPoints2());
			points->InsertNextPoint(normalCorrector->GetMinDiameterPoints1());
			points->InsertNextPoint(normalCorrector->GetMinDiameterPoints2());

			contourMeasure->SetPoints(points);

			for (int i=0; i<contourMeasure->GetNumberOfPoints(); i++) {

				contourMeasure->GetPoint(i,pointCoord);

				for(int j=0; j<3; j++) pointAligned[j] = 0;

				for(int j=0; j<3; j++)
				{
					pointAligned[0] += (pointCoord[j]-center[j])*e1[j];
					pointAligned[1] += (pointCoord[j]-center[j])*e2[j];
					pointAligned[2] += (pointCoord[j]-center[j])*normal[j];
				}
				pointAligned[0] += boundaryDisplacement;
				pointAligned[1] += boundaryDisplacement;

				contourMeasure->GetPoints()->SetPoint(i,pointAligned);
			}

			// Add the measurements to the contours
			line->InsertNextCell(2);
			line->InsertCellPoint(currPointID);
			line->InsertCellPoint(currPointID+1);
			line->InsertNextCell(2);
			line->InsertCellPoint(currPointID+2);
			line->InsertCellPoint(currPointID+3);
			contourMeasure->SetLines(line);

			writerC->SetInput(contourMeasure);
			writerC->SetFileName((outputRootFilename+"-Contour-AlignedMeasurements_"+itoa(p+firstLabel,10) + ".vtk").c_str());
			writerC->Update();

			line->Delete();
			points->Delete();
			contourMeasure->Delete();
		}
	}
	
	writerC->Delete();
	mesh->Delete();
	contour->Delete();
	filter->Delete();
	transf->Delete();

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
