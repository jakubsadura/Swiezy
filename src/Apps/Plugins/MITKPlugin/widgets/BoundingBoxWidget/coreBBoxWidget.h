/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreBBoxWidget_H
#define _coreBBoxWidget_H

#include "corePluginMacros.h"
#include "CILabNamespaceMacros.h"
#include "coreBBoxSceneManager.h"
#include "coreSelectionToolWidget.h"
#include "coreBBoxProcessor.h"
#include <wx/wx.h>
#include "wx/tglbtn.h"

// GuiBridgeLib
#include "gblWxConnectorOfWidgetChangesToSlotFunction.h"

namespace Core{
namespace Widgets{
		class DataEntityListBrowser;
	
#define wxID_btnResetBox wxID( "wxID_btnResetBox" )
#define wxID_btnExportBox wxID( "wxID_btnExportBox" )

/** 
\brief This widget allows an image to be cropped.
\ingroup ImageToolsPlugin
\author Chiara Riccobene
*/
class PLUGIN_EXPORT BBoxWidget : 
	public wxPanel,
	public Core::Widgets::SelectionToolWidget
{
public:
	coreDefineBaseWindowFactory( BBoxWidget )

	//!
	BBoxWidget(wxWindow* parent, 
		int id = wxID_ANY, 
		const wxPoint& pos=wxDefaultPosition, 
		const wxSize& size=wxDefaultSize, 
		long style=0);

	//!
	bool Enable(bool enable = true );

	//!
	itk::ImageRegion<3> GetBox();
	//!
	virtual bool IsSelectionEnabled( );

	//!
	void StartInteractor( );

	//!
	void StopInteraction( );

	//!
	boost::any GetData( );

	//!
	Core::DataEntity::Pointer GetDataEntity( );

private:
	
	//!
	void OnInit();
	//!
	void UpdateWidget();
	//!
	void UpdateDataHolders();
	/** Returns true iff all data holders are connected and all 
	controls have valid contents.
	*/
	bool Validate();

	//!
	void OnModifiedInput();

	void do_layout();

	void UpdateData();

	//!
	void OnButtonResetBox(wxCommandEvent& event);

	//!
	void OnButtonExportBox(wxCommandEvent& event);

	//!
	Core::BaseProcessor::Pointer GetProcessor( );

private:
	wxStaticBox* m_ptCroppingWidgetStaticBox;
	wxButton* m_buttonResetBox;
	wxButton* m_buttonExportBox;
	wxTextCtrl* m_textCtrlZMax;
	wxStaticText* m_staticTextTo3;
	wxTextCtrl* m_textCtrlZMin;
	wxStaticText* m_staticTextX3;
	wxTextCtrl* m_textCtrlYMax;
	wxStaticText* m_staticTextTo2;
	wxTextCtrl* m_textCtrlYMin;
	wxStaticText* m_staticTextX2;
	wxTextCtrl* m_textCtrlXMax;
	wxStaticText* m_staticTextTo1;
	wxTextCtrl* m_textCtrlXMin;
	wxStaticText* m_staticTextX;

	 DECLARE_EVENT_TABLE();

private:

	//!
	Core::BBoxProcessor::Pointer m_Processor;

	//!
	Core::BBoxSceneManager::Pointer m_SceneManager;

	//!
	gbl::wx::ConnectorOfWidgetChangesToSlotFunction changeInWidgetObserver;

};

}
}

#endif //_coreBBoxWidget_H
