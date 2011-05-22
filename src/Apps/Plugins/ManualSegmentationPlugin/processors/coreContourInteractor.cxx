/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

// CoreLib
#include "coreContourInteractor.h"
#include "coreAssert.h"
#include "coreException.h"
#include "coreEnvironment.h"
#include "coreLogger.h"
#include "coreKernel.h"
#include "coreReportExceptionMacros.h"
#include "coreDataTreeHelper.h"
#include "coreDataEntityHelper.h"
#include "coreVTKPolyDataHolder.h"
#include "coreVTKImageDataRenDataBuilder.h"
#include "coreRenderingTreeMITK.h"

// Mitk
#include "mitkRenderingManager.h"
#include "mitkSmartPointerProperty.h"
#include "mitkGlobalInteraction.h"
#include "mitkInteractionConst.h"
#include "mitkDataTreeNode.h"
#include "mitkDataTreeHelper.h"
#include "mitkVtkResliceInterpolationProperty.h"
#include "mitkDataStorage.h"
#include "mitkContourTool.h"

#include "vtkSmartPointer.h"
#include "vtkImageMathematics.h"
#include "blMITKUtils.h"

/**
 */
Core::ContourInteractor::ContourInteractor(
						Core::RenderingTree::Pointer renderingTree,
						Core::DataEntityHolder::Pointer selectedContour,
						Core::DataEntityHolder::Pointer selectedData )
{
	try{
		SetSelectedContourHolder( selectedContour );
		SetSelectedData( selectedData );
		SetRenderingTree( renderingTree );
		}
		coreCatchExceptionsAddTraceAndThrowMacro(
		"Core::ContourInteractor::ContourInteractor");

}


/**
 */
Core::ContourInteractor::~ContourInteractor( )
{
	if ( m_selectedContourHolder.IsNotNull() )
	{
		m_selectedContourHolder->RemoveObserver<ContourInteractor>(
			this, 
			&Self::OnContourModified );
	}

	DisconnectFromDataTreeNode();

	DestroyInteractor();
}

void Core::ContourInteractor::ConnectToDataTreeNode( )
{
	if ( !IsConnectedToRenderingTree (GetSelectedContourHolder()->GetSubject()))
		ConnectNodeToTree();

	CreateInteractor();

	ConnectInteractors();

	// Call the subclass
	OnInteractorConnected( );

	// add data entity to list
	OnContourModified();

	// Show the node after changing the size
	m_renderingTree->Show( GetSelectedContourDataEntity(), true );
}

void Core::ContourInteractor::DisconnectFromDataTreeNode()
{
	try
	{
		DisconnectInteractors();

		//Commented out. We don't want to disconnect the countour from the rendering node when the interactor is distructed. MB
		//DisconnectNodeFromTree();

		DestroyInteractor();
	}
	coreCatchExceptionsReportAndNoThrowMacro(
		"Core::ContourInteractor::DisconnectFromDataTreeNode()");
}

void Core::ContourInteractor::ConnectNodeToTree()
{
	if (GetSelectedDataEntity()->GetType() != Core::ImageTypeId )
	{
		throw Core::Exceptions::Exception( 
			"ContourInteractor", 
			"Input Data should be an image" );
	}
	// Hide the node, because the properties of the node (point size)
	//CreateEmptyWorkingData();

	//! add a check before doing the connections of the interactor
	if (GetSelectedContourDataEntity().IsNull() ) return;

	// will be set by the interactor class
	m_renderingTree->Add( 
		GetSelectedContourDataEntity( ), 
		false, 
		false );
}

void Core::ContourInteractor::DisconnectNodeFromTree()
{
	if ( GetSelectedContourDataEntity().IsNotNull() )
			m_renderingTree->Remove( 
				GetSelectedContourDataEntity( ),
				false );
}

void Core::ContourInteractor::ConnectInteractors()
{
	// MITK checks if the interactor is in the list. It will not be added twice
	m_toolManager->RegisterClient();				
	m_toolManager->ActivateTool(0);
	GetContourTool( )->SetPixelValue( GetPixelValue() );
	GetContourTool( )->SetEraseOnlySelectedColor( GetEraseOnlySelectedColor() );
}

void Core::ContourInteractor::DisconnectInteractors()
{
	if( m_toolManager )
	{
		m_toolManager->UnregisterClient();
		m_toolManager->SetWorkingData(NULL);
		//!TODO change SetReferenceData function in mitk::toolmanager in order not to accept null 
		m_toolManager->SetReferenceData(NULL);
	}

}

void Core::ContourInteractor::CreateInteractor()
{
	if ( m_toolManager.IsNull() )
	{
		RenderingTreeMITK* treeMITK = dynamic_cast<RenderingTreeMITK*> ( m_renderingTree.GetPointer( ) );
		m_toolManager = mitk::ToolManager::New( treeMITK->GetDataStorage() );
	}
}

bool Core::ContourInteractor::IsConnectedToRenderingTree(Core::DataEntity::Pointer dataEntity)
{
	mitk::DataTreeIteratorClone	itFound;
	bool bDataIsInRenderingTree = false;
	
	if( GetRenderingTree().IsNull() )
		return false;

	bDataIsInRenderingTree = GetRenderingTree()->IsDataEntityRendered( dataEntity );

	return bDataIsInRenderingTree;
}

void Core::ContourInteractor::DestroyInteractor()
{
	m_toolManager = NULL ;
}

void Core::ContourInteractor::OnInteractorConnected()
{
		m_toolManager->SetReferenceData(GetSelectedDataEntityNode());
		m_toolManager->SetWorkingData(GetSelectedContourNode());			
}

Core::DataEntity::Pointer Core::ContourInteractor::GetSelectedContourDataEntity()
{
	if ( m_selectedContourHolder.IsNull( ) )
	{
		return NULL;
	}

	return m_selectedContourHolder->GetSubject();
}

Core::DataEntity::Pointer Core::ContourInteractor::GetSelectedDataEntity()
{
	return m_selectedData->GetSubject();
}

mitk::DataTreeNode::Pointer Core::ContourInteractor::GetSelectedContourNode()
{
	mitk::DataTreeNode::Pointer node;
	boost::any anyData = GetRenderingTree()->GetNode( GetSelectedContourDataEntity( ) );
	Core::CastAnyProcessingData( anyData, node );
	return node;
}

mitk::DataTreeNode::Pointer Core::ContourInteractor::GetSelectedDataEntityNode()
{
	mitk::DataTreeNode::Pointer node;
	boost::any anyData = GetRenderingTree()->GetNode( GetSelectedDataEntity( ) );
	Core::CastAnyProcessingData( anyData, node );
	return node;
}

mitk::Image::Pointer Core::ContourInteractor::GetSelectedContourRenderingData()
{
	if ( GetSelectedContourDataEntity( ).IsNull() )
	{
		throw Core::Exceptions::Exception( 
			"ContourInteractor", 
			"Input Contour is NULL" );
	}
	mitk::BaseData::Pointer renderingData;
	boost::any anyData = GetSelectedContourDataEntity( )->GetRenderingData( typeid( renderingData ) );
	Core::CastAnyProcessingData( anyData, renderingData );
	mitk::Image* contour = NULL;
	if ( renderingData.IsNull() )
	{
		throw Core::Exceptions::Exception( 
			"ContourInteractor", 
			"Input Contour rendering data is NULL" );
	}
	contour = dynamic_cast<mitk::Image*>(renderingData.GetPointer()); 
	if ( contour == NULL )
	{
		throw Core::Exceptions::Exception( 
			"ContourInteractor", 
			"Input Contour rendering data is not correct" );
	}
	return contour;
}

Core::RenderingTree::Pointer Core::ContourInteractor::GetRenderingTree() const
{
	return m_renderingTree;
}

void Core::ContourInteractor::SetRenderingTree( Core::RenderingTree::Pointer val )
{
	m_renderingTree = val;
}

Core::DataEntityHolder::Pointer Core::ContourInteractor::GetSelectedContourHolder() const
{
	return m_selectedContourHolder;
}

void Core::ContourInteractor::SetSelectedContourHolder( Core::DataEntityHolder::Pointer val )
{
	if ( m_selectedContourHolder.IsNotNull() )
	{
		m_selectedContourHolder->RemoveObserver<ContourInteractor>(
			this, 
			&Self::OnContourModified );
	}

	// Observers to rendering data
	mitk::Image::Pointer mitkData = NULL;
	try
	{
		mitkData = GetSelectedContourRenderingData( );
	}
	catch ( ... )
	{
	}

	m_selectedContourHolder = val;

	// Set rendering builder to NULL because the interactor will take care of this
	/*Core::DataEntity::Pointer ContourDataEntity = GetSelectedContourDataEntity();
	Core::VTKImageDataRenDataBuilder::Pointer rdb;
	ContourDataEntity->SetRenDataBuilder( rdb );*/

	// Observers to processing data
	m_selectedContourHolder->AddObserver<ContourInteractor>(
		this, 
		&Self::OnContourModified );
}


void Core::ContourInteractor::OnContourModified( )
{
	if (GetSelectedContourHolder().IsNull() ) return;
	if (GetSelectedContourHolder()->GetSubject().IsNull() ) return;
    
	GetSelectedContourHolder()->GetSubject()->SetFather(GetSelectedDataEntity());
	Core::DataEntityHelper::AddDataEntityToList( GetSelectedContourHolder() , false);
	
}

Core::DataEntityHolder::Pointer Core::ContourInteractor::GetSelectedData() const
{
	return m_selectedData;
}

void Core::ContourInteractor::SetSelectedData( Core::DataEntityHolder::Pointer val )
{
	m_selectedData = val;
}

void Core::ContourInteractor::CreateEmptyWorkingData( )
{
	if ( GetSelectedData().IsNull() ||  GetSelectedContourHolder().IsNull()) return;

	Core::vtkImageDataPtr WorkingImage; 
	Core::DataEntityHelper::GetProcessingData(GetSelectedContourHolder(),
												WorkingImage);

	Core::vtkImageDataPtr ReferenceImage; 
	Core::DataEntityHelper::GetProcessingData(GetSelectedData(),
												ReferenceImage);

	WorkingImage->Initialize();
	vtkSmartPointer<vtkImageMathematics> operation = vtkSmartPointer<vtkImageMathematics>::New();
	operation->SetInput1(ReferenceImage);
	operation->SetInput2(ReferenceImage);
	operation->SetOperationToSubtract();
	operation->Update();
	WorkingImage->DeepCopy(operation->GetOutput());
	
	//GetSelectedContourDataEntity()->UpdateRenderingRepresentation();
}


//!
void Core::ContourInteractor::SetPixelValue(int n) {

	m_pixelValue = n;
	
	if ( GetContourTool( ) )
	{
		GetContourTool( )->SetPixelValue( m_pixelValue );
	}
}

//!
int Core::ContourInteractor::GetPixelValue() {
	return m_pixelValue;
}

void Core::ContourInteractor::SetEraseOnlySelectedColor(bool b) {
	m_bEraseOnlySelectedColor = b;

	if ( GetContourTool( ) )
	{
		GetContourTool( )->SetEraseOnlySelectedColor( GetEraseOnlySelectedColor() );
	}
}

bool Core::ContourInteractor::GetEraseOnlySelectedColor() {
	return m_bEraseOnlySelectedColor;
}

mitk::ContourTool* Core::ContourInteractor::GetContourTool()
{
	if( !m_toolManager )
	{
		return NULL;
	}

	mitk::Tool * tool = m_toolManager->GetToolById(0);
	mitk::ContourTool * contourTool = static_cast<mitk::ContourTool*>(tool);
	return contourTool;
}
