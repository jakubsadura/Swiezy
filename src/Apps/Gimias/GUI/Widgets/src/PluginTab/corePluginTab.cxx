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

#include "corePluginTab.h"
#include "corePluginTabFactory.h"

#include "coreAssert.h"
#include "coreKernel.h"
#include "coreException.h"
#include "coreReportExceptionMacros.h"
#include "coreWxMitkGraphicalInterface.h"
#include "coreDataContainer.h"
#include "coreMultiRenderWindow.h"
#include "coreRenderWindowContainer.h"
#include "coreMultiRenderWindow.h"
#include "coreProcessorInputWidget.h"

#include "coreProcessorWidgetsBuilder.h"
#include "coreKernel.h"
#include "coreMovieToolbar.h"
#include "coreCommandPanel.h"
#include "coreDataEntityListBrowser.h"
#include "coreUserHelperWidget.h"
#include "coreToolbarWindows.h"

#include <wx/notebook.h>
#include <wx/sizer.h>

#include <wxUnicode.h>

#include "dynWxAGUIBuilder.h"

using namespace Core::Widgets;

BEGIN_EVENT_TABLE(PluginTab, wxPanel)
  EVT_AUI_PANE_CLOSE(PluginTab::OnClosePane)	
END_EVENT_TABLE();

PluginTab::PluginTab(wxWindow* parent, const std::string& caption)
: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT("PluginTab")), m_Caption(caption)
{
	try
	{
		m_WindowsMapHolder = Core::DataHolder<WindowMapType>::New( );
		m_RenderingTreeManager = Core::RenderingTreeManager::New( );

		m_wxAuiManager.SetManagedWindow( this );
		m_wxAuiManager.SetFlags( 
			wxAUI_MGR_ALLOW_FLOATING |
			wxAUI_MGR_TRANSPARENT_HINT |
			wxAUI_MGR_HINT_FADE |
			wxAUI_MGR_NO_VENETIAN_BLINDS_FADE |
			wxAUI_MGR_ALLOW_ACTIVE_PANE );
	}
	coreCatchExceptionsAddTraceAndThrowMacro(PluginTab::PluginTab)
}

PluginTab::~PluginTab(void)
{	
	// Remove references to active MultiRenderWindow of current working area
	if ( GetWorkingAreaManager( ) )
	{
		GetWorkingAreaManager( )->SetActiveWorkingArea( -1 );
	}

	m_wxAuiManager.UnInit();

	CommandPanel* commandPanel;
	GetWidget(wxID_CommandPanel, commandPanel);

	// ImageContrastWidget has dependency to levelWindowWidget of WorkingArea
	// So we need to destroy first all dependent windows
	WindowMapType windowsMap = GetAllWindows();
	WindowMapType::iterator itWindow;
	for ( itWindow = windowsMap.begin() ; itWindow != windowsMap.end() ; itWindow++ )
	{
		// Don't remove command panel items
		if ( !GetWorkingArea( itWindow->first ) && 
			 wxID_CommandPanel != itWindow->first &&
			 wxID_WorkingArea != itWindow->first && 
			 commandPanel != NULL &&
			 commandPanel->GetItemByWidget( itWindow->second ) == NULL )
		{
			itWindow->second->Destroy( );
		}
	}

}

void Core::Widgets::PluginTab::RemoveWorkingArea( int ID )
{
	GetWorkingAreaManager()->RemoveWorkingArea( ID );
}

void Core::Widgets::PluginTab::SetCurrentWorkingArea( int ID )
{
	// Select it 
	GetWorkingAreaManager()->SetActiveWorkingArea( ID );
}

Core::BaseWindow* Core::Widgets::PluginTab::GetWorkingArea( int ID ) const
{
	if ( !GetWorkingAreaManager() )
	{
		return NULL;
	}
	return dynamic_cast<BaseWindow*> ( GetWorkingAreaManager()->GetWorkingArea( ID ) );
}

Core::BaseWindow* Core::Widgets::PluginTab::GetCurrentWorkingArea() const
{
	if ( GetWorkingAreaManager() == NULL )
	{
		return NULL;
	}
	return dynamic_cast<BaseWindow*> ( GetWorkingAreaManager()->GetActiveWorkingArea( ) );
}

WorkingAreaManager* Core::Widgets::PluginTab::GetWorkingAreaManager() const
{
	WorkingAreaManager* workingAreaManager;
	GetWidget( wxID_WorkingArea, workingAreaManager );
	return workingAreaManager;
}

std::string Core::Widgets::PluginTab::GetCaption() const
{
	return m_Caption;
}

void Core::Widgets::PluginTab::SetActiveTab( bool bVal )
{
	WindowMapType::iterator itWindow;
	WindowMapType windowsMap = GetAllWindows();
	for ( itWindow = windowsMap.begin() ; itWindow != windowsMap.end() ; itWindow++ )
	{
		if ( !GetWorkingArea( itWindow->first ) && 
			 !IsCommandPanelWidget( itWindow->first ) )
		{
			itWindow->second->Enable(bVal);
		}
	}

	if ( GetWorkingAreaManager() )
	{
		GetWorkingAreaManager()->Enable( bVal );
	}
}

void Core::Widgets::PluginTab::ResetLayout()
{
	wxWindowUpdateLocker lock( this );

	m_wxAuiManager.LoadPerspective( _U(m_BackupLayoutPerspective) );

	UpdateToolbarsSize();

	m_WindowsMapHolder->NotifyObservers( );
}

std::string Core::Widgets::PluginTab::GetLayoutConfiguration()
{
	return _U(m_wxAuiManager.SavePerspective());
}

void Core::Widgets::PluginTab::SetLayoutConfiguration( const std::string &strConfig )
{
	m_wxAuiManager.LoadPerspective( _U(strConfig) );

	// Update Obervers
	m_WindowsMapHolder->NotifyObservers( );
}

void Core::Widgets::PluginTab::UpdateLayoutConfiguration( const std::string &strConfig )
{
	m_wxAuiManager.LoadPerspective( _U(strConfig) );
	m_wxAuiManager.Update();

	// Update ShowWindow & Enable
	// When the layout is updated calling LoadPerspective,
	// there are some windows that are shown but they are not enabled
	// We need to call enable if the window is shown
	wxAuiPaneInfoArray& panes = m_wxAuiManager.GetAllPanes( );
	for ( size_t i = 0 ; i < panes.size( ) ; i++ )
	{
		ShowWindow( panes[ i ].window->GetId( ), panes[ i ].IsShown() );
	}

	UpdateToolbarsSize( );

	m_WindowsMapHolder->NotifyObservers( );
}

void Core::Widgets::PluginTab::BackupLayoutConfiguration()
{
	m_BackupLayoutPerspective = m_wxAuiManager.SavePerspective();
}

void Core::Widgets::PluginTab::EnableWindow( wxWindowID id, bool enable /*= true*/ )
{
	m_WindowEnabled[ id ] = enable;
	if ( enable == false )
	{
		ShowWindow( id, false );
	}
}

void Core::Widgets::PluginTab::MaximizeWindow( wxWindowID id )
{
	bool bMaximied;
	wxWindow *window = GetWindow( id );
	bMaximied = m_wxAuiManager.GetPane( window ).HasFlag( wxAuiPaneInfo::optionMaximized );
	if ( !bMaximied )
	{
		m_wxAuiManager.MaximizePane( m_wxAuiManager.GetPane( window ) );
	}
	else
	{
		m_wxAuiManager.RestorePane( m_wxAuiManager.GetPane( window ) );
	}

	m_wxAuiManager.Update();
}

wxAuiPaneInfo& Core::Widgets::PluginTab::GetAuiPaneInfo( wxWindow *window )
{
	return m_wxAuiManager.GetPane( window );
}

void Core::Widgets::PluginTab::UpdateAuiManager()
{
	m_wxAuiManager.Update();
}

wxWindow* Core::Widgets::PluginTab::GetWindow( wxWindowID windowID ) const
{
	WindowMapType windowsMap = GetAllWindows();
	WindowMapType::iterator it = windowsMap.find( windowID );
	if ( it != windowsMap.end() )
	{
		return windowsMap[ windowID ];
	}
	return NULL;
}

wxWindow* Core::Widgets::PluginTab::GetWindow( const wxString &name ) const
{
	WindowMapType windowsMap = GetAllWindows();
	WindowMapType::iterator it;
	for ( it = windowsMap.begin() ; it != windowsMap.end() ; it++ )
	{
		if ( it->second->GetName() == name )
		{
			return it->second;
		}
	}
	return NULL;
}

void PluginTab::AddWidgetToCommandPanel( wxWindow* aWidget )
{
	coreAssertMacro(aWidget != NULL && "The widget to set may not be NULL");

	CommandPanel* commandPanel;
	GetWidget(wxID_CommandPanel, commandPanel);

	CommandPanelItem* cmdItem = new CommandPanelItem(aWidget, aWidget->GetName(), commandPanel);
	commandPanel->Add((CommandPanelItem*)cmdItem);

	InitBaseWindow( dynamic_cast<Core::BaseWindow*> ( aWidget ) );

	GetAllWindows()[ aWidget->GetId() ] = aWidget;
	m_WindowsMapHolder->NotifyObservers();
}

void Core::Widgets::PluginTab::AddWindow( 
	Core::BaseWindow* baseWindow, 
	WindowConfig config )
{
	// Init processor observers
	baseWindow->InitProcessorObservers( config.GetCreateProcessorObservers() );

	// Add widget to plugin tab command panel
	wxWindow* window = dynamic_cast<wxWindow*> (baseWindow);
	if ( window == NULL )
	{
		throw Core::Exceptions::Exception( 
			"PluginTab::RegisterWindow", 
			"input widget is not a wxWindow" );
	}

	if ( GetWindow( window->GetName() ) )
	{
		std::ostringstream strError;
		strError 
			<< "A window with the name '" << window->GetName()
			<< "' is already added to the Tab '" << GetCaption() << "'";
		throw Core::Exceptions::Exception( 
			"PluginTab::RegisterWindow", 
			strError.str( ).c_str( ) );
	}

	if ( GetWindow( window->GetId() ) )
	{
		std::ostringstream strError;
		strError 
			<< "A window with the ID " << window->GetId()
			<< " is already added to the Tab '"<< GetCaption() << "'";
		throw Core::Exceptions::Exception( 
			"PluginTab::RegisterWindow", 
			strError.str( ).c_str( ) );
	}


	if ( config.GetCommandPanel( ) )
	{
		AddWidgetToCommandPanel( window );
		return;
	}

	if ( config.GetWorkingArea() && GetWorkingAreaManager() )
	{
		if ( GetWorkingAreaManager() == NULL )
		{
			throw Core::Exceptions::Exception( 
				"PluginTab::RegisterWindow", 
				"Cannot add a working area before adding the working Area manager" );
		}
		GetWorkingAreaManager()->AddWorkingArea( window );
	}

	wxAuiPaneInfo paneInfo;
	paneInfo.Layer(config.GetLayer());
	paneInfo.Position( config.GetPosition( ) );
	if ( config.GetToolbar() )
	{
		paneInfo.ToolbarPane();
	}
	switch( config.GetDock( ) )
	{
	case WIDGET_DOCK_TOP:paneInfo.Top();break;
	case WIDGET_DOCK_RIGHT:paneInfo.Right();break;
	case WIDGET_DOCK_BOTTOM:paneInfo.Bottom();break;
	case WIDGET_DOCK_LEFT:paneInfo.Left();break;
	case WIDGET_DOCK_CENTER:paneInfo.Center();break;
	}

	// Set min width of right panels
	if ( config.GetDock( ) == WIDGET_DOCK_RIGHT )
	{
		window->SetMinSize( wxSize(310, -1) );
	}

	paneInfo.CloseButton( config.GetCloseButton() );
	if ( config.GetFLoat() )
	{
		paneInfo.Float(  );
	}
	paneInfo.CaptionVisible( config.GetCaptionVisible() );
	paneInfo.MinimizeButton( );
	paneInfo.MaximizeButton( );

	paneInfo.MinSize( window->GetMinSize( ) );
	paneInfo.MaxSize( window->GetMaxSize( ) );
	paneInfo.BestSize( window->GetBestSize( ) );
	paneInfo.Caption( window->GetName( ) );
	paneInfo.Name( window->GetName( ) );
	paneInfo.Show( config.GetShow() );

	AddWindow( window, paneInfo);

	// By default hide all added windows
	ShowWindow( window->GetId(), config.GetShow() );

}

void Core::Widgets::PluginTab::InitBaseWindow( Core::BaseWindow* baseWindow )
{
	if ( baseWindow == NULL )
	{
		return;
	}

	// Add Help controls using ModuleDescription and automated GUI generation
	if ( baseWindow->GetModule() && 
		 baseWindow->GetModule()->GetTitle() != "Unknown" &&
		 baseWindow->GetModule()->GetParameterGroups().size() == 0 )
	{
		dynWxAGUIBuilder builder;
		builder.SetModule( baseWindow->GetModule() );
		wxWindow* win = dynamic_cast<wxWindow*> ( baseWindow);
		builder.SetParentWindow( win );
		builder.Update();
		dynBasePanel* panel = builder.GetPanel();
		win->GetSizer( )->Insert( 0, panel, 0, wxEXPAND );
	}

	// Create AcquiredInput widgets
	DataEntityListBrowser* dataEntityListBrowser;
	GetWidget( wxID_DataEntityListBrowser, dataEntityListBrowser );
	Core::ProcessorWidgetsBuilder::Init( 
		baseWindow->GetProcessor().GetPointer(), 
		baseWindow, 
		dataEntityListBrowser, 
		true );

	baseWindow->SetPluginTab( this );

	// Add widget as observer of active rendering tree
	if ( m_RenderingTreeManager.IsNotNull() )
	{
		m_RenderingTreeManager->AddActiveTreeObserver( baseWindow );
		int count = 0;
		BaseInputControl* widget;
		while ( widget = baseWindow->GetInputWidget( count ) )
		{
			// Add observer to rendering tree for each input widget
			GetRenderingTreeManager()->AddActiveTreeObserver( widget );
			// Add observer to multirender window for each input widget
			GetWorkingAreaManager()->AddObserverOnActiveWindow( widget );
			count++;
		}
	}

	// MultiRenderWindow
	if ( GetWorkingAreaManager() )
	{
		GetWorkingAreaManager()->AddObserverOnActiveWindow( baseWindow );
	}

	// Set plugin widgets
	UserHelper* userHelperWidget;
	GetWidget( wxID_UserHelper, userHelperWidget );
	baseWindow->SetHelperWidget( userHelperWidget );

	baseWindow->SetListBrowser( dataEntityListBrowser );

	MovieToolbar* movieToolbar;
	GetWidget( wxID_MovieToolbar, movieToolbar );
	if ( movieToolbar )
	{
		baseWindow->SetTimeStepHolder( movieToolbar->GetCurrentTimeStep() );
		movieToolbar->GetCurrentTimeStep()->AddObserver1(
			baseWindow,
			&Core::BaseWindow::SetTimeStep );
	}

	baseWindow->Init( );
}

Core::RenderingTreeManager::Pointer Core::Widgets::PluginTab::GetRenderingTreeManager() const
{
	return m_RenderingTreeManager;
}

std::map<wxWindowID, wxWindow*> &Core::Widgets::PluginTab::GetAllWindows() const
{
	return m_WindowsMapHolder->GetSubject( );
}

void Core::Widgets::PluginTab::ShowWindow( wxWindowID id, bool show /*= true */ )
{
	if ( m_WindowEnabled.find( id ) == m_WindowEnabled.end( ) )
	{
		return;
	}

	// If the panel is not enabled, throw an exception with the error
	if ( show && !m_WindowEnabled[ id ] )
	{
		return;
	}

	// If the window is a working area and show is enabled or is the only
	// working area -> Set it as active one
	if ( GetWorkingArea( id ) && 
		 ( show || GetWorkingAreaManager()->GetWorkingAreaMap( ).size() == 1 ) )
	{
		SetCurrentWorkingArea( id );
		return;
	}

	WindowMapType windowsMap = GetAllWindows();
	WindowMapType::iterator it = windowsMap.find( id );
	if ( it != windowsMap.end() )
	{
		m_wxAuiManager.GetPane( it->second ).Show( show );
		m_wxAuiManager.Update( );
		it->second->Enable( show );
		m_WindowsMapHolder->NotifyObservers( );
	}
}

void Core::Widgets::PluginTab::ShowWindow( const wxString &label, bool show /*= true */ )
{
	wxWindow *window = NULL;
	window = GetWindow( label );
	if ( window == NULL )
	{
		return;
	}
	ShowWindow( window->GetId(), show );
}

void Core::Widgets::PluginTab::AddWindow( wxWindow* window, wxAuiPaneInfo &auiInfo )
{
	// Init base window befor Enable
	Core::BaseWindow* baseWindow = dynamic_cast<Core::BaseWindow*> ( window );
	InitBaseWindow( baseWindow );

	// First add the window to the map because it can be needed when doing Enable
	// for example with WorkingAreaManager that creates new WorkingAreas
	WindowMapType windowsMap = GetAllWindows();
	windowsMap[ window->GetId() ] = window;
	m_WindowsMapHolder->SetSubject( windowsMap );

	if ( !GetWorkingArea( window->GetId() ) )
	{
		m_wxAuiManager.AddPane( window, auiInfo );
		m_wxAuiManager.Update();

		window->Enable( IsEnabled( ) && auiInfo.IsShown() );
	}

	EnableWindow( window->GetId( ) );

}

bool Core::Widgets::PluginTab::IsWindowShown( wxWindowID id )
{
	bool bShown = false;
	wxWindow *window = GetWindow( id );
	if ( window )
	{
		if ( GetWorkingArea( id ) )
		{
			bShown = GetWorkingAreaManager()->GetActiveWorkingAreaID( ) == id;
		}
		else
		{
			bShown = m_wxAuiManager.GetPane( window ).IsShown();
		}
	}
	return bShown;
}

Core::DataHolder<PluginTab::WindowMapType>::Pointer 
Core::Widgets::PluginTab::GetWindowsMapHolder() const
{
	return m_WindowsMapHolder;
}

void Core::Widgets::PluginTab::UpdateToolbarsSize()
{
	bool update = false;
	// Update size of toolbars when a plugin adds an icon to it and
	// the default size is not the correct one
	PluginTab::WindowMapType windowsMap = GetAllWindows();
	PluginTab::WindowMapType::iterator it;
	for ( it = windowsMap.begin() ; it != windowsMap.end() ; it++ )
	{
		wxToolBar* toolbar = dynamic_cast<wxToolBar*> ( it->second );
		if ( toolbar )
		{
			wxAuiPaneInfo& paneInfo = GetAuiPaneInfo( toolbar );
			if ( paneInfo.best_size != GetBestSize() )
			{
				paneInfo.best_size = toolbar->GetBestSize();
				// if the toolbar is empty creates drawing problems
				if ( paneInfo.best_size.GetX() <= 5 )
				{
					paneInfo.best_size.x = 5;
					paneInfo.Show( false );
				}
				update = true;
			}
		}
	}

	if ( update )
	{
		UpdateAuiManager();
	}
}

void PluginTab::EnableAllToolbars(bool enable)
{
	PluginTab::WindowMapType windowsMap = GetAllWindows();
	PluginTab::WindowMapType::iterator it;
	for ( it = windowsMap.begin() ; it != windowsMap.end() ; it++ )
	{
		wxToolBar* toolbar = dynamic_cast<wxToolBar*> ( it->second );
		if ( toolbar )
		{
			ShowWindow( toolbar->GetId( ), enable );
		}
	}
}

void PluginTab::OnClosePane( wxAuiManagerEvent& event )
{
	ShowWindow( event.pane->window->GetId(), false );
}

void PluginTab::GetProperties( blTagMap::Pointer tagMap )
{
	tagMap->AddTag( "Layout", GetLayoutConfiguration() );
}

void PluginTab::SetProperties( blTagMap::Pointer tagMap )
{
	blTag::Pointer tag = tagMap->FindTagByName( "Layout" );
	if ( tag.IsNotNull() )
	{
		std::string layout;
		tag->GetValue( layout );
		SetLayoutConfiguration( layout );
	}
}


bool Core::Widgets::PluginTab::IsCommandPanelWidget( const int id )
{
	CommandPanel* commandPanel;
	GetWidget(wxID_CommandPanel, commandPanel);
	if ( commandPanel == NULL )
	{
		return false;
	}

	return commandPanel->GetItemByWidget( GetWindow( id ) ) != NULL;
}

void Core::Widgets::PluginTab::RemoveWindow( Core::BaseWindow* baseWindow )
{
	// Add widget to plugin tab command panel
	wxWindow* window = dynamic_cast<wxWindow*> (baseWindow);
	if ( window == NULL )
	{
		throw Core::Exceptions::Exception( 
			"PluginTab::RegisterWindow", 
			"input widget is not a wxWindow" );
	}

	if ( !GetWindow( window->GetName() ) )
	{
		std::ostringstream strError;
		strError 
			<< "A window with the name '" << window->GetName()
			<< "' doesn't exist '" << GetCaption() << "'";
		throw Core::Exceptions::Exception( 
			"PluginTab::RegisterWindow", 
			strError.str( ).c_str( ) );
	}

	m_wxAuiManager.DetachPane( window );
	m_wxAuiManager.Update();

	GetAllWindows().erase( window->GetId() );

	CommandPanel* commandPanel;
	GetWidget(wxID_CommandPanel, commandPanel);
	if ( commandPanel && commandPanel->GetItemByWidget( window ) )
	{
		CommandPanelItem* item = dynamic_cast<CommandPanelItem*> 
			( commandPanel->GetItemByWidget( window ) );
		commandPanel->Remove( item );
		item->Destroy();
	}

	if ( GetWorkingAreaManager() )
	{
		GetWorkingAreaManager()->RemoveWorkingArea( window->GetId() );
		window->Destroy();
	}

	m_WindowsMapHolder->NotifyObservers();
}
