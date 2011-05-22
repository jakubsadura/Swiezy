/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef wxMitkRenderWindow_H
#define wxMitkRenderWindow_H

#include "wxMitkRenderingWin32Header.h"
#include <wx/window.h>
//#ifndef WX_PRECOMP
//#include <wx/wx.h>
//#endif

#include <mitkVtkPropRenderer.h>

#include <string>

#include "vtkPolyData.h"
#include "vtkSmartPointer.h"

#include "wxMitkVTKWindow.h"
#include "wxMitkApplicationCursorImplementation.h"

#include "blMitkScalarBar.h"

// Forward declarations
class wxKeyEvent;
class vtkRenderWindow;
class vtkMitkRenderProp;
class vtkLabeledDataMapper;
class vtkScalarBarActor;

namespace mitk
{
class KeyEvent;
class SliceNavigationController;
class CameraRotationController;
class BaseController;

/*
\brief wxWidgets specific implementation of mitk::RenderWindow.
\ingroup wxMitkRendering
\author Juan Antonio Moya
\date 11 Sep 2007

The RenderWindow is responsible for holding the VtkRenderWindow and link 
it to the openGL rendering context held by the GUI (wxWidget drawing openGL).

There's a m_MantainedKeyList that will disable autorepeat. We the user
mantains a key pressed, we must send only one event to mitk.

*/
class WXMITKRENDERING_EXPORT wxMitkRenderWindow : public wxMitkVTKWindow
{
public:

	//!
	wxMitkRenderWindow(
		wxWindow* parent, 
		wxWindowID id, 
		mitk::VtkPropRenderer* renderer = NULL, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = 0, 
		const wxString& name = wxT("unnamed renderwindow"));
	//!
	virtual ~wxMitkRenderWindow(void);
	
	//! 
	void InitRenderer(void);
	//! 
	mitk::SliceNavigationController* GetSliceNavigationController(void);
	//! 
	mitk::CameraRotationController* GetCameraRotationController(void);
	//! 
	mitk::BaseController* GetController(void);
	//! 
	mitk::VtkPropRenderer* GetRenderer(void);
	//! 
	void RequestUpdate(void);
	//! 
	void ForceImmediateUpdate(void);

	blMitkScalarBar::Pointer GetScalarBar(void);

	//!
	void EnableMouseWheel( bool enable );

	//! Enable legend for LUT
	void EnableColorLegend( 
		bool enable,
		const char* title = NULL,
		mitk::DataTreeNode* node = NULL );

private:
	//! 
	void OnResize(wxSizeEvent& event);
	//! 
	void OnWxMouseEvent(wxMouseEvent& event);

	//! Translates to mitk::KeyEvent and calls mitk::VtkPropRenderer::KeyPressEvent( )
	void OnKeyPressEvent(wxKeyEvent& event);

	//! Remove the key from the m_MantainedKeyList
	void OnKeyUpEvent(wxKeyEvent& event);

	//! Double click
	void OnDblClick( wxMouseEvent& event );

	//! 
	void OnMouseWheelEvent(wxMouseEvent& event);

	//! Maps the wxKeyEvent into a mitkKeyPressEvent 
	mitk::KeyEvent MapWxKeyEventToMitkEvent(wxKeyEvent& event, bool keypressed);

	//! Convert from wxMouseEvent to mitk::MouseEvent
	mitk::MouseEvent ConvertWxEventToMitkEvent( wxMouseEvent& event );
	//!
	void OnMitkMouseEvent( mitk::MouseEvent &mitkEvt );

	/**
	\brief This handler needs to call ReleaseMouse( ) when the
	window focus has changed and CaptureMouse( ) has been called
	is always called at OnMouseEvent( )
	*/
	void OnMouseCaptureLost(wxMouseCaptureLostEvent& event);

private:
	//! 
	mitk::VtkPropRenderer::Pointer m_Renderer;

	//! 
	vtkMitkRenderProp* m_RenderProp;

	/** 
	 * \brief List of pressed keys to disable autorepeat. 
	 * Xavi: I disabled it because I need to enable autorepeat
	 * for the blMitkPointSetInteractor pressing the A and Z keys.
	 * It seems to work fine for the rest of the interactions.
	 */
	std::list<int> m_MantainedKeyList;

	//! Cursor manager
	mitk::wxMitkApplicationCursorImplementation m_wxMitkApplicationCursor;

	//!
	bool m_EnableMouseWheel;

	//!
	blMitkScalarBar::Pointer m_ScalarBar;

protected:
    DECLARE_EVENT_TABLE();
    DECLARE_CLASS(wxMitkRenderWindow)
};
 
} // namespace mitk


#endif // wxMitkRenderWindow_H
