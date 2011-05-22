/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

// For compilers that don't support precompilation, include "wx/wx.h"
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
       #include <wx/wx.h>
#endif

#include "wxMitkVTKWindow.h"
#include "wxVTKRenderWindowInteractor.h"
#include "vtkInteractorStyle.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "wxMitkWindowHandle.h"

#if defined(__WXMAC__)
#ifdef __WXCOCOA__
	#include "vtkCocoaRenderWindow.h"
#else
	#include "vtkCarbonRenderWindow.h"
#endif
#endif(__WXMAC__)

using namespace mitk;


// ---------------------------------------------------------------------------------------------

//For more info on this class please go to:
//http://wxvtk.sf.net
//This hack is for some buggy wxGTK version:
#if wxCHECK_VERSION(2, 3, 2) && !wxCHECK_VERSION(2, 4, 1) && defined(__WXGTK__)
#  define WX_USE_X_CAPTURE 0
#else
#  define WX_USE_X_CAPTURE 1
#endif

#if defined(__WXGTK__) && defined(USE_WXGLCANVAS)
IMPLEMENT_CLASS(wxMitkVTKWindow, wxGLCanvas)
#else
IMPLEMENT_CLASS(wxMitkVTKWindow, wxWindow)
#endif  //__WXGTK__

// how to declare a custom event. this can go in a header
DECLARE_EVENT_TYPE(wxEVT_RERENDER, -1)
 
// how to define the custom event
DEFINE_EVENT_TYPE(wxEVT_RERENDER)

// ---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------
#if defined(__WXGTK__) && defined(USE_WXGLCANVAS)
BEGIN_EVENT_TABLE(wxMitkVTKWindow, wxGLCanvas)
#else
BEGIN_EVENT_TABLE(wxMitkVTKWindow, wxWindow)
#endif //__WXGTK__

	//refresh window by doing a Render
	EVT_PAINT       (wxMitkVTKWindow::OnPaint)
	EVT_ERASE_BACKGROUND(wxMitkVTKWindow::OnEraseBackground)
	EVT_MOTION      (wxMitkVTKWindow::OnMotion)

	EVT_COMMAND(wxID_ANY, wxEVT_RERENDER, wxMitkVTKWindow::OnRerender)

	//Bind the events to the event converters
	EVT_LEFT_DOWN   (wxMitkVTKWindow::OnButtonDown)
	EVT_MIDDLE_DOWN (wxMitkVTKWindow::OnButtonDown)
	EVT_RIGHT_DOWN  (wxMitkVTKWindow::OnButtonDown)
	EVT_LEFT_UP     (wxMitkVTKWindow::OnButtonUp)
	EVT_MIDDLE_UP   (wxMitkVTKWindow::OnButtonUp)
	EVT_RIGHT_UP    (wxMitkVTKWindow::OnButtonUp)
	EVT_ENTER_WINDOW(wxMitkVTKWindow::OnEnter)
	EVT_LEAVE_WINDOW(wxMitkVTKWindow::OnLeave)
	EVT_MOUSEWHEEL  (wxMitkVTKWindow::OnMouseWheel)
	EVT_KEY_DOWN    (wxMitkVTKWindow::OnKeyDown)
	EVT_KEY_UP      (wxMitkVTKWindow::OnKeyUp)
	EVT_CHAR        (wxMitkVTKWindow::OnChar)
	EVT_SIZE        (wxMitkVTKWindow::OnSize)
END_EVENT_TABLE()

// ---------------------------------------------------------------------------------------------



// ---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------
wxMitkVTKWindow::wxMitkVTKWindow(
	wxWindow *parent,
	wxWindowID id,
	const wxPoint &pos,
	const wxSize &size,
	long style,
	const wxString &name)
#if defined(__WXGTK__) && defined(USE_WXGLCANVAS)
	: wxGLCanvas(parent, id, pos, size, style, name)
#else
	: wxWindow(parent, id, pos, size, style, name)
#endif //__WXGTK__
{

	m_UseCaptureMouse = 0;
	m_Handle = 0;
	m_UseCaptureMouse = 0;
	m_Handle = 0;
	m_RenderAllowed = 0;
	m_Stereo = 0;
	m_RenderWhenDisabled = 1;
	m_Created = true;
	m_ActiveButton = wxEVT_NULL;
	m_RenWin = NULL;

#ifdef __WXMAC__
	// On Mac (Carbon) we don't get notified of the initial window size with an EVT_SIZE event,
	// so we update the size information of the interactor/renderwindow here
	this->UpdateSize(size.x, size.y);
#endif
}

//!
wxMitkVTKWindow::~wxMitkVTKWindow(void)
{
	// get rid of the VTK window
	this->SetRenderWindow(NULL);
}

//!
vtkRenderWindow* wxMitkVTKWindow::GetRenderWindow(void)
{
	if( m_RenWin == NULL)
	{
		// create a default vtk window
		vtkRenderWindow* win = vtkRenderWindow::New();
		this->SetRenderWindow(win);
		win->Delete();
	}
	return m_RenWin;
}

//!
void wxMitkVTKWindow::SetRenderWindow(vtkRenderWindow* renderWindow)
{
	// do nothing if we don't have to
	if(renderWindow == m_RenWin )
		return;

	// unregister previous window
	if( m_RenWin != NULL)
	{
		// clean up window as one could remap it
		if( m_RenWin->GetMapped())
			m_RenWin->Finalize();

		m_RenWin->SetDisplayId(NULL);
		m_RenWin->SetParentId(NULL);
		m_RenWin->SetWindowId(NULL);

		// Xavi: Interactor register a new reference to the m_VtkRenderWindow
		m_RenWin->SetInteractor( NULL );

		m_RenWin->UnRegister(NULL);

	}

	// now set the window
	m_RenWin = renderWindow;

	if(m_RenWin != NULL)
	{
		// register new window
		m_RenWin->Register(NULL);

		// if it is mapped somewhere else, unmap it
		if(m_RenWin->GetMapped())
			m_RenWin->Finalize();

#if defined (__WXX11__) || defined (__WXGTK__)
		void * windowHandle = GetHandleHack(this);
		// create a temporary (unvisible) window to satisfy vtk/opengl - it is very
		// vain and always wants to paint somewhere, even if we don't want it
		windowHandle = CreateTemporaryWindow();
		m_RenWin->SetWindowId( windowHandle );
#else
		m_RenWin->SetDisplayId( (void*) GetHandleHack( this ) );
		m_RenWin->SetWindowId( (void*) GetHandleHack( this ) );
#endif
		m_RenWin->SetParentId((void*) GetHandleHack(this->GetParent()));

		// tell the vtk window what the size of this window is
		wxSize size = this->wxWindow::GetSize();
		m_RenWin->vtkRenderWindow::SetSize(size.GetWidth(), size.GetHeight());
		wxPoint pos = this->wxWindow::GetPosition();
		m_RenWin->vtkRenderWindow::SetPosition(pos.x, pos.y);

		// Xavi P. Commented this line because when initializing the window
		// before the main window is shown, Start() is not called and
		// there's a rendering problem for meshes with opacity != 1
		// To fix this, we need to call Start( ) always
		//if(this->wxWindow::IsShownOnScreen())
			m_RenWin->Start();

		// if an interactor wasn't provided, we'll make one by default
		if(m_RenWin->GetInteractor() == NULL)
		{
			// create a default interactor
			wxVTKRenderWindowInteractor* iren = wxVTKRenderWindowInteractor::New();
			m_RenWin->SetInteractor( iren );
			iren->Initialize();
			iren->Delete( );
		}

		// now set the default style
		vtkInteractorStyle* interactorStyle = vtkInteractorStyleTrackballCamera::New();
		m_RenWin->GetInteractor()->SetInteractorStyle(interactorStyle);
		interactorStyle->Delete();

		// tell the interactor the size of this window
		m_RenWin->GetInteractor()->SetSize(size.GetWidth(), size.GetHeight());
	}
}

//!
wxVTKRenderWindowInteractor* wxMitkVTKWindow::GetInteractor(void)
{
	return static_cast<wxVTKRenderWindowInteractor*>(this->GetRenderWindow()->GetInteractor());
}

void wxMitkVTKWindow::OnRerender(wxCommandEvent& evt) {
	m_RenWin->Render();
}

//---------------------------------------------------------------------------
void wxMitkVTKWindow::OnPaint(wxPaintEvent& WXUNUSED(event))
{
	//do it here rather than in the cstor: this is safer.
	if(!m_Handle)
	{
		m_Handle = GetHandleHack( this );
		m_RenWin->SetWindowId(m_Handle);
#ifdef __WXMSW__ || __WXMAC__
		m_RenWin->SetParentId(reinterpret_cast<void *>(this->GetParent()->GetHWND()));
#else
		// On Linux a temporary window is created on initialization
		// When painting, the windowID is different and we need to
		// Initialize the X OpenGL window
		m_RenWin->Finalize( );
#endif //__WXMSW__
	}

	if(this->wxWindow::IsShownOnScreen())
		m_RenWin->Start();

	// get vtk to render to the wxWindows
	Render();

	//must always be here
	wxPaintDC pDC(this);

	wxCommandEvent evt(wxEVT_RERENDER, wxID_ANY);
	AddPendingEvent(evt);

	#ifdef __WXMAC__
	   // This solves a problem with repainting after a window resize
	   // See also: http://sourceforge.net/mailarchive/forum.php?thread_id=31690967&forum_id=41789
	   #ifdef __WXCOCOA__
			vtkCocoaRenderWindow * rwin = vtkCocoaRenderWindow::SafeDownCast(m_RenWin);
			if( rwin )
			{
				rwin->UpdateContext();
			}
	    #else
			vtkCarbonRenderWindow* rwin = vtkCarbonRenderWindow::SafeDownCast(m_RenWin);
			if( rwin )
			{
			 #if VTK_MAJOR_VERSION > 4 || (VTK_MAJOR_VERSION == 4 && VTK_MINOR_VERSION > 4)
				// Must be somewhere after VTK 4.4
				rwin->UpdateGLRegion();
			 #endif
			}
	    #endif
	#endif

}
//---------------------------------------------------------------------------
void wxMitkVTKWindow::OnEraseBackground(wxEraseEvent &event)
{
	//turn off background erase to reduce flickering on MSW
	event.Skip(false);
}
//---------------------------------------------------------------------------
void wxMitkVTKWindow::OnSize(wxSizeEvent& WXUNUSED(event))
{
	vtkRenderWindowInteractor* iren = NULL;
	if(m_RenWin)
	{
		iren = m_RenWin->GetInteractor();
	}

	if(!iren || !iren->GetEnabled())
	{
		return;
	}


	int w, h;
	GetClientSize(&w, &h);
	// When x or y is 0, it crashes on Linux
	if ( w == 0 )
	{
		w = 20;
	}
	if ( h == 0 )
	{
		h = 20;
	}
	SetClientSize(w,h);
	iren->UpdateSize(w, h);

	iren->InvokeEvent(vtkCommand::ConfigureEvent, NULL);
	//this will check for Handle
	//Render();
}
//---------------------------------------------------------------------------
void wxMitkVTKWindow::OnMotion(wxMouseEvent &event)
{
	vtkRenderWindowInteractor* iren = NULL;
	if(m_RenWin)
	{
		iren = m_RenWin->GetInteractor();
	}

	if(!iren || !iren->GetEnabled())
	{
		return;
	}

	iren->SetEventInformationFlipY(event.GetX(), event.GetY(), 
		event.ControlDown(), event.ShiftDown(), '\0', 0, NULL);

	iren->InvokeEvent(vtkCommand::MouseMoveEvent, NULL);

	event.Skip();
}
//---------------------------------------------------------------------------
void wxMitkVTKWindow::OnEnter(wxMouseEvent &event)
{
	vtkRenderWindowInteractor* iren = NULL;
	if(m_RenWin)
	{
		iren = m_RenWin->GetInteractor();
	}

	if(!iren || !iren->GetEnabled())
	{
		return;
	}

	iren->SetEventInformationFlipY(event.GetX(), event.GetY(), 
		event.ControlDown(), event.ShiftDown(), '\0', 0, NULL);

	iren->InvokeEvent(vtkCommand::EnterEvent, NULL);

	event.Skip();
}
//---------------------------------------------------------------------------
void wxMitkVTKWindow::OnLeave(wxMouseEvent &event)
{
	vtkRenderWindowInteractor* iren = NULL;
	if(m_RenWin)
	{
		iren = m_RenWin->GetInteractor();
	}

	if(!iren || !iren->GetEnabled())
	{
		return;
	}

	iren->SetEventInformationFlipY(event.GetX(), event.GetY(), 
		event.ControlDown(), event.ShiftDown(), '\0', 0, NULL);

	iren->InvokeEvent(vtkCommand::LeaveEvent, NULL);

	event.Skip();
}
//---------------------------------------------------------------------------
void wxMitkVTKWindow::OnKeyDown(wxKeyEvent &event)
{
	vtkRenderWindowInteractor* iren = NULL;
	if(m_RenWin)
	{
		iren = m_RenWin->GetInteractor();
	}

	if(!iren || !iren->GetEnabled())
	{
		event.Skip();
		return;
	}

	int keycode = event.GetKeyCode();
	char key = '\0';
	if (((unsigned int)keycode) < 256)
	{
		// TODO: Unicode in non-Unicode mode ??
		key = (char)keycode;
	}

	// This key makes a crash
	if ( key == 'f' || key == 'F' )
	{
		event.Skip();
		return;
	}

	// we don't get a valid mouse position inside the key event on every platform
	// so we retrieve the mouse position explicitly and pass it along
	wxPoint mousePos = ScreenToClient(wxGetMousePosition());
	iren->SetEventInformationFlipY(mousePos.x, mousePos.y, 
		event.ControlDown(), event.ShiftDown(), key, 0, NULL);
	iren->InvokeEvent(vtkCommand::KeyPressEvent, NULL);

	event.Skip();
}
//---------------------------------------------------------------------------
void wxMitkVTKWindow::OnKeyUp(wxKeyEvent &event)
{
	vtkRenderWindowInteractor* iren = NULL;
	if(m_RenWin)
	{
		iren = m_RenWin->GetInteractor();
	}

	if(!iren || !iren->GetEnabled())
	{
		return;
	}

	int keycode = event.GetKeyCode();
	char key = '\0';
	if (((unsigned int)keycode) < 256)
	{
		// TODO: Unicode in non-Unicode mode ??
		key = (char)keycode;
	}

	// This key makes a crash
	if ( key == 'f' || key == 'F' )
	{
		event.Skip();
		return;
	}

	// we don't get a valid mouse position inside the key event on every platform
	// so we retrieve the mouse position explicitly and pass it along
	wxPoint mousePos = ScreenToClient(wxGetMousePosition());
	iren->SetEventInformationFlipY(mousePos.x, mousePos.y, 
		event.ControlDown(), event.ShiftDown(), key, 0, NULL);
	iren->InvokeEvent(vtkCommand::KeyReleaseEvent, NULL);

	event.Skip();
}

//---------------------------------------------------------------------------
void wxMitkVTKWindow::OnChar(wxKeyEvent &event)
{
	vtkRenderWindowInteractor* iren = NULL;
	if(m_RenWin)
	{
		iren = m_RenWin->GetInteractor();
	}

	if(!iren || !iren->GetEnabled())
	{
		return;
	}

	int keycode = event.GetKeyCode();
	char key = '\0';
	if (((unsigned int)keycode) < 256)
	{
		// TODO: Unicode in non-Unicode mode ??
		key = (char)keycode;
	}

	// This key makes a crash
	if ( key == 'f' || key == 'F' )
	{
		event.Skip();
		return;
	}

	// we don't get a valid mouse position inside the key event on every platform
	// so we retrieve the mouse position explicitly and pass it along
	wxPoint mousePos = ScreenToClient(wxGetMousePosition());
	iren->SetEventInformationFlipY(mousePos.x, mousePos.y, 
		event.ControlDown(), event.ShiftDown(), key, 0, NULL);
	iren->InvokeEvent(vtkCommand::CharEvent, NULL);

	event.Skip();
}
//---------------------------------------------------------------------------
void wxMitkVTKWindow::OnButtonDown(wxMouseEvent &event)
{
	vtkRenderWindowInteractor* iren = NULL;
	if(m_RenWin)
	{
		iren = m_RenWin->GetInteractor();
	}

	if(!iren || !iren->GetEnabled())
	{
		return;
	}

	if (m_ActiveButton != wxEVT_NULL)
	{
		return;
	}
	m_ActiveButton = event.GetEventType();

	// On Mac (Carbon) and Windows we don't automatically get the focus when
	// you click inside the window
	// we therefore set the focus explicitly
	// Apparently we need that on linux (GTK) too:
	this->SetFocus();

	iren->SetEventInformationFlipY(event.GetX(), event.GetY(), 
		event.ControlDown(), event.ShiftDown(), '\0', 0, NULL);

	if(event.RightDown())
	{
		iren->InvokeEvent(vtkCommand::RightButtonPressEvent, NULL);
	}
	else if(event.LeftDown())
	{
		iren->InvokeEvent(vtkCommand::LeftButtonPressEvent, NULL);
	}
	else if(event.MiddleDown())
	{
		iren->InvokeEvent(vtkCommand::MiddleButtonPressEvent, NULL);
	}
	//save the button and capture mouse until the button is released
	//Only if :
	//1. it is possible (WX_USE_X_CAPTURE)
	//2. user decided to.
	if ((m_ActiveButton != wxEVT_NULL) && WX_USE_X_CAPTURE && m_UseCaptureMouse)
	{
		CaptureMouse();
	}
	event.Skip();
}
//---------------------------------------------------------------------------
void wxMitkVTKWindow::OnButtonUp(wxMouseEvent &event)
{
	vtkRenderWindowInteractor* iren = NULL;
	if(m_RenWin)
	{
		iren = m_RenWin->GetInteractor();
	}

	if(!iren || !iren->GetEnabled())
	{
		return;
	}

	//EVT_xxx_DOWN == EVT_xxx_UP - 1
	//This is only needed if two mouse buttons are pressed at the same time.
	//In wxWindows 2.4 and later: better use of wxMOUSE_BTN_RIGHT or 
	//wxEVT_COMMAND_RIGHT_CLICK
	if (m_ActiveButton != (event.GetEventType()-1)) 
	{
		return;
	}

	iren->SetEventInformationFlipY(event.GetX(), event.GetY(), 
		event.ControlDown(), event.ShiftDown(), '\0', 0, NULL);

	if(m_ActiveButton == wxEVT_RIGHT_DOWN)
	{
		iren->InvokeEvent(vtkCommand::RightButtonReleaseEvent, NULL);
	}
	else if(m_ActiveButton == wxEVT_LEFT_DOWN)
	{
		iren->InvokeEvent(vtkCommand::LeftButtonReleaseEvent, NULL);
	}
	else if(m_ActiveButton == wxEVT_MIDDLE_DOWN)
	{
		iren->InvokeEvent(vtkCommand::MiddleButtonReleaseEvent, NULL);
	}
	//if the ActiveButton is realeased, then release mouse capture
	if ((m_ActiveButton != wxEVT_NULL) && WX_USE_X_CAPTURE && m_UseCaptureMouse)
	{
		ReleaseMouse();
	}
	m_ActiveButton = wxEVT_NULL;

	event.Skip();
}
//---------------------------------------------------------------------------
void wxMitkVTKWindow::OnMouseWheel(wxMouseEvent& event)
{
	vtkRenderWindowInteractor* iren = NULL;
	if(m_RenWin)
	{
		iren = m_RenWin->GetInteractor();
	}

	if(!iren || !iren->GetEnabled())
	{
		return;
	}

	// Mouse wheel was only added after VTK 4.4 (I think...)
	// new style
	//Set vtk event information ... The numebr of wheel rotations is stored in
	//the x varible.  y varible is zero
	iren->SetEventInformationFlipY(event.GetWheelRotation() / event.GetWheelDelta(), 0, 
		event.ControlDown(), event.ShiftDown(), '\0', 0, NULL);
	if(event.GetWheelRotation() > 0)
	{
		//Send event to VTK
		iren->InvokeEvent(vtkCommand::MouseWheelForwardEvent, NULL);
	}
	else
	{
		//Send event to VTK
		iren->InvokeEvent(vtkCommand::MouseWheelBackwardEvent, NULL);
	}

	event.Skip();
}


//---------------------------------------------------------------------------
void wxMitkVTKWindow::Render()
{

	m_RenderAllowed = 1;
	if (!m_RenderWhenDisabled)
	{
		//the user doesn't want us to render when the toplevel frame
		//is disabled - first find the top level parent
		wxWindow *topParent = wxGetTopLevelParent(this);
		if (topParent)
		{
			//if it exists, check whether it's enabled
			//if it's not enabeld, RenderAllowed will be false
			m_RenderAllowed = topParent->IsEnabled();
		}
	}

	if (m_RenderAllowed)
	{
		if(m_Handle && (m_Handle == GetHandleHack( this )) )
		{
			m_RenWin->Render();
		}
#if VTK_MAJOR_VERSION > 4 || (VTK_MAJOR_VERSION == 4 && VTK_MINOR_VERSION > 2)
		else if(GetHandleHack( this ))
		{
			//this means the user has reparented us; let's adapt to the
			//new situation by doing the WindowRemap dance
			//store the new situation
			m_Handle = GetHandleHack( this );
			m_RenWin->SetNextWindowId( m_Handle );
			m_RenWin->WindowRemap();
			m_RenWin->Render();
		}
#endif
	}
}
//---------------------------------------------------------------------------
void wxMitkVTKWindow::SetRenderWhenDisabled(int newValue)
{
	//Change value of __RenderWhenDisabled ivar.
	//If __RenderWhenDisabled is false (the default), this widget will not
	//call Render() on the mRenWin if the top level frame (i.e. the
	//containing frame) has been disabled.

	//This prevents recursive rendering during wxSafeYield() calls.
	//wxSafeYield() can be called during the ProgressMethod() callback of
	//a VTK object to have progress bars and other GUI elements updated -
	//it does this by disabling all windows (disallowing user-input to
	//prevent re-entrancy of code) and then handling all outstanding
	//GUI events.

	//However, this often triggers an OnPaint() method for wxVTKRWIs,
	//resulting in a Render(), resulting in Update() being called whilst
	//still in progress.

	m_RenderWhenDisabled = (bool)newValue;
}
//---------------------------------------------------------------------------
//
// Set the variable that indicates that we want a stereo capable window
// be created. This method can only be called before a window is realized.
//
void wxMitkVTKWindow::SetStereo(int capable)
{
	if (m_Stereo != capable)
	{
		m_Stereo = capable;
		m_RenWin->StereoCapableWindowOn();
		m_RenWin->SetStereoTypeToCrystalEyes();
	}
}

//---------------------------------------------------------------------------
void wxMitkVTKWindow::Enable()
{
#if defined(__WXGTK__) && defined(USE_WXGLCANVAS)
	SetCurrent();
#endif
}
//---------------------------------------------------------------------------
bool wxMitkVTKWindow::Enable(bool enable)
{
#if defined(__WXGTK__) && defined(USE_WXGLCANVAS)
	return wxGLCanvas::Enable(enable);
#else
	return wxWindow::Enable(enable);
#endif
}

int mitk::wxMitkVTKWindow::GetStereo()
{
	return m_Stereo;
}

void mitk::wxMitkVTKWindow::SetUseCaptureMouse( int val )
{
	m_UseCaptureMouse = val;
}


#if defined (__WXX11__) || defined (__WXGTK__)

void * wxMitkVTKWindow::CreateTemporaryWindow()
{
    XVisualInfo *v;
    Display *DisplayId;
    Window ParentId, WindowId;

    // default display
    DisplayId = XOpenDisplay(static_cast<char *>(NULL));
    
    // get "some" visual info (we don't show the window anyway, so we pass "VisualNoMask" and NULL
    // to tell that we actually don't care about the features of the visual info)
    int num;
    v = XGetVisualInfo(DisplayId, VisualNoMask, NULL, &num);
    
    // root window as parent (again: we don't care, any window will do)
    ParentId = RootWindow(DisplayId, v->screen);

    // create the window
    WindowId = XCreateWindow(DisplayId, ParentId, 10, 10, 20, 20, 0,
                CopyFromParent, InputOutput, CopyFromParent, 0, NULL);
    
    // normally we would map the window at this point, but we DON'T - to keep the window invisible
    
    // Flush the buffer and clean up
    XSync(DisplayId,False);
    if (v) {
        XFree(v);
    }
    
    return (void*)WindowId;
}
#endif

#ifdef __WXMAC__
//NOT USED! ..__WXMAC__ acts as MSW: doesn't need any temp window!
/*
void * wxMitkVTKWindow::CreateTemporaryWindow()
{
	WindowRef ParentWindow;
	HIViewRef WindowId;
	cout <<"*********trying to create temp on MAC *********"<<endl;

	Rect rectWin =  {0, 0, 100, 100};

	WindowAttributes windowAttrs = (kWindowStandardDocumentAttributes | 
		kWindowLiveResizeAttribute |
		kWindowStandardHandlerAttribute |
		kWindowCompositingAttribute);

	if (noErr != CreateNewWindow (kDocumentWindowClass,
		windowAttrs,
		&rectWin, &(ParentWindow)))
	{
		//vtkErrorMacro("Could not create window, serious error!");
		cout<<"Could not create window, serious error!"<<endl;
		return (void *) NULL;
	}

	// get the content view
		
	HIViewFindByID(HIViewGetRoot(ParentWindow),
		kHIViewWindowContentID,
		&WindowId);

	//ShowWindow(ParentWindow);


	//// install event handler for updating gl region
	//// this works for a supplied HIView and an HIView made here
	//if(windowId)
	//{
	//	EventTypeSpec region_events [] = 
	//	{
	//		{ kEventClassControl, kEventControlOwningWindowChanged},
	//		{ kEventClassControl, kEventControlVisibilityChanged },
	//		{ kEventClassControl, kEventControlBoundsChanged }
	//	};
	//	this->RegionEventHandlerUPP = NewEventHandlerUPP(vtkCarbonRenderWindow::RegionEventProcessor);
	//	InstallControlEventHandler(windowId, this->RegionEventHandlerUPP, 
	//		GetEventTypeCount(region_events), region_events,
	//		reinterpret_cast<void*>(this), &this->RegionEventHandler);
	//}
	return (void*)WindowId;
}
*/

void wxMitkVTKWindow::UpdateSize(int x, int y)
{
	
	if( m_RenWin )
	{
		int *Size = m_RenWin->GetSize(); 
		// if the size changed tell render window
		if ( x != Size[0] || y != Size[1] )
		{
			// adjust our (vtkRenderWindow size)
			Size[0] = x;
			Size[1] = y;
			// and our RenderWindow's size
			m_RenWin->SetSize(x, y);
		}
	}
}
#endif //__WXMAC__
