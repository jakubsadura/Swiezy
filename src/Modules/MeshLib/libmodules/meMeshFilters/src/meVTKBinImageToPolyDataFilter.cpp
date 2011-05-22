/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "meVTKBinImageToPolyDataFilter.h"
#include "meMeshTypes.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkImageGaussianSmooth.h"
#include "vtkMarchingCubes.h"
#include "vtkCleanPolyData.h"
#include "vtkSmoothPolyDataFilter.h"
#include "vtkPolyDataNormals.h"

#include "blShapeUtils.h"

vtkStandardNewMacro(meVTKBinImageToPolyDataFilter);


meVTKBinImageToPolyDataFilter::meVTKBinImageToPolyDataFilter()
{
}


meVTKBinImageToPolyDataFilter::~meVTKBinImageToPolyDataFilter() 
{
}

int meVTKBinImageToPolyDataFilter::FillInputPortInformation(int, vtkInformation *info)
{
	info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkImageData");
	return 1;
}

int meVTKBinImageToPolyDataFilter::RequestData(
	vtkInformation *vtkNotUsed(request),
	vtkInformationVector **inputVector,
	vtkInformationVector *outputVector)
{

	// -------------------------------------------  
	// get the info objects
	vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
	vtkInformation *outInfo = outputVector->GetInformationObject(0);

	// get the input and ouptut
	vtkImageData *input = vtkImageData::SafeDownCast(
		inInfo->Get(vtkDataObject::DATA_OBJECT()));
	vtkPolyData *output = vtkPolyData::SafeDownCast(
		outInfo->Get(vtkDataObject::DATA_OBJECT()));

	try
	{
		//-------------------------------------------------------------------
		//Smooth the Volume Data
		//-------------------------------------------------------------------
		//Some "binary" volume would produce stepped surfaces if we do not blur
		//it. The Gaussian kernel specified accomplishes the smoothing. The amount
		//of smoothing is controlled by the Gaussian standard deviation that is
		//independently specified for each axis of the volume data.
		vtkSmartPointer<vtkImageGaussianSmooth> gaussian = vtkSmartPointer<vtkImageGaussianSmooth>::New();
		gaussian->SetDimensionality( 3 );
		gaussian->SetStandardDeviation( 0.5, 0.5, 0.5 );   //could be set by user
		gaussian->SetRadiusFactor( 0.5 );
		gaussian->SetInput( input );
		gaussian->Update();
		//-------------------------------------------------------------------
		//Generate Triangles
		//-------------------------------------------------------------------
		//The filter runs faster if gradient and normal calculations are turned off.
		vtkSmartPointer<vtkMarchingCubes> mcubes = vtkSmartPointer<vtkMarchingCubes>::New();
		mcubes->SetInput( gaussian->GetOutput() );
		mcubes->ComputeScalarsOff();
		mcubes->ComputeGradientsOff();
		mcubes->ComputeNormalsOff();
		mcubes->SetValue( 0, IsoValue );
		mcubes->Update();

		if ( mcubes->GetOutput()->GetNumberOfPolys() == 0) {
			output->DeepCopy( mcubes->GetOutput() );
			throw;
		}

		vtkSmartPointer<vtkCleanPolyData> cleanMesh = vtkSmartPointer<vtkCleanPolyData>::New();
		cleanMesh->SetInput(mcubes->GetOutput());
		cleanMesh->ConvertPolysToLinesOff();
		cleanMesh->SetTolerance (1.0e-10);
		cleanMesh->SetAbsoluteTolerance (1.0e-10);
		cleanMesh->Update();
		
		vtkSmartPointer< vtkPolyData > smoothInput; 
		smoothInput = cleanMesh->GetOutput();
		//-------------------------------------------------------------------
		//Smooth the Triangle Vertices
		//-------------------------------------------------------------------
		//Laplacian smoothing is used, and triangle vertices are adjusted as
		//an average of neighbouring vertices. Models that are heavily decimated
		//can sometimes be improved with additional polygonal smoothing.
		vtkSmartPointer<vtkSmoothPolyDataFilter> smoother = vtkSmartPointer<vtkSmoothPolyDataFilter>::New();
		smoother->SetInput( smoothInput ); //use output of new filter
		smoother->SetFeatureAngle( 60.0 );
		smoother->SetRelaxationFactor( 0.2 );	// before: 0.2
		smoother->SetNumberOfIterations( 10 ); // before: 5
		smoother->GenerateErrorVectorsOff();
		smoother->FeatureEdgeSmoothingOff();
		smoother->BoundarySmoothingOff();
		smoother->SetConvergence( 0.0 );
		smoother->Update();

		//-------------------------------------------------------------------
		//Generate Normals
		//-------------------------------------------------------------------
		vtkSmartPointer<vtkPolyDataNormals> normals = vtkSmartPointer<vtkPolyDataNormals>::New();
		normals->SetInput( smoother->GetOutput() );
		normals->SplittingOff();	//to avoid duplicate vertices due to sharp edges
		normals->Update();
		output->DeepCopy( normals->GetOutput() );


	}
	catch( vtkstd::exception& e )
	{
		cout << "ExceptionObject caught !" << endl;
		cout << e.what() << endl;
		return 0;
	}

	return 1;
	
}

void meVTKBinImageToPolyDataFilter::SetIsoValue (double val)
{
	IsoValue = val;
}

double meVTKBinImageToPolyDataFilter::GetIsoValue ()
{
	return IsoValue;
}
