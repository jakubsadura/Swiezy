/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreSurfaceInteractorSphereSelect.h"
#include "coreVTKPolyDataHolder.h"
#include "coreException.h"
#include "coreDataTreeMITKHelper.h"

#include "vtkPolyDataNormals.h"
#include "vtkCellData.h"

/**
 */
Core::SurfaceInteractorSphereSelect::SurfaceInteractorSphereSelect(
						Core::RenderingTree::Pointer renderingTree,
						Core::DataEntityHolder::Pointer selectedPoints,
						Core::DataEntityHolder::Pointer selectedData )
	:	SurfaceInteractor( renderingTree, selectedPoints, selectedData )
{
}


/**
 */
Core::SurfaceInteractorSphereSelect::~SurfaceInteractorSphereSelect( )
{
	DisconnectFromDataTreeNode();
}

void Core::SurfaceInteractorSphereSelect::CreateInteractor()
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
		m_surfaceSelectInteractor = blMitkSurfaceSelectInteractor::New( 
			"SurfaceSelectInteractor", 
			NULL,
			polyData,
			GetSphereNode(),
			GetPointSet());
	}
}

void Core::SurfaceInteractorSphereSelect::DestroyInteractor()
{
	m_surfaceSelectInteractor = NULL ;
}

void Core::SurfaceInteractorSphereSelect::OnInteractorConnected()
{
	/*vtkSphere* m_selectionSphere=vtkSphere::New();
	m_selectionSphere =GetSelectionSphere();
	m_selectionSphere->AddObserver( vtkCommand::LeftButtonReleaseEvent, this )*/;
}

mitk::Interactor* Core::SurfaceInteractorSphereSelect::GetInternalInteractor()
{
	return m_surfaceSelectInteractor;
}


int Core::SurfaceInteractorSphereSelect::GetDirection( )
{
	int iDirection = -1;
	if ( m_surfaceSelectInteractor.IsNotNull() )
	{
		iDirection = m_surfaceSelectInteractor->GetDirection( );
	}
	return iDirection;
}


void Core::SurfaceInteractorSphereSelect::SetBoundingBox( double bounds[ 6 ] )
{
	if ( m_surfaceSelectInteractor.IsNotNull() )
	{
		m_surfaceSelectInteractor->SetBoundingBox( bounds );
	}
}

vtkSphere* Core::SurfaceInteractorSphereSelect::GetSelectionSphere()
{
	vtkSphere* sphere = NULL;

	if ( m_surfaceSelectInteractor.IsNotNull( ) )
	{
		sphere = m_surfaceSelectInteractor->GetImplicitSphere();		
	}
	return sphere;
}

Core::DataEntityType Core::SurfaceInteractorSphereSelect::GetInputTypes( )
{
	return Core::DataEntityType( 
		Core::SurfaceMeshTypeId | 
		Core::SkeletonTypeId );
}

int Core::SurfaceInteractorSphereSelect::GetTimeStep()
{
	return m_surfaceSelectInteractor->GetTimeStep( );
}

void Core::SurfaceInteractorSphereSelect::SetSelectionSphereHolder( 
	Core::DataEntityHolder::Pointer sphereHolder )
{
	m_selectionSphere = sphereHolder;
	GetRenderingTree()->Add(m_selectionSphere->GetSubject(),
		false,
		false);
}

mitk::DataTreeNode::Pointer Core::SurfaceInteractorSphereSelect::GetSphereNode()
{
	mitk::DataTreeNode::Pointer node;
	boost::any anyData = GetRenderingTree()->GetNode( m_selectionSphere->GetSubject( ) );
	Core::CastAnyProcessingData( anyData, node );
	return node;
}

void Core::SurfaceInteractorSphereSelect::SelectSurface()
{
	if (GetPointSet()->GetSize() == 2 &&
		GetSelectionSphere()->GetRadius()>0)
	{
		if (!GetSelectionSphere())
		{
			return;
		}

		// Clean Input before extracting cells avoid misbehavior
		Core::vtkPolyDataPtr  polydataInput;
		GetSelectedDataEntity()->GetProcessingData(polydataInput);
	
		vtkSmartPointer<vtkIdList> pList= vtkSmartPointer<vtkIdList>::New();
		pList->DeepCopy( SelectCells(GetSelectionSphere() ));
		
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

		// For each cell id at input
		for (int j = 0; j< pList->GetNumberOfIds(); j++)
		{
			polydataInput->GetCellData()->GetArray("select")->SetTuple1(pList->GetId(j),selectionValue);
		}

		// Update the data entity and the holder
		polydataInput->GetCellData()->SetActiveScalars("select");
		Core::DataTreeMITKHelper::ApplyLookupTableToMesh( 
			GetSelectedDataEntityNode(), blMITKUtils::LUT_SCALAR_MODE_CELL_DATA );
		m_surfaceSelectInteractor->ResetSphere();
	}
}

void Core::SurfaceInteractorSphereSelect::DeselectSurface()
{

}