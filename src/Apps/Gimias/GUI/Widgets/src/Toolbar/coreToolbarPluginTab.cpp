/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreToolbarPluginTab.h"
#include "corePluginTab.h"
#include "corePluginTabFactory.h"

#include <wx/wupdlock.h>

using namespace Core::Widgets;

BEGIN_EVENT_TABLE(Core::Widgets::ToolbarPluginTab, wxToolBar)
END_EVENT_TABLE()


Core::Widgets::ToolbarPluginTab::ToolbarPluginTab(
	wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style, const wxString& name):
    wxToolBar(parent, id, pos, size, style, name)
{
	SetName( "Toolbar PluginTab" );
}

Core::BaseProcessor::Pointer Core::Widgets::ToolbarPluginTab::GetProcessor()
{
	return NULL;
}

void Core::Widgets::ToolbarPluginTab::SetPluginTab( Core::Widgets::PluginTab* val )
{
	BaseWindow::SetPluginTab( val );

	GetPluginTab()->GetWindowsMapHolder()->AddObserver( 
		this,
		&ToolbarPluginTab::UpdateState );

	UpdateState( );
}

void Core::Widgets::ToolbarPluginTab::UpdateState()
{
	std::map<wxWindowID, wxWindow*> windows;
	windows = GetPluginTab()->GetAllWindows();
	std::map<wxWindowID, wxWindow*>::iterator it;
	for ( it = windows.begin() ; it != windows.end() ; it++ )
	{
		wxWindowID windowID = it->first;
		std::map<wxWindowID,wxWindowID>::iterator itTool;
		itTool = m_ToolWidgetMap.find( windowID );

		// Add icon to toolbar
		if ( itTool == m_ToolWidgetMap.end() )
		{
			AddWidgetTool( GetPluginTab()->GetWindow( windowID ) );
		}
		else
		{
			wxToolBarToolBase* tool = FindById( itTool->second );
			if ( tool )
			{
				tool->SetShortHelp( it->second->GetName() );
			}
		}

		// Change status
		itTool = m_ToolWidgetMap.find( it->second->GetId( ) );
		if ( itTool != m_ToolWidgetMap.end() )
		{
			bool shown = GetPluginTab()->IsWindowShown( windowID );
			ToggleTool( itTool->second, shown );
			
		}
	}

	// Remove tools of removed windows
	std::map<wxWindowID,wxWindowID>::iterator itTool;
	std::list<wxWindowID> removedTools;
	for ( itTool = m_ToolWidgetMap.begin() ; itTool != m_ToolWidgetMap.end() ; itTool++ )
	{
		it = windows.find( itTool->first );
		if ( it == windows.end() )
		{
			RemoveTool( itTool->second );
			removedTools.push_back( itTool->first );
		}
	}

	std::list<wxWindowID>::iterator itRemoved;
	for ( itRemoved = removedTools.begin() ; itRemoved != removedTools.end() ; itRemoved++ )
	{
		m_ToolWidgetMap.erase( *itRemoved );
	}

	Realize( );

	// Update pane info
	wxAuiPaneInfo& paneInfo = GetPluginTab()->GetAuiPaneInfo( this );
	if ( paneInfo.best_size != GetBestSize() )
	{
		paneInfo.best_size = GetBestSize();
		GetPluginTab()->UpdateAuiManager();
	}

	// We need to repaint the parent, otherwise, sometimes the icons dissapear
	if ( GetParent() )
	{
		wxSizeEvent resEvent(GetParent()->GetBestSize(), GetParent()->GetId());
		resEvent.SetEventObject(GetParent());
		GetParent()->GetEventHandler()->ProcessEvent(resEvent);
	}
}

void Core::Widgets::ToolbarPluginTab::AddWidgetTool( wxWindow* window )
{
	Core::BaseWindow* baseWindow = dynamic_cast<Core::BaseWindow*> ( window );
	if ( baseWindow == NULL )
	{
		return;
	}

	wxBitmap bitmap;
	if ( baseWindow->GetBitmap() )
	{
		bitmap = wxBitmap( baseWindow->GetBitmap() );
	}
	else if ( !baseWindow->GetBitmapFilename().empty() )
	{
		Core::IO::File::Pointer file = Core::IO::File::New();
		file->SetFileNameFullPath( baseWindow->GetBitmapFilename() );
		if ( file->Exists() )
		{
			wxImage img;
			img.LoadFile( file->GetFileNameFullPath() );
			if ( img.IsOk() )
			{
				bitmap = wxBitmap( img );
			}
		}
	}
	else
	{
		BaseWindowFactories::Pointer baseWindowFactory;
		baseWindowFactory = Core::Runtime::Kernel::GetGraphicalInterface()->GetBaseWindowFactory( );

		WindowConfig config;
		bool success = baseWindowFactory->GetWindowConfig( baseWindow->GetFactoryName(), config );
		if ( success && config.GetBitmap() )
		{
			bitmap = wxBitmap( config.GetBitmap() );
		}
	}

	if ( !bitmap.IsOk() )
	{
		return;
	}

	wxWindowID id = wxNewId();
	AddTool(id, window->GetName(), bitmap );

	m_ToolWidgetMap[ window->GetId( ) ] = id;

	// Connect handler
	Connect(
		wxID_ANY,
		wxEVT_COMMAND_TOOL_CLICKED,
		wxCommandEventHandler(ToolbarPluginTab::OnSelectedTool)
		);
}

void Core::Widgets::ToolbarPluginTab::OnSelectedTool( wxCommandEvent& event )
{
	wxWindowUpdateLocker noUpdates( GetPluginTab() );

	wxToolBarToolBase* tool = FindById( event.GetId() );
	GetPluginTab()->ShowWindow( tool->GetLabel(), event.IsChecked() );
	event.Skip();
}

void Core::Widgets::ToolbarPluginTab::AddTool( 
	wxWindowID id, 
	const wxString &label, 
	const wxBitmap &bitmap )
{
	wxToolBar::AddTool(id, 
		label,
		bitmap, wxNullBitmap, wxITEM_CHECK,
		label, 
		label );
}
