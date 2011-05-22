/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreCloseHolesProcessor.h"
#include "coreDataEntityHelper.h"
#include "coreEnvironment.h"

#include "vtkPointSet.h"
#include "vtkCleanPolyData.h"

#include "meCloseHoles.h"
#include "meHoleFiller.h"

#include "blShapeUtils.h"
#include "blTextUtils.h"

Core::CloseHolesProcessor::CloseHolesProcessor( )
{
	SetNumberOfInputs( 3 );
	GetInputPort( 0 )->SetName( "Input surface" );
	GetInputPort( 0 )->SetDataEntityType( Core::SurfaceMeshTypeId );
	GetInputPort( 1 )->SetName( "Input Hole" );
	GetInputPort( 1 )->SetDataEntityType( Core::SurfaceMeshTypeId | Core::SkeletonTypeId );
	GetInputPort( 2 )->SetName( "Input Point Hole" );
	GetInputPort( 2 )->SetDataEntityType( Core::PointSetTypeId );
	SetNumberOfOutputs( 3 );
	GetOutputPort( 0 )->SetDataEntityType( Core::SurfaceMeshTypeId | Core::SkeletonTypeId );
	GetOutputPort( 0 )->SetReuseOutput( false );
	GetOutputPort( 0 )->SetDataEntityName( "HolesComputed" );
	GetOutputPort( 1 )->SetDataEntityType( Core::SurfaceMeshTypeId );
	GetOutputPort( 1 )->SetReuseOutput( false );
	GetOutputPort( 1 )->SetDataEntityName( "SurfaceClosed" );
	GetOutputPort( 2 )->SetDataEntityType( Core::SurfaceMeshTypeId | Core::SkeletonTypeId );
	GetOutputPort( 2 )->SetReuseOutput( true );
	GetOutputPort( 2 )->SetDataEntityName( "HoleSelected" );

	SetName( "CloseHolesProcessor" );
	
	m_param = meMeshVolumeClosingParamsPtr( new meMeshVolumeClosingParams);
	m_meCloseHoles = meCloseHoles::New();

	m_CloseHoleMethod = 4;
	m_closeAll = true;
	m_PatchOutput = false;
}

void Core::CloseHolesProcessor::Update()
{
	try
	{
		Core::vtkPolyDataPtr input;
		Core::vtkPolyDataPtr inputHole = NULL;
		Core::vtkPolyDataPtr output = Core::vtkPolyDataPtr::New();
		GetProcessingData(0,input);
		if (!m_closeAll)
			GetProcessingData(1,inputHole);

		std::string name;
		SetState(Core::Runtime::APP_STATE_PROCESSING);

		//"SingleCenterPoint", "RadialTriangles","LinearToCenterNR",
		//"LinearToCenter","SmoothToCenter","SmoothPatch"
		if(m_CloseHoleMethod < 6)
		{

			m_meCloseHoles->SetAlgorithm((meCloseHoles::closeHoleAlgorithmEnum)m_CloseHoleMethod);
			m_meCloseHoles->SetInput(input);

			if (m_closeAll)
				m_meCloseHoles->Update();
			else
			{
				m_meCloseHoles->SetInputHole(inputHole);
				m_meCloseHoles->Run();
			}	
		
			if (!m_PatchOutput)
			{
				name = "SurfaceClosed";
				output->DeepCopy(m_meCloseHoles->GetOutput());
				UpdateOutput(1, output, name , false ,1, GetInputDataEntity(0) );
			}
			else
			{	
				int size = m_meCloseHoles->GetPatchOutput().size();
				for (int i=0;i< size;i++)
				{
					std::ostringstream currentFileName;
					name = "Patch_";
					currentFileName << name << i;
					output->DeepCopy(m_meCloseHoles->GetPatchOutput().at(i));
					UpdateOutput(1, output, currentFileName.str() , false ,1, GetInputDataEntity(0) );
				}
			}

			

		}
		else if (m_CloseHoleMethod == 6) //"CloseRefinerFairer"
		{
			meHoleFiller::Pointer holeFilling = meHoleFiller::New();
			holeFilling->SetInput( input );
			holeFilling->SetDensityFactor( m_param->m_DensityFactor );
			holeFilling->SetAngle( m_param->m_Angle );
			holeFilling->SetOrder( m_param->m_Order );
			holeFilling->SetPercentage( 0.0001 );
			holeFilling->SetMinimumEdgesSwapped( 10 );
			if (m_closeAll)
			{
				holeFilling->Update();
				GetOutputPort( 1 )->UpdateOutput( holeFilling->GetOutput(), 0, GetInputDataEntity(0) );
			}
			else
			{
				GetOutputPort( 1 )->UpdateOutput( NULL, 0, GetInputDataEntity(0) );
			}

		}

		SetState(Core::Runtime::APP_STATE_IDLE);
	}
	coreCatchExceptionsAddTraceAndThrowMacro(Core::CloseHolesProcessor::Update)
}

void Core::CloseHolesProcessor::ComputeHoles()
{
	try
	{
		Core::vtkPolyDataPtr input;
		GetProcessingData(0,input);

		vtkSmartPointer<vtkPolyDataNormals>  normals = 
			vtkSmartPointer<vtkPolyDataNormals>::New();
		normals->SetInput(input);
		normals->SplittingOff();
		normals->SetFeatureAngle(180.0);
		normals->ComputePointNormalsOff();
		normals->ComputeCellNormalsOn();
		normals->AutoOrientNormalsOn();
		normals->ConsistencyOn();
		normals->Update();
		normals->GetOutput()->BuildLinks();

		vtkSmartPointer<vtkFeatureEdges>  edges = vtkSmartPointer<vtkFeatureEdges>::New();
		edges->SetInput(normals->GetOutput());
		edges->BoundaryEdgesOn();
		edges->FeatureEdgesOff();
		edges->ManifoldEdgesOff();
		edges->NonManifoldEdgesOff();
		edges->Update();          

		//PolyDataConnectivityFilter: each hole is identified as a region
		vtkSmartPointer<vtkPolyDataConnectivityFilter> connec = 
			vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
		connec->SetInput(edges->GetOutput());
		connec->SetExtractionModeToAllRegions();
		connec->SetColorRegions(1);
		connec->Update();

		Core::vtkPolyDataPtr output = Core::vtkPolyDataPtr::New();
		output->DeepCopy(connec->GetOutput());
		UpdateOutput(0,output,"Holes",true, 1 , GetInputDataEntity(0) );
	}
	coreCatchExceptionsAddTraceAndThrowMacro(Core::CloseHolesProcessor::ComputeHoles() );


}

bool Core::CloseHolesProcessor::CheckInputs()
{

	Core::vtkPolyDataPtr polydata ;
	GetProcessingData( 0 , polydata );
	return true;

}

void Core::CloseHolesProcessor::SetCloseAll( bool val )
{
	m_closeAll = val;
}

void Core::CloseHolesProcessor::SetParams( meMeshVolumeClosingParamsPtr param )
{
	m_param = param;
}

meMeshVolumeClosingParamsPtr Core::CloseHolesProcessor::GetParams()
{
	return m_param;
}

void Core::CloseHolesProcessor::SetMethod( int method )
{
	m_CloseHoleMethod = method;
}

void Core::CloseHolesProcessor::SelectHole()
{

	try
	{
		Core::vtkPolyDataPtr holes;
		Core::DataEntityHelper::GetProcessingData(
			GetOutputDataEntityHolder(0),
			holes);

		Core::vtkPolyDataPtr polydata ;
		GetProcessingData( 2 , polydata );

		if ( polydata->GetNumberOfPoints() != 1)
			return;
		
		vtkSmartPointer<vtkPolyDataConnectivityFilter> connectivity =
			vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
		connectivity->SetInput(holes);
		connectivity->SetExtractionModeToClosestPointRegion();
		connectivity->SetClosestPoint(polydata->GetPoint(0));
		connectivity->Update();

		UpdateOutput(2,connectivity->GetOutput(),"Selected Hole", true,1,GetOutputDataEntity(0));
	}
	coreCatchExceptionsAddTraceAndThrowMacro("SelectHole")


}

void Core::CloseHolesProcessor::SetPatchOnly( bool val )
{
	m_PatchOutput = val;
}