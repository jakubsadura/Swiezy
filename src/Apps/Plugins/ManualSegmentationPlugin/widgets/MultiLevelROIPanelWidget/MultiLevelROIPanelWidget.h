/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _MultiLevelROIPanelWidget_H
#define _MultiLevelROIPanelWidget_H

#include "MultiLevelROIPanelWidgetUI.h"
#include "mlrProcessor.h"

// CoreLib
#include "coreRenderingTree.h"
#include "coreFrontEndPlugin.h"
#include "coreCommonDataTypes.h"
#include "coreBaseWindow.h"

// GuiBridgeLib
#include "gblWxConnectorOfWidgetChangesToSlotFunction.h"

#include <vector>
#include <map>

#include "blMITKUtils.h"


namespace Core{ namespace Widgets {
	class AcquireDataEntityInputControl;
	class UserHelper;
	class DataEntityListBrowser;
}}

namespace Core{
	namespace Widgets {

/**
\ingroup MultiLevelROIPlugin
\author Albert Sanchez
\date 06 05 2010
*/

#define wxID_MultiLevelROIWidget wxID("wxID_MultiLevelROIWidget")


class MultiLevelROIPanelWidget : 
	public MultiLevelROIPanelWidgetUI,
	public Core::Widgets::ProcessingWidget
{

	// OPERATIONS
public:
	coreDefineBaseWindowFactory( MultiLevelROIPanelWidget )
	//!
	MultiLevelROIPanelWidget(
	wxWindow* parent, 
	int id = wxID_MultiLevelROIWidget,
	const wxPoint& pos=wxDefaultPosition, 
	const wxSize& size=wxDefaultSize, 
	long style=0);

	//!
	~MultiLevelROIPanelWidget( );

	bool Enable( bool enable /*= true */ );

	Core::BaseProcessor::Pointer GetProcessor();
	void OnInit();


private:
	DECLARE_EVENT_TABLE()

	//!
	void OnModifiedOutputPropagatedROI();

	//!
	void OnModifiedSignal();

	//!
	void OnPropagateBtn(wxCommandEvent& event);

	//!
	void OnCreateSignalBtn(wxCommandEvent& event);

	//!
	void OnUpdateMeasuresBtn(wxCommandEvent& event);

	//!
	void UpdateMeasures();

	//!
	void CalculateMeasures();

	//!
	void OnModifiedSelectedDataEntityHolder();

	//!
	void OnSelectLevel(wxCommandEvent &event);

	//!
	void EnableGUI(bool enable);


	// ATTRIBUTES
private:

	//! Processor
	MultiLevelROIProcessor::Pointer m_processor;

	int m_selectedLevel;

};
} //namespace Widgets
} //namespace Core

#endif //_MultiLevelROIPanelWidget_H
