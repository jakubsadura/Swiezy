/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _CmguiPluginRenderWidgetCanvas_H
#define _CmguiPluginRenderWidgetCanvas_H

extern "C"
{
#include "api/cmiss_scene_viewer.h"
}

namespace CmguiPlugin {

/**
\brief 
\ingroup CMGUIPlugin
\author Martin Stegh�fer
\date Oct 2010
*/
class RenderWidgetCanvas : public wxPanel {
public:
    RenderWidgetCanvas(wxWindow* parent, int id, Cmiss_scene_viewer_id sceneViewer = NULL);
    
    ~RenderWidgetCanvas();
    
    void exists();
    
    void OnSize(wxSizeEvent& event);
    void OnSizeRender(wxCommandEvent& event);
    void Redraw();
    
    void SetSceneViewer(Cmiss_scene_viewer_id sceneViewer);
    
	static int input_callback(
			struct Scene_viewer *scene_viewer, 
			struct Graphics_buffer_input *input,
			void *renderWidgetCanvas);

private:
	//!
	void OnWxMouseEvent(wxMouseEvent& event);

private:
	//!
    bool m_exists;
    
	//!
    Cmiss_scene_viewer_id m_sceneViewer;

	//!
	wxTimer* m_Timer;
    
    DECLARE_EVENT_TABLE()
};

} // namespace CmguiPlugin

#endif
