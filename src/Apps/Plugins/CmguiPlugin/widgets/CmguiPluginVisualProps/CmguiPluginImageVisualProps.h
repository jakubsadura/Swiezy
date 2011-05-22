/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _CmguiPluginImageVisualProps_H
#define _CmguiPluginImageVisualProps_H

#include "coreVisualPropertiesBase.h"
#include "CmguiPluginImageVisualPropsUI.h"

namespace CmguiPlugin {

class Cmgui;
/**
\brief 
\ingroup CMGUIPlugin
\author Xavi Planes
\date Nov 2010
*/
class ImageVisualProps : 
	public CmguiPluginImageVisualPropsUI, 
	public Core::Widgets::VisualPropertiesBase
{

public:
	//!
    coreDefineBaseWindowFactory1param(ImageVisualProps, Cmgui*)

    //!
    ImageVisualProps(Cmgui* cmgui, wxWindow* parent, int id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0);

    //!
    ~ImageVisualProps();

	//! Redefined
	virtual bool IsValidData( Core::DataEntity::Pointer dataEntity );
	//! Redefined
	virtual void SetInputHolder( Core::DataEntityHolder::Pointer inputHolder );


private:
    DECLARE_EVENT_TABLE()

	//!
	void OnInputHolderChangeSubject( );

	//!
	void UpdateWidget( );

	//!
	void UpdateVisibility( );

	//!
	void OnTreeSelChanged(wxTreeEvent &event);
	void OnShowAll(wxCommandEvent &event);
	void OnHideAll(wxCommandEvent &event);
	void OnShowSelected(wxCommandEvent &event);
	void OnItemStateClick( wxTreeEvent& event );

	//!
	void ShowSlice( wxTreeItemId item, bool show );

private:
	//!
	Core::DataEntityHolder::Pointer m_InputHolder;
	//!
	wxTreeItemId m_LastSelectedItem;
};


} // namespace CmguiPlugin

#endif //_CmguiPluginImageVisualProps_H

