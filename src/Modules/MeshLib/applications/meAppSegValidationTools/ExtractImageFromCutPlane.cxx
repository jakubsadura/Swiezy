/*=========================================================================
  Library   : GeoLib
  Module    : geoMedial
  File      : ExtractImageFromCutPlane.cxx
  Authors   : hbogunovic (Hrvoje Bogunovic)
			  jpozo (José María Pozo Soler);
              (C)opyright 2008
              See COPYRIGHT statement in top level directory.
  Purpose   : Scaled down version of ExtractImagesNormal2Centerline2. Which takes
			  as an input just a sequence of points and normals (planes) and extracts the  
			  cutting plane images.
  Date      : $Date: 2008/04/08 11:45:09 $
  Version   : $Revision: 1.0 $
  Changes   : $Locker:  $
              $Log: ExtractImageFromCutPlane.cxx,v $
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
//#include <vtkCellArray.h>
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
	std::cerr << "Usage: ExtractImageFromCutPlane inputList inputImage inputMesh outputRootName" << std::endl;
	exit(1); 
}

std::string itoa(int value, unsigned int base);

int main(int argc, char **argv)
{
	if (argc < 5){
		usage();
	}

	std::string outputRootFilename;	// pat10-3dra-1
	char *imageFilename=NULL;
	char *inputFilename=NULL;
	char *meshFilename=NULL;

	double normal[3], center[3]; //, maxD, minD;
	double e1[3], e2[3], e1Aux[3];
	double continuitySign;
	int firstLabel=0;

//	double stepLength, intermedialStepLength;

	meNormalPlaneToVessel::Pointer normalCorrector = meNormalPlaneToVessel::New();
	vtkTransformPolyDataFilter * filter = vtkTransformPolyDataFilter::New();
	vtkTransform * transf = vtkTransform::New();
	vtkPolyDataWriter * writerC = vtkPolyDataWriter::New();
	vtkImageReslice * reslice = vtkImageReslice::New();
	vtkPlane * plane = vtkPlane::New();
	vtkCutter * cutPlane = vtkCutter::New();
	vtkPolyData * aux = vtkPolyData::New();
	vtkPolyDataWriter * writerP = vtkPolyDataWriter::New();
	vtkClipPolyData * clip = vtkClipPolyData::New();

	vtkStructuredPoints * input = vtkStructuredPoints::New();
//	vtkStructuredPoints * output = vtkStructuredPoints::New();
	vtkPolyData * mesh = vtkPolyData::New();
	vtkPolyData * contour = vtkPolyData::New();

//    ofstream diametersFile, 
	ofstream outfile;

		// initialization
	for (int i=0; i<3; i++)
		e1[i] = 0;	



	inputFilename = argv[1];
	argc--; argv++;

	imageFilename = argv[1];
	argc--; argv++;

	meshFilename = argv[1];
	argc--; argv++;

	outputRootFilename = argv[1];
	argc--; argv++;


	ifstream inputPlanes(inputFilename);
	if (!inputPlanes) {
		std::cerr << "Problem opening file with centers and normals"<< std::endl;
		exit(1);	
	}

	vtkStructuredPointsReader * imageReader = vtkStructuredPointsReader::New();
	imageReader->SetFileName(imageFilename);
	imageReader->Update();
	input->DeepCopy(imageReader->GetOutput());
	imageReader->Delete();

	vtkPolyDataReader * polyDataReader = vtkPolyDataReader::New();
	polyDataReader->SetFileName(meshFilename);
	polyDataReader->Update();

	// this part crashes on my mesh
//	meCloseHoles::Pointer closer = meCloseHoles::New();
//	closer->SetInput(polyDataReader->GetOutput());
//	closer->Update();
//	mesh->DeepCopy(closer->GetOutput());
	mesh->DeepCopy(polyDataReader->GetOutput());
	polyDataReader->Delete();


	double spacing[3];
	for(int i=0; i<3; i++) 
		spacing[i]=input->GetSpacing()[0]/2;

	double boundaryDisplacement = 7.5;
	int extend = (int) (boundaryDisplacement/spacing[0]);	//??? 7.5->7
	for(int i=0; i<3; i++) 
		spacing[i]=boundaryDisplacement/extend;
	double boundaryPoint[3];
	double boundaryNormal[3];

/*	
	diametersFile.open( (outputRootFilename+"-Diameters.txt").c_str());
	if (!diametersFile) {
		std::cerr << "Problem opening output diameters file"<< std::endl;
		exit(1);
	}
*/
	char buffer[256];
	int p=0; // main counter

	while ( !inputPlanes.eof() ) {

		p++;
		
		inputPlanes.getline(buffer,256);
		sscanf(buffer,"Point 	= ( %lf , %lf , %lf )", &(center[0]), &(center[1]), &(center[2]));
		inputPlanes.getline(buffer,256);
		sscanf(buffer,"Normal = ( %lf , %lf , %lf )", &(normal[0]), &(normal[1]), &(normal[2]));
		
		
		std::cout << center[0] << " " << center[1] << " " << center[2] << std::endl;
		std::cout << normal[0] << " " << normal[1] << " " << normal[2] << std::endl;
	
	
		normalCorrector->SetSurfaceMesh(mesh);
		normalCorrector->SetClosedSurfaceMesh(mesh);
		normalCorrector->SetNormal(normal);
		normalCorrector->SetCenter(center);
		normalCorrector->GetConnectedCutPlane(normal);		
		normalCorrector->ComputeMaxAndMinDiameters();
/*
		normalCorrector->GetMaxAndMinDiameters(maxD,minD);
		diametersFile << maxD << " " << minD << std::endl;
		contour->DeepCopy(normalCorrector->GetContourCrossSection());
		
		filter->SetInput(contour);
		filter->SetTransform(transf);
		filter->Update();
		contour->DeepCopy(filter->GetOutput());		
	
		writerC->SetInput(contour);
		writerC->SetFileName((outputRootFilename+"-Contour_"+itoa(p+firstLabel,10) + ".vtk").c_str());
		writerC->Update();
*/

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
/*
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

		writerC->SetInput(contour);
		writerC->SetFileName((outputRootFilename+"-Contour-Aligned_"+itoa(p+firstLabel,10) + ".vtk").c_str());
		writerC->Update();
*/

		input->UpdateInformation();
		reslice->SetInput(input);
		reslice->SetResliceAxesOrigin(center);
		reslice->SetResliceAxesDirectionCosines(e1, e2, normal);
		reslice->SetInterpolationModeToCubic();
		reslice->SetBackgroundLevel(0);
		reslice->SetOutputSpacing(spacing);
		reslice->SetOutputOrigin(0,0,0);
		reslice->SetOutputExtent(-extend,extend,-extend,extend,0,100);
		reslice->SetOutputDimensionality(2);
		reslice->Update();
		reslice->GetOutput()->UpdateInformation();

		int dim[3];
		reslice->GetOutput()->GetDimensions(dim);

		outfile.open( (outputRootFilename+"-Cut_"+itoa(p+firstLabel,10) + ".mhd").c_str(), ios::trunc );

		outfile << "NDims = 2" << endl;
		outfile << "DimSize = " << 2*extend+1 << " " << 2*extend+1 << endl;
		outfile << "ElementType = MET_FLOAT"<< endl;
		outfile << "Offset = 0.0 0.0"<< endl;
		outfile << "ElementSpacing = " << reslice->GetOutput()->GetSpacing()[0] << " "  
				<< reslice->GetOutput()->GetSpacing()[1] << endl;
		outfile << "ElementByteOrderMSB = False"<< endl;
		outfile << "ElementDataFile = " << (outputRootFilename+"-Cut_"+itoa(p+firstLabel,10) + ".raw").c_str() << endl;

		outfile.close();

		outfile.open( (outputRootFilename+"-Cut_"+itoa(p+firstLabel,10) + ".raw").c_str(), ios::trunc | ios::binary );
		float * data = new float[dim[0]*dim[1]];
		int ij=0;
		for(int i=-(dim[0]-1)/2; i<=(dim[0]-1)/2; i++)
		{
			for(int j=-(dim[1]-1)/2; j<=(dim[1]-1)/2; j++)
			{
				data[ij]=reslice->GetOutput()->GetScalarComponentAsDouble(j,i,0,0);
				ij++;
			}
		}

		outfile.write((char*)data,sizeof(float)*dim[0]*dim[1]);
		outfile.close();
		delete[] data;

		
		plane->SetOrigin(center);
		plane->SetNormal(normal);
		
		cutPlane->CreateDefaultLocator();
		cutPlane->SetInput(input);
		cutPlane->SetCutFunction(plane);
		cutPlane->Update();

		
		aux->DeepCopy(cutPlane->GetOutput());
		

		for(int i=0; i<3; i++) 
			boundaryNormal[i] = e1[i];

		clip->SetInput(aux);
		
		for(int i=0; i<3; i++) 
			boundaryPoint[i] = center[i]-boundaryDisplacement*boundaryNormal[i];
		
		plane->SetOrigin(boundaryPoint);
		plane->SetNormal(boundaryNormal);
		clip->SetClipFunction(plane);
		clip->Update();
		aux->DeepCopy(clip->GetOutput());

		for(int i=0; i<3; i++) 
			boundaryNormal[i] = -e1[i];

		clip->SetInput(aux);

		for(int i=0; i<3; i++) 
			boundaryPoint[i] = center[i]-boundaryDisplacement*boundaryNormal[i];

		plane->SetOrigin(boundaryPoint);
		plane->SetNormal(boundaryNormal);
		clip->SetClipFunction(plane);
		clip->Update();
		aux->DeepCopy(clip->GetOutput());

		for(int i=0; i<3; i++) 
			boundaryNormal[i] = e2[i];

		clip->SetInput(aux);
		for(int i=0; i<3; i++) 
			boundaryPoint[i] = center[i]-boundaryDisplacement*boundaryNormal[i];

		plane->SetOrigin(boundaryPoint);
		plane->SetNormal(boundaryNormal);
		clip->SetClipFunction(plane);
		clip->Update();
		aux->DeepCopy(clip->GetOutput());

		for(int i=0; i<3; i++) 
			boundaryNormal[i] = -e2[i];

		clip->SetInput(aux);
		for(int i=0; i<3; i++) 
			boundaryPoint[i] = center[i]-boundaryDisplacement*boundaryNormal[i];

		plane->SetOrigin(boundaryPoint);
		plane->SetNormal(boundaryNormal);
		clip->SetClipFunction(plane);
		clip->Update();
		aux->DeepCopy(clip->GetOutput());

		
		writerP->SetInput(aux);
		writerP->SetFileName((outputRootFilename+"-Cut-3D_"+itoa(p+firstLabel,10) + ".vtk").c_str());
		writerP->Update();
		
	}
	
	writerC->Delete();
	writerP->Delete();
	mesh->Delete();
	contour->Delete();
	filter->Delete();
	transf->Delete();
	plane->Delete();
	cutPlane->Delete();	
	reslice->Delete();
	clip->Delete();

//	diametersFile.close();
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
