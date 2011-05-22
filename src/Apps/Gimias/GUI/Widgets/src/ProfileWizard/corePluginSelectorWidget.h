/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef corePluginSelectorWidget_H
#define corePluginSelectorWidget_H

#include "gmWidgetsWin32Header.h"
#include "coreObject.h"
#include "coreProfile.h"
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
class GMWIDGETS_EXPORT PluginSelectorWidget : 
	public wxPanel
	,public Core::Widgets::PreferencesPage
{
public:
	coreDefineBaseWindowFactory( PluginSelectorWidget )

	//!
	PluginSelectorWidget(
		wxWindow* parent, 
		int id  = wxID_ANY, 
		const wxPoint& pos=wxDefaultPosition, 
		const wxSize& size=wxDefaultSize, 
		long style=0);

	//!
	~PluginSelectorWidget(void);
	//!
	Core::Profile::Pointer GetSelectedProfile() const;

	//! Redefined
	void UpdateData( );

	//! Redefined
	void UpdateWidget( );

protected:
	void SetupProfileCheckboxes(void);

	//! Update tree state
	void UpdateTree(
		wxTreeItemId parentItem,
		Core::Profile::ProfileSet availableSet,
		Core::Profile::Pointer userProfile );

	//! Update profile state
	void UpdateProfile( wxTreeItemId item );

	//!
	void do_layout();

private:

	//!
	void OnCheckBoxItemToggle(wxTreeEvent& event);

	Core::Profile::Pointer m_SelectedProfile;
	blwxTreeCtrl* m_ChkTree;

	bool m_Modified;

	DECLARE_EVENT_TABLE();
};
}
}

#endif // corePluginSelectorWidget_H
