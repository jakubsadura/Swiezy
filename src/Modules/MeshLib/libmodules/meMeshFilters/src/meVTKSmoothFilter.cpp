/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "meVTKSmoothFilter.h"
#include "vtkWindowedSincPolyDataFilter.h"
#include "meVTKPolyDataFairer.h"
#include "vtkSmartPointer.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "blShapeUtils.h"

vtkStandardNewMacro(meVTKSmoothFilter);


meVTKSmoothFilter::meVTKSmoothFilter()
{
	m_params = meMeshSmoothingParamsPtr( new meMeshSmoothingParams);
}


meVTKSmoothFilter::~meVTKSmoothFilter() 
{
}


int meVTKSmoothFilter::RequestData(
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

	vtkSmartPointer<vtkPolyData> smoothInput = vtkSmartPointer<vtkPolyData>::New();
	vtkSmartPointer<vtkPolyData> append = vtkSmartPointer<vtkPolyData>::New();

	if (input->GetCellData()->HasArray("select"))
	{
		smoothInput  = blShapeUtils::ShapeUtils::GetShapeRegion(input,20,20, "select");
		append = blShapeUtils::ShapeUtils::GetShapeRegion(input,0,0, "select");
		smoothInput->GetCellData()->RemoveArray("select");
		append->GetCellData()->RemoveArray("select");
	}
	else
		smoothInput->DeepCopy(input);
	
	switch ( m_params->m_Type )
	{
	case MESH_SMOOTHING_TYPE_TAUBIN:
		{

			vtkSmartPointer<vtkWindowedSincPolyDataFilter>	pFilter;
			pFilter = vtkSmartPointer<vtkWindowedSincPolyDataFilter>::New( );
			pFilter->SetInput( smoothInput );
			pFilter->BoundarySmoothingOff();
			try
			{
				pFilter->Update( );
			}
			catch( vtkstd::exception& e )
			{
				cout << "ExceptionObject caught !" << endl;
				cout << e.what() << endl;
			}

			if (input->GetCellData()->HasArray("select"))
			{
				vtkSmartPointer<vtkAppendPolyData> appendFilter =
					vtkSmartPointer<vtkAppendPolyData>::New();
				appendFilter->AddInput(pFilter->GetOutput() );
				appendFilter->AddInput(append);
				appendFilter->Update();
				vtkSmartPointer<vtkCleanPolyData> cleanFilter =
					vtkSmartPointer<vtkCleanPolyData>::New();
				cleanFilter->SetInput(appendFilter->GetOutput());
				cleanFilter->Update();
				output->DeepCopy(cleanFilter->GetOutput());
			}
			else
				output->DeepCopy( pFilter->GetOutput() );
		}
		return 1;
		break;

	case MESH_SMOOTHING_TYPE_UMBRELLA:
		{

		/*	if ( GetInput(1) != NULL )
			{
				meVTKPolyDataFairer::Pointer fairer = meVTKPolyDataFairer::New();
				fairer->SetInput( GetInput(1), GetInput(0) );
				fairer->SetOrder( m_params->m_UmbrellaOrder );
				fairer->SetPercentage( m_params->m_Percentage );
				fairer->SetNumberOfIterations( m_params->m_UsePercentage ? 0 : m_params->m_NumIterations );
				try
				{
					fairer->Update();
				}
				catch( vtkstd::exception& e )
				{
					cout << "ExceptionObject caught !" << endl;
					cout << e.what() << endl;
				}

				//vtkOutput = vtkSmartPointer<vtkPolyData>::New();
				SetRequestedOutput( fairer->GetOutput() );
			}
			else
			{
				// Nothing to do, there is no selection, returning as output empty polydata
			} */
		}
		return 1;
		break;

	default:
		return 1;
		break;
	}

	return 1;
}	


void meVTKSmoothFilter::SetParams( meMeshSmoothingParamsPtr params)
{
	m_params = params;
}
