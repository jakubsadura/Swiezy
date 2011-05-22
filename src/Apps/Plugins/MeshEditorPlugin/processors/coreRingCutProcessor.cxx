/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreRingCutProcessor.h"
#include "coreDataEntityHelper.h"
#include "coreEnvironment.h"

#include "vtkPointSet.h"
#include "vtkCleanPolyData.h"

#include "meRingCut.h"

#include "blShapeUtils.h"


Core::RingCutProcessor::RingCutProcessor( )
{
	SetNumberOfInputs( 2 );
	GetInputPort( 0 )->SetName( "Input surface" );
	GetInputPort( 0 )->SetDataEntityType( Core::SurfaceMeshTypeId );
	GetInputPort( 1 )->SetName( "Input point" );
	GetInputPort( 1 )->SetDataEntityType( Core::PointSetTypeId );
	SetNumberOfOutputs( 3 );
	GetOutputPort( 0 )->SetDataEntityType( Core::SurfaceMeshTypeId );
	GetOutputPort( 0 )->SetReuseOutput( false );
	GetOutputPort( 0 )->SetDataEntityName( "Surface1" );
	GetOutputPort( 1 )->SetDataEntityType( Core::SurfaceMeshTypeId );
	GetOutputPort( 1 )->SetReuseOutput( false );
	GetOutputPort( 1 )->SetDataEntityName( "Surface2" );
	GetOutputPort( 2 )->SetDataEntityType( Core::SurfaceMeshTypeId );
	GetOutputPort( 2 )->SetReuseOutput( true );
	GetOutputPort( 2 )->SetDataEntityName( "Ring" );

	SetName( "RingCutProcessor" );
	
	m_meRingCut = meRingCut::New();
}

void Core::RingCutProcessor::Update()
{
	try
	{
		SetState(Core::Runtime::APP_STATE_PROCESSING);

		vtkPolyDataPtr out1 = vtkPolyDataPtr::New();
		out1->DeepCopy( m_meRingCut->GetFirstOutput() );
		GetOutputPort( 0 )->UpdateOutput( out1, 0, GetInputDataEntity(0) );
		vtkPolyDataPtr out2 = vtkPolyDataPtr::New();
		out2->DeepCopy( m_meRingCut->GetSecondOutput() );
		GetOutputPort( 1 )->UpdateOutput( out2, 0, GetInputDataEntity(0) );

		SetState(Core::Runtime::APP_STATE_IDLE);
	}
	coreCatchExceptionsAddTraceAndThrowMacro(Core::RingCutProcessor::Update)
}

void Core::RingCutProcessor::ComputePlaneUsingRingCut()
{
	try
	{
		
		vtkIdType pointId;

		
		Core::vtkPolyDataPtr vtkPoint;
		Core::DataEntityHelper::GetProcessingData(GetInputDataEntityHolder( 1 ),
			vtkPoint);
		Core::vtkPolyDataPtr surface;
		Core::DataEntityHelper::GetProcessingData(GetInputDataEntityHolder( 0 ),
			surface);
		
		if ( !(vtkPoint->GetNumberOfPoints()>0 &&
			GetInputDataEntity( 1 )->GetFather() == GetInputDataEntity( 0 ) ))
		{
			throw Core::Exceptions::Exception(
				"RingCutProcessor::UpdatePlane",
				"Landmarks is not correct" );
		}

		vtkSmartPointer<vtkCleanPolyData> clean = vtkSmartPointer<vtkCleanPolyData>::New();
		clean->SetInput(surface);
		clean->Update();

		pointId = clean->GetOutput()->FindPoint(
			vtkPoint->GetPoint(0)[0],
			vtkPoint->GetPoint(0)[1],
			vtkPoint->GetPoint(0)[2]);

		m_meRingCut->SetInput(clean->GetOutput());
		m_meRingCut->SetOriginMeshId(pointId);
		m_meRingCut->Update();

		Core::vtkPolyDataPtr polydata = m_meRingCut->GetContourCrossSection();

		GetOutputPort( 2 )->UpdateOutput( polydata, 0, GetInputDataEntity(0) );
	}
	coreCatchExceptionsAddTraceAndThrowMacro(Core::RingCutProcessor::ComputePlaneUsingRingCut() );


}

bool Core::RingCutProcessor::CheckInputs()
{

	Core::vtkPolyDataPtr polydata ;
	GetProcessingData( 0 , polydata );
	return true;

}
