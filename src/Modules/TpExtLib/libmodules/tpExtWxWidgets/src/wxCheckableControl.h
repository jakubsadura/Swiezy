/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _wxCheckableControl_H
#define _wxCheckableControl_H

#include "TpExtLibWxWidgetsWin32Header.h"

/** 
\brief wxPanel with a checkbox at the left and a control at the right.
When the checkbox is checked, the control is enabled, otherwise, it is 
disabled.

\ingroup TpExtLibWxWidgets
\author Xavi Planes
\date 04 August 2010
*/
class TPEXTLIBWXWIDGETS_EXPORT wxCheckableControl : public wxPanel
{
public:

    wxCheckableControl(wxWindow *parent, 
				const wxWindowID id = wxID_ANY,
				const wxPoint& pos = wxDefaultPosition, 
				const wxSize& size = wxDefaultSize,
				long style = wxTAB_TRAVERSAL | wxNO_BORDER);
    virtual ~wxCheckableControl();

	//!
	void SetValue(bool val);

	//!
	bool GetValue( ) const;

	//!
	virtual void SetLabel(const wxString& label);

	//!
	void SetCheckBox( wxCheckBox* val, wxSizerFlags& flags );
	wxCheckBox* GetCheckBox( );

	//!
	void AddWindow( wxWindow* win, wxSizerFlags& flags );

	//!
	void OnCheckBox(wxCommandEvent& event);

private:
	//!
	void UpdateWidget( );

private:

	//!
	wxCheckBox* m_CheckBox;

	//!
	wxSizer* m_Sizer;

    DECLARE_EVENT_TABLE()
};

#endif // _wxCheckableControl_H
