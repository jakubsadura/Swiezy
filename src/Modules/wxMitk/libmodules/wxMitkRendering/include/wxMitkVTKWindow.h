/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#ifndef wxMitkVTKWindow_H
#define wxMitkVTKWindow_H

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#  pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/timer.h>
#include <wx/dcclient.h>

#include "wxMitkRenderingWin32Header.h"
#include <wx/window.h>
#include <string>
#include "vtkRenderWindow.h"
#include "vtkMitkRenderProp.h"
#include "wxVTKRenderWindowInteractor.h"

// Apparently since wxGTK 2.8.0 one can finally use wxWindow (just as in any
// other port):
// MM: tested on 2008/04/08: experienced some heavy flickering with wx-widget 2.6.0
// using a wxWindow instead of wxGLCanvas fixed the symptoms
//#if (!wxCHECK_VERSION(2, 6, 0))
#if (!wxCHECK_VERSION(2, 8, 0))
#define USE_WXGLCANVAS
#endif

#if defined(__WXGTK__) && defined(USE_WXGLCANVAS)
#  if wxUSE_GLCANVAS
#    define Region xNameConflictWorkaroundRegion
#    include <wx/glcanvas.h>
#    undef Region
#  else
#    error "problem of wxGLCanvas, you need to build wxWidgets with opengl"
#  endif //wxUSE_GLCANVAS
#endif //__WXGTK__

// Motif version (renamed into wxX11 for wxWindow 2.4 and newer)
#if defined(__WXMOTIF__) 
# error This GUI is not supported by wxVTKRenderWindowInteractor for now
#endif

// Forward declarations
class wxPaintEvent;
class wxMouseEvent;
class wxTimerEvent;
class wxKeyEvent;
class wxSizeEvent;
class wxKeyEvent;
class vtkRenderWindow;
class vtkMitkRenderProp;
class wxVTKRenderWindowInteractor;

namespace mitk
{

/*
\brief Display a vtkRenderWindow in a WxWidget.
\ingroup wxMitkRendering
\author Xavi Planes
\date 12 06 08

This is the porting of QVTKWidget class for Qt. Only a part of it has been 
ported. 

*/
#if defined(__WXGTK__) && defined(USE_WXGLCANVAS)
	class WXMITKRENDERING_EXPORT wxMitkVTKWindow : public wxGLCanvas
#else
	class WXMITKRENDERING_EXPORT wxMitkVTKWindow : public wxWindow
#endif //__WXGTK__
{

public:

	wxMitkVTKWindow(
		wxWindow* parent, 
		wxWindowID id, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = 0, 
		const wxString& name = wxT("unnamed renderwindow"));
	virtual ~wxMitkVTKWindow(void);
	
	//! 
	vtkRenderWindow* GetRenderWindow();
	//! 
	void SetRenderWindow(vtkRenderWindow* renderWindow);
	//! 
	wxVTKRenderWindowInteractor* GetInteractor(void);

	// event handlers
	void OnPaint(wxPaintEvent &event);
	void OnEraseBackground (wxEraseEvent& event);
	void OnMotion(wxMouseEvent &event);

	void OnButtonDown(wxMouseEvent &event);
	void OnButtonUp(wxMouseEvent &event);
	void OnEnter(wxMouseEvent &event);
	void OnLeave(wxMouseEvent &event);
	void OnKeyDown(wxKeyEvent &event);
	void OnKeyUp(wxKeyEvent &event);
	void OnChar(wxKeyEvent &event);
	void OnSize(wxSizeEvent &event);
	void OnMouseWheel(wxMouseEvent& event);

	void OnRerender(wxCommandEvent& evt);

	//!
	void Render();

	//!
	void SetRenderWhenDisabled(int newValue);

	// Description:
	// Prescribe that the window be created in a stereo-capable mode. This
	// method must be called before the window is realized. Default if off.
	int GetStereo();
	void SetStereo(int capable);

	// Description:
	// As CaptureMouse could be a problem sometimes on a window box
	// This method allow to set or not the CaptureMouse.
	// This method actually will works only if WX_USE_X_CAPTURE was set to 1
	void SetUseCaptureMouse(int val);

	void Enable();
	bool Enable(bool enable);

#ifdef __WXMAC__
	void UpdateSize(int x, int y);
#endif //__WXMAC__

protected:
	DECLARE_EVENT_TABLE();
	DECLARE_CLASS( wxMitkVTKWindow )

private:
	void* m_Handle;
	int m_UseCaptureMouse;
	int m_Stereo;
	bool m_Created;
	int m_RenderWhenDisabled;
	int m_RenderAllowed;
	int m_ActiveButton;

	// the vtk render window
	vtkRenderWindow* m_RenWin;


#if defined (__WXX11__) || defined (__WXGTK__) //|| defined(__WXMAC__)
private:
    //! Create a temporary window to let VTK/OpenGL draw in while the real window isn't ready yet
    void * CreateTemporaryWindow();
#endif

};
 
} // namespace mitk


#endif // wxMitkVTKWindow_H
