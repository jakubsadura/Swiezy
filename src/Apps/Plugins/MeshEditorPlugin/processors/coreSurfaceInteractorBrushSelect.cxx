/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreSurfaceInteractorBrushSelect.h"
#include "coreVTKPolyDataHolder.h"
#include "coreException.h"
#include "vtkCellData.h"
#include "coreDataTreeMITKHelper.h"

/**
 */
Core::SurfaceInteractorBrushSelect::SurfaceInteractorBrushSelect(
						Core::RenderingTree::Pointer renderingTree,
						Core::DataEntityHolder::Pointer selectedPoints,
						Core::DataEntityHolder::Pointer selectedData )
	:SurfaceInteractor( renderingTree, selectedPoints, selectedData )
{
}


/**
 */
Core::SurfaceInteractorBrushSelect::~SurfaceInteractorBrushSelect( )
{
	DisconnectFromDataTreeNode();
}

void Core::SurfaceInteractorBrushSelect::CreateInteractor()
{
	vtkPolyDataPtr polyData;
	if ( GetSelectedDataEntity( ) )
	{
		GetSelectedDataEntity( )->GetProcessingData( polyData );
	}

	if ( polyData == NULL )
	{
		throw Core::Exceptions::Exception( 
			"Core::SurfaceInteractor::CreateInteractor",
			"Input mesh cannot be NULL" );
	}

	if ( m_surfaceSelectInteractor.IsNull() )
	{
		m_surfaceSelectInteractor = blMitkTriangleSelectInteractor::New( 
			"SurfaceSelectBrushInteractor", 
			//GetSelectedPointsNode(),
			NULL,
			polyData,
			GetPointSet());
	}
}

void Core::SurfaceInteractorBrushSelect::DestroyInteractor()
{
	m_surfaceSelectInteractor = NULL ;
}

void Core::SurfaceInteractorBrushSelect::OnInteractorConnected()
{
}

mitk::Interactor* Core::SurfaceInteractorBrushSelect::GetInternalInteractor()
{
	return m_surfaceSelectInteractor;
}

Core::DataEntityType Core::SurfaceInteractorBrushSelect::GetInputTypes( )
{
	return Core::DataEntityType( 
		Core::SurfaceMeshTypeId | 
		Core::SkeletonTypeId );
}

int Core::SurfaceInteractorBrushSelect::GetTimeStep()
{
	return m_surfaceSelectInteractor->GetTimeStep( );
}

void Core::SurfaceInteractorBrushSelect::SelectSurface()
{
	m_surfaceSelectInteractor->SetRadius(GetValue());
	vtkSmartPointer<vtkIdList> pList = vtkSmartPointer<vtkIdList>::New() ;
	vtkIdType currentPointId;
	vtkIdType currentCellId = -1;
	Core::vtkPolyDataPtr  polydataInput;
	GetSelectedDataEntity()->GetProcessingData(polydataInput);

	Core::vtkPolyDataPtr  pointInput;
	GetSelectedPointsDataEntity()->GetProcessingData(pointInput);


	currentPointId = polydataInput->FindPoint(pointInput->GetPoint(0));
	
	/*vtkSmartPointer<vtkSphere> sphere = vtkSmartPointer<vtkSphere>::New();
	sphere->SetCenter(polydataInput->GetPoint(currentPointId));
	sphere->SetRadius(0.5);*/

	if (!polydataInput->GetCellData()->HasArray("select"))
	{
		vtkShortArray *selArray = vtkShortArray::New();
		selArray->SetName("select");
		selArray->SetNumberOfValues( polydataInput->GetNumberOfCells());
		// For each cell id at input
		for ( vtkIdType i = 0; i < polydataInput->GetNumberOfCells(); i++ )
		{		
				selArray->SetTuple1(i, 0);		
		}
		polydataInput->GetCellData()->AddArray(selArray);
	}

	int selectionValue;
	if (GetFlagSelection())	selectionValue = 20;
	else selectionValue = 0;
	
	//Select Triangles of brush
	vtkSmartPointer<vtkIdList> cellsList = vtkSmartPointer<vtkIdList>::New() ;
	
	cellsList->DeepCopy(SelectCells(GetSelectionSphere()));
	for (int j = 0; j< cellsList->GetNumberOfIds(); j++)
	{
		polydataInput->GetCellData()->GetArray("select")->SetTuple1(cellsList->GetId(j),selectionValue);
	}

	// Update the data entity and the holder
	polydataInput->GetCellData()->SetActiveScalars("select");
	Core::DataTreeMITKHelper::ApplyLookupTableToMesh( 
		GetSelectedDataEntityNode(), blMITKUtils::LUT_SCALAR_MODE_CELL_DATA );


}

void Core::SurfaceInteractorBrushSelect::DeselectSurface()
{
	
}

vtkSphere* Core::SurfaceInteractorBrushSelect::GetSelectionSphere()
{
	vtkSphere* sphere = NULL;

	if ( m_surfaceSelectInteractor.IsNotNull( ) )
	{
		sphere = m_surfaceSelectInteractor->GetImplicitSphere();		
	}
	return sphere;
}

void Core::SurfaceInteractorBrushSelect::SetRadius( double val )
{
	m_surfaceSelectInteractor->SetRadius(val);		
}

double Core::SurfaceInteractorBrushSelect::GetRadius()
{
	return m_surfaceSelectInteractor->GetRadius();
}