/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _corePreferencesDialog_H
#define _corePreferencesDialog_H

#include "gmWidgetsWin32Header.h"
#include "coreObject.h"
#include <wx/treebook.h>
#include "corePreferencesDialogUI.h"

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
class GMWIDGETS_EXPORT PreferencesDialog : public corePreferencesDialogUI
{
public:
	//!
	PreferencesDialog(
		wxWindow* parent, int id, const wxString& title, 
		const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, 
		long style=wxDEFAULT_DIALOG_STYLE);

	//!
	~PreferencesDialog(void);

	//! 
	int ShowModal();

protected:

	//!
	void UpdateTreebook( );

	//!
	void OnOK(wxCommandEvent &event);
	void OnCancel(wxCommandEvent &event);
	void OnApply(wxCommandEvent &event);

private:
	//!
	wxTreebook* m_PreferencesWindow;

};
}
}

#endif // _corePreferencesDialog_H
