/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _svPlotManagerWidget_H
#define _svPlotManagerWidget_H

#include <wx/aui/aui.h>

// Core
#include "coreProcessingWidget.h"
#include "coreInputControl.h"
#include "coreDataEntityListBrowser.h"
#include "corePluginMacros.h"

class svPlotWidget;
class svSignalPlotAnnotationsWidget;
class svPlotWidgetToolbar;
class svPlotFilterAnnotationsWidget;
class svConfigurationWidget;

#define wxID_PlotManagerWidget wxID("wxID_PlotManagerWidget")

/**
\brief Layout for plot widgets

It has the input holder that will be used by all widgets

\ingroup SignalViewerPlugin
\author Xavi Planes
\date 4 Jan 2010
*/
class PLUGIN_EXPORT svPlotManagerWidget: public wxPanel, public Core::Widgets::ProcessingWidget{

public:
	coreDefineBaseWindowFactory( svPlotManagerWidget )

	//! Constructor
	svPlotManagerWidget(wxWindow* parent, 
				int id = wxID_PlotManagerWidget, 
				const wxPoint& pos=wxDefaultPosition, 
				const wxSize& size=wxDefaultSize, 
				long style=0);

	//! Destructor
	~svPlotManagerWidget();

	//!
	svPlotWidget* GetPlotWidget() const;

	//!
	void Show( wxWindowID id, bool show = true );

	//!
	void UpdateAuiManager( );

	//!
	void SetListBrowser( 
		Core::Widgets::DataEntityListBrowser* listBrowser );

	//!
	void SetRenderingTree(Core::RenderingTree::Pointer val);

	//!
	void SetTimeStepHolder( Core::IntHolderType::Pointer val );

	//!
	Core::BaseProcessor::Pointer GetProcessor( );

private:

	DECLARE_EVENT_TABLE();

	//!
	void OnClosePane( wxAuiManagerEvent& event );

private:

	//! Input Data
	Core::DataEntityHolder::Pointer m_InputDataHolder;

	//! Input data control
	Core::Widgets::InputControl *m_AcquireInputWidget;

	//!
	svPlotWidget* m_svPlotWidget;

	//!
	svSignalPlotAnnotationsWidget* m_svSignalPlotAnnotationsWidget;

	//!
	svPlotWidgetToolbar* m_svPlotWidgetToolbar;

	//!
	svPlotFilterAnnotationsWidget* m_svPlotFilterAnnotationsWidget;

	//!
	svConfigurationWidget* m_svConfigurationWidget;

	//!
	wxAuiManager m_wxAuiManager;
}; 


#endif // _svPlotManagerWidget_H
