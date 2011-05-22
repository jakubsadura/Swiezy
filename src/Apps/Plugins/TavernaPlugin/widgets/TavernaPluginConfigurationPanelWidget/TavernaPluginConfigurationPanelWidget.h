/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _TavernaPluginConfigurationPanelWidget_H
#define _TavernaPluginConfigurationPanelWidget_H

#include "TavernaPluginConfigurationPanelWidgetUI.h"

#include "corePreferencesPage.h"
	
namespace TavernaPlugin{

/** 

\ingroup TavernaPlugin
\author Xavi Planes
\date 23 Dec 2010
*/
class ConfigurationPanelWidget : 
	public TavernaPluginConfigurationPanelWidgetUI,
	public Core::Widgets::PreferencesPage
{

// OPERATIONS
public:
	coreDefineBaseWindowFactory( ConfigurationPanelWidget );
	
	//!
	ConfigurationPanelWidget(
		wxWindow* parent, int id= wxID_ANY,
		const wxPoint&  pos = wxDefaultPosition, 
		const wxSize&  size = wxDefaultSize, 
		long style = 0);

	//!
	~ConfigurationPanelWidget( );

	//! Add button events to the bridge and call UpdateWidget()
	void OnInit(  );
	
private:
	//! Update GUI from working data
	void UpdateWidget();

	//! Update working data from GUI
	void UpdateData();

	//!
	void OnBrowse(wxCommandEvent &event);

// ATTRIBUTES
private:

};

} //namespace TavernaPlugin{

#endif //_TavernaPluginConfigurationPanelWidget_H
