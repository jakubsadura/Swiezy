/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreBaseMainWindow_H
#define coreBaseMainWindow_H

#include <string>
#include <wx/frame.h>
#include "coreWindowConfig.h"
#include "coreWorkflow.h"

class ModuleDescription;

namespace Core
{
	class BaseWindow;

namespace Widgets 
{ 
	class PluginTab;


	/** 
	\brief This is an abstract class for the main window. It is created by the 
	main app.

	\ingroup gmKernel
	\author Xavi Planes
	\date 19 Jun 2009
	*/
	class BaseMainWindow : public wxFrame
	{
	public:
		BaseMainWindow(
			wxWindow* parent, 
			wxWindowID id, 
			const wxString& title, 
			const wxPoint& pos, 
			const wxSize& size, 
			long style, 
			const wxString& name) : wxFrame( parent, id, title, pos, size, style, name )
		{
		};
		virtual void Initialize( ) = 0;
		virtual void ShowLogFileBrowser(void) = 0;
		virtual void ShowProfileManager(void) = 0;
		virtual void ShowImportConfigurationWizard(void) = 0;
		virtual void ShowAboutScreen(void) = 0;
		virtual void ShowRegistrationForm(void) = 0;
		virtual void ReportWarning(const std::string& message, bool showAlsoDialog) = 0;
		virtual void ReportError(const std::string& message, bool showAlsoDialog) = 0;
		virtual void ReportMessage(const std::string& message, bool showAlsoDialog) = 0;
		virtual Core::Widgets::PluginTab* GetCurrentPluginTab(void) = 0;
		virtual Core::Widgets::PluginTab* GetLastPluginTab(void) = 0;
		virtual std::list<Core::Widgets::PluginTab*> GetAllPluginTabs( ) = 0;
		virtual bool AttachPluginGUI(Core::Widgets::PluginTab* page) = 0;
		virtual void DetachPluginGUI(Core::Widgets::PluginTab* page) = 0;
		virtual void CloseWindow( ) = 0;
		virtual void CreatePluginTab( const std::string &caption ) = 0;
		virtual void RegisterWindow( 
			Core::BaseWindow *baseWindow,
			WindowConfig config = WindowConfig( ).CommandPanel() ) = 0;

		/**
		Add an instance of a widget to all the already created plugin tabs
		*/
		virtual void RegisterWindow( 
			const std::string &name,
			WindowConfig config = WindowConfig( ) ) = 0;

		//! Attach a new workflow to main window
		virtual void AttachWorkflow(Workflow::Pointer workflow) = 0;

		//! Show WorkflowEditor dialog
		virtual void ShowWorkflowEditor( bool show = true ) = 0;

		//! Show WorkflowManager dialog
		virtual void ShowWorkflowManager( bool show = true ) = 0;

		//! Update active workflow with the current tab pages layout
		virtual void UpdateActiveWorkflowLayout() = 0;

		//! Restart application
		virtual void RestartApplication( ) = 0;

		//! Update layout for all plugin tabs
		virtual void UpdateLayout( ) = 0;

		//! Things to do when all is initialized
		virtual void OnGraphicalInterfaceInitialized( ) = 0;

		//! Register a ModuleDescription
		virtual void RegisterModule( ModuleDescription *module ) = 0;

		//! Returns false if the user pressed Cancel in the wizard.
		virtual bool ShowMissingDataEntityFieldsWizard(
			Core::DataEntity::Pointer dataEntity) = 0;

		//! Show preferences dialog
		virtual void ShowPreferencesdialog(void) = 0;

	};


}
}

#endif
