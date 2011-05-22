/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _svSignalNotebookPage_H
#define _svSignalNotebookPage_H

#include "svSignalPlotMapper.h"
#include "coreDataEntityListBrowser.h"

/**
Notebook page conatining all svSignalPlotWindow for one DataEntity

\ingroup SignalViewerPlugin
\author Xavi Planes
\date 03 Feb 2010 
*/
class svSignalNotebookPage : public wxScrolledWindow
{
public:

	svSignalNotebookPage( wxWindow *parent, wxWindowID winid = wxID_ANY);

	//!
	~svSignalNotebookPage( );

	//!
	void SetTimeStepHolder(Core::IntHolderType::Pointer timeStepHolder);

	//!
	void SetInputDataEntity(Core::DataEntity::Pointer dataEntity);

	//!
	bool GetDataEntityId( unsigned int& );

	//! Add a new mathPlotWindow to the plot widget
	void UpdateMapper( );

	//!
	int GetPageId( wxWindow *window );

	//!
	void SetDataEntityListBrowser(Core::Widgets::DataEntityListBrowser* val);

	//!
	svSignalPlotMapper::Pointer GetMapper() const;

protected:
	DECLARE_EVENT_TABLE( );

	/** Callback function for updating the display of the plot whenever the
	time step changes
	*/
	void OnTimeStepperChanged();

	//!
	void OnModifiedInputDataEntity( );

	//!
	void OnZoomInYAll( wxCommandEvent& event );

	//!
	void OnZoomOutYAll( wxCommandEvent& event );

	//!
	void OnFitAll( wxCommandEvent& event );

protected:

	//!
	svSignalPlotMapper::Pointer m_Mapper;

	//!
	Core::IntHolderType::Pointer m_timeStepHolder;

	//!
	boost::signals::connection m_timeStepHolderConnection;

	//! List browser
	Core::Widgets::DataEntityListBrowser* m_DataEntityListBrowser;
};

#endif //_svSignalNotebookPage_H
