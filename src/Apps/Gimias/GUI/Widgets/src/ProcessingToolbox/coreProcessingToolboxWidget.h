/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreProcessingToolboxWidget_H
#define coreProcessingToolboxWidget_H

#include "coreObject.h"
#include "coreDataEntityHolder.h"
#include "coreDataEntityListBrowser.h"
#include "coreRenderingTree.h"
#include "coreProcessingWidget.h"
#include "coreUserHelperWidget.h"
#include "coreBaseWindow.h"
#include "coreBaseToolboxWidget.h"
#include <wx/panel.h>

// Forward declarations
class wxButton;
class wxStaticText;

namespace Core
{
	class DataEntity;
	class DataEntityList;
}

namespace Core{
namespace Widgets{

#define wxID_ProcessingToolboxWidget wxID("wxID_ProcessingToolboxWidget")

/** 
\brief class that display the parameters for the processors of the tools menu
\ingroup gmWidgets
\author Chiara Riccobene
\date 05 Nov 2009
*/
class GMWIDGETS_EXPORT ProcessingToolboxWidget : public BaseToolboxWidget
{
public:
	//!
	coreDefineBaseWindowFactory( ProcessingToolboxWidget );

	coreClassNameMacro(ProcessingToolboxWidget);

	ProcessingToolboxWidget(wxWindow* parent, 
						wxWindowID id = wxID_ProcessingToolboxWidget, 
						const wxPoint& pos = wxDefaultPosition, 
						const wxSize& size = wxDefaultSize, 
						long style = wxBORDER_NONE | wxFULL_REPAINT_ON_RESIZE, 
						const wxString& name = wxT("Tools Processor "));

	//!
	void SetToolWindow( wxWindow* win);

	//!
	ProcessingWidget* GetProcessorWidget();

	//!
	wxWindow* GetToolWindow();

private:
	virtual ~ProcessingToolboxWidget(void);	
	//!
	void set_properties();
	void do_layout();

	//!
	void SetToolByFactoryName( const std::string &factoryName );

	//!
	virtual std::string GetNameMenuItem( );

	//!
	virtual WIDGET_TYPE GetWidgetState( );

	DECLARE_EVENT_TABLE( );
private:
	//!
	ProcessingWidget* m_processorWidget;
};

} // Widgets
} // Core

#endif // coreProcessingToolboxWidget_H
