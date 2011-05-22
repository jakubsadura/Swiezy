/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _svPlotFilterAnnotationsWidget_H
#define _svPlotFilterAnnotationsWidget_H

#include "itkCommand.h"

// Core
#include "coreCommonDataTypes.h"
#include "svPlotFilterAnnotationsWidgetUI.h"

#define wxID_FILTER_ANNOTATIONS wxID( "wxID_FILTER_ANNOTATIONS" )

/**
\brief Allows the user to filter annotations
\ingroup SignalViewerPlugin
\author 
\sa Core::DataEntity 
*/
class svPlotFilterAnnotationsWidget: public svPlotFilterAnnotationsWidgetUI{

public:
	//! Constructor
	svPlotFilterAnnotationsWidget(wxWindow* parent, int id = wxID_FILTER_ANNOTATIONS, 
				const wxPoint& pos=wxDefaultPosition, 
				const wxSize& size=wxDefaultSize, 
				long style=0);

	//! Destructor
	~svPlotFilterAnnotationsWidget();

private:

	void InitTree( );
private:

}; 


#endif // _svPlotFilterAnnotationsWidget_H
