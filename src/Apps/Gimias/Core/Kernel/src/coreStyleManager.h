/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreStyleManager_H
#define coreStyleManager_H

#include "gmKernelWin32Header.h"
#include "coreObject.h"
#include <wx/font.h>
#include <wx/colour.h>

class wxWindow;
class wxTextCtrl;
class wxStaticText;
class wxToolBar;
class wxControl;
class wxFrame;

namespace Core
{

/**
\brief Default border for all widgets added to the panel widget of a plugin
*/
const int DEFAULT_WIDGETS_BORDER = 5;


namespace Runtime
{
/** 
\brief The StyleManager class is to provide the style pattern for the 
look-and-feel of the application. It also provides
convenience access functions for retrieving resource files and bitmaps.
It is a low level manager driven by the GraphicalInterface class. 

\note You should access and get the instance for this class through the 
runtime high level manager object: wxMitkGraphicalInterface
\ingroup gmKernel
\sa wxMitkGraphicalInterface
\author Juan Antonio Moya
\date 04 Jan 2008
*/
class GMKERNEL_EXPORT StyleManager : public Core::SmartPointerObject
{
public:
	coreDeclareSmartPointerClassMacro(StyleManager, Core::SmartPointerObject);

	// Error and message formatting
	void SetWarningFormat(wxWindow* widget);
	void SetErrorFormat(wxWindow* widget);
	void SetCurrentFormat(wxWindow* widget);

	// Prebuilt style configurators
	void ConfigureSplashScreenLog(wxTextCtrl* label);
	void ConfigureNoPluginsLoadedLabel(wxStaticText* label);
	void ConfigureMainWindow(wxFrame* frame);
	void ConfigureCommandPanel(wxWindow* commandPannel);
	void ConfigureCommandPanelItem(wxWindow* item);
	void ConfigureWorkingArea(wxWindow* workingArea);
	void ConfigurePageContainer(wxWindow* container);
	void ConfigureLeftToolbar(wxWindow* container);
	void ConfigureButtonForLeftToolbar(wxControl* button);
	void ConfigureConsoleWindow(wxTextCtrl* text); 
	void ConfigureAcquireInputControl(wxWindow* control);
	void ConfigureProgressPanel(wxWindow* progressPannel);
	void ConfigureProcessingDialog(wxWindow* window);

	// Some style add-ons 
	wxColour GetBasicTextColor(void) const; 
	wxColour GetErrorTextColor(void) const;
	wxColour GetWarningTextColor(void) const;

protected:
	StyleManager(void);
	virtual ~StyleManager(void);

private:
	coreDeclareNoCopyConstructors(StyleManager);

};

}
}

#endif
