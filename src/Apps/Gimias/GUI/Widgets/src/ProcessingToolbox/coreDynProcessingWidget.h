/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _DynProcessingWidget_H
#define _DynProcessingWidget_H

#include "coreProcessingWidget.h"
#include "coreDynProcessor.h"

class dynBasePanel;

namespace Core{
namespace Widgets{

/**
\brief Dynamic processing widget using Automated GUI
\ingroup gmWidgets
\author Xavi Planes
\date 15 July 2010
*/
class GMWIDGETS_EXPORT DynProcessingWidget: 
	public wxPanel, 
	public ProcessingWidget {
public:

	//!
	DynProcessingWidget(wxWindow* parent, int id, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=0);
	 
	virtual ~DynProcessingWidget( );

	//!	
	virtual void UpdateData();

	//!
	virtual void UpdateWidget();

	//!
	Core::BaseProcessor::Pointer GetProcessor( );

	//!
	void SetModule( ModuleDescription* module );

	//! Build automated GUI
	void OnInit();

protected:
	DECLARE_EVENT_TABLE();

	//!
	virtual void OnApply(wxCommandEvent &event);

	//!
	virtual void OnSaveScript(wxCommandEvent &event);

	//!
	void OnRunAsCommandLine(wxCommandEvent& event);

	//!
	virtual void OnBtnRegion(wxCommandEvent &event);

	//!
	virtual void OnBtnPoint(wxCommandEvent &event);
	
	//!
	void AddInteractionButtons( );

	//!
	void StartInteraction( wxObject* win, const std::string &name );

private:

	//!
	DynProcessor::Pointer m_Processor;

	//!
	dynBasePanel* m_DynPanel;

	//!
	wxTextCtrl* m_CurrentInteractionControl;
};

}
}

#endif // DynProcessingWidget_H
