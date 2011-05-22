/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "meVTKLocalRefinerFilter.h"
#include "meVTKPolyDataRefiner.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"

#include "blShapeUtils.h"

vtkStandardNewMacro(meVTKLocalRefinerFilter);


meVTKLocalRefinerFilter::meVTKLocalRefinerFilter()
{
	m_params = meMeshLocalRefinerParamsPtr (
		new meMeshLocalRefinerParams);
}


meVTKLocalRefinerFilter::~meVTKLocalRefinerFilter() 
{
}

int meVTKLocalRefinerFilter::RequestData(
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

	vtkSmartPointer<vtkPolyData> refinerInput = vtkSmartPointer<vtkPolyData>::New();
	vtkSmartPointer<vtkPolyData> append = vtkSmartPointer<vtkPolyData>::New();

	if (input->GetCellData()->HasArray("select"))
	{
		refinerInput = blShapeUtils::ShapeUtils::GetShapeRegion(input,20,20, "select");
		append = blShapeUtils::ShapeUtils::GetShapeRegion(input,0,0, "select");
		refinerInput->GetCellData()->RemoveArray("select");
		append->GetCellData()->RemoveArray("select");
	}
	else
		refinerInput->DeepCopy(input);

	meVTKPolyDataRefiner::Pointer refiner = meVTKPolyDataRefiner::New();
	refiner->SetInput( refinerInput );
	refiner->SetDensityFactor( m_params->m_DensityFactor );
	refiner->SetAngle( m_params->m_Angle );
	refiner->SetMinimumEdgesSwapped( m_params->m_MinimumEdgesSwapped );
	
	try
	{
		refiner->Update();
	}
	catch( vtkstd::exception& e )
	{
		cout << "ExceptionObject caught !" << endl;
		cout << e.what() << endl;
		return 0;
	}

	if (input->GetCellData()->HasArray("select"))
	{
		vtkSmartPointer<vtkAppendPolyData> appendFilter =
			vtkSmartPointer<vtkAppendPolyData>::New();
		appendFilter->AddInput(refiner->GetOutput() );
		appendFilter->AddInput(append);
		appendFilter->Update();
		output->DeepCopy(appendFilter->GetOutput());
	}
	else
		output->DeepCopy( refiner->GetOutput() );

	return 1;
}

void meVTKLocalRefinerFilter::SetParams( meMeshLocalRefinerParamsPtr params)
{
	m_params = params;
}

meMeshLocalRefinerParamsPtr meVTKLocalRefinerFilter::GetParams()
{
	return m_params;
}
