/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _svSignalPlotAnnotationsWidget_H
#define _svSignalPlotAnnotationsWidget_H

#include "itkCommand.h"

// Core
#include "coreCommonDataTypes.h"
#include "svSignalPlotAnnotationsWidgetUI.h"
#include "coreDataEntityHolder.h"
#include "coreInputControl.h"

#include "blSignalAnnotation.h"
#include "blSignalCollective.h"

// GuiBridgeLib
#include "gblWxConnectorOfWidgetChangesToSlotFunction.h"


struct svSignalsAnnotation 
{
	//! Annotation pointer
	blSignalAnnotation::Pointer m_annotation;
	//! Signal index
	int m_signalId;
	//! Annotation index
	int m_annotationId;
};
#define wxID_ANNOTATIONS wxID( "wxID_ANNOTATIONS" )

/**
\brief This widget displays all the data entities of type Core::SignalTypeId on the widget
\ingroup SignalViewerPlugin
\author 
\sa Core::DataEntity 
*/
class svSignalPlotAnnotationsWidget: public svSignalPlotAnnotationsWidgetUI{

public:

	//typedef std::vector<SignalPlotInfo*> InternalSignalListType;
	//! Constructor
	svSignalPlotAnnotationsWidget(wxWindow* parent, int id = wxID_ANNOTATIONS, 
				const wxPoint& pos=wxDefaultPosition, 
				const wxSize& size=wxDefaultSize, 
				long style=0);

	//! Destructor
	~svSignalPlotAnnotationsWidget();
	
	//!
	void SetInputDataHolder(Core::DataEntityHolder::Pointer input);

	//!
	void SetDataEntityListBrowser(Core::Widgets::DataEntityListBrowser* val);

private:
	//!
	void OnSelectedAnnotation(wxCommandEvent &event);
	//!
	void OnComboSelected(wxCommandEvent &event);
	//!
	void OnRemoveButton(wxCommandEvent &event);
	//!
	void OnDuration(wxCommandEvent &event);
	//!
	void OnInputDataChanged();
	//!
	void UpdateWidget();
	//!
	void UpdateData();
	//!
	void CreateComboBoxList();
	//!
	void UpdateAnnotationsList();

private:
	//! 
	std::vector<svSignalsAnnotation> m_listAnnotation;
	//! Input Data
	Core::DataEntityHolder::Pointer m_InputDataHolder;
	//! Output Data
	Core::DataEntityHolder::Pointer m_OutputDataHolder;
	//! List browser
	Core::Widgets::DataEntityListBrowser* m_DataEntityListBrowser;
}; 


#endif // _svSignalPlotAnnotationsWidget_H
