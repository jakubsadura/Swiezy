/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreWxMitkCoreMainWindow_H
#define coreWxMitkCoreMainWindow_H

#include "gmWidgetsWin32Header.h"
#include "coreObject.h"
#include "coreBaseMainWindow.h"
#include "coreWindowConfig.h"
#include "coreWorkingAreaStorage.h"

#include <wx/frame.h>
#include <wx/docview.h>
#include <wx/aui/auibook.h>
#include "wxID.h"

#include <string>

#define wxID_TabContainer wxID( "wxID_TabContainer" )

class wxNotebook;
class wxProgressDialog;

namespace Core
{
namespace Runtime { class wxMitkGraphicalInterface; }

namespace Widgets
{

class MacroPannelWidget;
class LogFileViewer;
class SplashScreen;
class PluginTab;
class MainMenu;
class WorkflowEditorWidget;
class WorkflowManagerWidget;
class PreferencesDialog;

#ifdef WIN32
	class FuraPopupWidget;
#endif

/** 
\brief Main window for the Core application. It is instantiated when the 
graphical interface is created.
It is implemented using wxWidgets and Mitk.

The main window contains a macro panel. This panel displays messages and 
warnings, and is intended to be used for typing script commands. Also, 
it displays the script-equivalent of operations performed in the GUI.

\ingroup gmWidgets
\author Juan Antonio Moya
\date 03 Jan 2008
*/
class GMWIDGETS_EXPORT wxMitkCoreMainWindow 
	: public Core::Widgets::BaseMainWindow
{

public:
	coreClassNameMacro(wxMitkCoreMainWindow)
	typedef wxMitkCoreMainWindow Self; 
	typedef boost::signal0<void> SignalType;
	typedef SignalType::slot_function_type SlotType;
	typedef std::pair<int, SlotType> IdCallbackPair;
	typedef std:: map <int, SlotType> MenuEventCallbackMap;

	enum DialogResult { 
		DialogResult_YES, 
		DialogResult_NO, 
		DialogResult_OK, 
		DialogResult_CANCEL, 
		DialogResult_ERROR };

	wxMitkCoreMainWindow(
		wxWindow* parent, 
		wxWindowID id, 
		const wxString& title, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize,
		long style = wxDEFAULT_FRAME_STYLE | wxMAXIMIZE, 
		const wxString& name = wxT("wxMitkCoreMainWindow"));
	~wxMitkCoreMainWindow(void);
	
	//!
	void Initialize( );

	//!
	wxMitkCoreMainWindow::DialogResult AskQuestionToUserYesOrNo(
		const std::string& message, 
		const std::string& caption = "Question");
	//!
	wxMitkCoreMainWindow::DialogResult AskQuestionToUserOkOrCancel(
		const std::string& message, 
		const std::string& caption = "Question");
	//!
	void ReportWarning(const std::string& message, bool showAlsoDialog);
	//!
	void ReportError(const std::string& message, bool showAlsoDialog);
	//!
	void ReportMessage(const std::string& message, bool showAlsoDialog);
	//!
	void WriteVerbatimToMacroPanel(const std::string& message);

	/**
	\brief Returns the PluginTab currently selected and displayed in the 
	main window. So it is actually the GUI of the plugin that the user 
	is currently working with.
	*/
	Core::Widgets::PluginTab* GetCurrentPluginTab(void);

	//!
	Core::Widgets::PluginTab* GetLastPluginTab(void);

	//!
	virtual std::list<Core::Widgets::PluginTab*> GetAllPluginTabs( );

	//!
	void CreatePluginTab( const std::string &caption );

	//!
	bool AttachPluginGUI(Core::Widgets::PluginTab* page);
	//!
	void DetachPluginGUI(Core::Widgets::PluginTab* page);
	//!
	void ShowLogFileBrowser(void);
	//!
	void ShowProfileManager(void);
	//!
	void ShowImportConfigurationWizard(void);
	//!
	void ShowAboutScreen(void);
	//!
	void ShowRegistrationForm(void);
	//!
	MainMenu* GetMainMenu( );

	//! Redefined
	void RegisterWindow( 
		Core::BaseWindow *baseWindow,
		Core::WindowConfig config = Core::WindowConfig( ) );

	//! Redefined
	void RegisterWindow( 
		const std::string &factoryName,
		Core::WindowConfig config = Core::WindowConfig( ) );

	//! Redefined
	void AttachWorkflow(Workflow::Pointer workflow);

	//! Redefined
	void ShowWorkflowEditor( bool show = true );

	//! Redefined
	void ShowWorkflowManager( bool show = true );

	//! Redefined
	void UpdateActiveWorkflowLayout( );

	//! Redefined
	void RestartApplication( );

	//! Redefined
	void UpdateLayout( );

	//! Redefined
	void OnGraphicalInterfaceInitialized( );

	//! Redefined
	void RegisterModule( ModuleDescription *module );

	//! Redefined
	bool ShowMissingDataEntityFieldsWizard(
		Core::DataEntity::Pointer dataEntity);

	//! Redefined
	void ShowPreferencesdialog(void);

protected:
	//!
	void CreateWidgets(void);
	/**
	\brief Creates the menu for the graphical interface and attaches the 
	event handlers to the menu items
	*/
	void CreateMainMenu(void);

	//! Creates the context menu
	void CreateContextMenu(void);

	//!
	void CloseWindow(void);

	//! Create custom status bar
	wxStatusBar* OnCreateStatusBar(
		int number, 
		long style, 
		wxWindowID id, 
		const wxString& name);

private:
	//!
	void OnWxMouseEvent(wxMouseEvent& event);

	//!
	void OnClose(wxCloseEvent& event);

	//! m_TabContainer page has changed
	void OnPageChanged( wxAuiNotebookEvent& event );

	//! Create processing dialog
	void CreateProcessingDialog( );

	//! Add observer to state holder
	void AddStateHolderObserver( );

	//! Show/Hide processing dialog
	void ShowProcessingDialog( bool show );

	/**
	\brief Called when Core::Runtime::Environment::GetAppStateHolder() changes
	and show a progress dialog or hide it
	*/
	void OnAppStateChanged( );

	//! Update the m_ProgressDialog
	void OnProgressTicketChanged( );

	//!
	void DisconnectProgressTicketObserver( );

	//! Check if a file exist
	bool FileExists(std::string strFilename);

private:
	//!
	wxAuiNotebook* m_TabContainer;
	//!
	Core::Widgets::MacroPannelWidget* m_MacroPannel;
	//!
	wxStaticText* m_EmptyLabel;

	#ifdef WIN32
		Core::Widgets::FuraPopupWidget *m_FuraWidget;
	#endif

	/**
	\brief If this is true, at the end of release right button, context 
	menu will be shown
	*/
	bool m_bShowContextMenu;

	//! Context menu
	wxMenu* m_ContextMenu;

	//! Status bar
	wxStatusBar* m_StatusBar;

	//! Processing dialog: "Processing... Please wait"
	wxFrame* m_processingDialog;

	//! Progress dialog
	wxProgressDialog *m_ProgressDialog;

	//!
	WorkflowEditorWidget* m_WorkflowEditorWidget;

	//!
	WorkflowManagerWidget* m_WorkflowManagerWidget;

	//!
	PreferencesDialog* m_PreferencesDialog;

	DECLARE_EVENT_TABLE()
};

} // namespace Core
} // namespace Widgets


#endif
