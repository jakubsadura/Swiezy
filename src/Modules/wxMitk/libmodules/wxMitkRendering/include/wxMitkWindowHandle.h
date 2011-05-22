/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _wxMitkWindowHandle_H
#define _wxMitkWindowHandle_H


// For compilers that don't support precompilation, include "wx/wx.h"
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
       #include <wx/wx.h>
#endif


// ---------------------------------------------------------------------------------------------
/**
wxWidgets is not providing a method for retrieving the window id, needed for the rendering context.
This function returns it, using platform-dependent calls
*/


//Keep this for compatibilty reason, this was introduced in wxGTK 2.4.0
#if (!wxCHECK_VERSION(2, 4, 0))
wxWindow* wxGetTopLevelParent(wxWindow *win)
{
	while ( win && !win->IsTopLevel() )
		win = win->GetParent();
	return win;
}
#endif

// To access objc calls on cocoa
#ifdef __WXCOCOA__
#ifdef VTK_USE_COCOA
#import <Cocoa/Cocoa.h>
// This trick is no longer need in VTK CVS, should get rid of that:
#define id Id
#else
#error Build mismatch you need both wxWidgets and VTK to be configure against Cocoa to work
#endif //VTK_USE_COCOA
#endif //__WXCOCOA__

#ifdef __WXGTK__
#    include <gdk/gdkx.h> // GDK_WINDOW_XWINDOW is found here in wxWidgets 2.8.0
#    include "gdk/gdkprivate.h"
#if wxCHECK_VERSION(2, 8, 0)
#ifdef __WXGTK20__
#include <wx/gtk/win_gtk.h>
#else
#include <wx/gtk1/win_gtk.h>
#endif
#else
#include <wx/gtk/win_gtk.h>
#endif
#define GetXWindow(wxwin) (wxwin)->m_wxwindow ? \
	GDK_WINDOW_XWINDOW(GTK_PIZZA((wxwin)->m_wxwindow)->bin_window) : \
	GDK_WINDOW_XWINDOW((wxwin)->m_widget->window)
#endif

#ifdef __WXX11__
#include "wx/x11/privx.h"
#define GetXWindow(wxwin)   ((Window)(wxwin)->GetHandle())
#endif


//---------------------------------------------------------------------------
// NOTE on implementation:
// Bad luck you ended up in the only tricky place of this code.
// A few note, wxWidgets still refuse to provide such convenient method
// so I have to maintain it myself, eventhough this is completely integrated
// in wxPython...
// Anyway if this happen to break for you then compare to a recent version of wxPython
// and look for the function long wxPyGetWinHandle(wxWindow* win)
// in wxPython/src/helpers.cpp
inline void* GetHandleHack( wxWindow *window )
{
	//helper function to hide the MSW vs GTK stuff
	long handle_tmp = 0;

	// __WXMSW__ is for Win32
	//__WXMAC__ stands for using Carbon C-headers, using either the CarbonLib/CFM or the native Mach-O builds (which then also use the latest features available)
	// __WXGTK__ is for both gtk 1.2.x and gtk 2.x
#if defined(__WXMSW__) || defined(__WXMAC__)
	handle_tmp = (long)window->GetHandle();
#endif //__WXMSW__

	//__WXCOCOA__ stands for using the objective-c Cocoa API
#ifdef __WXCOCOA__
	// Here is how to find the NSWindow
	wxTopLevelWindow* toplevel = dynamic_cast<wxTopLevelWindow*>(
		wxGetTopLevelParent( window ) );
	if (toplevel != NULL )    
	{
		handle_tmp = (long)toplevel->GetNSWindow();
	}
	// The NSView will be deducted from 
	// [(NSWindow*)Handle contentView]
	// if only I knew how to write that in c++
#endif //__WXCOCOA__

	// Find and return the actual X-Window.
#if defined(__WXGTK__) || defined(__WXX11__)
	return reinterpret_cast<void *>(GetXWindow(window));
#endif

	//#ifdef __WXMOTIF__
	//    handle_tmp = (long)this->GetXWindow();
	//#endif

	return reinterpret_cast<void *> (handle_tmp);
}

#endif // _wxMitkWindowHandle_H
