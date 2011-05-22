/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

// For compilers that don't support precompilation, include "wx/wx.h"
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/string.h"

#include <wxUnicode.h>

#include "blVTKHelperTools.h"

#include "coreLandmarkSelectorWidget.h"
#include "coreKernel.h"
#include "coreDataEntityHelper.h"
#include "coreMultiRenderWindow.h"
#include "coreWxMitkGraphicalInterface.h"
#include "coreInputControl.h"
#include "coreDataEntityListBrowser.h"
#include "corePointsTableWidget.h"
#include "coreReportExceptionMacros.h"
#include "corePointInteractorPointSet.h"
#include "corePointInteractorPointSelect.h"
#include "coreDataTreeHelper.h"
#include "coreProcessorInputWidget.h"

#include "vtkStringArray.h"
#include "vtkPointData.h"

#include "wxMitkRenderWindow.h"
#include "mitkRenderingManager.h"

#include "blSignalCollective.h"

using namespace Core::Widgets;
using namespace mitk;

const std::string STR_LANDMARK_VECTOR_NAME = "LandmarksName";

// Event the widget
BEGIN_EVENT_TABLE(LandmarkSelectorWidget, coreLandmarkSelectorWidgetUI)
  EVT_TEXT(wxID_TXT_LANDMARK_SELECTOR_WIDGET_LANDMARK_NAME, LandmarkSelectorWidget::OnTxtCtrlLandmarkName)
  EVT_RADIOBOX(wxID_RADIO_BOX_INTERACTOR_TYPE, LandmarkSelectorWidget::OnRadBoxInteractorType)
END_EVENT_TABLE()

//!
LandmarkSelectorWidget::LandmarkSelectorWidget(
	wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style)
: coreLandmarkSelectorWidgetUI(parent, id, pos, size, style)
{
	m_Processor = Processor::New();
	m_Processor->GetInputDataEntityHolder( INPUT_DATA )->AddObserver( 
		this, 
		&LandmarkSelectorWidget::OnModifiedInput,
		DH_NEW_SUBJECT );

	m_Processor->GetInputDataEntityHolder( INPUT_LANDMARKS )->AddObserver( 
		this, 
		&LandmarkSelectorWidget::OnModifiedLandmarks, 
		DH_SUBJECT_MODIFIED );
	m_Processor->GetInputDataEntityHolder( INPUT_LANDMARKS )->AddObserver( 
		this, 
		&LandmarkSelectorWidget::OnNewLandmarksSubject, 
		DH_NEW_SUBJECT );

	m_PointsTableWidget = new Core::Widgets::PointsTableWidget( this, wxID_ANY);
	GetSizer()->Insert(3, m_PointsTableWidget, 1, wxEXPAND | wxALL, 4);
	m_PointsTableWidget->GetSelectedItemHolder()->AddObserver( 
		this, 
		&LandmarkSelectorWidget::OnSelectedLandmark );
	m_PointsTableWidget->SetInput( m_Processor->GetOutputDataEntityHolder( 0 ) );

	m_InteractorStateHolder = InteractorStateHolderType::New();
	m_InteractorStateHolder->SetSubject( INTERACTOR_DISABLED );

	m_interactorType = Core::PointInteractor::POINT_SET;

	m_LandmarksName = "Landmarks";
}

//!
LandmarkSelectorWidget::~LandmarkSelectorWidget(void)
{
	m_Processor->GetInputDataEntityHolder( INPUT_DATA )->RemoveObserver( 
		this, 
		&LandmarkSelectorWidget::OnModifiedInput );
}

void Core::Widgets::LandmarkSelectorWidget::SetMultiRenderWindow( 
	Core::Widgets::RenderWindowBase* val )
{
	BaseWindow::SetMultiRenderWindow( val );
	m_PointsTableWidget->SetMultiRenderWindow( GetMultiRenderWindow( ) );
}

void Core::Widgets::LandmarkSelectorWidget::SetTimeStepHolder( 
	Core::IntHolderType::Pointer timeStepHolder )
{
	// First we update the table widget with the current selection
	m_PointsTableWidget->SetTimeStepHolder( timeStepHolder );

	// Later we update the text control of the current landmark name
	BaseWindow::SetTimeStepHolder( timeStepHolder );
}

bool Core::Widgets::LandmarkSelectorWidget::Enable( bool enable /*= true */ )
{
	bool bReturn = coreLandmarkSelectorWidgetUI::Enable( enable );

	m_PointsTableWidget->Enable( enable );

	if ( !enable )
	{
		StopInteraction();
	}

	UpdateWidgets();

	return bReturn;
}

void Core::Widgets::LandmarkSelectorWidget::SetListBrowser( 
	Core::Widgets::DataEntityListBrowser* listBrowser )
{
	BaseWindow::SetListBrowser( listBrowser );

	SetDefaultAllowedInputDataTypes();
}

void Core::Widgets::LandmarkSelectorWidget::StartInteractor()
{
	Core::DataEntity::Pointer inputDataEntity; 
	inputDataEntity = m_Processor->GetInputDataEntity( INPUT_DATA );
	if ( inputDataEntity.IsNull( ) )
	{
		return;
	}

	// Add input data entity to the rendering tree before creating the landmarks
	if ( !GetRenderingTree( )->IsDataEntityRendered( inputDataEntity ) )
	{
		GetRenderingTree( )->Add( inputDataEntity );
	}

	BuildLandmarksDataEntity();

	CreateInteractor();

	m_InteractorStateHolder->SetSubject( INTERACTOR_ENABLED );
}

void Core::Widgets::LandmarkSelectorWidget::StopInteraction()
{
	if ( m_InteractorStateHolder->GetSubject( ) == INTERACTOR_DISABLED )
	{
		return;
	}

	m_Processor->SetOutputDataEntity( 0, NULL );
	m_PointInteractor = NULL;
	m_InteractorStateHolder->SetSubject( INTERACTOR_DISABLED );
	m_LandmarksName = "Landmarks";
}

Core::Widgets::LandmarkSelectorWidget::InteractorStateHolderType::Pointer 
Core::Widgets::LandmarkSelectorWidget::GetInteractorStateHolder() const
{
	return m_InteractorStateHolder;
}

void Core::Widgets::LandmarkSelectorWidget::UpdateWidgets()
{
	vtkStringArray *landmarksName = GetCurrentLandmarksNameArray();

	// Get current selected item
	int iSelectedItem;
	iSelectedItem = m_PointsTableWidget->GetSelectedItemHolder( )->GetSubject();

	vtkStdString vtkString;
	// When we change of time step, it could be possible that the current
	// selected item is from the old time step
	if ( landmarksName != NULL && 
		 iSelectedItem != -1 && 
		 iSelectedItem < landmarksName->GetNumberOfValues() )
	{
		vtkString = landmarksName->GetValue( iSelectedItem );
	}

	// ChangeValue( ) -> Don't send the event to update the data
	m_txtCtrlLandmarkName->ChangeValue( wxString( vtkString, wxConvUTF8) );

	// Check input data type
	std::vector<Core::DataEntityType> inputEntityTypes;
	inputEntityTypes.push_back( Core::PointInteractorPointSet::GetInputTypes() );
	inputEntityTypes.push_back( Core::PointInteractorPointSelect::GetInputTypes() );
	
	Core::DataEntity::Pointer inputDataEntity; 
	inputDataEntity = m_Processor->GetInputDataEntity( INPUT_DATA );

	if ( inputDataEntity.IsNotNull() )
	{
		m_radBoxInteractorType->Enable( 0, 
			inputDataEntity->GetType() & inputEntityTypes[ 0 ] );
		m_radBoxInteractorType->Enable( 1, 
			inputDataEntity->GetType() & inputEntityTypes[ 1 ] );

		// if selected type is not valid -> change it
		if ( !( inputDataEntity->GetType() & 
			inputEntityTypes[ m_radBoxInteractorType->GetSelection() ] ) )
		{
			for ( int i = 0 ; i < 2 ; i++ )
			{
				if ( inputDataEntity->GetType() & inputEntityTypes[ i ] )
				{
					m_radBoxInteractorType->SetSelection( i );
					break;
				}
			}
		}
	}

	if ( IsSelectionEnabled() )
	{
		m_btnStart->SetLabel( "Stop" );
		m_btnStart->SetValue( true );
	}
	else
	{
		m_btnStart->SetLabel( "Start" );
		m_btnStart->SetValue( false );
	}
}

void Core::Widgets::LandmarkSelectorWidget::UpdateData()
{
	vtkStringArray *landmarksName = GetCurrentLandmarksNameArray();

	// Get current selected item
	int iSelectedItem;
	iSelectedItem = m_PointsTableWidget->GetSelectedItemHolder( )->GetSubject();
	if ( iSelectedItem == -1 )
	{
		return;
	}

	// change landmark name
	landmarksName->SetValue( iSelectedItem, vtkStdString(m_txtCtrlLandmarkName->GetValue().ToAscii()));
	//m_Processor->SetOutputDataEntity( 0, m_Processor->GetInputDataEntity( INPUT_LANDMARKS ), true );

	Core::DataEntity::Pointer landmarksDataEntity;
	landmarksDataEntity = m_Processor->GetInputDataEntity( INPUT_LANDMARKS );
	if ( landmarksDataEntity.IsNotNull( ) )
	{
		landmarksDataEntity->Modified( );
	}

	// Update the point table widget
	m_PointsTableWidget->UpdateListControl();
}

vtkStringArray * Core::Widgets::LandmarkSelectorWidget::GetCurrentLandmarksNameArray()
{
	vtkStringArray *landmarksName = NULL;

	try{

		// If the user removed the default array -> Return null
		Core::vtkPolyDataPtr currentLandmarks = GetCurrentLandmarks();

		landmarksName = blVTKHelperTools::GetPointDataArray<vtkStringArray>( 
			currentLandmarks, 
			STR_LANDMARK_VECTOR_NAME.c_str() );
	}
	catch(...)
	{

	}

	return landmarksName;
}

Core::vtkPolyDataPtr Core::Widgets::LandmarkSelectorWidget::GetCurrentLandmarks()
{
	Core::DataEntity::Pointer landmarksDataEntity;
	landmarksDataEntity = m_Processor->GetInputDataEntity( INPUT_LANDMARKS );
	if ( landmarksDataEntity.IsNull( ) )
	{
		throw Core::Exceptions::Exception( 
			"LandmarkSelectorWidget", 
			"Input landmarks data entity is NULL" );
	}

	int timeStep = GetTimeStep();
	Core::vtkPolyDataPtr currentLandmarks;
	landmarksDataEntity->GetProcessingData( currentLandmarks, timeStep );

	return currentLandmarks;
}


void LandmarkSelectorWidget::OnModifiedInput()
{
	try{

		if ( !IsEnabled() ||
			m_Processor->GetInputDataEntity( INPUT_DATA ).IsNull( ) || 
			m_Processor->GetInputDataEntity( INPUT_LANDMARKS ).IsNull( ) )
		{
			StopInteraction();
			return;
		}

		// If the input is new -> Stop interaction
		if ( m_InteractorStateHolder->GetSubject() == INTERACTOR_ENABLED )
		{
			StopInteraction();
		}

	}
	coreCatchExceptionsLogAndNoThrowMacro( LandmarkSelectorWidget::OnModifiedInput );
}

void Core::Widgets::LandmarkSelectorWidget::OnModifiedLandmarks()
{
	try{

		if ( !IsEnabled() ||
			m_Processor->GetInputDataEntity( INPUT_DATA ).IsNull( ) )
		{
			return;
		}

		// Get the data entity
		Core::DataEntity::Pointer landmarksDataEntity;
		landmarksDataEntity = m_Processor->GetInputDataEntity( INPUT_LANDMARKS );
		if ( landmarksDataEntity.IsNull( ) )
		{
			StopInteraction();
			return;
		}

		// Add the default landmark name
		std::string landmarkName;
		for ( unsigned i = 0 ; i < landmarksDataEntity->GetNumberOfTimeSteps() ; i++ )
		{
			Core::vtkPolyDataPtr currentLandmarks = GetCurrentLandmarks();
			vtkStringArray *landmarksNameArray = GetCurrentLandmarksNameArray( );

			if ( landmarksNameArray != NULL )
			{
				// Insert default name to new points
				for (   vtkIdType landmarkId = landmarksNameArray->GetNumberOfValues(); 
					landmarkId < currentLandmarks->GetNumberOfPoints();
					landmarkId++ )
				{
					std::ostringstream strLandmarkName;
					strLandmarkName << m_LandmarksName << landmarkId;
					landmarkName = strLandmarkName.str();
					landmarksNameArray->InsertNextValue( landmarkName );
				}
			}

		}

		UpdateWidgets();

		// \todo Notify only when landmarks has been modified
		// Not notify when the modified signal is received from another
		// holder and nothing has changed
		m_Processor->GetOutputDataEntityHolder( 0 )->NotifyObservers( );

	}
	coreCatchExceptionsLogAndNoThrowMacro( LandmarkSelectorWidget::OnModifiedLandmarks );

}

void Core::Widgets::LandmarkSelectorWidget::OnNewLandmarksSubject()
{
	try{

		if ( !IsEnabled() ||
			m_Processor->GetInputDataEntity( INPUT_DATA ).IsNull( ) )
		{
			return;
		}

		// Get the data entity
		Core::DataEntity::Pointer landmarksDataEntity;
		landmarksDataEntity = m_Processor->GetInputDataEntity( INPUT_LANDMARKS );
		if ( landmarksDataEntity.IsNull( ) )
		{
			StopInteraction();
			return;
		}

		Core::DataContainer::Pointer dataContainer = Core::Runtime::Kernel::GetDataContainer();
		Core::DataEntityHolder::Pointer holder;
		holder = dataContainer->GetDataEntityList( )->GetSelectedDataEntityHolder();
		holder->SetSubject( landmarksDataEntity );

		SetLandmarksName( landmarksDataEntity->GetMetadata()->GetName() );
		
		// If user changes landmarks or creates new->Start again
		// else the landmarks are already being created by StartInteractor( )
		if ( m_InteractorStateHolder->GetSubject( ) == INTERACTOR_ENABLED )
		{
			StartInteractor( );
		}


	}
	coreCatchExceptionsLogAndNoThrowMacro( LandmarkSelectorWidget::OnModifiedLandmarks );


}

void Core::Widgets::LandmarkSelectorWidget::SetTimeStep( int time )
{
	try
	{
		UpdateWidgets( );
	}
	coreCatchExceptionsLogAndNoThrowMacro( 
		LandmarkSelectorWidget::OnModifiedTimeStep );
}

void Core::Widgets::LandmarkSelectorWidget::OnTxtCtrlLandmarkName( wxCommandEvent& event )
{
	try
	{
			UpdateData();
	}
	coreCatchExceptionsLogAndNoThrowMacro( 
		LandmarkSelectorWidget::OnTxtCtrlLandmarkName );

}

void Core::Widgets::LandmarkSelectorWidget::OnSelectedLandmark()
{
	try
	{
		UpdateWidgets();
	}
	coreCatchExceptionsLogAndNoThrowMacro( 
		LandmarkSelectorWidget::OnSelectedLandmark );
}

void Core::Widgets::LandmarkSelectorWidget::BuildLandmarksDataEntity(
	bool reuseChild /* =true */ )
{
	Core::DataEntity::Pointer inputDataEntity; 
	inputDataEntity = m_Processor->GetInputDataEntity( INPUT_DATA );
	if ( inputDataEntity.IsNull( ) )
	{
		return;
	}

	// Check if we have a children of type landmarks and select it automatically
	// otherwise, a new landmarks will be created when the user changes
	// from one image to a surface and comes back
	// The children should be in the list, otherwise, when the user removes
	// the children, it is not in the list, but still is a children
	Core::DataEntity::Pointer landmarksDataEntity;
	landmarksDataEntity = m_Processor->GetInputDataEntity( INPUT_LANDMARKS );
	if ( landmarksDataEntity.IsNotNull() )
	{
		if ( landmarksDataEntity->GetMetadata()->GetName() != GetLandmarksName() )
		{
			landmarksDataEntity = NULL;
		}

		Core::DataEntity::Pointer dataEntity;
		dataEntity = GetListBrowser( )->GetSelectedDataEntityHolder()->GetSubject();
		if ( dataEntity.IsNotNull() && 
			 dataEntity->IsPointSet() && 
			 dataEntity->GetFather( ) == inputDataEntity )
		{
			landmarksDataEntity = dataEntity;
			m_LandmarksName = landmarksDataEntity->GetMetadata()->GetName();
		}
	}

	if ( reuseChild && landmarksDataEntity.IsNull() )
	{
		Core::DataEntityMetadata::Pointer metadata;
		metadata = Core::DataEntityMetadata::New( 0 );
		metadata->RemoveAll();
		metadata->SetName( m_LandmarksName );
		landmarksDataEntity = GetListBrowser( )->GetDataEntityList( )->FindChild( 
			inputDataEntity,
			Core::PointSetTypeId,
			metadata );

		// Now we can set the holder because the data entity is in the list
		m_Processor->SetInputDataEntity( INPUT_LANDMARKS, landmarksDataEntity );
	}

	if ( landmarksDataEntity.IsNull() )
	{
		// Create data entity of points
		landmarksDataEntity = Core::DataEntity::New( PointSetTypeId );
		landmarksDataEntity->GetMetadata()->SetName( m_LandmarksName );
		landmarksDataEntity->Resize( inputDataEntity->GetNumberOfTimeSteps(), typeid( vtkPolyDataPtr ) );
		landmarksDataEntity->SetFather( inputDataEntity );

		// Add the string array
		for ( unsigned i = 0 ; i < inputDataEntity->GetNumberOfTimeSteps() ; i++ )
		{
			Core::vtkPolyDataPtr processingData;
			landmarksDataEntity->GetProcessingData( processingData, i );

			vtkSmartPointer<vtkStringArray> stringData;
			stringData = vtkSmartPointer<vtkStringArray>::New();
			stringData->SetName( STR_LANDMARK_VECTOR_NAME.c_str() );
			processingData->GetPointData()->AddArray( stringData );
		}

	}

	// Don't set the m_LandmarksHolder because the data entity is still not yet
	// in the list and gives and error in the acquired input control
	// XAVI: We need to set it, otherwise, it can be NULL and 
	// AddDataEntityToList() will fail
	m_Processor->SetOutputDataEntity( 0, landmarksDataEntity );
	m_Processor->SetInputDataEntity( INPUT_LANDMARKS, landmarksDataEntity );

	// Add to list and connect output holder to the list holder
	Core::DataEntityHelper::AddDataEntityToList( m_Processor->GetOutputDataEntityHolder( 0 ) );
}

void Core::Widgets::LandmarkSelectorWidget::CreateInteractor()
{
	Core::DataEntity::Pointer inputDataEntity; 
	inputDataEntity = m_Processor->GetInputDataEntity( INPUT_DATA );

	// Create interactor
	switch ( m_interactorType )
	{

	// PointSet
	case 0:
		m_PointInteractor = Core::PointInteractorPointSet::New( 
			GetRenderingTree( ), 
			m_Processor->GetInputDataEntityHolder( INPUT_LANDMARKS ),
			m_Processor->GetInputDataEntityHolder( INPUT_DATA ) );
		break;

	// SkeletonPointSelect
	case 1:
		{
		Core::PointInteractorPointSelect::Pointer interactor;
		interactor = Core::PointInteractorPointSelect::New( 
			GetRenderingTree( ), 
			m_Processor->GetInputDataEntityHolder( INPUT_LANDMARKS ),
			m_Processor->GetInputDataEntityHolder( INPUT_DATA ) );

		// Set type
		if ( inputDataEntity->IsSkeletonMesh() )
		{
			interactor->SetCuttingType( SKELETON_TYPE );
		}
		else
		{
			interactor->SetCuttingType( RING_TYPE );
		}

		// Set bounding box
		interactor->SetBoundingBox( inputDataEntity->GetFather() );

		m_PointInteractor = interactor;
		}
		break;
	}


	m_PointInteractor->ConnectToDataTreeNode();

	// Only hide the node when the interactor is disabled, not
	// remove the rendering node, to keep the properties like the
	// point size
	m_PointInteractor->SetHideNode( false );
}

void Core::Widgets::LandmarkSelectorWidget::OnRadBoxInteractorType( 
	wxCommandEvent& event )
{
	m_interactorType = static_cast<Core::PointInteractor::INTERACTOR_POINT_TYPE>(
		m_radBoxInteractorType->GetSelection());
	// If the input changed -> Create new interaction
	if ( m_InteractorStateHolder->GetSubject() == INTERACTOR_ENABLED )
	{
		CreateInteractor();
	}	
}

Core::PointInteractor::Pointer Core::Widgets::LandmarkSelectorWidget::GetPointInteractor() const
{
	return m_PointInteractor;
}

void Core::Widgets::LandmarkSelectorWidget::SetAllowedInputDataTypes( 
	Core::DataEntityType val )
{
	m_Processor->GetInputPort( INPUT_DATA )->SetDataEntityType( val );
}

void Core::Widgets::LandmarkSelectorWidget::SetDefaultAllowedInputDataTypes()
{
	m_Processor->GetInputPort( INPUT_DATA )->SetDataEntityType(
		Core::PointInteractorPointSet::GetInputTypes() |
		Core::PointInteractorPointSelect::GetInputTypes()  );
}

void Core::Widgets::LandmarkSelectorWidget::SetInputDataEntity( Core::DataEntity::Pointer val )
{
	m_Processor->SetInputDataEntity( INPUT_DATA, val );
}

Core::DataEntity::Pointer Core::Widgets::LandmarkSelectorWidget::GetInputDataEntity( )
{
	return m_Processor->GetInputDataEntity( INPUT_DATA );
}


void Core::Widgets::LandmarkSelectorWidget::SetInteractorType( 
		Core::PointInteractor::INTERACTOR_POINT_TYPE interactorType )
{
	m_interactorType = interactorType;
	m_radBoxInteractorType->SetSelection(interactorType);
}

Core::BaseProcessor::Pointer Core::Widgets::LandmarkSelectorWidget::GetProcessor()
{
	return m_Processor.GetPointer( );
}

bool Core::Widgets::LandmarkSelectorWidget::IsSelectionEnabled()
{
	return GetInteractorStateHolder()->GetSubject() == INTERACTOR_ENABLED;
}

void Core::Widgets::LandmarkSelectorWidget::OnNewLandmarks(wxCommandEvent &event)
{
	bool enabled = IsSelectionEnabled( );
	StopInteraction();
	m_Processor->SetInputDataEntity( INPUT_LANDMARKS, NULL );
	BuildLandmarksDataEntity( false );
	m_Processor->SetInputDataEntity( INPUT_LANDMARKS, m_Processor->GetOutputDataEntity( 0 ) );
	if ( enabled )
	{
		StartInteractor();
	}
}

std::string Core::Widgets::LandmarkSelectorWidget::GetLandmarksName() const
{
	return m_LandmarksName;
}

void Core::Widgets::LandmarkSelectorWidget::SetLandmarksName( std::string val )
{
	m_LandmarksName = val;
}

void Core::Widgets::LandmarkSelectorWidget::OnStart( wxCommandEvent &event )
{
	try
	{
		if ( IsSelectionEnabled( ) )
		{
			StopInteraction();
		}
		else
		{
			StartInteractor();
		}
	}
	coreCatchExceptionsReportAndNoThrowMacro( LandmarkSelectorWidget::OnStart )

	UpdateWidgets();
}

void Core::Widgets::LandmarkSelectorWidget::OnInit()
{
	GetInputWidget( INPUT_LANDMARKS )->SetAutomaticSelection( false );
	GetInputWidget( INPUT_LANDMARKS )->SetFatherHolder( 
		m_Processor->GetInputDataEntityHolder( INPUT_DATA ) );

	// Disable all observers
	for ( size_t i = 0 ; i < m_Processor->GetNumberOfOutputs() ; i++ )
	{
		GetProcessorOutputObserver( i )->SetEnable( false );
	}
}

boost::any Core::Widgets::LandmarkSelectorWidget::GetData()
{
	if ( GetPointInteractor( ).IsNull( ) || GetPointInteractor( )->GetSelectedPointsDataEntity().IsNull( ) )
	{
		return NULL;
	}

	return GetCurrentLandmarks( );
}

Core::DataEntity::Pointer Core::Widgets::LandmarkSelectorWidget::GetDataEntity()
{
	if ( GetPointInteractor( ).IsNull( ) )
	{
		return NULL;
	}

	return GetPointInteractor( )->GetSelectedPointsDataEntity();
}

Core::Widgets::LandmarkSelectorWidget::Processor::Processor()
{
	SetNumberOfInputs( 2 );
	GetInputPort( INPUT_DATA )->SetName( "Input data" );
	GetInputPort( INPUT_LANDMARKS )->SetName( "Input landmarks" );
	GetInputPort( INPUT_LANDMARKS )->SetDataEntityType( Core::PointSetTypeId );
	SetNumberOfOutputs( 1 );
	GetOutputPort( 0 )->SetDataEntityType( Core::SignalTypeId );
	GetOutputPort( 0 )->SetName( "Output landmarks" );
}
