/*=========================================================================
  Module    : meshLib 
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

typedef struct {
	double center[3];
	double normal[3];
	double e1[3];
	double e2[3];
	double boundaryDisplacement;
} ResliceDataType;


void usage() // TODO: Do.
{
	std::cerr << "Usage: ExtractAneurImageFromCutPlane inputList inputImage inputImageCroppedPlane inputMesh outputRootName" << std::endl;
	std::cerr << "Options:	-aneurImageSize [float]" << std::endl;
	exit(1); 
}

std::string itoa(int value, unsigned int base);

int main(int argc, char **argv)
{
	if (argc < 5){
		usage();
	}

	std::string outputRootFilename;	// pat10-3dra-1
	char *imageNeckFilename=NULL;
	char *imageAneurFilename=NULL;
	char *inputFilename=NULL;
	char *meshFilename=NULL;

	double normal[3], neckNormal[3], center[3];
	double e1[3], e2[3], e1Aux[3], e2_norm;
	double continuitySign;
	int firstLabel=0;	

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
	vtkStructuredPoints * inputNeck = vtkStructuredPoints::New();
	vtkStructuredPoints * inputAneur = vtkStructuredPoints::New();
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

	imageNeckFilename = argv[1];
	argc--; argv++;
	
	// image which has voxels turned to 0 below the neck plane
	imageAneurFilename = argv[1];
	argc--; argv++;

	meshFilename = argv[1];
	argc--; argv++;

	outputRootFilename = argv[1];
	argc--; argv++;

	bool ok;
	double aneurSize=0;
	while (argc > 1)
	{
		ok = false;
		//Options:
		if ((ok == false) && (strcmp(argv[1], "-aneurImageSize")==0))
		{
			argc--;
			argv++;
			aneurSize = atof(argv[1]);
			argc--;
			argv++;
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

	vtkStructuredPointsReader * imageNeckReader = vtkStructuredPointsReader::New();
	imageNeckReader->SetFileName(imageNeckFilename);
	imageNeckReader->Update();
	inputNeck->DeepCopy(imageNeckReader->GetOutput());
	imageNeckReader->Delete();

	vtkStructuredPointsReader * imageAneurReader = vtkStructuredPointsReader::New();
	imageAneurReader->SetFileName(imageAneurFilename);
	imageAneurReader->Update();
	inputAneur->DeepCopy(imageAneurReader->GetOutput());
	imageAneurReader->Delete();
	
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


	double spacing[3], spacingAneur[3], spacingNeck[3];
	for(int i=0; i<3; i++) {
		spacingAneur[i]=inputAneur->GetSpacing()[0]/2;
		spacingNeck[i]=inputNeck->GetSpacing()[0]/2;
	}

	double boundaryDisplacement;
	double boundaryDisplacementAneur;

	if (aneurSize != 0)
		boundaryDisplacementAneur = aneurSize;
	else	// default
		boundaryDisplacementAneur = 12;	//[mm]
	
	double boundaryDisplacementNeck = 7.5; // for vessels put to 7.5 [mm]
	int extend;
	int extendAneur = (int) (boundaryDisplacementAneur/spacingAneur[0]);	//??? 7.5->7
	int extendNeck = (int) (boundaryDisplacementNeck/spacingNeck[0]);	//??? 7.5->7
	for(int i=0; i<3; i++) { 
		spacingAneur[i]=boundaryDisplacementAneur/extendAneur;
		spacingNeck[i]=boundaryDisplacementNeck/extendNeck;
	}
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

	ofstream tmpcorrfile;
	tmpcorrfile.open( "AneurCut-corrections.txt", ios::app );

	ResliceDataType resliceData;
	ofstream resliceDataFile;
	resliceDataFile.open( (outputRootFilename + "-ResliceData.dat").c_str(), ios::binary | ios::trunc );

	while ( !inputPlanes.eof() ) {

		p++;
		
		inputPlanes.getline(buffer,256);
		sscanf(buffer,"Point 	= ( %lf , %lf , %lf )", &(center[0]), &(center[1]), &(center[2]));
		inputPlanes.getline(buffer,256);
		sscanf(buffer,"Normal = ( %lf , %lf , %lf )", &(normal[0]), &(normal[1]), &(normal[2]));
		
		
		std::cout << center[0] << " " << center[1] << " " << center[2] << std::endl;
		std::cout << normal[0] << " " << normal[1] << " " << normal[2] << std::endl;
	
		if (p==1) {
			input = inputNeck;
			for(int i=0; i<3; i++) {
				neckNormal[i] = normal[i];
				spacing[i] = spacingNeck[i];
			}
			boundaryDisplacement = boundaryDisplacementNeck;
			extend = extendNeck;

		} else {
			input = inputAneur;
			for(int i=0; i<3; i++) {
				spacing[i] = spacingAneur[i];
				
			}
			boundaryDisplacement = boundaryDisplacementAneur;
			extend = extendAneur;
		}
	
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
		for (int i=0; i<3; i++)
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
/*
		if (p!=1) {
			for(int i=0; i<3; i++)
				e1[i] = neckNormal[i];
			
			e2[0] = e1[1]*normal[2]
				- e1[2]*normal[1];
			e2[1] = e1[2]*normal[0]
				- e1[0]*normal[2];
			e2[2] = e1[0]*normal[1]
				- e1[1]*normal[0];

			
			for(int i=0; i<3; i++)
				center[i] += boundaryDisplacementAneur*3/4*normal[i];
		}
*/		
		if (p!=1) {
			// e2 is orthogonal to neckNormal and normal => vectorial product
			e2[0] = neckNormal[1]*normal[2]
			- neckNormal[2]*normal[1];
			e2[1] = neckNormal[2]*normal[0]
			- neckNormal[0]*normal[2];
			e2[2] = neckNormal[0]*normal[1]
			- neckNormal[1]*normal[0];
			
			// e2 is not normalized any more so we need to do that now!
			e2_norm = sqrt(pow(e2[0],2)+pow(e2[1],2)+pow(e2[2],2));
			std::cout << "e2 norm before normalization: " << e2_norm << std::endl;
			tmpcorrfile << e2_norm << endl;
			e2[0]/=e2_norm;
			e2[1]/=e2_norm;
			e2[2]/=e2_norm;
			

			// e1 is orthogonal to e2 and normal => vectorial product
			e1[0] = e2[1]*normal[2]
			- e2[2]*normal[1];
			e1[1] = e2[2]*normal[0]
			- e2[0]*normal[2];
			e1[2] = e2[0]*normal[1]
			- e2[1]*normal[0];
			
			for(int i=0; i<3; i++) {
				e1[i] = -e1[i];
				center[i] += boundaryDisplacementAneur*3/4*e1[i];
			}
		} else
			tmpcorrfile << 1 << endl;



		input->UpdateInformation();
		reslice->SetInput(input);
		reslice->SetOutputSpacing(spacing);
		reslice->SetOutputExtent(-extend,extend,-extend,extend,0,100);
		reslice->SetResliceAxesOrigin(center);
		reslice->SetResliceAxesDirectionCosines(e1, e2, normal);
		reslice->SetInterpolationModeToCubic();
		reslice->SetBackgroundLevel(0);
		reslice->SetOutputOrigin(0,0,0);
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

		for (int i=0; i<3; i++) {
			resliceData.e1[i] = e1[i];
			resliceData.e2[i] = e2[i];
			resliceData.normal[i] = normal[i];
			resliceData.center[i] = center[i];
		}
		resliceData.boundaryDisplacement = boundaryDisplacement;
		resliceDataFile.write( (char*)&resliceData, sizeof (ResliceDataType) );

		
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
	tmpcorrfile.close();
	resliceDataFile.close();

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
