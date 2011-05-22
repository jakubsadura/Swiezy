/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreSurfaceInteractorTriangleSelect.h"
#include "coreVTKPolyDataHolder.h"
#include "coreException.h"
#include "vtkCellData.h"
#include "coreDataTreeMITKHelper.h"

/**
 */
Core::SurfaceInteractorTriangleSelect::SurfaceInteractorTriangleSelect(
						Core::RenderingTree::Pointer renderingTree,
						Core::DataEntityHolder::Pointer selectedPoints,
						Core::DataEntityHolder::Pointer selectedData )
	:SurfaceInteractor( renderingTree, selectedPoints, selectedData )
{
}


/**
 */
Core::SurfaceInteractorTriangleSelect::~SurfaceInteractorTriangleSelect( )
{
	DisconnectFromDataTreeNode();
}

void Core::SurfaceInteractorTriangleSelect::CreateInteractor()
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
			"SurfaceSelectTriangleInteractor", 
			//GetSelectedPointsNode(),
			NULL,
			polyData,
			GetPointSet());
	}
}

void Core::SurfaceInteractorTriangleSelect::DestroyInteractor()
{
	m_surfaceSelectInteractor = NULL ;
}

void Core::SurfaceInteractorTriangleSelect::OnInteractorConnected()
{
}

mitk::Interactor* Core::SurfaceInteractorTriangleSelect::GetInternalInteractor()
{
	return m_surfaceSelectInteractor;
}

Core::DataEntityType Core::SurfaceInteractorTriangleSelect::GetInputTypes( )
{
	return Core::DataEntityType( 
		Core::SurfaceMeshTypeId | 
		Core::SkeletonTypeId );
}

int Core::SurfaceInteractorTriangleSelect::GetTimeStep()
{
	return m_surfaceSelectInteractor->GetTimeStep( );
}

void Core::SurfaceInteractorTriangleSelect::SetSelectionSphereHolder( 
	Core::DataEntityHolder::Pointer sphereHolder )
{
	m_selectionSphere = sphereHolder;
	GetRenderingTree()->Add(m_selectionSphere->GetSubject(),
		false,
		false);
}

void Core::SurfaceInteractorTriangleSelect::SelectSurface()
{
	
	vtkSmartPointer<vtkIdList> pList = vtkSmartPointer<vtkIdList>::New() ;
	vtkIdType currentCellId = -1;
	vtkIdType currentPointId;
	Core::vtkPolyDataPtr  polydataInput;
	GetSelectedDataEntity()->GetProcessingData(polydataInput);

	Core::vtkPolyDataPtr  pointInput;
	GetSelectedPointsDataEntity()->GetProcessingData(pointInput);

	currentPointId = polydataInput->FindPoint(pointInput->GetPoint(0));
	polydataInput->GetPointCells(currentPointId,pList);
	for (int k =0;k<pList->GetNumberOfIds();k++)
	{
		double bounds[6];
		bounds[0]=bounds[1]=bounds[2]=bounds[3]=bounds[4]=bounds[5]=0;
		polydataInput->GetCellBounds(pList->GetId(k), bounds);
		if ( pointInput->GetPoint(0)[0]>bounds[0] && pointInput->GetPoint(0)[0]<bounds[1]&&
			pointInput->GetPoint(0)[1]>bounds[2] && pointInput->GetPoint(0)[1]<bounds[3] &&
			pointInput->GetPoint(0)[2]>bounds[4] && pointInput->GetPoint(0)[2]<bounds[5])
			currentCellId = pList->GetId(k);
	}

	if (currentCellId == -1)
		currentCellId = pList->GetId(0);

	if (currentCellId < -1)
		return;

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

	polydataInput->GetCellData()->GetArray("select")->SetTuple1(currentCellId,selectionValue);

	// Update the data entity and the holder
	polydataInput->GetCellData()->SetActiveScalars("select");
	Core::DataTreeMITKHelper::ApplyLookupTableToMesh( 
		GetSelectedDataEntityNode(), blMITKUtils::LUT_SCALAR_MODE_CELL_DATA );
}

void Core::SurfaceInteractorTriangleSelect::DeselectSurface()
{
	vtkSmartPointer<vtkIdList> pList = vtkSmartPointer<vtkIdList>::New() ;
	vtkIdType currentCellId;
	vtkIdType currentPointId;
	Core::vtkPolyDataPtr  polydataInput;
	GetSelectedDataEntity()->GetProcessingData(polydataInput);

	Core::vtkPolyDataPtr  pointInput;
	GetSelectedPointsDataEntity()->GetProcessingData(pointInput);

	currentPointId = polydataInput->FindPoint(pointInput->GetPoint(0));
	polydataInput->GetPointCells(currentPointId,pList);
	currentPointId = polydataInput->FindPoint(pointInput->GetPoint(0));
	polydataInput->GetPointCells(currentPointId,pList);
	for (int k =0;k<pList->GetNumberOfIds();k++)
	{
		double bounds[6];
		bounds[0]=bounds[1]=bounds[2]=bounds[3]=bounds[4]=bounds[5]=0;
		polydataInput->GetCellBounds(pList->GetId(k), bounds);
		if ( pointInput->GetPoint(0)[0]>bounds[0] && pointInput->GetPoint(0)[0]<bounds[1]&&
			pointInput->GetPoint(0)[1]>bounds[2] && pointInput->GetPoint(0)[1]<bounds[3] &&
			pointInput->GetPoint(0)[2]>bounds[4] && pointInput->GetPoint(0)[2]<bounds[5])
			currentCellId = pList->GetId(k);
	}

	if (currentCellId == -1)
		currentCellId = pList->GetId(0);

	//if (!polydataInput->GetCellData()->HasArray("select"));
	//{
	//	std::cout<< "Error! you cannot deselect if you have not selected!"<<std::endl;
	//	return;
	//}

	polydataInput->GetCellData()->GetArray("select")->SetTuple1(currentCellId,0);

	// Update the data entity and the holder
	polydataInput->GetCellData()->SetActiveScalars("select");
	blMITKUtils::LUT_SCALAR_MODE scalarMode;
	scalarMode = Core::DataTreeMITKHelper::SetDefaultActiveScalars( GetSelectedDataEntity( ) );
	Core::DataTreeMITKHelper::ApplyLookupTableToMesh( GetSelectedDataEntityNode(), scalarMode );

}