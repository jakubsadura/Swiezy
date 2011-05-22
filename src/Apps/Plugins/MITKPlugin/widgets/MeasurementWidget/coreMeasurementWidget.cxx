/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreMeasurementWidget.h"
#include "coreDataEntityHelper.h"
#include "coreReportExceptionMacros.h"
#include "coreProcessorInputWidget.h"
#include "coreVTKPolyDataHolder.h"

#include <mitkBaseRenderer.h>
#include <mitkDataTree.h>
#include <mitkRenderingManager.h>

#include "wxMitkRenderWindow.h"

using namespace Core::Widgets;

#define wxID_BTN_START wxID( "wxID_BTN_START" )

BEGIN_EVENT_TABLE(MeasurementWidget, wxScrolledWindow)
  EVT_TOGGLEBUTTON(wxID_BTN_START, MeasurementWidget::OnStart)
END_EVENT_TABLE();

MeasurementWidget::MeasurementWidget(
	wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style):
		wxScrolledWindow(parent, id, pos, size, wxTAB_TRAVERSAL)
{
	m_btnStart = new wxToggleButton(this, wxID_BTN_START, wxT("Start"));

	SetScrollRate(10, 10);

	wxBoxSizer* sizer_1 = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* sizer_4 = new wxBoxSizer(wxHORIZONTAL);
	sizer_4->Add(m_btnStart, 0, wxALL, 5);
	sizer_1->Add(sizer_4, 0, wxALIGN_RIGHT, 0);
	SetSizer(sizer_1);
	sizer_1->Fit(this);

	m_Processor = Processor::New();
	m_Processor->GetInputDataEntityHolder( 0 )->AddObserver( 
		this, 
		&MeasurementWidget::OnModifiedInput );

	m_InteractorStateHolder = InteractorStateHolderType::New( );
	m_InteractorStateHolder->SetSubject( INTERACTOR_DISABLED );
	m_MeasureName = "Measure";
	m_measureColor[0] = 1; m_measureColor[1] = 0; m_measureColor[1] = 0; //mitk default red
}

Core::Widgets::MeasurementWidget::~MeasurementWidget()
{
	StopInteraction();
}

bool MeasurementWidget::Enable( bool enable /*= true*/ )
{
	m_enable = enable;

	UpdateWidget( );

	return m_enable;
}

void MeasurementWidget::StartInteractor()
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
	Core::DataEntity::Pointer measurementsDataEntity;
	measurementsDataEntity = m_Processor->GetInputDataEntity( INPUT_MEASUREMENT );
	if ( measurementsDataEntity.IsNotNull() )
	{
		measurementsDataEntity = NULL;

		Core::DataEntity::Pointer selectedDataEntity;
		selectedDataEntity = GetListBrowser( )->GetSelectedDataEntityHolder()->GetSubject();
		if ( selectedDataEntity.IsNotNull() && 
			selectedDataEntity->IsPointSet() && 
			selectedDataEntity->GetFather( ) == inputDataEntity )
		{
			measurementsDataEntity = selectedDataEntity;
		}
	}

	if ( measurementsDataEntity.IsNull() )
	{
		Core::DataEntity::Pointer measurementDataEntity;
		measurementDataEntity = Core::DataEntity::New( DataEntityType( PointSetTypeId | MeasurementTypeId ) );
		measurementDataEntity->GetMetadata()->SetName( m_MeasureName );
		measurementDataEntity->Resize( inputDataEntity->GetNumberOfTimeSteps(), typeid( vtkPolyDataPtr ) );
		measurementDataEntity->SetFather( inputDataEntity );

		m_Processor->SetInputDataEntity( INPUT_MEASUREMENT, measurementDataEntity );

		// Add to data entity list
		Core::DataEntityHelper::AddDataEntityToList( 
			m_Processor->GetInputDataEntityHolder( INPUT_MEASUREMENT ), false );
	}


	// Create interactor
	m_PointInteractor = Core::PointInteractorPointSet::New( 
		GetRenderingTree(), 
		m_Processor->GetInputDataEntityHolder( INPUT_MEASUREMENT ),
		m_Processor->GetInputDataEntityHolder( INPUT_DATA ) );
	m_PointInteractor->SetMaxPoints( 2);
	m_PointInteractor->SetEnableContinuousMoveEvent( false );
	m_PointInteractor->SetIncrementalAdd( true );
	m_PointInteractor->ConnectToDataTreeNode();
	m_PointInteractor->SetMeasurementPointSet(true);
	m_PointInteractor->SetContourColor(m_measureColor);

	m_InteractorStateHolder->SetSubject( INTERACTOR_ENABLED );
	UpdateWidget( );
}

void MeasurementWidget::StopInteraction( )
{
	if ( m_PointInteractor.IsNull() )
	{
		return;
	}

	Core::DataEntity::Pointer measurementDataEntity;
	measurementDataEntity = m_PointInteractor->GetSelectedPointsDataEntity();
	m_PointInteractor->DisconnectFromDataTreeNode();
	m_PointInteractor = NULL;

	if ( measurementDataEntity.IsNotNull())
	{
		GetRenderingTree()->Add( measurementDataEntity );
	}

	m_InteractorStateHolder->SetSubject( INTERACTOR_DISABLED );
	UpdateWidget( );
}

void MeasurementWidget::OnModifiedInput()
{
	try{

		if ( !m_enable || 
			 m_Processor->GetInputDataEntity( INPUT_DATA ).IsNull( ) )
		{
			StopInteraction( );
			return;
		}

		if ( m_PointInteractor.IsNotNull() )
		{
			StopInteraction();
			StartInteractor();
		}

		UpdateWidget( );
	}
	coreCatchExceptionsLogAndNoThrowMacro( MeasurementWidget::OnModifiedInput );
}

Core::Widgets::MeasurementWidget::InteractorStateHolderType::Pointer 
Core::Widgets::MeasurementWidget::GetInteractorStateHolder() const
{
	return m_InteractorStateHolder;
}

Core::BaseProcessor::Pointer Core::Widgets::MeasurementWidget::GetProcessor()
{
	return m_Processor.GetPointer();
}

bool Core::Widgets::MeasurementWidget::IsSelectionEnabled()
{
	return m_InteractorStateHolder->GetSubject( ) == INTERACTOR_ENABLED;
}


Core::DataEntityHolder::Pointer Core::Widgets::MeasurementWidget::GetMeasurementHolder() {
	return m_Processor->GetInputDataEntityHolder( INPUT_MEASUREMENT );
}



void Core::Widgets::MeasurementWidget::SetMeasureName(std::string measureName)
{
	m_MeasureName = measureName;
}



void Core::Widgets::MeasurementWidget::SetMeasureColor(double measureColor[3])
{
	m_measureColor[0] = measureColor[0];
	m_measureColor[1] = measureColor[1];
	m_measureColor[2] = measureColor[2];

}

void Core::Widgets::MeasurementWidget::OnStart( wxCommandEvent &event )
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
	coreCatchExceptionsReportAndNoThrowMacro( MeasurementWidget::OnStart )

	UpdateWidget();
}

void Core::Widgets::MeasurementWidget::UpdateWidget()
{
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

void Core::Widgets::MeasurementWidget::OnInit()
{
	GetInputWidget( INPUT_MEASUREMENT )->SetAutomaticSelection( false );
	GetInputWidget( INPUT_MEASUREMENT )->SetFatherHolder( 
		m_Processor->GetInputDataEntityHolder( INPUT_DATA ) );

	// Disable all observers
	for ( size_t i = 0 ; i < m_Processor->GetNumberOfOutputs() ; i++ )
	{
		GetProcessorOutputObserver( i )->SetEnable( false );
	}
}

boost::any Core::Widgets::MeasurementWidget::GetData()
{
	if ( m_PointInteractor.IsNull( ) || m_PointInteractor->GetSelectedPointsDataEntity().IsNull( ) )
	{
		return NULL;
	}
	return NULL;
}

Core::DataEntity::Pointer Core::Widgets::MeasurementWidget::GetDataEntity()
{
	if ( m_PointInteractor.IsNull( ) )
	{
		return NULL;
	}

	return m_PointInteractor->GetSelectedPointsDataEntity();
}

Core::Widgets::MeasurementWidget::Processor::Processor()
{
	SetNumberOfInputs( 2 );
	GetInputPort( 0 )->SetName( "Input data" );
	GetInputPort( 0 )->SetDataEntityType( Core::PointInteractorPointSet::GetInputTypes() );
	GetInputPort( 1 )->SetName( "Input measurement" );
	GetInputPort( 1 )->SetDataEntityType( Core::MeasurementTypeId );
}
