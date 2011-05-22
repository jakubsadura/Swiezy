/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "corePointInteractor.h"

// BaseLib
#include "blMitkInteractorHelper.h"

// CoreLib
#include "coreAssert.h"
#include "coreException.h"
#include "coreEnvironment.h"
#include "coreLogger.h"
#include "coreKernel.h"
#include "coreReportExceptionMacros.h"
#include "coreDataEntityHelper.h"
#include "coreVTKPolyDataHolder.h"
#include "corePointSetRenDataBuilder.h"
#include "coreRenDataFactory.h"

// Mitk
#include "mitkRenderingManager.h"
#include "mitkSmartPointerProperty.h"
#include "mitkGlobalInteraction.h"
#include "mitkInteractionConst.h"
#include "mitkDataTreeNode.h"
#include "mitkPointSet.h"
#include "mitkDataTreeHelper.h"
#include "mitkPointOperation.h"
#include "mitkPointOperation.h"

#include "vtkPointData.h"
#include "vtkStringArray.h"
#include "vtkSmartPointer.h"

//ITK
#include "itkCommand.h"

const char *STR_DEFAULT_NODE_NAME = "Connected point";

/**
 */
Core::PointInteractor::PointInteractor(
						Core::RenderingTree::Pointer renderingTree,
						Core::DataEntityHolder::Pointer selectedPoints,
						Core::DataEntityHolder::Pointer selectedData )
{
	try{
		SetSelectedPointsHolder( selectedPoints );
		SetSelectedData( selectedData );
		SetRenderingTree( renderingTree );

		m_bDisableMitkEvents = false;
		m_bUpdatingVtkPointSet = false;
		m_hideNode = false;

	}coreCatchExceptionsAddTraceAndThrowMacro(
		"Core::PointInteractor::PointInteractor");
}


/**
 */
Core::PointInteractor::~PointInteractor( )
{
	SetSelectedPointsHolder( NULL );
}



/**
 */
void Core::PointInteractor::ConnectToDataTreeNode( )
{
	m_bDisableMitkEvents = true;

	ConnectNodeToTree();

	CreateInteractor();

	ConnectInteractors();

	// Call the subclass
	OnInteractorConnected( );

	m_bDisableMitkEvents = false;

	// Update the MITK points
	OnVtkDataPointModified();

	// Show the node after changing the size
	m_renderingTree->Show( GetSelectedPointsDataEntity(), true );
}

/**
 */
void Core::PointInteractor::DisconnectFromDataTreeNode()
{
	try
	{
		DisconnectInteractors();

		DisconnectNodeFromTree();

		DestroyInteractor();
	}
	coreCatchExceptionsReportAndNoThrowMacro(
		"Core::PointInteractor::DisconnectFromDataTreeNode()");
}

void Core::PointInteractor::ConnectNodeToTree()
{
	// Hide the node, because the properties of the node (point size)
	// will be set by the interactor class
	m_renderingTree->Add( 
		GetSelectedPointsDataEntity( ), 
		false, 
		false );

	SetSelectedPoints( GetSelectedPointsDataEntity( ) );
}

void Core::PointInteractor::DisconnectNodeFromTree()
{
	if (GetSelectedPointsDataEntity().IsNull() )
	{
		return;
	}

	if ( m_hideNode )
	{
		m_renderingTree->Show( 
			GetSelectedPointsDataEntity( ),
			false );
		mitk::RenderingManager::GetInstance(  )->RequestUpdateAll();
	}



	mitk::RenderingManager::GetInstance(  )->RequestUpdateAll();
}

void Core::PointInteractor::ConnectInteractors()
{
	// MITK checks if the interactor is in the list. It will not be added twice
	mitk::GlobalInteraction::GetInstance()->AddInteractor( GetInternalInteractor() );
}

void Core::PointInteractor::DisconnectInteractors()
{
	if( GetInternalInteractor() )
	{
		mitk::GlobalInteraction::GetInstance()->RemoveInteractor( GetInternalInteractor() );
	}

}

/**
 */
void Core::PointInteractor::OnVtkDataPointModified()
{
	try{

		if ( !m_bUpdatingVtkPointSet && GetInternalInteractor() )
		{
			m_bDisableMitkEvents = true;
			m_bUpdatingVtkPointSet = true;

			mitk::PointSet::Pointer mitkData = GetSelectedPointsRenderingData( );

			// Update rendering node data 
			m_selectedPointsHolder->GetSubject()->Modified();

			mitk::RenderingManager::GetInstance()->RequestUpdateAll();

			m_bUpdatingVtkPointSet = false;
			m_bDisableMitkEvents = false;
		}

	// Avoid exceptions when calling GetProcessingData
	}coreCatchExceptionsLogAndNoThrowMacro(
		"Core::PointInteractor::OnMitkDataPointModified()");
}


int Core::PointInteractor::GetDirection( )
{
	return -1;
}

void Core::PointInteractor::SetBoundingBox( double bounds[ 6 ] )
{
}

void Core::PointInteractor::SetBoundingBox( Core::DataEntity::Pointer dataEntity )
{
	
}

int Core::PointInteractor::GetSelectedPoint( mitk::Point3D &selectedPoint )
{
	mitk::PointSet::Pointer mitkData = GetSelectedPointsRenderingData( );

	for ( int i = 0 ; i < mitkData->GetSize( GetTimeStep( ) ) ; i++ )
	{
		if ( mitkData->GetSelectInfo( i, GetTimeStep() ) )
		{
			selectedPoint = mitkData->GetPoint( i, GetTimeStep() );
			return i;
		}
	}

	return -1;
}

void Core::PointInteractor::OnPointSetMoveEvent()
{
	try{

		if ( !CheckUpdateDataOnEvent( INTERACTOR_EVENT_MOVE ) )
		{
			return;
		}

		UpdateVtkPointSet();

	}coreCatchExceptionsReportAndNoThrowMacro(
		"Core::PointInteractor::OnPointSetMoveEvent()");
}


bool Core::PointInteractor::CheckUpdateDataOnEvent( INTERACTOR_EVENT_TYPE eventType )
{
	return true;
}

void Core::PointInteractor::OnPointSetRemoveEvent()
{
	try{

		// If we are copying info from m_selectedPointsHolder -> m_MitkPointSet,
		// don't update m_selectedPointsHolder back
		if ( !m_bDisableMitkEvents )
		{
			// Update PointData
			UpdateVtkPointData( );

			// Update pointSet
			UpdateVtkPointSet( );
		}

	}coreCatchExceptionsReportAndNoThrowMacro(
		"Core::PointInteractor::OnPointSetRemoveEvent()");
}


void Core::PointInteractor::UpdateVtkPointSet()
{

	// If we are copying info from m_selectedPointsHolder -> m_MitkPointSet,
	// don't update m_selectedPointsHolder back
	if ( !m_bDisableMitkEvents )
	{
		m_bDisableMitkEvents = true;
		m_bUpdatingVtkPointSet = true;

		if ( GetTimeStep() >= m_selectedPointsHolder->GetSubject()->GetNumberOfTimeSteps( ) )
		{
			std::cout << "Error: Core::PointInteractor::UpdateVtkPointSet() " << std::endl;
		}

		vtkPolyDataPtr	processingData;
		Core::DataEntityHelper::GetProcessingData( 
			m_selectedPointsHolder, 
			processingData,
			GetTimeStep() );

		vtkPointSet *pointSet = processingData;
		vtkSmartPointer<vtkPoints> pointsVtk = pointSet->GetPoints( );
		if ( pointsVtk == NULL )
		{
			pointsVtk = vtkSmartPointer<vtkPoints>::New();
			pointSet->SetPoints( pointsVtk );
		}

		mitk::PointSet::Pointer mitkData = GetSelectedPointsRenderingData( );
		pointsVtk->SetNumberOfPoints( mitkData->GetSize( GetTimeStep() ) );
		mitk::PointSet::DataType *itkPointSet = mitkData->GetPointSet( GetTimeStep() );
		mitk::PointSet::PointsContainer *mitkPoints = itkPointSet->GetPoints();
		mitk::PointSet::PointsContainer::Iterator	it, end;
		int position = 0;
		end = mitkPoints->End();
		for( it = mitkPoints->Begin(); it != end; it++ )
		{
			if ( mitkPoints->IndexExists(it->Index()) )
			{
				mitk::PointSet::PointType p = it->Value();
				pointsVtk->SetPoint( position, p[ 0 ], p[ 1 ], p[ 2 ] );
				position++;
			}
		}

		// We need to update the time because other filters can depend on this
		pointSet->Modified();
		m_selectedPointsHolder->GetSubject( )->Modified( );
		m_selectedPointsHolder->NotifyObservers();

		m_bDisableMitkEvents = false;
		m_bUpdatingVtkPointSet = false;
	}
}

void Core::PointInteractor::UpdateVtkPointData()
{
	if( !GetInternalInteractor() )
	{
		return;
	}

	if ( GetTimeStep() >= m_selectedPointsHolder->GetSubject()->GetNumberOfTimeSteps( ) )
	{
		std::cout << "Error: Core::PointInteractor::UpdateVtkPointData() " << std::endl;
	}

	vtkPolyDataPtr	processingData;
	Core::DataEntityHelper::GetProcessingData( 
		m_selectedPointsHolder, 
		processingData,
		GetTimeStep() );
	vtkPointSet *pointSet = processingData;
	vtkPoints *pointsVtk = pointSet->GetPoints( );

	mitk::PointSet::Pointer mitkData = GetSelectedPointsRenderingData( );

	// Find removed point ID
	vtkIdType	removedPointID = -1;
	mitk::Point3D p;

	if ( pointsVtk != NULL )
	{
		for( int i = 0; i < pointsVtk->GetNumberOfPoints( ); i++ )
		{
			double x[ 3 ];
			pointsVtk->GetPoint( i, x );
			p[ 0 ] = (mitk::Point3D::ValueType) x[ 0 ];
			p[ 1 ] = (mitk::Point3D::ValueType) x[ 1 ];
			p[ 2 ] = (mitk::Point3D::ValueType) x[ 2 ];
			int iPos = mitkData->SearchPoint( p, 0, GetTimeStep() );

			// The point is not found
			if ( iPos == -1 )
			{
				removedPointID = i;
			}
		}
	}

	// Remove point data attached to the removed point
	vtkPointData* pointData = pointSet->GetPointData( );
	if ( pointData )
	{
		for ( int i = 0 ; i < pointData->GetNumberOfArrays( ) ; i++ )
		{
			// Remove the value from vtkDataArray
			vtkAbstractArray	*abstractArray;
			abstractArray = pointData->GetAbstractArray( i );
			vtkDataArray *dataArray;
			dataArray = vtkDataArray::SafeDownCast( abstractArray );
			if ( dataArray )
			{
				dataArray->RemoveTuple( removedPointID );
			}

			// Remove the value from vtkStringArray
			vtkStringArray *stringArray;
			stringArray = vtkStringArray::SafeDownCast( abstractArray );
			if ( stringArray )
			{
				for ( int j = removedPointID + 1 ; j < stringArray->GetNumberOfValues(); j++ )
				{
					vtkStdString str = stringArray->GetValue( j );
					stringArray->SetValue( j - 1, str );
				}
				stringArray->Resize( stringArray->GetNumberOfValues() - 1 );
			}
		}

	}

}


void Core::PointInteractor::OnPointSetAddEvent()
{
	try{

		// Update pointSet
		UpdateVtkPointSet( );

	}coreCatchExceptionsReportAndNoThrowMacro(
		"Core::PointInteractor::OnPointSetAddEvent()");
}

Core::DataEntity::Pointer Core::PointInteractor::GetSelectedPointsDataEntity()
{
	if ( m_selectedPointsHolder.IsNull( ) )
	{
		return NULL;
	}

	return m_selectedPointsHolder->GetSubject();
}

Core::DataEntity::Pointer Core::PointInteractor::GetSelectedDataEntity()
{
	return m_selectedData->GetSubject();
}

mitk::DataTreeNode::Pointer Core::PointInteractor::GetSelectedPointsNode()
{
	mitk::DataTreeNode::Pointer node;
	Core::CastAnyProcessingData( m_renderingTree->GetNode( GetSelectedPointsDataEntity( ) ), node );
	return node;
}

mitk::DataTreeNode::Pointer Core::PointInteractor::GetSelectedDataEntityNode()
{
	mitk::DataTreeNode::Pointer node;
	Core::CastAnyProcessingData( m_renderingTree->GetNode( GetSelectedDataEntity( ) ), node );
	return node;
}

mitk::PointSet::Pointer Core::PointInteractor::GetSelectedPointsRenderingData()
{
	if ( GetSelectedPointsDataEntity( ).IsNull() )
	{
		throw Core::Exceptions::Exception( 
			"PointInteractor", 
			"Input point set is NULL" );
	}
	mitk::BaseData::Pointer renderingData;
	boost::any anyData = GetSelectedPointsDataEntity( )->GetRenderingData( typeid( renderingData ) );
	Core::CastAnyProcessingData( anyData, renderingData );
	mitk::PointSet* pointSet = NULL;
	if ( renderingData.IsNull() )
	{
		throw Core::Exceptions::Exception( 
			"PointInteractor", 
			"Input points rendering data is NULL" );
	}
	pointSet = dynamic_cast<mitk::PointSet*>(renderingData.GetPointer()); 
	if ( pointSet == NULL )
	{
		throw Core::Exceptions::Exception( 
			"PointInteractor", 
			"Input points rendering data is not correct" );
	}
	return pointSet;
}

void Core::PointInteractor::OnInteractorConnected()
{

}

Core::RenderingTree::Pointer Core::PointInteractor::GetRenderingTree() const
{
	return m_renderingTree;
}

void Core::PointInteractor::SetRenderingTree( Core::RenderingTree::Pointer val )
{
	m_renderingTree = val;
}

Core::DataEntityHolder::Pointer Core::PointInteractor::GetSelectedPointsHolder() const
{
	return m_selectedPointsHolder;
}

void Core::PointInteractor::SetSelectedPointsHolder( 
	Core::DataEntityHolder::Pointer val )
{
	if ( m_selectedPointsHolder.IsNotNull() )
	{
		m_selectedPointsHolder->RemoveObserver<PointInteractor>(
			this, 
			&Self::OnVtkDataPointModified );
		m_selectedPointsHolder->RemoveObserver<PointInteractor>(
			this, 
			&Self::OnNewSelectedPoints,
			DH_NEW_SUBJECT );
	}

	m_selectedPointsHolder = val;

	if ( m_selectedPointsHolder.IsNotNull() )
	{
		SetSelectedPoints( m_selectedPointsHolder->GetSubject() );

		// Observers to processing data
		m_selectedPointsHolder->AddObserver<PointInteractor>(
			this, 
			&Self::OnVtkDataPointModified );
		m_selectedPointsHolder->AddObserver<PointInteractor>(
			this, 
			&Self::OnNewSelectedPoints,
			DH_NEW_SUBJECT );
	}
	else
	{
		SetSelectedPoints( NULL );
	}

}

Core::DataEntityHolder::Pointer Core::PointInteractor::GetSelectedData() const
{
	return m_selectedData;
}

void Core::PointInteractor::SetSelectedData( Core::DataEntityHolder::Pointer val )
{
	m_selectedData = val;
}

bool Core::PointInteractor::GetHideNode() const
{
	return m_hideNode;
}

void Core::PointInteractor::SetHideNode( bool val )
{
	m_hideNode = val;
}

void Core::PointInteractor::OnNewSelectedPoints()
{
	// Set new rendering data
	SetSelectedPoints( m_selectedPointsHolder->GetSubject() );
}

void Core::PointInteractor::SetSelectedPoints( 
	Core::DataEntity::Pointer pointsDataEntity )
{

	// Observers to rendering data
	try
	{
		mitk::BaseData::Pointer mitkData;
		if ( m_PointsDataEntity.IsNotNull() )
		{
			boost::any anyData = m_PointsDataEntity->GetRenderingData( typeid( mitkData ) );
			Core::CastAnyProcessingData( anyData, mitkData );
		}
		if ( mitkData.IsNotNull() )
		{
			// Remove observers to OLD mitkData
			mitkData->RemoveObserver( m_OnPointSetAddEventObserverTag );
			mitkData->RemoveObserver( m_OnPointSetMoveEventObserverTag );
			mitkData->RemoveObserver( m_OnPointSetRemoveEventObserverTag );

			// Set back the default rendering builder 
			//Core::PointSetRenDataBuilder::Pointer rdb = Core::PointSetRenDataBuilder::New();
			//m_PointsDataEntity->SetRenDataBuilder(rdb);

			// Set pointer to NULL
			m_PointsDataEntity = NULL;
		}
	}
	catch ( ... )
	{
	}

	m_PointsDataEntity = pointsDataEntity;

	mitk::BaseData::Pointer mitkData;
	if ( m_PointsDataEntity.IsNotNull() )
	{
		boost::any anyData = m_PointsDataEntity->GetRenderingData( typeid( mitkData ) );
		Core::CastAnyProcessingData( anyData, mitkData );

		// If rendering data is NULL, create a new one
		if ( mitkData.IsNull( ) )
		{
			// Build rendering data
			Core::RenDataFactory::Pointer factory = Core::RenDataFactory::New( );
			factory->SetInputDataEntity( 0, m_PointsDataEntity );
			factory->Update();
			boost::any anyData = m_PointsDataEntity->GetRenderingData( typeid( mitkData ) );
			Core::CastAnyProcessingData( anyData, mitkData );
		}

	}

	if ( mitkData.IsNotNull() )
	{
		// Set rendering builder to NULL because the interactor will take care of this
		//Core::DataEntity::Pointer pointsDataEntity = GetSelectedPointsDataEntity();
		//Core::PointSetRenDataBuilder::Pointer rdb;
		//pointsDataEntity->SetRenDataBuilder( rdb );

		//! Call back called when the user adds a point to the point set with the mouse.
		typedef itk::SimpleMemberCommand<PointInteractor>
			MemberCommandType;

		typedef MemberCommandType::Pointer MemberCommandPointerType;
		MemberCommandPointerType dataChangedCommand;
		dataChangedCommand = MemberCommandType::New();
		dataChangedCommand->SetCallbackFunction( this, &Self::OnPointSetAddEvent );
		m_OnPointSetAddEventObserverTag = mitkData->AddObserver( 
			mitk::PointSetAddEvent(), 
			dataChangedCommand);

		dataChangedCommand = MemberCommandType::New();
		dataChangedCommand->SetCallbackFunction( this, &Self::OnPointSetRemoveEvent );
		m_OnPointSetRemoveEventObserverTag = mitkData->AddObserver( 
			mitk::PointSetRemoveEvent(), 
			dataChangedCommand);

		dataChangedCommand = MemberCommandType::New();
		dataChangedCommand->SetCallbackFunction( this, &Self::OnPointSetMoveEvent );
		m_OnPointSetMoveEventObserverTag = mitkData->AddObserver( 
			mitk::PointSetMoveEvent(), 
			dataChangedCommand);
	}

}

