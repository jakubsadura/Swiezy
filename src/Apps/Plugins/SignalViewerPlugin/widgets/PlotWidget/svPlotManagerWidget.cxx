/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "svPlotManagerWidget.h"
#include "svSignalPlotAnnotationsWidget.h"
#include "svPlotWidget.h"
#include "svPlotWidgetToolbar.h"
#include "svPlotFilterAnnotationsWidget.h"
#include "svConfigurationWidget.h"
#include "blSignalEvent.h"
#include "coreSettings.h"

#include "Signal.xpm"

BEGIN_EVENT_TABLE(svPlotManagerWidget, wxPanel)
  EVT_AUI_PANE_CLOSE(svPlotManagerWidget::OnClosePane)	
END_EVENT_TABLE()

/**
\brief Layout for plot widgets
\ingroup SignalViewerPlugin
\author Xavi Planes
\date 4 Jan 2010
*/
svPlotManagerWidget::svPlotManagerWidget(wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style)
: wxPanel(parent, id, pos, size, style)
{

	SetName( wxT( "Signal viewer" ) );
	SetBitmap( signal_xpm );

	blSignalEventTable::SetDirectory(Core::Runtime::Kernel::GetApplicationSettings()->GetApplicationPathSubPath(""));

	// Configure input holder
	m_InputDataHolder = Core::DataEntityHolder::New();
	m_InputDataHolder->SetName( "svPlotManagerWidget: Input" );
	m_AcquireInputWidget = new Core::Widgets::InputControl(
		this, 
		wxID_ANY,
		true);
	//GetSizer()->Insert(0, m_AcquireInputWidget, 0, wxEXPAND | wxALL, 4);
	m_AcquireInputWidget->Hide();


	m_svPlotWidget = new svPlotWidget(this, wxID_ANY);
	m_svPlotWidget->SetInputDataHolder( m_InputDataHolder );

	m_svSignalPlotAnnotationsWidget = new svSignalPlotAnnotationsWidget(this);
	m_svSignalPlotAnnotationsWidget->SetInputDataHolder( m_InputDataHolder );
	m_svSignalPlotAnnotationsWidget->Hide();

	m_svPlotWidgetToolbar = new svPlotWidgetToolbar( this, wxID_ANY);
	m_svPlotWidgetToolbar->SetsvPlotManagerWidget( this );
	
	m_svPlotFilterAnnotationsWidget = new svPlotFilterAnnotationsWidget( this );
	m_svPlotFilterAnnotationsWidget->Hide( );

	m_svConfigurationWidget = new svConfigurationWidget( this );
	m_svConfigurationWidget->Hide();
	m_svConfigurationWidget->SetPlotWidget( m_svPlotWidget );

	m_wxAuiManager.SetManagedWindow( this );

	wxAuiPaneInfo paneInfo;

	paneInfo.CaptionVisible( false ).Show( true ).Center()
		.CloseButton( false )
		.Name( m_svPlotWidget->GetName() );
	m_wxAuiManager.AddPane( m_svPlotWidget, paneInfo );

	paneInfo.CaptionVisible( true ).Show( false ).Right()
		.Name( m_svSignalPlotAnnotationsWidget->GetName() )
		.Caption( m_svSignalPlotAnnotationsWidget->GetName() )
		.CloseButton( true )
		.Float();
	m_wxAuiManager.AddPane( m_svSignalPlotAnnotationsWidget, paneInfo );

	paneInfo.CaptionVisible( true ).Show( false ).Right()
		.Name( m_svPlotFilterAnnotationsWidget->GetName() )
		.Caption( m_svPlotFilterAnnotationsWidget->GetName() )
		.CloseButton( true )
		.Float();
	m_wxAuiManager.AddPane( m_svPlotFilterAnnotationsWidget, paneInfo );

	paneInfo.CaptionVisible( true ).Show( false ).Right()
		.Name( m_svConfigurationWidget->GetName() )
		.Caption( m_svConfigurationWidget->GetName() )
		.CloseButton( true )
		.Float();
	m_wxAuiManager.AddPane( m_svConfigurationWidget, paneInfo );

	paneInfo.CaptionVisible( false ).Show( true ).Row( 0 ).Top( )
		.Name( m_svPlotWidgetToolbar->GetName() ).Dock().ToolbarPane();
	m_wxAuiManager.AddPane( m_svPlotWidgetToolbar, paneInfo );

	m_wxAuiManager.Update( );
}


/**
*/
svPlotManagerWidget::~svPlotManagerWidget()
{
	m_wxAuiManager.UnInit();
}


svPlotWidget* svPlotManagerWidget::GetPlotWidget() const
{
	return m_svPlotWidget;
}

void svPlotManagerWidget::UpdateAuiManager()
{
	m_wxAuiManager.Update();
}

void svPlotManagerWidget::SetListBrowser( 
	Core::Widgets::DataEntityListBrowser* listBrowser )
{
	BaseWindow::SetListBrowser( listBrowser );

	m_svPlotWidget->SetListBrowser( listBrowser );
	m_svSignalPlotAnnotationsWidget->SetDataEntityListBrowser( listBrowser );

	m_AcquireInputWidget->SetDataEntityListBrowser( listBrowser ); 
	m_AcquireInputWidget->SetAcquiredInputDataHolder( 
		m_InputDataHolder.GetPointer(),
		Core::SignalTypeId );
}

void svPlotManagerWidget::SetRenderingTree( Core::RenderingTree::Pointer val )
{
	BaseWindow::SetRenderingTree( val );
	m_svPlotWidget->SetRenderingTree( val );
}

Core::BaseProcessor::Pointer svPlotManagerWidget::GetProcessor()
{
	return NULL;
}

void svPlotManagerWidget::SetTimeStepHolder( Core::IntHolderType::Pointer val )
{
	Core::Widgets::ProcessingWidget::SetTimeStepHolder( val );
	m_svPlotWidget->SetTimeStepHolder( val );
}


void svPlotManagerWidget::OnClosePane( wxAuiManagerEvent& event )
{
	m_svPlotWidgetToolbar->ToggleTool( event.pane->window->GetId(), false );
}

void svPlotManagerWidget::Show( wxWindowID id, bool show /*= true */ )
{
	wxAuiPaneInfoArray& panes = m_wxAuiManager.GetAllPanes( );
	for ( int i = 0 ; i < panes.size( ) ; i++ )
	{
		if ( panes[ i ].window->GetId() == id )
		{
			panes[ i ].Show( show );
		}
	}

	m_wxAuiManager.Update();
}
