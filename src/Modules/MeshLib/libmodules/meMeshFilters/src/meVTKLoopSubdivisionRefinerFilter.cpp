/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "meVTKLoopSubdivisionRefinerFilter.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkLoopSubdivisionFilter.h"
#include "vtkObjectFactory.h"

#include "blShapeUtils.h"

vtkStandardNewMacro(meVTKLoopSubdivisionRefinerFilter);

meVTKLoopSubdivisionRefinerFilter::meVTKLoopSubdivisionRefinerFilter()
{
	m_params = meMeshLoopSubdivisionParamsPtr(
		new meMeshLoopSubdivisionParams);
}


meVTKLoopSubdivisionRefinerFilter::~meVTKLoopSubdivisionRefinerFilter() 
{
}


int meVTKLoopSubdivisionRefinerFilter::RequestData(
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

	vtkSmartPointer<vtkPolyData> loopInput = vtkSmartPointer<vtkPolyData>::New();
	vtkSmartPointer<vtkPolyData> append = vtkSmartPointer<vtkPolyData>::New();

	if (input->GetCellData()->HasArray("select"))
	{
		loopInput  = blShapeUtils::ShapeUtils::GetShapeRegion(input,20,20, "select");
		append = blShapeUtils::ShapeUtils::GetShapeRegion(input,0,0, "select");
		loopInput->GetCellData()->RemoveArray("select");
		append->GetCellData()->RemoveArray("select");
	}
	else
		loopInput->DeepCopy(input);



	vtkSmartPointer<vtkLoopSubdivisionFilter> subdivisionFilter = 
		vtkSmartPointer<vtkLoopSubdivisionFilter>::New();
	subdivisionFilter->SetNumberOfSubdivisions(m_params->m_NumberOfSubdivisions);
	subdivisionFilter->SetInput( loopInput);

	try
	{
		subdivisionFilter->Update();
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
		appendFilter->AddInput(subdivisionFilter->GetOutput() );
		appendFilter->AddInput(append);
		appendFilter->Update();
		vtkSmartPointer<vtkCleanPolyData> cleanFilter =
			vtkSmartPointer<vtkCleanPolyData>::New();
		cleanFilter->SetInput(appendFilter->GetOutput());
		cleanFilter->Update();
		output->DeepCopy(cleanFilter->GetOutput());
	}
	else
		output->DeepCopy( subdivisionFilter->GetOutput() );
	
	return 1;
}	

void meVTKLoopSubdivisionRefinerFilter::SetParams( meMeshLoopSubdivisionParamsPtr params)
{
	m_params = params;
}
