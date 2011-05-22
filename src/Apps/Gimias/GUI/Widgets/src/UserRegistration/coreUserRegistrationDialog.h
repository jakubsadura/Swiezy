/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreUserRegistrationDialog_H
#define _coreUserRegistrationDialog_H

#include "coreUserRegistrationDialogUI.h"
#include "wx/socket.h"

namespace Core
{

namespace Widgets
{

/** 
User registration form
\ingroup gmWidgets
\author Xavi Planes
\date Nov 2010
*/
class UserRegistrationDialog: public coreUserRegistrationDialogUI {
public:
    // begin wxGlade: coreUserRegistrationDialog::ids
    // end wxGlade

    UserRegistrationDialog(wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_DIALOG_STYLE);

	~UserRegistrationDialog( );

private:
	void OnRegister(wxCommandEvent &event);
	void OnRemindMe(wxCommandEvent &event);
	void OnDoNotAsk(wxCommandEvent &event);
	void OnSocketEvent(wxSocketEvent& event);

	DECLARE_EVENT_TABLE();

protected:
	wxSocketClient *m_sock;
};

} // namespace Core
} // namespace Widgets

#endif // COREUSERREGISTRATIONDIALOG_H
