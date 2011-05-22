/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef WXCXXTESTFRAME_H
#define WXCXXTESTFRAME_H

#include <wx/wx.h>
#include "wxID.h"
#include "TpExtLibWxWidgetsWin32Header.h"
#include "boost/function.hpp"

/**
This frame can be used to write CxxTest tests that need wxWidgets widgets.
For an example of using this frame, see ShapeComparisonWindowTest.h.

\ingroup TpExtLibWxWidgets
\author Maarten Nieber
\date 29 may 2008
*/

class TPEXTLIBWXWIDGETS_EXPORT wxCxxTestFrame : public wxFrame
{
public:
	typedef wxCxxTestFrame Self;
	typedef boost::function<void (void)> CallbackType;

	//! Create a frame with an area for client widgets (see GetLayout).
	wxCxxTestFrame(
		wxWindow* parent, 
		wxWindowID id, 
		const wxString& title,             
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize
	);
	//! Start the main loop of wxWidgets. This frame has a Close button which exits the mainloop.
	//! This allows the testrunner to go to the next test.
	void StartMainLoop();
	//! Get the layout for client widgets. Use this layout to add your widgets to the test frame.
	wxSizer* GetLayout() const;
	//! Respond to the close button (see StartMainLoop).
	void OnButtonClose(wxEvent& event);
	//! This function is call when the user presses the Send Debug Event button
	void SetCallback(CallbackType callback);
	//! Respond to the Send Debug Event button.
	void OnButtonDebugEvent(wxEvent& event);

private:
	wxButton* buttonClose;
	wxButton* buttonDebugEvent;
	wxSizer* layoutOfClientWidgets;
	wxBoxSizer* layout;
	CallbackType m_Callback;
};

#endif //WXCXXTESTFRAME_H
