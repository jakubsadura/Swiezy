/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreSurfaceInteractor.h"

// BaseLib
#include "blMitkInteractorHelper.h"

// CoreLib
#include "coreAssert.h"
#include "coreException.h"
#include "coreEnvironment.h"
#include "coreLogger.h"
#include "coreKernel.h"
#include "coreReportExceptionMacros.h"
#include "coreDataTreeHelper.h"
#include "coreDataEntityHelper.h"
#include "coreVTKPolyDataHolder.h"
#include "coreRenDataFactory.h"
#include "coreDataTreeMITKHelper.h"

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
#include "vtkPolyDataNormals.h"

#include "meVTKExtractPolyDataGeometryCellsID.h"

//ITK
#include "itkCommand.h"

/**
 */
Core::SurfaceInteractor::SurfaceInteractor(
						Core::RenderingTree::Pointer renderingTree,
						Core::DataEntityHolder::Pointer selectedPoints,
						Core::DataEntityHolder::Pointer selectedData )
{
	try{
		m_pointSet = mitk::PointSet::New();
		if ( m_pointSet.IsNotNull() )
		{
			//! Call back called when the user adds a point to the point set with the mouse.
			typedef itk::SimpleMemberCommand<SurfaceInteractor>
				MemberCommandType;

			typedef MemberCommandType::Pointer MemberCommandPointerType;
			MemberCommandPointerType dataChangedCommand;
			dataChangedCommand = MemberCommandType::New();
			dataChangedCommand->SetCallbackFunction( this, &Self::OnPointSetAddEvent );
			m_OnPointSetAddEventObserverTag = m_pointSet->AddObserver( 
				mitk::PointSetAddEvent(), 
				dataChangedCommand);

			dataChangedCommand = MemberCommandType::New();
			dataChangedCommand->SetCallbackFunction( this, &Self::OnPointSetRemoveEvent );
			m_OnPointSetRemoveEventObserverTag = m_pointSet->AddObserver( 
				mitk::PointSetRemoveEvent(), 
				dataChangedCommand);
			/*
			dataChangedCommand = MemberCommandType::New();
			dataChangedCommand->SetCallbackFunction( this, &Self::OnPointSetMoveEvent );
			m_OnPointSetMoveEventObserverTag = m_pointSet->AddObserver( 
				mitk::PointSetMoveEvent(), 
				dataChangedCommand);*/
		}

		SetSelectedPointsHolder( selectedPoints );
		SetSelectedData( selectedData );
		SetRenderingTree( renderingTree );

		m_bUpdatingMitkPointSet = false;
		m_bUpdatingVtkPointSet = false;
		m_hideNode = false;
		m_value = 5.0;
		m_selectFlag = true;

	}coreCatchExceptionsAddTraceAndThrowMacro(
		"Core::SurfaceInteractor::SurfaceInteractor");
}


/**
 */
Core::SurfaceInteractor::~SurfaceInteractor( )
{
	SetSelectedPointsHolder( NULL );
	if ( m_pointSet.IsNotNull() )
	{
		// Remove observers to OLD mitkData
		m_pointSet->RemoveObserver( m_OnPointSetAddEventObserverTag );
		//m_pointSet->RemoveObserver( m_OnPointSetMoveEventObserverTag );
		m_pointSet->RemoveObserver( m_OnPointSetRemoveEventObserverTag );
	}
}



/**
 */
void Core::SurfaceInteractor::ConnectToDataTreeNode( )
{
	ConnectNodeToTree();

	CreateInteractor();

	ConnectInteractors();

	// Call the subclass
	OnInteractorConnected( );

	// Update the MITK points
//	OnVtkDataPointModified();

	// Show the node after changing the size
//	m_renderingTree->Show( GetSelectedPointsDataEntity(), true );
}

/**
 */
void Core::SurfaceInteractor::DisconnectFromDataTreeNode()
{
	try
	{
		DisconnectInteractors();

		DisconnectNodeFromTree();

		DestroyInteractor();
	}
	coreCatchExceptionsReportAndNoThrowMacro(
		"Core::SurfaceInteractor::DisconnectFromDataTreeNode()");
}

void Core::SurfaceInteractor::ConnectNodeToTree()
{
	// Hide the node, because the properties of the node (point size)
	// will be set by the interactor class
// 	m_renderingTree->Add( 
// 		GetSelectedPointsDataEntity( ), 
// 		false, 
// 		false );

//	SetSelectedPoints( GetSelectedPointsDataEntity( ) );
}

void Core::SurfaceInteractor::DisconnectNodeFromTree()
{
	if (GetSelectedPointsDataEntity().IsNull() )
	{
		return;
	}

	if ( m_hideNode )
	{
	/*	m_renderingTree->Show( 
			GetSelectedPointsDataEntity( ),
			false );*/
		mitk::RenderingManager::GetInstance(  )->RequestUpdateAll();
	}



	mitk::RenderingManager::GetInstance(  )->RequestUpdateAll();
}

void Core::SurfaceInteractor::ConnectInteractors()
{
	// MITK checks if the interactor is in the list. It will not be added twice
	mitk::GlobalInteraction::GetInstance()->AddInteractor( GetInternalInteractor() );
}

void Core::SurfaceInteractor::DisconnectInteractors()
{
	if( GetInternalInteractor() )
	{
		mitk::GlobalInteraction::GetInstance()->RemoveInteractor( GetInternalInteractor() );
	}

}

/**
 */
void Core::SurfaceInteractor::OnVtkDataPointModified()
{
	try{

		if ( !m_bUpdatingVtkPointSet && GetInternalInteractor() )
		{
			m_bUpdatingMitkPointSet = true;

			mitk::PointSet::Pointer mitkData = GetSelectedPointsRenderingData( );

			// Resize the MITK point set if necessary
			unsigned iNumTimeSteps = m_selectedPointsHolder->GetSubject( )->GetNumberOfTimeSteps( );
			if ( mitkData->GetTimeSteps() < iNumTimeSteps )
			{
				// NOTE: This method will never REDUCE the vector size
				mitkData->Expand( iNumTimeSteps );
			}

			// We should use iNumTimeSteps
			for ( unsigned iTimeStep = 0 ; iTimeStep < iNumTimeSteps ; iTimeStep++ )
			{
				vtkPolyDataPtr	processingData;
				Core::DataEntityHelper::GetProcessingData( 
					m_selectedPointsHolder, 
					processingData, 
					iTimeStep );

				// Remove all points
				mitk::PointSet::PointsContainer *pointsItk;
				mitk::PointSet::PointsContainer::iterator it;
				pointsItk = mitkData->GetPointSet( iTimeStep )->GetPoints();
				while ( pointsItk->size( ) )
				{
					mitk::ScalarType timeInMS = 0.0;
					mitk::Point3D point;
					timeInMS = mitkData->GetTimeSlicedGeometry()->TimeStepToMS( iTimeStep );
					mitk::PointOperation* doOp = new mitk::PointOperation(
						mitk::OpREMOVE, timeInMS, point, pointsItk->begin( )->first );
					mitkData->ExecuteOperation( doOp );
				}

				// Add all points
				vtkPoints *points = processingData->GetPoints( );
				if ( points != NULL )
				{
					for( int i = 0; i < points->GetNumberOfPoints(); i++ )
					{
						mitk::Point3D point;
						point[ 0 ] = points->GetPoint( i )[ 0 ];
						point[ 1 ] = points->GetPoint( i )[ 1 ];
						point[ 2 ] = points->GetPoint( i )[ 2 ];
						mitkData->InsertPoint( i, point, iTimeStep );
					}
				}
			}

			UpdateRenderingNodeLabels( );

			mitk::RenderingManager::GetInstance()->RequestUpdateAll();

			m_bUpdatingMitkPointSet = false;
		}

	// Avoid exceptions when calling GetProcessingData
	}coreCatchExceptionsLogAndNoThrowMacro(
		"Core::SurfaceInteractor::OnMitkDataPointModified()");
}


int Core::SurfaceInteractor::GetDirection( )
{
	return -1;
}

void Core::SurfaceInteractor::SetBoundingBox( double bounds[ 6 ] )
{
}

void Core::SurfaceInteractor::SetBoundingBox( Core::DataEntity::Pointer dataEntity )
{
	
}

int Core::SurfaceInteractor::GetSelectedPoint( mitk::Point3D &selectedPoint )
{
	//mitk::PointSet::Pointer mitkData = GetSelectedPointsRenderingData( );

	for ( int i = 0 ; i < m_pointSet->GetSize( GetTimeStep( ) ) ; i++ )
	{
		if ( m_pointSet->GetSelectInfo( i, GetTimeStep() ) )
		{
			selectedPoint = m_pointSet->GetPoint( i, GetTimeStep() );
			return i;
		}
	}

	return -1;
}

void Core::SurfaceInteractor::OnPointSetMoveEvent()
{
	try{

		if ( !CheckUpdateDataOnEvent( INTERACTOR_EVENT_MOVE ) )
		{
			return;
		}

		UpdateVtkPointSet();

	}coreCatchExceptionsReportAndNoThrowMacro(
		"Core::SurfaceInteractor::OnPointSetMoveEvent()");
}


bool Core::SurfaceInteractor::CheckUpdateDataOnEvent( INTERACTOR_EVENT_TYPE eventType )
{
	return true;
}

void Core::SurfaceInteractor::OnPointSetRemoveEvent()
{
	try{

			/*UpdateMitkToVtkPoints();
			DeselectSurface();*/

	}coreCatchExceptionsReportAndNoThrowMacro(
		"Core::SurfaceInteractor::OnPointSetRemoveEvent()");
}


void Core::SurfaceInteractor::UpdateVtkPointSet()
{

	// If we are copying info from m_selectedPointsHolder -> m_MitkPointSet,
	// don't update m_selectedPointsHolder back
	if ( !m_bUpdatingMitkPointSet )
	{

		m_bUpdatingVtkPointSet = true;

		if ( GetTimeStep() >= m_selectedPointsHolder->GetSubject()->GetNumberOfTimeSteps( ) )
		{
			std::cout << "Error: Core::SurfaceInteractor::UpdateVtkPointSet() " << std::endl;
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

		// After notifying to observers -> Add the labels to the rendering node
		UpdateRenderingNodeLabels( );

		m_bUpdatingVtkPointSet = false;
	}
}

void Core::SurfaceInteractor::UpdateVtkPointData()
{
	if ( GetTimeStep() >= m_selectedPointsHolder->GetSubject()->GetNumberOfTimeSteps( ) )
	{
		std::cout << "Error: Core::SurfaceInteractor::UpdateVtkPointData() " << std::endl;
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


void Core::SurfaceInteractor::OnPointSetAddEvent()
{
	try{

		UpdateMitkToVtkPoints();
		SelectSurface();

	}coreCatchExceptionsReportAndNoThrowMacro(
		"Core::SurfaceInteractor::OnPointSetAddEvent()");
}

Core::DataEntity::Pointer Core::SurfaceInteractor::GetSelectedPointsDataEntity()
{
	if ( m_selectedPointsHolder.IsNull( ) )
	{
		return NULL;
	}

	return m_selectedPointsHolder->GetSubject();
}

Core::DataEntity::Pointer Core::SurfaceInteractor::GetSelectedDataEntity()
{
	return m_selectedData->GetSubject();
}

mitk::DataTreeNode::Pointer Core::SurfaceInteractor::GetSelectedPointsNode()
{
	mitk::DataTreeNode::Pointer node;
	boost::any anyData = GetRenderingTree()->GetNode( GetSelectedPointsDataEntity( ) );
	Core::CastAnyProcessingData( anyData, node );
	return node;
}

mitk::DataTreeNode::Pointer Core::SurfaceInteractor::GetSelectedDataEntityNode()
{
	mitk::DataTreeNode::Pointer node;
	boost::any anyData = GetRenderingTree()->GetNode( GetSelectedDataEntity( ) );
	Core::CastAnyProcessingData( anyData, node );
	return node;
}

mitk::PointSet::Pointer Core::SurfaceInteractor::GetSelectedPointsRenderingData()
{
	if ( GetSelectedPointsDataEntity( ).IsNull() )
	{
		throw Core::Exceptions::Exception( 
			"SurfaceInteractor", 
			"Input point set is NULL" );
	}
	mitk::BaseData::Pointer renderingData;
	boost::any anyData = GetSelectedPointsDataEntity( )->GetRenderingData( typeid( renderingData ) );
	Core::CastAnyProcessingData( anyData, renderingData );
	mitk::PointSet* pointSet = NULL;
	if ( renderingData.IsNull() )
	{
		throw Core::Exceptions::Exception( 
			"SurfaceInteractor", 
			"Input points rendering data is NULL" );
	}
	pointSet = dynamic_cast<mitk::PointSet*>(renderingData.GetPointer()); 
	if ( pointSet == NULL )
	{
		throw Core::Exceptions::Exception( 
			"SurfaceInteractor", 
			"Input points rendering data is not correct" );
	}
	return pointSet;
}

void Core::SurfaceInteractor::OnInteractorConnected()
{

}

Core::RenderingTree::Pointer Core::SurfaceInteractor::GetRenderingTree() const
{
	return m_renderingTree;
}

void Core::SurfaceInteractor::SetRenderingTree( Core::RenderingTree::Pointer val )
{
	m_renderingTree = val;
}

Core::DataEntityHolder::Pointer Core::SurfaceInteractor::GetSelectedPointsHolder() const
{
	return m_selectedPointsHolder;
}

void Core::SurfaceInteractor::SetSelectedPointsHolder( 
	Core::DataEntityHolder::Pointer val )
{
	/*if ( m_selectedPointsHolder.IsNotNull() )
	{
		m_selectedPointsHolder->RemoveObserver<SurfaceInteractor>(
			this, 
			&Self::OnVtkDataPointModified );
		m_selectedPointsHolder->RemoveObserver<SurfaceInteractor>(
			this, 
			&Self::OnNewSelectedPoints,
			DH_NEW_SUBJECT );
	}*/

	m_selectedPointsHolder = val;

	/*if ( m_selectedPointsHolder.IsNotNull() )
	{
		SetSelectedPoints( m_selectedPointsHolder->GetSubject() );

		// Observers to processing data
		m_selectedPointsHolder->AddObserver<SurfaceInteractor>(
			this, 
			&Self::OnVtkDataPointModified );
		m_selectedPointsHolder->AddObserver<SurfaceInteractor>(
			this, 
			&Self::OnNewSelectedPoints,
			DH_NEW_SUBJECT );
	}
	else
	{
		SetSelectedPoints( NULL );
	}*/

}

Core::DataEntityHolder::Pointer Core::SurfaceInteractor::GetSelectedData() const
{
	return m_selectedData;
}

void Core::SurfaceInteractor::SetSelectedData( Core::DataEntityHolder::Pointer val )
{
	m_selectedData = val;
}

/**
*/
void Core::SurfaceInteractor::UpdateRenderingNodeLabels()
{
	Core::DataTreeMITKHelper::UpdateRenderingNodeLabels( GetSelectedPointsNode() );
}

bool Core::SurfaceInteractor::GetHideNode() const
{
	return m_hideNode;
}

void Core::SurfaceInteractor::SetHideNode( bool val )
{
	m_hideNode = val;
}

void Core::SurfaceInteractor::OnNewSelectedPoints()
{
	// Set new rendering data
	SetSelectedPoints( m_selectedPointsHolder->GetSubject() );
}

void Core::SurfaceInteractor::SetSelectedPoints( 
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
		typedef itk::SimpleMemberCommand<SurfaceInteractor>
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

void Core::SurfaceInteractor::UpdateMitkToVtkPoints()
{
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

	//mitk::PointSet::Pointer mitkData = GetSelectedPointsRenderingData( );
	pointsVtk->SetNumberOfPoints( m_pointSet->GetSize( GetTimeStep() ) );
	mitk::PointSet::DataType *itkPointSet = m_pointSet->GetPointSet( GetTimeStep() );
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
}

mitk::PointSet* Core::SurfaceInteractor::GetPointSet()
{
	return m_pointSet.GetPointer();
}

vtkIdList* Core::SurfaceInteractor::SelectCells( vtkImplicitFunction* implicitFunction )
{
	vtkIdList* list = vtkIdList::New();

	// Clean Input before extracting cells avoid misbehavior
	Core::vtkPolyDataPtr  polydataInput;
	GetSelectedDataEntity()->GetProcessingData(polydataInput);
	vtkSmartPointer<vtkPolyDataNormals> cleanfilter =
		vtkSmartPointer<vtkPolyDataNormals>::New();
	cleanfilter->SetInput(polydataInput);
	cleanfilter->Update();


	// -----------------------------------------
	// Extract the cells
	// -----------------------------------------
	vtkSmartPointer<meVTKExtractPolyDataGeometryCellsID>	extractFilter;
	extractFilter = vtkSmartPointer<meVTKExtractPolyDataGeometryCellsID>::New( );
	extractFilter->SetImplicitFunction( implicitFunction );
	extractFilter->ExtractInsideOn( );
	extractFilter->SetExtractBoundaryCells( 1 );
	extractFilter->SetInput( cleanfilter->GetOutput() );
	extractFilter->Update( );
	list->DeepCopy(extractFilter->GetArrayCellsId( ));

	return list;
}

void Core::SurfaceInteractor::SetValue( double val )
{
	m_value = val;
}

double Core::SurfaceInteractor::GetValue()
{
	return m_value;
}