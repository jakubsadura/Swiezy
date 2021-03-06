/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

// -*- C++ -*- generated by wxGlade 0.6.3 on Tue Dec 23 09:06:03 2008

#include <wx/wx.h>
#include <wx/image.h>

#ifndef wxMitkMemoryUsageIndicator_H
#define wxMitkMemoryUsageIndicator_H

// begin wxGlade: ::dependencies
// end wxGlade

// begin wxGlade: ::extracode

// end wxGlade

#include "TpExtLibWxWidgetsWin32Header.h"
#include <wx/panel.h>


/**
\brief Memory usage indicator
\ingroup TpExtLibWxWidgets
\author Xavi Planes
\date 22 Dec 2008
*/
class TPEXTLIBWXWIDGETS_EXPORT wxMemoryUsageIndicator : public wxPanel
{
public:
	// begin wxGlade: wxMemoryUsageIndicator::ids
	// end wxGlade
	
public:
	wxMemoryUsageIndicator(
		wxWindow* parent, 
		wxWindowID id, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = 0, 
		const wxString& name = wxT("wxMemoryUsageIndicator"));

private:
	// begin wxGlade: wxMemoryUsageIndicator::methods
	void set_properties();
	void do_layout();
	// end wxGlade

	//! Update the controls
	void UpdateMemoryUsage( );

	//! On timer event handler
	void OnTimer(wxTimerEvent& event);

	//!
	std::string FormatMemorySize( size_t size );

	//!
	std::string FormatPercentage( double val );

	//!
	std::string GetMemoryDescription( size_t processSize, float percentage );

	DECLARE_EVENT_TABLE();

protected:
	// begin wxGlade: wxMemoryUsageIndicator::attributes
	wxStaticText* m_label;
	wxStaticBitmap* m_bitmap;
	// end wxGlade

	wxBitmap m_GreenLed;
	wxBitmap m_RedLed;
	wxBitmap m_OrangeLed;

	wxTimer m_Timer;

	double m_maxMemory;

}; // wxGlade: end class


#endif // wxMemoryUsageIndicator
