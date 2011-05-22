/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef COREptCroppingWidget_H
#define COREptCroppingWidget_H

#include "corePluginMacros.h"
#include "CILabNamespaceMacros.h"
#include "coreBBoxWidget.h"
#include "coreCroppingProcessor.h"
#include <wx/wx.h>
#include "wx/tglbtn.h"

// GuiBridgeLib
#include "gblWxConnectorOfWidgetChangesToSlotFunction.h"

namespace Core{
namespace Widgets{
		class DataEntityListBrowser;
}
}
	
#define wxID_btnEnableInteraction wxID_HIGHEST + 2
#define wxID_btnCrop wxID_HIGHEST + 3
/** 
\brief This widget allows an image to be cropped.
\ingroup ImageToolsPlugin
\author Chiara Riccobene
*/
class PLUGIN_EXPORT ptCroppingWidget : 
	public wxPanel,
	public Core::Widgets::ProcessingWidget
{
public:
	coreDefineBaseWindowFactory( ptCroppingWidget )

	//!
	ptCroppingWidget(wxWindow* parent, 
		int id = wxID_ANY, 
		const wxPoint& pos=wxDefaultPosition, 
		const wxSize& size=wxDefaultSize, 
		long style=0);

	//!
	itk::ImageRegion<3> GetBox();


private:
	//!
	void OnButtonCrop(wxCommandEvent& event);

	//!
	void OnButtonShowBox( wxCommandEvent& event );
	
	//!
	void OnInit();

	void do_layout();

	//!
	Core::BaseProcessor::Pointer GetProcessor( );

private:
	wxStaticBox* m_ptCroppingWidgetStaticBox;
	wxButton* m_buttonCrop;
	wxToggleButton* m_toggleBtnEnable;

	 DECLARE_EVENT_TABLE();

private:

	//!
	Core::CroppingProcessor::Pointer m_Processor;

};

#endif //COREptCroppingWidget_H
