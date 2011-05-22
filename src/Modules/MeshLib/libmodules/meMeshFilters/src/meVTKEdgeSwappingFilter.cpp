/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "meVTKEdgeSwappingFilter.h"
#include "meVTKPolyDataEdgeSwapper.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"

vtkStandardNewMacro(meVTKEdgeSwappingFilter);

meVTKEdgeSwappingFilter::meVTKEdgeSwappingFilter()
{
	m_params = meMeshEdgeSwappingParamsPtr(
		new meMeshEdgeSwappingParams);
}


meVTKEdgeSwappingFilter::~meVTKEdgeSwappingFilter() 
{
}


int meVTKEdgeSwappingFilter::RequestData(
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

	// BEWARE: edge swapper is an IN PLACE filter
	meVTKPolyDataEdgeSwapper::Pointer swapper = meVTKPolyDataEdgeSwapper::New();
	swapper->SetInput( input );
	swapper->SetAngle( m_params->m_Angle );
	swapper->SetMinimumEdgesSwapped( m_params->m_MinimumEdgesSwapped );
	swapper->Update();
	
	output->DeepCopy( input ); //????

	return 1;
}	

void meVTKEdgeSwappingFilter::SetParams( meMeshEdgeSwappingParamsPtr params)
{
	m_params = params;
}

meMeshEdgeSwappingParamsPtr meVTKEdgeSwappingFilter::GetParams()
{
	return m_params;
}
