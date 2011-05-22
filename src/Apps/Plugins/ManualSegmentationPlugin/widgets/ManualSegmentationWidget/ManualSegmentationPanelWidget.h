/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _ManualSegmentationPanelWidget_H
#define _ManualSegmentationPanelWidget_H

#include "ManualSegmentationPanelWidgetUI.h"
#include "msProcessor.h"

#include "coreROIInteractorHelper.h"
// CoreLib
#include "coreRenderingTree.h"
#include "coreFrontEndPlugin.h"
#include "coreCommonDataTypes.h"
#include "coreROIInteractorHelper.h"
#include "coreSelectionToolWidget.h"

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
\ingroup ManualSegmentationPlugin
\author Albert Sanchez
\date 06 05 2010
*/

#define wxID_ManualSegmentationWidget wxID("wxID_ManualSegmentationWidget")

class ManualSegmentationPanelWidget : 
	public ManualSegmentationPanelWidgetUI,
	public SelectionToolWidget
{

	// OPERATIONS
public:
	coreDefineBaseWindowFactory( ManualSegmentationPanelWidget )

	//!
	cilabDeclareExceptionMacro( Exception, std::exception );

		ManualSegmentationPanelWidget(
		wxWindow* parent, 
		int id = wxID_ManualSegmentationWidget,
		const wxPoint& pos=wxDefaultPosition, 
		const wxSize& size=wxDefaultSize, 
		long style=0);

	//!
	~ManualSegmentationPanelWidget( );

	//!
	void OnInit();

	//!
	bool Enable( bool enable /*= true */ );

	//!
	Core::BaseProcessor::Pointer GetProcessor( );

	//!
	void StartInteractor( );

	//!
	void StopInteraction( );

	//!
	bool IsSelectionEnabled( );


private:
	DECLARE_EVENT_TABLE()

	//! Checkbox "erase only current level" changed
	void OnChkColorChanged(wxCommandEvent& event);

	//! Returns a color from the table
	void getTableColor(int i, double col[3]);

	//! Sets the color that the next added ROI will have
	void setNextRoiColor();

	//!
	void OnModifiedSelectedReferenceImageDataEntityHolder();
	
	//!
	void OnModifiedSelectedInputROIDataEntityHolder();

	//!
	void OnNewSelectedInputROIDataEntityHolder( );

	//!
	void OnNewLabel(wxCommandEvent& event);

	//!
	void OnNewComboSelection(wxCommandEvent& event);

	//!
	void OnNewComboLUTSelection(wxCommandEvent& event);

	//!
	void OnEnableInteraction(wxCommandEvent& event);

	//!
	void OnAddBtn(wxCommandEvent& event);

	//!
	void InitComboLUT();

	//!
	void UpdateWidget();

	//!
	std::string RemoveSpaces(std::string str);

	//!
	void EnableGUI(bool bEnable);

	//! Update m_ROIInteractorHelper and Mask Image data entity ROI levels and names
	void UpdateData( );

	//!
	mitk::DataTreeNode::Pointer GetROINode( );

	//!
	vtkLookupTable* GetLUT( );

	//!
	void SetRenderingTree( RenderingTree::Pointer tree );

	// ATTRIBUTES
private:

	//! Processor
	ManualSegmentationProcessor::Pointer m_processor;

	//! ROI interactor helper
	Core::ROIInteractorHelper::Pointer m_ROIInteractorHelper;

	//! Rename levels
	std::map<int,std::string> m_MapLevelsToLabel;

	//Core::DataEntity::Pointer m_dataEntity;
	//Core::DataEntityHolder::Pointer m_dataEntityHolder;
	bool m_bEraseOnlySel;
	int m_Selection;
	int m_countLabels;

	boost::signals::connection m_ConnOnAdd;
	boost::signals::connection m_ConnOnRem;

	Core::BaseFilterOutputPort::Pointer m_OutputPort;

	//!
	bool m_LevelWindowInteractorState;
};

} //namespace Widgets
} //namespace Core

#endif //_ManualSegmentationPanelWidget_H
