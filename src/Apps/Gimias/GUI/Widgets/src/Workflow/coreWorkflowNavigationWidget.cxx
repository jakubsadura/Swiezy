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

#include <wx/wupdlock.h>

#include "coreWorkflowNavigationWidget.h"
#include "coreBaseWindowFactories.h"
#include "coreWxMitkGraphicalInterface.h"
#include "corePluginTab.h"
#include "coreWorkflowManager.h"
#include "coreWxMitkCoreMainWindow.h"

#include "add.xpm"
#include "back.xpm"
#include "next.xpm"

BEGIN_EVENT_TABLE(Core::Widgets::WorkflowNavigationWidget, coreWorkflowNavigationWidgetUI)
END_EVENT_TABLE()

/** 
\brief Menu item for Workflow navigation context menu

\ingroup gmWidgets
\author Xavi Planes
\date 31 May 2010
*/
class wxWorkflowContextMenuItem : public wxMenuItem
{
public:
	wxWorkflowContextMenuItem( const wxString& stepName,
		wxMenu*  parentMenu = NULL, 
		int id = wxID_SEPARATOR, 
		const wxString&  text = "", 
		const wxString&  helpString = "", 
		wxItemKind kind = wxITEM_NORMAL, 
		wxMenu*  subMenu = NULL)
		: wxMenuItem( parentMenu, id, text, helpString, kind, subMenu )
	{
		m_WorkflowStepName = stepName;
	}

	std::string GetWorkflowStepName( )
	{
		return m_WorkflowStepName;
	}

	std::string m_WorkflowStepName;
};

Core::Widgets::WorkflowNavigationWidget::WorkflowNavigationWidget( 
	wxWindow* parent, int id, const wxPoint& pos /*= wxDefaultPosition*/, 
	const wxSize& size /*= wxDefaultSize*/, long style /*= 0*/ )
: coreWorkflowNavigationWidgetUI( parent, id, pos, size, style )
{
	m_bmpAdd->SetBitmapLabel( wxBitmap( add_xpm ) );
	m_bmpAdd->Enable( false );
	m_bmpBack->SetBitmapLabel( wxBitmap( back_xpm ) );
	m_bmpBack->Enable( false );
	m_bmpNext->SetBitmapLabel( wxBitmap( next_xpm ) );
	m_bmpNext->Enable( false );

	m_CurrentStep = -1;
	m_ProcessingWidget = m_EmptyPanel;
	m_ContextMenu = NULL;

	m_lblCaption->SetLabel( "None" );
}

Core::Widgets::WorkflowNavigationWidget::~WorkflowNavigationWidget( void )
{
	if ( m_ContextMenu != NULL )
	{
		delete m_ContextMenu;
	}
}

Core::BaseProcessor::Pointer Core::Widgets::WorkflowNavigationWidget::GetProcessor( void )
{
	return NULL;
}

void Core::Widgets::WorkflowNavigationWidget::SetWorkflowStep( 
	Core::WorkflowStep::Pointer workflowStep )
{
	m_WorkflowStep = workflowStep;
	if ( m_WorkflowStep.IsNotNull() )
	{
		m_CurrentStep = 0;
	}
	else
	{
		m_CurrentStep = -1;
	}
	UpdateWidget();
}

void Core::Widgets::WorkflowNavigationWidget::OnBack( wxCommandEvent &event )
{
	try
	{
		if ( m_WorkflowStep.IsNull() )
		{
			return;
		}

		if ( m_CurrentStep != 0 )
		{
			m_CurrentStep--;
			m_AlternativeStep = "";
		}
		else
		{
			wxWindow* mainWindow;
			mainWindow = Core::Runtime::Kernel::GetGraphicalInterface()->GetMainWindow( );
			wxWindow* win = mainWindow->FindWindow( wxID_TabContainer );
			wxAuiNotebook* tabContainer = wxDynamicCast( win, wxAuiNotebook);
			wxWindowUpdateLocker noUpdates(tabContainer);
			if ( tabContainer )
			{
				tabContainer->AdvanceSelection( false );
			}
		}

		UpdateWidget( );
	}
	coreCatchExceptionsLogAndNoThrowMacro( WorkflowNavigationWidget::OnConextMenu )
}

void Core::Widgets::WorkflowNavigationWidget::OnAdd( wxCommandEvent &event )
{
	try
	{
		if ( m_WorkflowStep.IsNull() )
		{
			return;
		}

		// Fill context menu
		WorkflowSubStep::Pointer subStep = m_WorkflowStep->Get( m_CurrentStep );
		BaseWindowFactories::Pointer baseWindowFactory;
		baseWindowFactory = Core::Runtime::Kernel::GetGraphicalInterface()->GetBaseWindowFactory( );

		m_ContextMenu = new wxMenu( );
		WorkflowSubStep::AlternativesListType::iterator it;
		for ( it = subStep->GetAlternatives().begin(); 
			it != subStep->GetAlternatives().end();
			it++ )
		{
			wxWindowID id = wxNewId();
			
			// Get caption from window Config
			Core::WindowConfig config;
			baseWindowFactory->GetWindowConfig( *it, config );
			wxWorkflowContextMenuItem* menuItem = new wxWorkflowContextMenuItem( 
				*it,
				m_ContextMenu, id, wxT( config.GetCaption() ), wxT( config.GetCaption() ) );
			m_ContextMenu->Append( menuItem );

			// Connect to event handler
			Connect(
				id,
				wxEVT_COMMAND_MENU_SELECTED,
				wxCommandEventHandler(WorkflowNavigationWidget::OnConextMenu)
				);

		}
		PopupMenu( m_ContextMenu );
	}
	coreCatchExceptionsLogAndNoThrowMacro( WorkflowNavigationWidget::OnConextMenu )
}

void Core::Widgets::WorkflowNavigationWidget::OnNext( wxCommandEvent &event )
{
	try
	{
		if ( m_WorkflowStep.IsNull() )
		{
			return;
		}

		if ( m_CurrentStep != m_WorkflowStep->GetSubStepVector().size() - 1 )
		{
			m_CurrentStep++;
			m_AlternativeStep = "";
		}
		else
		{
			wxWindow* mainWindow;
			mainWindow = Core::Runtime::Kernel::GetGraphicalInterface()->GetMainWindow( );
			wxWindow* win = mainWindow->FindWindow( wxID_TabContainer );
			wxAuiNotebook* tabContainer = wxDynamicCast( win, wxAuiNotebook);
			wxWindowUpdateLocker noUpdates(tabContainer);
			if ( tabContainer )
			{
				tabContainer->AdvanceSelection();
			}
		}

		UpdateWidget( );
	}
	coreCatchExceptionsLogAndNoThrowMacro( WorkflowNavigationWidget::OnConextMenu )
}

void Core::Widgets::WorkflowNavigationWidget::UpdateWidget()
{
	if ( m_WorkflowStep.IsNull() )
	{
		return;
	}

	std::string factoryName = CheckCurrentClassname();
	if ( factoryName.empty() )
	{
		return;
	}

	ReplaceWidget( factoryName );

	// Set title of sizer
	if ( m_ProcessingWidget && m_ProcessingWidget != m_EmptyPanel && !factoryName.empty() )
	{
		BaseWindowFactories::Pointer baseWindowFactory;
		baseWindowFactory = Core::Runtime::Kernel::GetGraphicalInterface()->GetBaseWindowFactory( );
		Core::WindowConfig config;
		baseWindowFactory->GetWindowConfig( factoryName, config );
		m_lblCaption->SetLabel( config.GetCaption() );
	}
	else
	{
		m_lblCaption->SetLabel( factoryName );
	}

	Core::Workflow::Pointer workflow;
	workflow = Core::Runtime::Kernel::GetWorkflowManager()->GetActiveWorkflow( );
	Core::Workflow::StepListType stepVector = workflow->GetStepVector( );
	int pos = 0;
	Core::Workflow::StepListType::iterator it;
	for ( it  = stepVector.begin() ; it != stepVector.end() ; it++ )
	{
		if ( *it == m_WorkflowStep )
		{
			break;
		}
		pos++;
	}

	// Enable / Disable buttons
	m_bmpBack->Enable( !( m_CurrentStep == 0 && pos == 0 ) );

	bool isLastSubStep = m_CurrentStep == m_WorkflowStep->GetSubStepVector().size() - 1;
	m_bmpNext->Enable( !( isLastSubStep && pos == stepVector.size() - 1 ) );
	WorkflowSubStep::Pointer subStep = m_WorkflowStep->Get( m_CurrentStep );
	m_bmpAdd->Enable( subStep->GetAlternatives().size() > 1 );

	// Resize this
	// Cast a resize event
	wxSizeEvent resEvent(this->GetBestSize(), this->GetId());
	resEvent.SetEventObject(this);
	this->GetEventHandler()->ProcessEvent(resEvent);
}

void Core::Widgets::WorkflowNavigationWidget::OnConextMenu( wxCommandEvent& event )
{
	try
	{
		wxMenuItem* item = m_ContextMenu->FindItem( event.GetId() );
		wxWorkflowContextMenuItem* workflowItem = dynamic_cast<wxWorkflowContextMenuItem*> (item);
		m_AlternativeStep = workflowItem->GetWorkflowStepName( );
		UpdateWidget();
	}
	coreCatchExceptionsLogAndNoThrowMacro( WorkflowNavigationWidget::OnConextMenu )
}

std::string Core::Widgets::WorkflowNavigationWidget::CheckCurrentClassname()
{

	Core::WorkflowSubStep::Pointer subStep = m_WorkflowStep->Get( m_CurrentStep );
	std::string factoryName;
	if ( subStep.IsNotNull() && m_ProcessingWidget != NULL )
	{
		// Get selected step
		if ( m_AlternativeStep.empty() )
		{
			factoryName = subStep->GetName( );
		}
		else
		{
			factoryName = m_AlternativeStep;
		}
	}

	return factoryName;
}

void Core::Widgets::WorkflowNavigationWidget::ReplaceWidget( const std::string &factoryName )
{
	BaseWindowFactories::Pointer baseWindowFactory;
	baseWindowFactory = Core::Runtime::Kernel::GetGraphicalInterface()->GetBaseWindowFactory( );

	wxWindowUpdateLocker noUpdates( this );

	wxWindow* widget = NULL;
	if ( !factoryName.empty() )
	{
		try
		{
			BaseWindow *baseWindow = baseWindowFactory->CreateWindow( 
				factoryName, 
				this, 
				m_ProcessingWidget->GetPosition(),
				m_ProcessingWidget->GetSize() );
			widget = dynamic_cast<wxWindow*> ( baseWindow );
			widget->Hide();
			widget->Enable( false );

			// Init processor observers
			baseWindow->InitProcessorObservers( true );

			// OnInit can change the processor observers
			GetPluginTab( )->InitBaseWindow( baseWindow );
		}
		catch( Core::Exceptions::Exception& e )
		{
			std::cerr << e.what( );
			e.AddTrace( "WorkflowNavigationWidget::ReplaceWidget" );
			Core::LogException(e);

			widget = m_EmptyPanel;
		}
	}
	else
	{
		widget = m_EmptyPanel;
	}

	// Remove old sizer
	wxSizer* sizer = m_ProcessingWidget->GetContainingSizer();
	sizer->Clear(false);
	GetSizer( )->Remove( sizer );

	// Create new sizer
	sizer_3_staticbox = new wxStaticBox(this, -1, wxEmptyString);
	wxStaticBoxSizer* sizer_3 = new wxStaticBoxSizer(sizer_3_staticbox, wxVERTICAL);
	sizer_3->Add( widget, 1, wxEXPAND, 5);
	GetSizer( )->Add(sizer_3, 1, wxALL|wxEXPAND, 5);

	// Destroy old window
	if ( m_ProcessingWidget != m_EmptyPanel )
	{
		m_ProcessingWidget->Destroy( );
		m_EmptyPanel->Hide();
	}
	m_ProcessingWidget = widget;

	// Enable new window
	widget->Show();
	widget->Enable( true );

	SelectionToolWidget* selection = dynamic_cast<SelectionToolWidget*> ( widget );
	if ( selection )
	{
		selection->StartInteractor( );
	}

	FitInside( );


}
