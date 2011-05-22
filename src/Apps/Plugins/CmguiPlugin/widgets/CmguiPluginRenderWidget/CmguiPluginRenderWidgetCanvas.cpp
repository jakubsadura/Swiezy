/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "CmguiPluginRenderWidgetCanvas.h"

extern "C" {
#include "graphics/scene_viewer.h"
}

using namespace CmguiPlugin;


class timer : public wxTimer {
private:
    RenderWidgetCanvas* m_canvas;
public:
    timer(RenderWidgetCanvas* canvas) {
        m_canvas = canvas;
        Start(50, false);
    }
    void Notify() {
        m_canvas->Redraw();
    }
};


DECLARE_EVENT_TYPE(wxEVT_SIZERENDER, -1)
DEFINE_EVENT_TYPE(wxEVT_SIZERENDER)

BEGIN_EVENT_TABLE(RenderWidgetCanvas, wxPanel)
    EVT_SIZE(RenderWidgetCanvas::OnSize)
    EVT_COMMAND(wxID_ANY, wxEVT_SIZERENDER, RenderWidgetCanvas::OnSizeRender)
	EVT_MOUSE_EVENTS		(RenderWidgetCanvas::OnWxMouseEvent)
END_EVENT_TABLE()


RenderWidgetCanvas::RenderWidgetCanvas(wxWindow* parent, int id, Cmiss_scene_viewer_id sceneViewer)
: wxPanel(parent, id),
m_sceneViewer(sceneViewer)
{
    m_exists = false;
    m_Timer = new timer(this);
}

void RenderWidgetCanvas::SetSceneViewer(Cmiss_scene_viewer_id sceneViewer) 
{
    m_sceneViewer = sceneViewer;
	Scene_viewer_add_input_callback( 
		sceneViewer, RenderWidgetCanvas::input_callback, this, 0 );
}

RenderWidgetCanvas::~RenderWidgetCanvas() 
{
	delete m_Timer;
}

void RenderWidgetCanvas::exists() {
    m_exists = true;
}

void RenderWidgetCanvas::OnSize(wxSizeEvent& event) {
    //std::cout << event.m_size.x << std::endl;
    wxCommandEvent evt(wxEVT_SIZERENDER, wxID_ANY);
    AddPendingEvent(evt);
    event.Skip();
}

void RenderWidgetCanvas::Redraw() 
{
    if ( IsEnabled( ) && m_exists && m_sceneViewer) 
	{
        Scene_viewer_redraw_now(m_sceneViewer);
    }
}

void RenderWidgetCanvas::OnSizeRender(wxCommandEvent& event) {
    Redraw();
}

void RenderWidgetCanvas::OnWxMouseEvent(wxMouseEvent& event)
{
	SetFocus( );

	event.Skip();

	// The mouse event is not send to the parent by default (see documentation)
	event.ResumePropagation( wxEVENT_PROPAGATE_MAX );
}

int RenderWidgetCanvas::input_callback(
struct Scene_viewer *scene_viewer, struct Graphics_buffer_input *input,
	void *renderWidgetCanvas)
{
	int return_code;
	return_code = 1;

	enum Cmiss_scene_viewer_input_event_type event_type;
	Cmiss_scene_viewer_input_get_event_type( input, &event_type );
	int button_number = Cmiss_scene_viewer_input_get_button_number( input );
	int x = Cmiss_scene_viewer_input_get_x_position( input );
	int y = Cmiss_scene_viewer_input_get_y_position( input );

	if ( button_number == wxMOUSE_BTN_RIGHT )
	{
		int i = 0;
	}

	if ( button_number == wxMOUSE_BTN_LEFT && 
		( event_type == CMISS_SCENE_VIEWER_INPUT_BUTTON_PRESS ||
		event_type == CMISS_SCENE_VIEWER_INPUT_BUTTON_RELEASE ) )
	{
		RenderWidgetCanvas* canvas;
		canvas = reinterpret_cast<RenderWidgetCanvas*> ( renderWidgetCanvas );

		wxMouseEvent event;

		event.SetEventObject( canvas );
		if ( event_type == CMISS_SCENE_VIEWER_INPUT_BUTTON_PRESS )
		{
			event.SetEventType( wxEVT_LEFT_DOWN );
		}
		else if ( event_type == CMISS_SCENE_VIEWER_INPUT_BUTTON_RELEASE )
		{
			event.SetEventType( wxEVT_LEFT_UP );
		}

		event.m_x = x;
		event.m_y = y;
		event.m_leftDown = true;

		canvas->GetEventHandler()->ProcessEvent( event );
	}

	return return_code;
}

