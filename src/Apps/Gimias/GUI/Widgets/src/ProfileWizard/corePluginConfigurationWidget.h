/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef corePluginConfigurationWidget_H
#define corePluginConfigurationWidget_H

#include "gmWidgetsWin32Header.h"
#include "coreObject.h"
#include "coreProfile.h"
#include <wx/wizard.h>
#include "corePluginConfigurationUI.h"
#include "corePreferencesPage.h"

class wxWizardEvent;

namespace Core
{
namespace Widgets
{
/** 
\brief Select the plugins to load.

\ingroup gmWidgets
\author Xavi Planes
\date Nov 2010
*/
class GMWIDGETS_EXPORT PluginConfigurationWidget 
	: public corePluginConfigurationUI
	,public Core::Widgets::PreferencesPage
{
public:
	coreDefineBaseWindowFactory( PluginConfigurationWidget )

	//!
	PluginConfigurationWidget(
		wxWindow* parent, 
		int id  = wxID_ANY, 
		const wxPoint& pos=wxDefaultPosition, 
		const wxSize& size=wxDefaultSize, 
		long style=0);

	//!
	~PluginConfigurationWidget(void);

protected:
	void OnAddPluginFolder(wxCommandEvent &event);
	void OnRemovePluginFolder(wxCommandEvent &event);
	void OnReload(wxCommandEvent &event);
	void UpdateWidget( );
	void UpdateData( );

private:
};
}
}

#endif // corePluginConfigurationWidget_H
