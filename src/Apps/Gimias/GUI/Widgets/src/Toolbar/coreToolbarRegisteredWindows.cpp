/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreToolbarRegisteredWindows.h"
#include "coreBaseWindowFactories.h"
#include "coreWxMitkGraphicalInterface.h"
#include "corePluginTab.h"
#include "coreSelectionToolboxWidget.h"
#include "corePluginTabFactory.h"

#include <wx/wupdlock.h>

using namespace Core::Widgets;

BEGIN_EVENT_TABLE(Core::Widgets::ToolbarRegisteredWindows, ToolbarBase)
END_EVENT_TABLE()


Core::Widgets::ToolbarRegisteredWindows::ToolbarRegisteredWindows(
	wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style, const wxString& name):
    ToolbarBase(parent, id, pos, size, style, name)
{
	SetName( "Toolbar Window Factory" );

	BaseWindowFactories::Pointer baseWindowFactory;
	baseWindowFactory = Core::Runtime::Kernel::GetGraphicalInterface()->GetBaseWindowFactory( );
	baseWindowFactory->GetFactoriesHolder()->AddObserver(
		this,
		&ToolbarRegisteredWindows::UpdateTools );

}

void Core::Widgets::ToolbarRegisteredWindows::OnInit( )
{
	GetPluginTab()->GetWindowsMapHolder()->AddObserver( 
		this,
		&ToolbarRegisteredWindows::UpdateTools );
}

void Core::Widgets::ToolbarRegisteredWindows::UpdateTools( )
{
	BaseWindowFactories::Pointer baseWindowFactory;
	baseWindowFactory = Core::Runtime::Kernel::GetGraphicalInterface()->GetBaseWindowFactory( );

	std::list<std::string> windowsList;
	windowsList = baseWindowFactory->GetWindowsList( m_ToolsType );

	SelectionToolboxWidget* selectionToolboxWidget;
	GetPluginTab()->GetWidget( wxID_SelectionToolboxWidget, selectionToolboxWidget );
	if ( selectionToolboxWidget == NULL )
	{
		return;
	}

	std::list<std::string>::iterator it;
	for ( it = windowsList.begin( ) ; it != windowsList.end() ; it++ )
	{
		ToolsMapType::iterator itTool;
		itTool = m_ToolWidgetMap.find( *it );

		// Add icon to toolbar
		if ( itTool == m_ToolWidgetMap.end() )
		{
			AddWidgetTool( *it );
		}

		// Change status
		itTool = m_ToolWidgetMap.find( *it );
		if ( GetPluginTab() && itTool != m_ToolWidgetMap.end() )
		{
			SelectionToolWidget* widget;
			widget = selectionToolboxWidget->GetSelectionToolWidget( );
			bool enabled = false;
			if ( widget )
			{
				enabled = typeid( *widget ).name( ) == itTool->first;
			}
			ToggleTool( itTool->second, enabled );
		}
	}

	if ( GetPluginTab() == NULL )
	{
		return;
	}

	// Update pane info
	wxAuiPaneInfo& paneInfo = GetPluginTab()->GetAuiPaneInfo( this );
	if ( paneInfo.best_size != GetBestSize() )
	{
		paneInfo.best_size = GetBestSize();
		// if the toolbar is empty creates drawing problems
		if ( paneInfo.best_size.GetX() <= 5 )
		{
			paneInfo.best_size.x = 5;
			paneInfo.Show( false );
		}
		GetPluginTab()->UpdateAuiManager();
	}
}

void Core::Widgets::ToolbarRegisteredWindows::OnSelectedTool( wxCommandEvent& event )
{
	try
	{
		wxToolBarToolBase* tool = FindById( event.GetId() );

		wxWindowUpdateLocker noUpdates( GetPluginTab() );

		SelectionToolboxWidget* selectionToolboxWidget;
		GetPluginTab()->GetWidget( wxID_SelectionToolboxWidget, selectionToolboxWidget );

		if ( event.IsChecked() )
		{
			selectionToolboxWidget->SetToolByName( tool->GetLabel().c_str() );
			selectionToolboxWidget->Start();
		}
		else
		{
			selectionToolboxWidget->Stop();
			selectionToolboxWidget->SetToolByName( "None" );
		}

		GetPluginTab()->ShowWindow( wxID_SelectionToolboxWidget, event.IsChecked() );
	}
	coreCatchExceptionsReportAndNoThrowMacro( ToolbarRegisteredWindows::OnSelectedTool )

	UpdateTools( );
}


void Core::Widgets::ToolbarRegisteredWindows::AddWidgetTool( 
	const std::string &factoryName )
{
	BaseWindowFactories::Pointer baseWindowFactory;
	baseWindowFactory = Core::Runtime::Kernel::GetGraphicalInterface()->GetBaseWindowFactory( );
	Core::WindowConfig config;
	baseWindowFactory->GetWindowConfig( factoryName, config );

	const char* const* xpmBitmap = config.GetBitmap();
	if ( xpmBitmap == NULL )
	{
		return;
	}

	wxBitmap bitmap;
	bitmap = wxBitmap( xpmBitmap );
	if ( !bitmap.IsOk() )
	{
		return;
	}

	wxWindowID id = wxNewId();
	wxToolBar::AddTool(id, 
		config.GetCaption(),
		bitmap, wxNullBitmap, wxITEM_CHECK,
		config.GetCaption(), 
		config.GetCaption() );

	m_ToolWidgetMap[ factoryName ] = id;

	Realize( );

	// Connect handler
	Connect(
		wxID_ANY,
		wxEVT_COMMAND_TOOL_CLICKED,
		wxCommandEventHandler(ToolbarRegisteredWindows::OnSelectedTool)
		);
}

void Core::Widgets::ToolbarRegisteredWindows::SetPluginTab( Core::Widgets::PluginTab* val )
{
	ToolbarBase::SetPluginTab( val );

	UpdateTools();
}