/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef corePluginTab_H
#define corePluginTab_H

#include "coreObject.h"
#include "coreDataEntityList.h"
#include "coreRenderingTreeManager.h"
#include "coreDataHolder.h"
#include "coreDataEntity.h"
#include "coreWorkingAreaManager.h"
#include "coreBaseWindow.h"
#include "coreWindowConfig.h"

#include <wx/panel.h>
#include <wx/aui/aui.h>

// Forward declarations
class wxNotebook;
class wxBoxSizer;

namespace Core
{
namespace Widgets
{
class CommandPanel;


/**
\brief The PluginTab arranges all the default GUI for a plugin.
There is always one PluginTab per plugin, that holds:

- a working area placeholder, where the user can set the GUI he designed 
for his plugin.
- a toolbar placeholder on the left side where to place toolbars that 
contain quick buttons for the most common functions available at the GUI.
- a command panel where to place the widgets designed for setting 
parameters, running comands or displaying info
- a browser for the DataEntity list of the application, showing all the 
DataEntities available for processing

When constructing a plugin, the programmer shall insert all the command 
panel items to the CommandPanel provided by this class. He shall provide 
a widget for the working area, and a toolbar optionally. The other widgets 
are provided and arranged by default

\ingroup gmWidgets
\sa FrontEndPlugin
\author Juan A. Moya
\date 10 Jan 2008
*/
class GMWIDGETS_EXPORT PluginTab : public wxPanel
{
public:
	typedef std::map<wxWindowID, wxWindow*> WindowMapType;

public:
	coreClassNameMacro(PluginTab);

	PluginTab(wxWindow* parent, const std::string& caption);
	virtual ~PluginTab(void);

	//! Caption
	std::string GetCaption() const;

	//!
	void MaximizeWindow( wxWindowID id );

	//! Remove a working area and destroy the window
	void RemoveWorkingArea( int ID );

	//!
	BaseWindow* GetCurrentWorkingArea( ) const;

	//!
	BaseWindow* GetWorkingArea( int ID = 0 ) const;

	//!
	void AddWindow( Core::BaseWindow* baseWindow, Core::WindowConfig config );

	//!
	void RemoveWindow( Core::BaseWindow* baseWindow );

	//! Perform dynamic_cast to get a concrete widget
	template <class T>
	void GetWidget( int id, T* &widget ) const {
		widget = dynamic_cast<T*> ( GetWindow( id ) );
	}

	/**
	\brief Enables or disables the space for the toolbar at the left of 
	tab window. Use it when you don't want that to have default space for 
	the toolbar.
	*/
	void EnableAllToolbars(bool enable = true);

	//! Set active tab when the user changes it
	void SetActiveTab( bool bVal );

	//!
	std::string GetLayoutConfiguration( );

	//!
	void BackupLayoutConfiguration( );

	//!
	void SetLayoutConfiguration( const std::string &strConfig );

	//! Call SetLayoutConfiguration but keep initial Show/hide state
	void UpdateLayoutConfiguration( const std::string &strConfig );

	//! 
	void ResetLayout( );

	//!
	wxAuiPaneInfo& GetAuiPaneInfo( wxWindow *window );

	//!
	void UpdateAuiManager( );

	//!
	WorkingAreaManager* GetWorkingAreaManager() const;

	//!
	void InitBaseWindow( Core::BaseWindow* baseWindow );

	//!
	Core::RenderingTreeManager::Pointer GetRenderingTreeManager() const;

	//!
	WindowMapType &GetAllWindows( ) const;

	//!
	wxWindow* GetWindow( wxWindowID id ) const;

	//!
	wxWindow* GetWindow( const wxString &label ) const;

	//!
	void ShowWindow( wxWindowID id, bool show = true );

	//!
	void ShowWindow( const wxString &label, bool show = true );

	//!
	void AddWindow( wxWindow* window, wxAuiPaneInfo &auiInfo );

	//! Check if is shown
	bool IsWindowShown( wxWindowID id );

	//!
	Core::DataHolder<WindowMapType>::Pointer GetWindowsMapHolder() const;

	//!
	void EnableWindow( wxWindowID id, bool enable = true );

	//! Redefined
	void GetProperties( blTagMap::Pointer tagMap );

	//! Redefined
	void SetProperties( blTagMap::Pointer tagMap );

	//! Change the current working area and enable it. If ID == -1 -> None
	void SetCurrentWorkingArea( int ID );

protected:	
	//!
	void SetDefaultShowWidgets( );

	//!
	void OnSelectedWorkingAreaWindow( );

	//!
	void OnClosePane( wxAuiManagerEvent& event );

	//! Add widget to command panel
	void AddWidgetToCommandPanel(wxWindow* aWidget);

	//!
	void UpdateToolbarsSize( );

	//!
	bool IsCommandPanelWidget( const int id );

	DECLARE_EVENT_TABLE( );

private:
	//!
	std::string m_Caption;

	//!
	wxAuiManager m_wxAuiManager;

	//!
	Core::RenderingTreeManager::Pointer m_RenderingTreeManager;

	//!
	Core::DataHolder<WindowMapType>::Pointer m_WindowsMapHolder;

	//! If the panel is disabled, it will not be shown never
	std::map<wxWindowID,bool> m_WindowEnabled;

	//! Backup layout perspective when calling SetLayoutConfiguration( )
	std::string m_BackupLayoutPerspective;
};

} // namespace Widgets
} // namespace Core

#endif // corePluginTab_H

