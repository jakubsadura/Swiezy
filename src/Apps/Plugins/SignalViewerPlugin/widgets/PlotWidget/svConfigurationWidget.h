/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _svConfigurationWidget_H
#define _svConfigurationWidget_H

// Core
#include "coreCommonDataTypes.h"
#include "svConfigurationWidgetUI.h"
#include "coreDataEntityHolder.h"
#include "coreInputControl.h"

#include "blSignalAnnotation.h"
#include "blSignalCollective.h"

// GuiBridgeLib
#include "gblWxConnectorOfWidgetChangesToSlotFunction.h"

class svPlotWidget;

#define wxID_SIGNAL_VIEWER_CONFIGURATION wxID( "wxID_SIGNAL_VIEWER_CONFIGURATION" )

/**
\brief This widget displays all the data entities of type Core::SignalTypeId on the widget
\ingroup SignalViewerPlugin
\author 
\sa Core::DataEntity 
*/
class svConfigurationWidget: public svConfigurationWidgetUI{

public:

	//typedef std::vector<SignalPlotInfo*> InternalSignalListType;
	//! Constructor
	svConfigurationWidget(wxWindow* parent, int id = wxID_SIGNAL_VIEWER_CONFIGURATION, 
				const wxPoint& pos=wxDefaultPosition, 
				const wxSize& size=wxDefaultSize, 
				long style=0);

	//! Destructor
	~svConfigurationWidget();

	//!
	void SetPlotWidget(svPlotWidget* val);
private:
	void OnOffset(wxCommandEvent &event);
	void OnSize(wxCommandEvent &event);

	//!
	void UpdateWidget( );

	//!
	void UpdateData( );
private:
	//!
	svPlotWidget* m_svPlotWidget;
}; 


#endif // _svConfigurationWidget_H
