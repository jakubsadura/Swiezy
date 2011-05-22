/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "meVTKVolumeClosingFilter.h"
#include "meHoleFiller.h"
#include "vtkCleanPolyData.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"

vtkStandardNewMacro(meVTKVolumeClosingFilter);


meVTKVolumeClosingFilter::meVTKVolumeClosingFilter()
{
	m_params = meMeshVolumeClosingParamsPtr(new meMeshVolumeClosingParams );
}


meVTKVolumeClosingFilter::~meVTKVolumeClosingFilter() 
{
}


int meVTKVolumeClosingFilter::RequestData(
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

	meHoleFiller::Pointer holeFilling = meHoleFiller::New();
	holeFilling->SetInput( input );
	holeFilling->SetDensityFactor( m_params->m_DensityFactor );
	holeFilling->SetAngle( m_params->m_Angle );
	holeFilling->SetMinimumEdgesSwapped( m_params->m_MinimumEdgesSwapped );
	holeFilling->SetOrder( m_params->m_Order );


	try
	{
		holeFilling->Update( );
	}
	catch( vtkstd::exception& e )
	{
		cout << "ExceptionObject caught !" << endl;
		cout << e.what() << endl;
		return 0;
	}

	vtkSmartPointer<vtkCleanPolyData> clean = vtkSmartPointer<vtkCleanPolyData>::New();
	clean->SetInput( holeFilling->GetOutput() );
	clean->Update();

	output->DeepCopy( clean->GetOutput() );

	return 1;
}

void meVTKVolumeClosingFilter::SetParams( meMeshVolumeClosingParamsPtr params)
{
	m_params = params;
}

meMeshVolumeClosingParamsPtr meVTKVolumeClosingFilter::GetParams()
{
	return m_params;
}
