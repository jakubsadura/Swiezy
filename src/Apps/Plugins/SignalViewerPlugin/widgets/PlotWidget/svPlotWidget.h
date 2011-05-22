/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _svPlotWidget_H
#define _svPlotWidget_H

#include <wx/aui/aui.h>

#include "itkCommand.h"

// Core
#include "coreCommonDataTypes.h"
#include "svPlotWidgetUI.h"
#include "coreDataEntityHolder.h"
#include "svSignalPlotMapper.h"
#include "coreInputControl.h"
#include "coreProcessingWidget.h"
#include "corePluginMacros.h"

#include "mathplot.h"

#include "mitkDataTreeNode.h"

class svSignalNotebookPage;
class svSignalPlotAnnotationsWidget;

// begin wxGlade: ::extracode
// end wxGlade


/**
\brief This widget displays all the data entities of type Core::SignalTypeId on the widget
\ingroup SignalViewerPlugin
\author 
\sa Core::DataEntity 
*/
class PLUGIN_EXPORT svPlotWidget: public svPlotWidgetUI , public Core::Widgets::ProcessingWidget{

public:

	//! Constructor
	svPlotWidget(wxWindow* parent, int id, 
				const wxPoint& pos=wxDefaultPosition, 
				const wxSize& size=wxDefaultSize, 
				long style=0);

	//! Destructor
	~svPlotWidget();

	
	/** Sets the time step holder. 
	Used to add a callback function whenever the time stepper changes
	*/
	void SetTimeStepHolder(Core::IntHolderType::Pointer timeStepHolder);

	//!
	bool Enable(bool enable = true );

	//!
	void SynchronizeAll(bool val);

	//!
	void SetInputDataHolder(Core::DataEntityHolder::Pointer val);

	//!
	void SetRenderingTree(Core::RenderingTree::Pointer val);

	//!
	wxNotebook* GetPlotNotebook() const;

	//!
	Core::BaseProcessor::Pointer GetProcessor( );

	//!
	svPlotParametersHolderType::Pointer GetParamHolder() const;
private:

	//!
	void RefreshSelectedInput( );

	//!
	void OnModifiedInputDataEntity();

	//!
	bool GetsvSignalNotebookPageID( Core::DataEntity::Pointer dataEntity, size_t &pageID );

	//! Add new tab to plot the data entity
	void Add( Core::DataEntity::Pointer dataEntity );

	//! Remove tab
	void Remove( Core::DataEntity::Pointer dataEntity );

	//!
	void OnModifiedRenderingTree( );

	//!
	void OnPageChanged(wxNotebookEvent &event);

	//!
	void OnModifiedParams( );

	DECLARE_EVENT_TABLE()

private:

	//! Input Data
	Core::DataEntityHolder::Pointer m_InputDataHolder;

	//! Time step holder
	Core::IntHolderType::Pointer m_TimeStepHolder;

	//! Map Core::DataEntity ID with observer of property node TAG
	std::map<mitk::DataTreeNode*, unsigned long> m_MapObserversNode;

	//! 
	boost::signals::connection m_RenderingTreeObserver;

	//!
	svPlotParametersHolderType::Pointer m_ParamHolder;
}; 


#endif // _svPlotWidget_H
