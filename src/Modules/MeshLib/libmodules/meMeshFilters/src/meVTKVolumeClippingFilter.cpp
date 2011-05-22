/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "meVTKVolumeClippingFilter.h"
#include "vtkCleanPolyData.h"
#include "vtkClipDataSet.h"
#include "vtkDataSetSurfaceFilter.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"

vtkStandardNewMacro(meVTKVolumeClippingFilter);


meVTKVolumeClippingFilter::meVTKVolumeClippingFilter()
{
}


meVTKVolumeClippingFilter::~meVTKVolumeClippingFilter() 
{
}


int meVTKVolumeClippingFilter::RequestData(
	vtkInformation *vtkNotUsed(request),
	vtkInformationVector **inputVector,
	vtkInformationVector *outputVector)
{

	// -------------------------------------------  
	// get the info objects
	vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
	vtkInformation *outInfo = outputVector->GetInformationObject(0);

	// get the input and ouptut
	vtkPolyData *input = vtkPolyData::SafeDownCast(
		inInfo->Get(vtkDataObject::DATA_OBJECT()));

	vtkPolyData *output = vtkPolyData::SafeDownCast(
		outInfo->Get(vtkDataObject::DATA_OBJECT()));


	vtkSmartPointer<vtkClipDataSet> clipper = vtkSmartPointer<vtkClipDataSet>::New();
	//vtkClipPolyData* clipper = vtkClipPolyData::New();// use this instead?
	clipper->SetInput( input );
	clipper->SetClipFunction(paramImplicitFunction);
	clipper->SetInsideOut(paramInsideOut);

	try
	{
		clipper->Update();
	}
	catch( vtkstd::exception& e )
	{
		cout << "ExceptionObject caught !" << endl;
		cout << e.what() << endl;
		return 0;
	}

	vtkSmartPointer<vtkDataSetSurfaceFilter> unstructToPolyDataFilter = 
		vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
	unstructToPolyDataFilter->SetInput( (vtkDataObject*) clipper->GetOutput() );
	unstructToPolyDataFilter->Update();

	vtkSmartPointer<vtkCleanPolyData> clean = vtkSmartPointer<vtkCleanPolyData>::New();
	clean->SetInput(unstructToPolyDataFilter->GetOutput() );
	clean->Update();

	output->DeepCopy( clean->GetOutput() );
	
	return 1;
}

void meVTKVolumeClippingFilter::SetImplicitFunction( 
					vtkSmartPointer<vtkImplicitFunction> implicitFunction,
					 bool insideOut)
{
	paramImplicitFunction = implicitFunction;
	paramInsideOut = insideOut;
}