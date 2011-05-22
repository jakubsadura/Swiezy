/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreWxMitkGraphicalInterface_H
#define coreWxMitkGraphicalInterface_H

#include "coreObject.h"

#include "coreFrontEndPluginManager.h"
#include "coreStyleManager.h"
#include "coreSplashScreen.h"
#include "coreBaseMainWindow.h"
#include "coreBaseWindowFactories.h"
#include "coreBaseWorkingAreaStorage.h"

#include "wx/busyinfo.h"

#include <map>
#include <string>

class wxWindow;
class wxMessageDialog;
class wxDialog;

namespace Core
{
// Forward declarations
namespace Exceptions { class Exception; }
namespace Widgets 
{ 
	class SplashScreen;
	class SimpleProgressDialog;
	class PluginTab;
}

namespace Runtime
{

/** 
\brief This class is the main class in the presentation layer.
It implements the ways to access the Graphical interface done in wxMitk.
The graphical interface has members for interacting with the presentation 
layer. 
It dynamically loads all the plug-ins that are selected for the chosen user 
profile, and provides access to other managers.

\note You should access and get the instance for this class through the 
runtime high-end object: Kernel

\note The main window BaseMainWindow should be the top window and 
the parent of all windows because when this window is destroyed, all other 
windows will be destroyed automatically. Otherwise, the application will not 
finish correctly.

\sa Core::Runtime::Kernel, Core::Runtime::FrontEndPluginManager

\ingroup gmKernel
\author Juan Antonio Moya
\date 03 Jan 2008
*/

class GMKERNEL_EXPORT wxMitkGraphicalInterface 
	: public Core::SmartPointerObject
{
public:
	typedef std::map<unsigned int, Core::Widgets::PluginTab*> AttachedPluginsList;
	typedef std::pair<AttachedPluginsList::iterator, bool> AttachedPluginsResultType;

	coreDeclareSmartPointerClassMacro(
		wxMitkGraphicalInterface, 
		Core::SmartPointerObject);
	
	Core::Widgets::BaseMainWindow* GetMainWindow() const;
	Core::Runtime::StyleManager::Pointer GetStyleManager() const;
	FrontEndPluginManager::Pointer GetFrontEndPluginManager() const;
	Core::BaseWindowFactories::Pointer GetBaseWindowFactory( ) const;
	BaseWorkingAreaStorage::Pointer GetWorkingAreaStorage() const;
	void SetWorkingAreaStorage(BaseWorkingAreaStorage* val);

	void Start( );
	void Show(bool show = true);

	void ShowAboutScreen(void);
	void ShowLogFileBrowser(void);
	void ShowProfileManager(void);

	void LogMessage(const std::string& message);
	void LogException(Core::Exceptions::Exception& e);
	void ReportWarning(
		const std::string& message, 
		bool showAlsoDialog = true, 
		wxWindow* widget = NULL);
	void ReportError(
		const std::string& message, 
		bool showAlsoDialog = true, 
		wxWindow* widget = NULL);
	void ReportMessage(
		const std::string& message, 
		bool showAlsoDialog = true);
	void ClearErrorFormat(wxWindow* widget);
	
	bool IsRunning(void);

	//! Create a default plugin tab and set it as the active one
	void CreatePluginTab( const std::string &caption );

	/** Register a widget class that will be used to create an instance and
	add it to the new plugin tabs. Add an instance to all the already created 
	plugin tabs
	*/
	void RegisterFactory( 
		BaseWindowFactory::Pointer factory,
		WindowConfig config = WindowConfig( ) );

protected:
	wxMitkGraphicalInterface(void);
	~wxMitkGraphicalInterface(void);

	//! Create splash screen
	void CreateSpashScreen( );

	//! Create plugin manager
	void CreatePluginManager( );

	//! Create the Main Window 
	void CreateMainWindow( );

	//! Show main window
	void ShowMainWindow( );

	//! Hide splash screen
	void HideSplashScreen( );

	//! Things to do when all is initialized
	void OnGraphicalInterfaceInitialized( );

	//! Create the Style Manager
	void CreateStyleManager( );

private:
	//! A plugin manager class that loads widgets at runtime
	FrontEndPluginManager::Pointer m_FrontEndPluginManager;	
	//! Main Window of the graphical interface is type of (wxWidgets)wxTopLevelWindow
	Core::Widgets::BaseMainWindow* m_MainWindow;
	//! The startup SplashScreen
	Core::Widgets::SplashScreen* m_SplashScreen;		
	//! The StyleManager for the look&feel of the application
	Core::Runtime::StyleManager::Pointer m_StyleManager;	
	//! Factory for BaseWindow objects
	Core::BaseWindowFactories::Pointer m_BaseWindowFactory;
	//! True if application has initialized the graphical interface
	bool isRunning;	
	//!
	BaseWorkingAreaStorage::Pointer m_BaseWorkingAreaStorage;

	/** The following two lines allow to set the pointer to NULL when the 
	Splash screen is unloaded, so the Graphical interface can release the 
	SplashScreen at will. Issue due to wxSplashScreen behaviour :(
	*/
	friend class Core::Widgets::SplashScreen;
	void SplashScreenWasUnloaded(void);

	/** As long as we're using itk::SmartPointers, it is not possible to 
	define copy-constructors. They're listed here in order to remind it. 
	It is recommended to do the same on child classes
	*/
	coreDeclareNoCopyConstructors(wxMitkGraphicalInterface)

};

} // namespace Runtime
} // namespace Core

#endif
