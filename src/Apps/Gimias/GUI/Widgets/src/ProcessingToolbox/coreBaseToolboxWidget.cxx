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

// Core
#include "coreBaseToolboxWidget.h"
#include "coreReportExceptionMacros.h"
#include "coreBaseWindowFactories.h"
#include "coreWxMitkGraphicalInterface.h"
#include "corePluginTab.h"

using namespace Core::Widgets;

//!
BaseToolboxWidget::BaseToolboxWidget(wxWindow* parent, 
	wxWindowID id, 
	const wxPoint& pos, 
	const wxSize& size, 
	long style, 
	const wxString& name)
: wxScrolledWindow(parent, id, pos, size, style, name)
{
	//SetBitmap( processingtools_xpm );

	m_EmptyLabel = new wxStaticText(
		this, 
		wxID_ANY, 
		wxT("\n\nThere isn't any selected tool"), 
		wxDefaultPosition, 
		wxDefaultSize );

	set_properties( );
	do_layout( );
}

void BaseToolboxWidget::set_properties()
{
	SetScrollRate(10, 10);
}

void BaseToolboxWidget::do_layout()
{
	// layout them
	wxBoxSizer* vlayout = new wxBoxSizer(wxVERTICAL);
	vlayout->Add(m_EmptyLabel, 0, wxEXPAND | wxALL, 4);
	SetSizer(vlayout);
	vlayout->Fit(this);

}
//!
BaseToolboxWidget::~BaseToolboxWidget(void)
{
}

void Core::Widgets::BaseToolboxWidget::SetToolByName( const std::string &caption )
{
	if ( GetToolWindow( ) && GetToolWindow( )->GetName( ) == caption )
	{
		return;
	}

	wxWindowUpdateLocker noUpdates( this );

	BaseWindowFactories::Pointer baseWindowFactory;
	baseWindowFactory = Core::Runtime::Kernel::GetGraphicalInterface()->GetBaseWindowFactory( );

	// Create widget
	std::list<std::string> windowsList;
	windowsList = baseWindowFactory->GetWindowsList( 
		GetWidgetState(), caption );
	if ( windowsList.empty() )
	{
		Clean( );
		return;
	}

	SetToolByFactoryName( windowsList.front() );

	InitTool( );
}

void Core::Widgets::BaseToolboxWidget::SetToolByID( wxWindowID id )
{
	if ( GetToolWindow( ) && GetToolWindow( )->GetId( ) == id )
	{
		return;
	}

	wxWindowUpdateLocker noUpdates( this );

	BaseWindowFactories::Pointer baseWindowFactory;
	baseWindowFactory = Core::Runtime::Kernel::GetGraphicalInterface()->GetBaseWindowFactory( );

	// Create widget
	std::list<std::string> windowsList;
	windowsList = baseWindowFactory->GetWindowsList( 
		GetWidgetState(), "", "", id );
	if ( windowsList.empty() )
	{
		Clean( );
		return;
	}

	SetToolByFactoryName( windowsList.front() );

	InitTool( );
}

Core::BaseProcessor::Pointer Core::Widgets::BaseToolboxWidget::GetProcessor()
{
	return NULL;
}

void Core::Widgets::BaseToolboxWidget::Clean()
{
	if ( GetToolWindow( ) == NULL )
	{
		return;
	}

	wxWindowUpdateLocker noUpdates(this);

	// Destroy old window
	GetSizer()->Clear( true );
	m_EmptyLabel = NULL;
	m_lblCaption = NULL;
	SetToolWindow( NULL );

	m_EmptyLabel = new wxStaticText(
		this, 
		wxID_ANY, 
		wxT("\n\nThere isn't any selected tool"), 
		wxDefaultPosition, 
		wxDefaultSize );

	do_layout();

	CreateCaptionLabel( );

}

bool Core::Widgets::BaseToolboxWidget::Enable( bool enable /*= true */ )
{
	if ( enable == false )
	{
		Clean( );
	}

	return wxScrolledWindow::Enable( enable );
}

void Core::Widgets::BaseToolboxWidget::CreateCaptionLabel()
{
	m_lblCaption = new wxStaticText(this, wxID_ANY, wxT("None"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
	m_lblCaption->SetMinSize(wxSize(-1, 20));
	m_lblCaption->SetFont(wxFont(9, wxDEFAULT, wxNORMAL, wxBOLD, 0, wxT("")));
	m_lblCaption->SetWindowStyle(wxALIGN_CENTRE | wxBORDER_SUNKEN);
	GetSizer()->Insert(0,m_lblCaption, 0, wxALL|wxEXPAND, 5);

	if ( GetToolWindow() )
	{
		m_lblCaption->SetLabel( GetToolWindow()->GetName() );
	}

	m_lblCaption->Connect(wxEVT_LEFT_UP, wxMouseEventHandler( BaseToolboxWidget::OnMouseClick ), NULL, this );
	m_lblCaption->Connect(wxEVT_RIGHT_UP, wxMouseEventHandler( BaseToolboxWidget::OnMouseClick ), NULL, this );
}

void Core::Widgets::BaseToolboxWidget::OnMouseClick( wxMouseEvent& event )
{
	wxMenuBar* menuBar;
	menuBar = Core::Runtime::Kernel::GetGraphicalInterface()->GetMainWindow( )->GetMenuBar();

	std::string itemName = GetNameMenuItem();
	int menuId = menuBar->FindMenu( itemName );
	if ( menuId == wxNOT_FOUND )
	{
		return;
	}

	wxMenu* toolsMenu = menuBar->GetMenu( menuId );
	PopupMenu( toolsMenu );
}

void Core::Widgets::BaseToolboxWidget::OnInit()
{
	CreateCaptionLabel( );
}

void Core::Widgets::BaseToolboxWidget::InitTool()
{
	// Enable the window when everything is initialized
	// Is not possible to call Enable because the widget is not initialized
	GetToolWindow( )->Enable( false );

	BaseWindow* baseWindow = dynamic_cast<BaseWindow*> ( GetToolWindow( ) );
	if ( baseWindow == NULL )
	{
		throw Core::Exceptions::Exception( 
			"BaseToolboxWidget::InitTool", 
			"input widget is not a BaseWindow" );
	}

	// Init processor observers
	baseWindow->InitProcessorObservers( true );

	// OnInit can change the processor observers
	GetPluginTab( )->InitBaseWindow( baseWindow );

	// Enable
	GetToolWindow( )->Enable();

	GetSizer()->Clear(true);
	m_EmptyLabel = NULL;
	m_lblCaption = NULL;

	// layout them
	wxBoxSizer* vlayout = new wxBoxSizer(wxVERTICAL);
	vlayout->Add(GetToolWindow( ), 1, wxEXPAND | wxALL, 4);

	SetSizer(vlayout);

	CreateCaptionLabel( );

	FitInside( );
}


void Core::Widgets::BaseToolboxWidget::SetToolByFactoryName( const std::string &factoryName )
{
	BaseWindowFactories::Pointer baseWindowFactory;
	baseWindowFactory = Core::Runtime::Kernel::GetGraphicalInterface()->GetBaseWindowFactory( );

	Core::BaseWindow* baseWindow;
	baseWindow = baseWindowFactory->CreateWindow( factoryName, this );

	wxWindow* window = dynamic_cast<wxWindow*> (baseWindow);
	if ( !window )
	{
		throw Core::Exceptions::Exception( 
			"BaseToolboxWidget::SetToolByFactoryName", 
			"input widget is not a wxWindow" );
	}

	SetToolWindow( window );
}
