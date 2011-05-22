/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreMainMenu_H
#define coreMainMenu_H

#include "gmWidgetsWin32Header.h"
#include "coreObject.h"
#include "coreWindowConfig.h"
#include "coreBaseWindow.h"

#include <wx/frame.h>
#include <wx/docview.h>
#include <wx/notebook.h>

#include <string>

class wxNotebook;
class wxProgressDialog;

// Define the id's for the widgets and menu items
#define wxID_OpenDataMenuItem				wxID("OpenDataMenuItem")
#define wxID_OpenDirectoryMenuItem			wxID("wxID_OpenDirectoryMenuItem")
#define wxID_SaveDataMenuItem				wxID("SaveDataMenuItem")
#define wxID_SaveImageMenuItem				wxID("SaveImageMenuItem")
#define wxID_OpenSessionMenuItem			wxID("OpenSessionMenuItem")
#define wxID_SaveSessionMenuItem			wxID("SaveSessionMenuItem")
#define wxID_ExitMenuItem					wxID("ExitMenuItem")

#define wxID_ImportConfigurationMenuItem	wxID("ImportConfigurationMenuItem")
#define wxID_ShowLogMenuItem				wxID("ShowLogMenuItem")
#define wxID_Preferences					wxID("wxID_Preferences")

#define wxID_UserRegistration				wxID("UserRegistration")
#define wxID_ReportBug						wxID("ReportBug")
#define wxID_DevelopersSite					wxID("DevelopersSite")
#define wxID_FrameworkVideos				wxID("FrameworkVideos")
#define wxID_ShowAboutMenuItem				wxID("ShowAboutMenuItem")

#define wxID_CopyMenuItem					wxID("CopyMenuItem")
#define wxID_PasteMenuItem					wxID("PasteMenuItem")

#define wxID_WindowsMenuItem				wxID("WindowsMenuItem")
#define wxID_WindowResetLayoutMenuItem		wxID("WindowResetLayoutMenuItem")
#define wxID_WindowMaximizeWorkingAreaMenuItem	wxID("WindowMaximizeWorkingAreaMenuItem")

#define wxID_BasicSurfaceEditingMenuItem wxID("BasicSurfaceEditing")
#define wxID_AdvancedSurfaceEditingMenuItem wxID("AdvancedSurfaceEditingMenuItem")

#define wxID_ToolbarsMenuItem wxID("wxID_ToolbarsMenuItem")
#define wxID_WorkingAreaMenuItem wxID("wxID_WorkingAreaMenuItem")

#define wxID_WorkflowManagerMenuItem			wxID("wxID_WorkflowManagerMenuItem")
#define wxID_PerspectiveMenuItem			wxID("wxID_PerspectiveMenuItem")
#define wxID_PerspectivePluginMenuItem		wxID("wxID_PerspectivePluginMenuItem")
#define wxID_PerspectiveWorkflowMenuItem	wxID("wxID_PerspectiveWorkflowMenuItem")

#define wxID_ShowAllToolbarsMenuItem		wxID("wxID_ShowAllToolbarsMenuItem")
#define wxID_HideAllToolbarsMenuItem		wxID("wxID_HideAllToolbarsMenuItem")

namespace Core
{
namespace Runtime { class wxMitkGraphicalInterface; }

namespace Widgets
{

class PluginTab;
class RenderWindowBase;

/** 
\brief Main menu for the Core application

\ingroup gmWidgets
\author Xavi Planes
\date 8 April 2010
*/
class GMWIDGETS_EXPORT MainMenu : public wxMenuBar, public BaseWindow
{

public:
	coreClassNameMacro(MainMenu)
	typedef MainMenu Self; 
	typedef boost::signal0<void> SignalType;
	typedef SignalType::slot_function_type SlotType;
	typedef std::pair<int, SlotType> IdCallbackPair;
	typedef std:: map <int, SlotType> MenuEventCallbackMap;

	MainMenu( );
	~MainMenu(void);

	//! Update the menu state (checked items)
	void UpdateMenus();

	//! Load data entity and render it
	void LoadDataEntity( std::vector<std::string> pathFilenames );

	//!
	virtual void SetPluginTab(Core::Widgets::PluginTab* val);

	//!
	Core::BaseProcessor::Pointer GetProcessor( );

	//!
	wxFileHistory * GetFileHistory() const;

protected:

	/**
	\brief Returns the PluginTab currently selected and displayed in the 
	main window. So it is actually the GUI of the plugin that the user 
	is currently working with.
	*/
	Core::Widgets::PluginTab* GetCurrentPluginTab(void);

	//! 
	Core::Widgets::RenderWindowBase* GetCurrentMultiRenderWindow( );

	//!
	int GetCurrentPluginTimeStep();

	//! 
	blTagMap::Pointer GetCurrentMultiRenderWindowState( );

	//!
	void CreateMainMenu( );

	//!
	void Initialize();

	//!
	bool FileExists( std::string strFilename );

	//!
	void OnLoadSingleDataEntity( 
		Core::DataEntity::Pointer dataEntity );

	//! Create the menu items for View Menu
	void UpdateViewMenuItems( );

	//! Update Tools And Selection menu items
	void UpdateRegisteredWindowsMenuItems( );

	/** Update a main menu item, using a specific flag of registered windows
	and a specific event handler
	*/
	void UpdateRegisteredWindowsMenu( 
		wxString menuName,
		WIDGET_TYPE state, 
		wxObjectEventFunction func );

private:
	void OnMenuOpenFile(wxCommandEvent& event);
	void OnMenuOpenDirectory(wxCommandEvent& event);
	void OnMenuSaveFile(wxCommandEvent& event);
	void OnMenuSaveImage(wxCommandEvent& event);
	void OnMenuSaveSession(wxCommandEvent& event);
	void OnMenuOpenSession(wxCommandEvent& event);
	void OnMenuExit(wxCommandEvent& event);

	void OnMenuShowLog(wxCommandEvent& event);
	void OnMenuImportConfiguration(wxCommandEvent& event);
	void OnMenuPreferences(wxCommandEvent& event);

	void OnMenuUserRegistration(wxCommandEvent& event);
	void OnMenuReportBug(wxCommandEvent& event);
	void OnMenuDevelopersSite(wxCommandEvent& event);
	void OnMenuFrameworkVideos(wxCommandEvent& event);
	void OnMenuShowAbout(wxCommandEvent& event);

	void OnMRUFile(wxCommandEvent& event);

	void OnMenuCut(wxCommandEvent& event);
	void OnMenuCopy(wxCommandEvent& event);
	void OnMenuPaste(wxCommandEvent& event);

	void OnMenuWindowResetLayout(wxCommandEvent& event);
	void OnMenuWindowMaximizeWorkingAreaMenuItem(wxCommandEvent& event);

	void OnMenuItemWorkflowManager( wxCommandEvent& event );
	void OnMenuItemPerspectivePlugin( wxCommandEvent& event );
	void OnMenuItemPerspectiveWorkflow( wxCommandEvent& event );

	void OnMenuShowAllToolbars(wxCommandEvent& event);
	void OnMenuHideAllToolbars(wxCommandEvent& event);

	//! A menu item from automated menu has been clicked
	void OnAutomatedMenu( wxCommandEvent& event );

	//! A processing menu item from tools menu has been clicked
	void OnProcessingToolsMenu( wxCommandEvent& event );

	//! A selection menu item from tools menu has been clicked
	void OnSelectionToolsMenu( wxCommandEvent& event );

	//!
	void SetActiveMultiRenderWindow( Core::Widgets::RenderWindowBase*  window);

	//!
	void ShowReaderHeaderDialog( blTagMap::Pointer tagMap );

	//!
	void ShowAllWindows( const std::string &category, bool show );
private:
	//!
	wxFileHistory *m_FileHistory;

	//! Connection
	boost::signals::connection m_MultiRenderWindowStateConnection;

	//! Connection
	boost::signals::connection m_PluginTabConnection;

	//! Connection
	boost::signals::connection m_CurrentMultiRenderWindowConnection;

	DECLARE_EVENT_TABLE()
};

} // namespace Core
} // namespace Widgets

#endif
