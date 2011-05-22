/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "meVTKSkeletonizationFilter.h"
#include "vtkCleanPolyData.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "meVTKSkeleton.h"

vtkStandardNewMacro(meVTKSkeletonizationFilter);

meVTKSkeletonizationFilter::meVTKSkeletonizationFilter()
{
	m_params = meMeshSkeletonizationParamsPtr(
		new meMeshSkeletonizationParams);
}


meVTKSkeletonizationFilter::~meVTKSkeletonizationFilter() 
{
}


int meVTKSkeletonizationFilter::RequestData(
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

	vtkPolyData* copyInput = vtkPolyData::New();
	copyInput->DeepCopy(input);

	try
	{
	
		vtkSmartPointer<meVTKSkeleton> centerline = 
					vtkSmartPointer<meVTKSkeleton>::New();
		double spacing[3];
		spacing[0] = m_params->m_SkeletonizationPrecisionInMm;
		spacing[1] = m_params->m_SkeletonizationPrecisionInMm;
		spacing[2] = m_params->m_SkeletonizationPrecisionInMm;
		double offset[3] = { 0.1, 0.1, 0.1 };

		centerline->SetInput(copyInput);
		//centerline->SetInputAsSurface( input );
		centerline->SetDistanceTransformFilterSpacing( spacing );
		centerline->SetDistanceTransformFilterOffset( offset );
		centerline->SetDistanceTransformFilterSing( true );

		centerline->SetDistanceTransformSmoothingSigma( 1.0 );
		centerline->SetMedialCurveThreshold( 0.0 );
		centerline->Update();
		centerline->SetSplineSmoothOutput(false);

		vtkSmartPointer<vtkPolyData> polydata = 
					vtkSmartPointer<vtkPolyData>::New();
		polydata->DeepCopy( centerline->GetOutput() );

		vtkSmartPointer<vtkCleanPolyData> clean = vtkSmartPointer<vtkCleanPolyData>::New();
		clean->SetInput( polydata );
		clean->Update();
		output->DeepCopy( clean->GetOutput() );
			
	}
	catch( vtkstd::exception& e )
	{
		cout << "ExceptionObject caught !" << endl;
		cout << e.what() << endl;
		throw;
	}
	
	return 1;
}

void meVTKSkeletonizationFilter::SetParams( meMeshSkeletonizationParamsPtr params)
{
	m_params = params;
	this->Modified();
}

meMeshSkeletonizationParamsPtr meVTKSkeletonizationFilter::GetParams()
{
	return m_params;
}
