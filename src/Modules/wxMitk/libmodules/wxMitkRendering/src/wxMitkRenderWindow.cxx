// For compilers that don't support precompilation, include "wx/wx.h"
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
       #include <wx/wx.h>
#endif

#include "wxMitkRenderWindow.h"
#include <wxMitkVTKRenderWindowInteractor.h>

#include <vtkRenderWindow.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkCommand.h>
#include <vtkMitkRenderProp.h>

#include <mitkDisplayPositionEvent.h>
#include <mitkInteractionConst.h>
#include <mitkSliceNavigationController.h>
#include <mitkSliceNavigationController.h>
#include <mitkCameraRotationController.h>
#include "mitkVtkInteractorCameraController.h"
#include "mitkVtkLayerController.h"
#include "mitkLookupTableProperty.h"

#include "vtkLabeledDataMapper.h"
#include "vtkActor2D.h"
#include "vtkScalarBarActor.h"
#include "vtkLookupTable.h"

#include <wxUnicode.h>

using namespace mitk;

IMPLEMENT_CLASS(wxMitkRenderWindow, wxWindow)

// ---------------------------------------------------------------------------------------------
#ifdef __WXX11__
#  include "wx/x11/privx.h"
#  define GetWindowHandle(wxwin)   (void*)((Window)(wxwin)->GetHandle())
#endif

/**
wxWidgets is not providing a method for retrieving the window id, needed for the rendering context.
This function returns it, using platform-dependent calls
*/
#ifdef WIN32 
#	define GetWindowHandle(wxwin) (void*)(wxwin->GetHandle())
#endif // WIN32


// ---------------------------------------------------------------------------------------------

BEGIN_EVENT_TABLE(wxMitkRenderWindow, wxMitkVTKWindow)
	EVT_SIZE				(wxMitkRenderWindow::OnResize)
	EVT_KEY_DOWN			(wxMitkRenderWindow::OnKeyPressEvent)
	EVT_KEY_UP				(wxMitkRenderWindow::OnKeyUpEvent)
	EVT_MOUSE_EVENTS		(wxMitkRenderWindow::OnWxMouseEvent)
	EVT_MOUSE_CAPTURE_LOST	(wxMitkRenderWindow::OnMouseCaptureLost)
	EVT_LEFT_DCLICK			(wxMitkRenderWindow::OnDblClick)
END_EVENT_TABLE()

// ---------------------------------------------------------------------------------------------

/**
Initialize a static Factory to dynamically allocate and override the standard RenderingManager
in order to use wxWidgets specific timing mechanism for triggering the updates
*/
#include "wxMitkRenderingManagerFactory.h"
wxMitkRenderingManagerFactory a_wxMitkRenderingManagerFactory;

// ---------------------------------------------------------------------------------------------


//!
wxMitkRenderWindow::wxMitkRenderWindow(wxWindow* parent, wxWindowID id, mitk::VtkPropRenderer* renderer, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
: wxMitkVTKWindow(parent, id, pos, size, style | wxNO_FULL_REPAINT_ON_RESIZE | wxWANTS_CHARS, name), 
m_Renderer(renderer)
{
	if(this->m_Renderer.IsNull())
	{
		std::string rendererName(_U(name));
		this->m_Renderer = mitk::VtkPropRenderer::New(rendererName.c_str(), this->GetRenderWindow());
	}

	this->m_Renderer->InitRenderer(this->GetRenderWindow());
	mitk::BaseRenderer::AddInstance(this->GetRenderWindow(), this->m_Renderer);
	mitk::RenderingManager::GetInstance()->AddRenderWindow(this->GetRenderWindow());
	// Bug fix: Remove following line when mitk ppl have commited a fix for the insertion of invalid
	// values in the list of renderwindows to mitkRenderingManager, due to non existing calls to ForceImmediateUpdate
	mitk::RenderingManager::GetInstance()->ForceImmediateUpdate(this->GetRenderWindow());
	/////////// -----
	this->m_RenderProp = vtkMitkRenderProp::New();
	this->m_RenderProp->SetPropRenderer(this->m_Renderer);
	this->m_Renderer->GetVtkRenderer()->AddViewProp(this->m_RenderProp);

	if((this->GetRenderWindow()->GetSize()[0] > 10) && (this->GetRenderWindow()->GetSize()[1] > 10))
		this->m_Renderer->InitSize(this->GetRenderWindow()->GetSize()[0], this->GetRenderWindow()->GetSize()[1]);

	m_EnableMouseWheel = true;

	m_wxMitkApplicationCursor.SetWindow( GetRenderWindow(), this );

	m_ScalarBar = blMitkScalarBar::New();
}

//!
wxMitkRenderWindow::~wxMitkRenderWindow(void)
{
	EnableColorLegend( false );

	mitk::RenderingManager::GetInstance()->RemoveRenderWindow(this->GetRenderWindow());
	mitk::BaseRenderer::RemoveInstance(this->GetRenderWindow());
	// get rid of the renderer
	this->m_Renderer->GetVtkRenderer()->RemoveViewProp(this->m_RenderProp);
	this->m_RenderProp->Delete();

	// We must destroy the renderer before the RenderWindow
	// because when an image is rendered, it has pointers to the
	// render window. So we need to keep alive the window until all render
	// objects are destroyed.
	this->m_Renderer = NULL;
}

/**
 */
void wxMitkRenderWindow::OnMouseWheelEvent(wxMouseEvent& event)
{
	if ( this->GetSliceNavigationController()->GetSliceLocked() )
		return;

	mitk::Stepper* stepper;
	stepper = this->GetSliceNavigationController()->GetSlice();
	if(stepper->GetSteps() <= 1)
		stepper = this->GetSliceNavigationController()->GetTime();

	if (event.GetWheelRotation() * event.GetWheelDelta() > 0) 
		stepper->Next();
	else
		stepper->Previous();

	wxMitkVTKWindow::OnMouseWheel( event );

	event.Skip();
}

/**
*/
void wxMitkRenderWindow::OnWxMouseEvent(wxMouseEvent& event)
{
	if (this->GetRenderer() == NULL)
		return;

	// If the mouse event is a mouse wheel, treat that directly: 
	// increase / decrease slice step
	if( m_EnableMouseWheel && event.GetEventType() == wxEVT_MOUSEWHEEL)
	{
		OnMouseWheelEvent( event );
		return;
	}

	// If the mouse event is not a mouse wheel, proceed in translating 
	// it into mitk::Event
	mitk::MouseEvent mitkEvt = ConvertWxEventToMitkEvent( event );

	// Handle it
	OnMitkMouseEvent( mitkEvt );

	// When the user presses Ctrl and left mouse button, 
	// VTK default behaviour is Spin. It conflicts with other
	// MITK interactors like cropping. So it is disabled
	if ( mitkEvt.GetButtonState( ) & mitk::BS_ControlButton )
	{
		return;
	}

	// Skip must be called in order to allow the default processing to take place as otherwise the window
	// under mouse wouldn't get the focus.
	event.Skip();

	// The mouse event is not send to the parent by default (see documentation)
	// We need to propagate to the parent to show the context menu
	event.ResumePropagation( wxEVENT_PROPAGATE_MAX );

	// Don't Use VTK Standard interactor behaviour
	//if ( event.ButtonDown() )
	//{
	//	wxMitkVTKWindow::OnButtonDown( event );
	//}
	//else if ( event.ButtonUp( ) )
	//{
	//	wxMitkVTKWindow::OnButtonUp( event );
	//}
	//else if ( event.Moving( ) || event.Dragging() )
	//{
	//	wxMitkVTKWindow::OnMotion( event );
	//}

}

/** 
*/
void wxMitkRenderWindow::OnKeyPressEvent(wxKeyEvent& event)
{
	if ( this->m_Renderer.IsNotNull() )
	{
		mitk::KeyEvent mitkEvent = this->MapWxKeyEventToMitkEvent(event, true);
		this->GetRenderer()->KeyPressEvent(&mitkEvent);
	}

	// Special case for UP and DOWN keys
	if ( this->GetSliceNavigationController()->GetSliceLocked() )
		return;

	mitk::Stepper* stepper;
	stepper = this->GetSliceNavigationController()->GetSlice();
	if(stepper->GetSteps() <= 1)
		stepper = this->GetSliceNavigationController()->GetTime();

	if ( event.GetKeyCode() == WXK_UP )
	{
		stepper->Next();
	}
	else if ( event.GetKeyCode() == WXK_DOWN )
	{
		stepper->Previous();
	}

	wxMitkVTKWindow::OnKeyDown( event );

	event.Skip();

}

/**
*/
void wxMitkRenderWindow::OnKeyUpEvent(wxKeyEvent& event)
{
	if ( this->m_Renderer.IsNotNull() )
	{
		mitk::KeyEvent mitkEvent = this->MapWxKeyEventToMitkEvent(event, false);
		this->GetRenderer()->KeyPressEvent(&mitkEvent);
	}

	wxMitkVTKWindow::OnKeyUp( event );

	event.Skip();
}


/** 
\brief Handles Resize events from wxWidgets and translates them to mitk Resize behavior 
*/
void wxMitkRenderWindow::OnResize(wxSizeEvent& event)
{
	wxMitkVTKWindow::OnSize( event );

	if ( IsShown() )
	{
		if(m_Renderer.IsNotNull())
		{
			wxSize size = event.GetSize();
			this->GetRenderer()->Resize(size.GetWidth(), size.GetHeight());
		}
	}

	//event.Skip();
}

void wxMitkRenderWindow::OnMouseCaptureLost(wxMouseCaptureLostEvent& event)
{
	if(this->HasCapture())
		this->ReleaseMouse();
	//	event.Skip();
}

void mitk::wxMitkRenderWindow::OnDblClick( wxMouseEvent& event )
{
	// Send it to the parent
	if ( GetParent() && GetParent()->GetParent() )
	{
		// Get position of this window referenced to the parent (wxMitkSelectableGLWidget)
		wxPoint position1 = GetPosition();

		// Get position of the parent window referenced to the parent (wxMitkMultiRenderWindow)
		wxPoint position2 = GetParent()->GetPosition();

		event.m_x += position1.x + position2.x;
		event.m_y += position1.y + position2.y;

		GetParent()->GetParent()->ProcessEvent( event );
	}
	// Don't propagate to the parent
	//event.Skip();
}

/**
*/
void wxMitkRenderWindow::OnMitkMouseEvent( mitk::MouseEvent &mitkEvt )
{
	// Cast the event
	switch( mitkEvt.GetType() )
	{
	case mitk::Type_MouseButtonPress: 
		this->CaptureMouse();
		this->GetRenderer()->MousePressEvent(&mitkEvt);
		break;

	case mitk::Type_MouseButtonRelease:
		if(this->HasCapture())
		{
			this->ReleaseMouse();
		}
		this->GetRenderer()->MouseReleaseEvent(&mitkEvt);
		break;

	case mitk::Type_MouseMove:
		this->GetRenderer()->MouseMoveEvent(&mitkEvt);
		break;

	case mitk::Type_Wheel:
		this->GetRenderer()->MousePressEvent(&mitkEvt);
		break;

	default:
		break;
	}


}

/**
*/
mitk::MouseEvent wxMitkRenderWindow::ConvertWxEventToMitkEvent(wxMouseEvent& event)
{
	mitk::Point2D pt;
	int button = mitk::BS_NoButton;
	int modifKey = mitk::Key_unknown;
	int eventType = mitk::Type_None;
	int state = mitk::BS_NoButton;

	wxPoint wxpos;
	wxpos = event.GetPosition();
	pt[0] = wxpos.x;
	pt[1]= wxpos.y;

	switch(event.GetButton())
	{
	case wxMOUSE_BTN_LEFT:
		button = mitk::BS_LeftButton;
		if(event.LeftDown())
			eventType = mitk::Type_MouseButtonPress;
		else
		{
			eventType = mitk::Type_MouseButtonRelease;
			state = mitk::BS_LeftButton;
		}
		break;

	case wxMOUSE_BTN_MIDDLE:
		button = mitk::BS_MidButton;
		if(event.MiddleDown())
			eventType = mitk::Type_MouseButtonPress;
		else
		{
			eventType = mitk::Type_MouseButtonRelease;
			state = mitk::BS_MidButton;
		}
		break;

	case wxMOUSE_BTN_RIGHT:
		button = mitk::BS_RightButton;
		if(event.RightDown())
			eventType = mitk::Type_MouseButtonPress;
		else
		{
			eventType = mitk::Type_MouseButtonRelease;
			state = mitk::BS_RightButton;
		}
		break;

	default:
		button = mitk::BS_NoButton;
		if ( event.GetEventType() == wxEVT_MOUSEWHEEL )
		{
			eventType = mitk::Type_Wheel;
			if (event.GetWheelRotation() * event.GetWheelDelta() > 0) 
				button = mitk::BS_LeftButton;
			else
				button = mitk::BS_RightButton;
		}
		break;
	}
	
	// xplanes: 01-04-08. The MITK interactors are expecting Key_unknown
	// like in the Qt version QmitkRenderWindow::mousePressEvent(QMouseEvent *me) 
	// If you don't set this to Key_unknown, interactors don't work in 3D views
	// (you can see mitk::BaseRenderer::MousePressEvent(mitk::MouseEvent *me) )
	// You can also see the StateMachine.xml file with the events definition
	modifKey = mitk::Key_unknown;
	if(event.ShiftDown())
	{
		//modifKey = mitk::Key_Shift;
		state |= mitk::BS_ShiftButton;
	}
	if(event.CmdDown())
	{
		//modifKey = mitk::Key_Control;
		state |= mitk::BS_ControlButton;
	}
	if(event.AltDown())
	{
		//modifKey = mitk::Key_Alt;
		state |= mitk::BS_AltButton;
	}

	// Mouse drag event
	if(event.Dragging() || event.Moving( ) )
	{
		if(event.MiddleIsDown()) 
			state = state | mitk::BS_MidButton;
		if(event.RightIsDown()) 
			state = state | mitk::BS_RightButton;
		if(event.LeftIsDown()) 
			state = state | mitk::BS_LeftButton;
		eventType = mitk::Type_MouseMove;
	}

	// Generate the mitk event
	mitk::MouseEvent mitkEvt(this->GetRenderer(), eventType, button, state, modifKey, pt);

	return mitkEvt;
}
/** 
*/
mitk::KeyEvent wxMitkRenderWindow::MapWxKeyEventToMitkEvent(
	wxKeyEvent& event, 
	bool keypressed)
{
	// Initial values
	int type = mitk::Type_None;
	int key = mitk::Key_unknown;
	int buttonState = mitk::BS_NoButton;
	int button = mitk::BS_NoButton;
	std::string text = "";
	mitk::Point2D point2D;

	// type: Key press
	if ( keypressed )
	{
		type = mitk::Type_KeyPress;
	}
	else
	{
		type = mitk::Type_KeyRelease;
	}

	// key: To initialize key we need to map from the current codes
	// to mitk codes
	// state: not used, take a look at mitk::BaseRenderer::KeyPressEvent( )
	if(event.ShiftDown())
	{
		//modifKey = mitk::Key_Shift;
		buttonState |= mitk::BS_ShiftButton;
	}
	if(event.ControlDown())
	{
		//modifKey = mitk::Key_Control;
		buttonState |= mitk::BS_ControlButton;
	}
	if(event.AltDown())
	{
		//modifKey = mitk::Key_Alt;
		buttonState |= mitk::BS_AltButton;
	}


	// text: we don't change it
	// autoRepeat: not changed
	// count: not changed

	switch ( event.GetKeyCode() )
	{
	case WXK_BACK: key = Key_BackSpace; break;
	case WXK_TAB: key = Key_Tab; break;
	case WXK_RETURN: key = Key_Return; break;
	case WXK_ESCAPE: key = Key_Escape; break;
	case WXK_SPACE: key = Key_Space; break;
	case WXK_DELETE: key = Key_Delete; break;
	case WXK_SHIFT: key = Key_Shift; break;
	case WXK_CONTROL: key = Key_Control; break;
	default:
		key = event.GetKeyCode();
		break;
	}


	// This is important when pressing shift key because
	// The current mouse position is used to set the last position of the
	// interactor
	wxPoint wxpos;
	wxpos = event.GetPosition();
	point2D[ 0 ] = wxpos.x;
	point2D[ 1 ] = wxpos.y;

	
	mitk::KeyEvent mitkEvent(
		m_Renderer,
		type, 
		button,
		buttonState, 
		key, 
		text, 
		point2D);
	return mitkEvent;
}

void mitk::wxMitkRenderWindow::EnableMouseWheel( bool enable )
{
	m_EnableMouseWheel = enable;
}

void mitk::wxMitkRenderWindow::EnableColorLegend( 
	bool enable, 
	const char* title /*= NULL*/, 
	mitk::DataTreeNode* node /*= NULL */ )
{
	m_ScalarBar->Enable( false );
	m_ScalarBar->SetRenderWindow( GetRenderWindow( ) );
	m_ScalarBar->SetDataTreeNode( node );
	m_ScalarBar->Enable( enable );
}


//!
mitk::SliceNavigationController* wxMitkRenderWindow::GetSliceNavigationController(void)
{
	return GetRenderer( )->GetSliceNavigationController();
}

//!
mitk::CameraRotationController* wxMitkRenderWindow::GetCameraRotationController(void)
{
	return GetRenderer( )->GetCameraRotationController();
}

//!
mitk::BaseController* wxMitkRenderWindow::GetController(void)
{
	switch (GetRenderer( )->GetMapperID())
	{
	case mitk::BaseRenderer::Standard2D:
		return this->GetSliceNavigationController();
	case mitk::BaseRenderer::Standard3D:
		return this->GetCameraRotationController();
	default:
		return this->GetSliceNavigationController();
	}
}

//!
mitk::VtkPropRenderer* wxMitkRenderWindow::GetRenderer(void)
{
	return this->m_Renderer;
}

//!
blMitkScalarBar::Pointer wxMitkRenderWindow::GetScalarBar(void)
{
	return this->m_ScalarBar;
}


//!
void wxMitkRenderWindow::InitRenderer(void)
{
}

/**
Tells the rendering manager to request update on the render windows managed by this widget
*/
void wxMitkRenderWindow::RequestUpdate(void)
{
	mitk::RenderingManager::GetInstance()->RequestUpdate(this->GetRenderWindow());
}

/**
Tells the rendering manager to request update on the render windows managed by this widget
*/
void wxMitkRenderWindow::ForceImmediateUpdate(void)
{
	mitk::RenderingManager::GetInstance()->ForceImmediateUpdate(this->GetRenderWindow());
}



