/*=========================================================================
  Library   : GeoLib
  Module    : geoMedial
  File      : ExtractImagesNormal2Centerline.cxx
  Authors   : jpozo (José María Pozo Soler);
              (C)opyright 2008
              See COPYRIGHT statement in top level directory.
  Purpose   : Extract a series of 2D images by cliping a volume with a 
              sequence of planes along a vessel and perpendicular to its 
              centerline.
  Date      : $Date: 2008/04/08 11:45:09 $
  Version   : $Revision: 1.0 $
  Changes   : $Locker:  $
              $Log: ExtractImagesNormal2Centerline.cxx,v $
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


void Usage() // TODO: Do.
{
	exit(1); 
}

std::string itoa(int value, unsigned int base);

int main(int argc, char **argv)
{
	if (argc < 3){
		Usage();
	}
	std::string outputRootFilename;
	char *inputFilename=NULL;
	char *meshFilename=NULL;
	char *centerlineFilename=NULL;
	int pointId1,
	    pointId2,
	    numPoints,
	    FirstLabel;
	double stepLength,
	       intermedialStepLength;
	std::vector<int> pointIds;

	vtkStructuredPoints * input = vtkStructuredPoints::New();
//	vtkStructuredPoints * output = vtkStructuredPoints::New();
	vtkPolyData * mesh = vtkPolyData::New();
	vtkPolyData * centerline = vtkPolyData::New();
	vtkPolyData * contour = vtkPolyData::New();

	ofstream diametersFile;

	inputFilename = argv[1];
	argc--; argv++;

	meshFilename = argv[1];
	argc--; argv++;

	centerlineFilename = argv[1];
	argc--; argv++;

	outputRootFilename = argv[1];
	argc--; argv++;

	FirstLabel = atoi(argv[1]);
	argc--; argv++;

	pointId1 = atoi(argv[1]);
	argc--; argv++;

	pointId2 = atoi(argv[1]);
	argc--; argv++;

	numPoints = atoi(argv[1]);
	argc--; argv++;

	stepLength = atof(argv[1]);
	argc--; argv++;

	intermedialStepLength = atof(argv[1]);
	argc--; argv++;

	bool ok,
		binary = false,
		circularRadius = false,
		vtkOutput = false;

    while (argc > 1)
    {
		ok = false;
		if (!ok && strcmp(argv[1], "-binary") == 0)
		{
			argc--;
			argv++;
			binary = true;
			ok =  true;
		}
		// Flag for computing inscribed and circunscribed radius instead of max and min diameters.
		if (!ok && strcmp(argv[1], "-circularRadius") == 0)
		{
			argc--;
			argv++;
			circularRadius = true;
			ok =  true;
		}
		// Flag for writing the output slices as vtkStructuredPoints instead of in mhd format.
		if (!ok && strcmp(argv[1], "-vtkOutput") == 0)
		{
			argc--;
			argv++;
			vtkOutput = true;
			ok =  true;
		}
		if (!ok)
		{
			cerr << "Can not parse argument " << argv[1] << endl;
			Usage();
		}		
	}

	vtkStructuredPointsReader * imageReader = vtkStructuredPointsReader::New();
	imageReader->SetFileName(inputFilename);
	imageReader->Update();
	input->DeepCopy(imageReader->GetOutput());
	imageReader->Delete();

	vtkPolyDataReader * polyDataReader = vtkPolyDataReader::New();
	polyDataReader->SetFileName(meshFilename);
	polyDataReader->Update();

	meCloseHoles::Pointer closer = meCloseHoles::New();
	closer->SetInput(polyDataReader->GetOutput());
	closer->Update();
	mesh->DeepCopy(closer->GetOutput());

	polyDataReader->SetFileName(centerlineFilename);
	polyDataReader->Update();
	centerline->DeepCopy(polyDataReader->GetOutput());
	polyDataReader->Delete();



	double normal[3],
	       e1[3],
	       e2[3],
	       e1Aux[3],
	       center[3],
	       continuitySign,
	       maxD,
	       minD;
	meNormalPlaneToVessel::Pointer normalCorrector = meNormalPlaneToVessel::New();
	
	for(int i=0; i<3; i++)
	{
		normal[i] = centerline->GetPoint(pointId2)[i]
				- centerline->GetPoint(pointId1)[i];
	}

	for(int i=0; i<3; i++) center[i] = centerline->GetPoint(pointId1)[i];

	normalCorrector->SetSurfaceMesh(mesh);
	normalCorrector->SetOrigin(center);
	normalCorrector->InitializeNormal(normal);
	normalCorrector->Update();
	normalCorrector->GetNormal(normal);
	normalCorrector->GetCenter(center);

	int intermedialSteps = (int) (stepLength/intermedialStepLength);
	intermedialStepLength = stepLength/intermedialSteps;
	int intermSteps = 0;
	double spacing[3];
	for(int i=0; i<3; i++) spacing[i]=input->GetSpacing()[0]/2;

	double boundaryDisplacement = 7.5;
	int extend = (int) boundaryDisplacement/spacing[0];
	for(int i=0; i<3; i++) spacing[i]=boundaryDisplacement/extend;
	double boundaryPoint[3];
	double boundaryNormal[3];


	diametersFile.open( (outputRootFilename+"-Diameters.txt").c_str(), ios::app );
	diametersFile << "Diameters for " << outputRootFilename << endl;

	for(int p=0; p<numPoints; p++)
	{
		for(int q=0; q<intermSteps; q++)
		{
			for(int i=0; i<3; i++) center[i] += intermedialStepLength*normal[i];
			normalCorrector->SetOrigin(center);
			normalCorrector->InitializeNormal(normal);
			normalCorrector->Update();
			normalCorrector->GetNormal(normal);
			normalCorrector->GetCenter(center);
		}
		intermSteps = intermedialSteps;

cout << "Point 	= ( " << center[0] << " , " << center[1] << " , " << center[2] << " )" << endl;
cout << "Normal = ( " << normal[0] << " , " << normal[1] << " , " << normal[2] << " )" << endl;

		if(circularRadius)
		{
			normalCorrector->ComputeInscribedCenterAndRadius();
			minD = normalCorrector->GetInscribedRadius();
			normalCorrector->ComputeCircumscribingCenterAndRadius();
			maxD = normalCorrector->GetCircumscribingRadius();
		}
		else
		{
			normalCorrector->GetMaxAndMinDiameters(maxD,minD);
		}



		diametersFile << maxD << " " << minD << endl;
		contour->DeepCopy(normalCorrector->GetContourCrossSection());

		for(int i=0; i<3; i++)
		{
			e1Aux[i]=e1[i];
		}
		normalCorrector->GetMaxAndMinDiameterDirections(e1,e2);
cout << "e1 = ( " << e1[0] << " , " << e1[1] << " , " << e1[2] << " )" << endl;
cout << "e2 = ( " << e2[0] << " , " << e2[1] << " , " << e2[2] << " )" << endl;
		
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

		vtkImageReslice * reslice = vtkImageReslice::New();
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
		if(vtkOutput)
		{
			vtkStructuredPointsWriter * writer = vtkStructuredPointsWriter::New();
			writer->SetInput(reslice->GetOutput());
			writer->SetFileName((outputRootFilename+"_"+itoa(p+1,10) + ".vtk").c_str());
			writer->Update();
			writer->Delete();
		}
		else
		{
			int dim[3];
			reslice->GetOutput()->GetDimensions(dim);

			ofstream outfile;
			outfile.open( (outputRootFilename+"-Cut_"+itoa(p+FirstLabel,10) + ".mhd").c_str(), ios::trunc );
			
			outfile << "NDims = 2" << endl;
			outfile << "DimSize = " << 2*extend+1 << " " << 2*extend+1 << endl;
			outfile << "ElementType = MET_FLOAT"<< endl;
			outfile << "Offset = 0.0 0.0"<< endl;
			outfile << "ElementSpacing = " << reslice->GetOutput()->GetSpacing()[0] << " "  
			<< reslice->GetOutput()->GetSpacing()[1] << endl;
			outfile << "ElementByteOrderMSB = False"<< endl;
			outfile << "ElementDataFile = " << (outputRootFilename+"-Cut_"+itoa(p+FirstLabel,10) + ".raw").c_str() << endl;
			
			outfile.close();
			
			outfile.open( (outputRootFilename+"-Cut_"+itoa(p+FirstLabel,10) + ".raw").c_str(), ios::trunc | ios::binary );
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
		}

		reslice->Delete();
		
		vtkTransformPolyDataFilter * filter = vtkTransformPolyDataFilter::New();
		vtkTransform * transf = vtkTransform::New();
		filter->SetInput(contour);
		filter->SetTransform(transf);
		filter->Update();
		contour->DeepCopy(filter->GetOutput());
		

		vtkPolyDataWriter * writerC = vtkPolyDataWriter::New();
		writerC->SetInput(contour);
		writerC->SetFileName((outputRootFilename+"-Contour_"+itoa(p+FirstLabel,10) + ".vtk").c_str());
		if(binary) writerC->SetFileTypeToBinary();
		writerC->Update();

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
		writerC->SetFileName((outputRootFilename+"-Contour-Aligned_"+itoa(p+FirstLabel,10) + ".vtk").c_str());
		if(binary) writerC->SetFileTypeToBinary();
		writerC->Update();
		writerC->Delete();


		vtkPlane * plane = vtkPlane::New();
		plane->SetOrigin(center);
		plane->SetNormal(normal);
		vtkCutter * cutPlane = vtkCutter::New();
		cutPlane->CreateDefaultLocator();
		cutPlane->SetInput(input);
		cutPlane->SetCutFunction(plane);
		cutPlane->Update();

		vtkPolyData * aux = vtkPolyData::New();
		aux->DeepCopy(cutPlane->GetOutput());

		vtkClipPolyData * clip = vtkClipPolyData::New();

		for(int i=0; i<3; i++) boundaryNormal[i] = e1[i];

		clip->SetInput(aux);
		for(int i=0; i<3; i++) boundaryPoint[i] = center[i]-boundaryDisplacement*boundaryNormal[i];
		plane->SetOrigin(boundaryPoint);
		plane->SetNormal(boundaryNormal);
		clip->SetClipFunction(plane);
		clip->Update();
		aux->DeepCopy(clip->GetOutput());

		for(int i=0; i<3; i++) boundaryNormal[i] = -e1[i];

		clip->SetInput(aux);
		for(int i=0; i<3; i++) boundaryPoint[i] = center[i]-boundaryDisplacement*boundaryNormal[i];
		plane->SetOrigin(boundaryPoint);
		plane->SetNormal(boundaryNormal);
		clip->SetClipFunction(plane);
		clip->Update();
		aux->DeepCopy(clip->GetOutput());

		for(int i=0; i<3; i++) boundaryNormal[i] = e2[i];

		clip->SetInput(aux);
		for(int i=0; i<3; i++) boundaryPoint[i] = center[i]-boundaryDisplacement*boundaryNormal[i];
		plane->SetOrigin(boundaryPoint);
		plane->SetNormal(boundaryNormal);
		clip->SetClipFunction(plane);
		clip->Update();
		aux->DeepCopy(clip->GetOutput());

		for(int i=0; i<3; i++) boundaryNormal[i] = -e2[i];

		clip->SetInput(aux);
		for(int i=0; i<3; i++) boundaryPoint[i] = center[i]-boundaryDisplacement*boundaryNormal[i];
		plane->SetOrigin(boundaryPoint);
		plane->SetNormal(boundaryNormal);
		clip->SetClipFunction(plane);
		clip->Update();
		aux->DeepCopy(clip->GetOutput());

		clip->Delete();

		vtkPolyDataWriter * writerP = vtkPolyDataWriter::New();
		writerP->SetInput(aux);
		writerP->SetFileName((outputRootFilename+"-Cut-3D_"+itoa(p+FirstLabel,10) + ".vtk").c_str());
		writerP->Update();

		plane->Delete();
		cutPlane->Delete();
		writerP->Delete();

	}

	diametersFile.close();

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
