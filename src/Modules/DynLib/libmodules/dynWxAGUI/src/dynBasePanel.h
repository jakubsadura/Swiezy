/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef DYNBASEPANEL_H
#define DYNBASEPANEL_H

#include "dynBasePanelUI.h"
#include "dynWxGUIUpdater.h"
#include "wx/collpane.h"
#include "wxID.h"

#define wxID_RUN_AS_COMMAND_LINE wxID( "wxID_RUN_AS_COMMAND_LINE" )
#define wxID_SAVE_AS_SCRIPT wxID( "wxID_SAVE_AS_SCRIPT" )
#define wxID_RESET_BUTTON wxID( "wxID_RESET_BUTTON" )
#define wxID_BROWSE_BUTTON wxID("wxID_BROWSE_BUTTON")


/**
BasePanel for automated GUI with automatic update when pressing Apply

\author Xavi Planes
\date 15 July 2010
\ingroup DynLib
*/
class DYNWXAGUILIB_EXPORT dynBasePanel: public dynBasePanelUI {
public:
	//!
    dynBasePanel(wxWindow* parent, int id, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=0);

	//!
	ModuleDescription* GetModule() const;
	void SetModule(ModuleDescription* val);

	//!
	dynWxGUIUpdater::Pointer GetUpdater() const;
	void SetUpdater(dynWxGUIUpdater::Pointer val);

private:
	//!
	virtual void OnApply(wxCommandEvent &event);

	//!
	virtual void OnResetBtn(wxCommandEvent &event);

	//!
	virtual void OnCollapsiblePanel( wxCollapsiblePaneEvent &event );

	//!
	virtual void OnBrowseBtn(wxCommandEvent &event);

	DECLARE_EVENT_TABLE( );

protected:

	//!
	ModuleDescription* m_DefaultModule;
	//!
	ModuleDescription* m_Module;
	//!
	dynWxGUIUpdater::Pointer m_Updater;
};


#endif // DYNBASEPANEL_H
