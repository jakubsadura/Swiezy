/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreClippingProcessor.h"
#include "coreVTKPolyDataHolder.h"
#include "coreDataEntityHelper.h"
#include "vtkClipPolyData.h"


Core::ClippingProcessorData::ClippingProcessorData()
{
	m_planeImplFct = vtkPlane::New();
}

Core::ClippingProcessor::ClippingProcessor( Core::ClippingProcessorData::Pointer data )
{
	Core::Initialize(this->m_Output, Core::DataEntity::Pointer());
	Core::Initialize(this->m_Input, Core::DataEntity::Pointer());
	Core::Initialize(this->m_ProcessorDataHolder, data);
	if( data.IsNull() )
	{
		this->m_ProcessorDataHolder->SetSubject(Core::ClippingProcessorData::New());
	}
}

Core::ClippingProcessor::ClippingProcessorDataHolder::Pointer Core::ClippingProcessor::GetProcessorDataHolder() const
{
	return m_ProcessorDataHolder;
}

void Core::ClippingProcessor::SetProcessorDataHolder( Core::ClippingProcessor::ClippingProcessorDataHolder::Pointer val )
{
	if( m_ProcessorDataHolder == val )
		return;
	m_ProcessorDataHolder = val;
}

void Core::ClippingProcessor::Run()
{
	if(! this->m_Input->GetSubject() )
		return;

	if( this->m_Input->GetSubject()->GetType() == Core::SurfaceMeshTypeId )
	{
		//TODO clipping!!!
		this->m_ProcessorDataHolder->GetSubject()->m_planeImplFct->SetOrigin(this->m_ProcessorDataHolder->GetSubject()->m_center);
		this->m_ProcessorDataHolder->GetSubject()->m_planeImplFct->SetNormal(this->m_ProcessorDataHolder->GetSubject()->m_normal);

		Core::vtkPolyDataPtr poly_in;
		this->m_Input->GetSubject()->GetProcessingData(poly_in);
		vtkClipPolyData* clip = vtkClipPolyData::New();
		clip->SetInput( poly_in );
		clip->SetClipFunction(this->m_ProcessorDataHolder->GetSubject()->m_planeImplFct);
		clip->Update();

		Core::vtkPolyDataPtr poly_out = clip->GetOutput();
		Core::DataEntity::Pointer dataEntity = Core::DataEntity::New( Core::SurfaceMeshTypeId );
		dataEntity->AddTimeStep( poly_out );
		dataEntity->GetMetadata()->SetName( "Clipped image" );

		this->GetOutputDataEntityHolder()->SetSubject(dataEntity);
		//blShapeUtils::ShapeUtils::SaveShapeToFile(poly_out,"clipped.vtk");
	}
}

void Core::ClippingProcessor::ResetPlane()
{
	if(! this->m_Input->GetSubject() )
		return;

	if( this->m_Input->GetSubject()->GetType() == Core::SurfaceMeshTypeId )
	{
		//TODO set plane center to bounding box center (to be used both with polydata and unstructuredgrid
		// 
		Core::vtkPolyDataPtr polydataInput;
		m_Input->GetSubject()->GetProcessingData(polydataInput);
		if (polydataInput != NULL)
		{
			for(int k =0; k<6; k++)
			    this->m_ProcessorDataHolder->GetSubject()->m_box[k] = polydataInput->GetBounds()[k];
		}

		polydataInput->GetCenter(this->m_ProcessorDataHolder->GetSubject()->m_center);
		this->m_ProcessorDataHolder->GetSubject()->m_normal[0] = 1;
		this->m_ProcessorDataHolder->GetSubject()->m_normal[2] = 0;
		this->m_ProcessorDataHolder->GetSubject()->m_normal[1] = 0;

		this->m_ProcessorDataHolder->GetSubject()->m_planeImplFct->SetOrigin(
			this->m_ProcessorDataHolder->GetSubject()->m_center); 
		this->m_ProcessorDataHolder->GetSubject()->m_planeImplFct->SetNormal(
			this->m_ProcessorDataHolder->GetSubject()->m_normal); 

		this->m_ProcessorDataHolder->NotifyObservers();
	}
}

Core::DataEntityHolder::Pointer Core::ClippingProcessor::GetInputDataEntityHolder() const
{
	return m_Input;
}

void Core::ClippingProcessor::SetInputDataEntityHolder( Core::DataEntityHolder::Pointer val )
{
	if( m_Input == val )
		return;
	m_Input = val;
}

Core::DataEntityHolder::Pointer Core::ClippingProcessor::GetOutputDataEntityHolder() const
{
	return m_Output;
}

void Core::ClippingProcessor::SetOutputDataEntityHolder( Core::DataEntityHolder::Pointer val )
{
	if( m_Output == val )
		return;
	m_Output = val;
}
