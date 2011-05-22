/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _CmguiPluginRenderWidget_H
#define _CmguiPluginRenderWidget_H

//#include "wx/glcanvas.h"

#include "CmguiPluginRenderWidgetCanvas.h"
#include "CmguiPluginCmgui.h"
#include "CmguiPluginMacros.h"
#include "CmguiPluginRenderingTree.h"

extern "C"
{
#include "api/cmiss_command_data.h"
#include "api/cmiss_field.h"
#include "api/cmiss_texture.h"
#include "api/cmiss_scene_viewer.h"
}

#define wxID_PLOT wxID("CmguiPluginRenderWidgetPlot")

namespace CmguiPlugin {


/**
\brief 
\ingroup CMGUIPlugin
\author Martin Stegh�fer
\date Oct 2010
*/
class RenderWidget : public wxPanel, public Core::Widgets::RenderWindowBase
{

public:
	//!
    class Factory : public Core::BaseWindowFactory
    {
    public: 
        coreDeclareSmartPointerTypesMacro(Factory,BaseWindowFactory)
        coreFactorylessNewMacro1Param( Factory, Cmgui* )
        coreClassNameMacro(Factory)
        coreCommonFactoryFunctionsCreateWindow1param( RenderWidget )
        std::string GetName() const
        {
            return "class CmguiPlugin::RenderWidget";
        }
    private:
        Cmgui* m_p1;
        Factory(Cmgui* p1) {
            m_p1 = p1;
        }
    };

	//!
	RenderWidget(Cmgui *cmgui, wxWindow* parent, int id = wxID_ANY, const wxPoint&  pos = wxDefaultPosition, const wxSize&  size = wxDefaultSize, long style = 0);

    //!
    ~RenderWidget( );

	//! Redefined
	void RenderScene(void);;

	//! Redefined
	Core::RenderingTree::Pointer GetPrivateRenderingTree() const;

	//! Redefined
	bool Enable(bool enable = true );

	//! Redefined
	std::string GetName( );

	//! Redefined
	int GetTime( );

	//! Redefined
	void SetTime( int time );

	//! Redefined
	void GetTimeRange( int &min, int &max );

	//! Redefined
	void SetTimeStep( int time );

	//!
	static void repaint_callback( Cmiss_scene_viewer_id scene_viewer, void *callback_data,void *renderWidget);

	//! Update 3D camera
	void UpdateCamera3D( Core::Camera3D::Pointer cam );

	//! Redefined
	void RequestUpdateAll( );

private:
    DECLARE_EVENT_TABLE()

	void SetRenderingTreeInstance(Core::RenderingTree::Pointer val);

	void OnPaint(wxPaintEvent& event);

	void OnEraseBackground(wxEraseEvent &event);

    void Init();

    void do_layout();

	bool getBoundings(Cmiss_region_id region, double *lower, double *upper);

	//!
	void ComputeTimeRange( );

private:
	//!
    RenderWidgetCanvas* m_plot;
	//!
    Cmgui *m_cmgui;
	//!
    wxBoxSizer* m_sizer;

private:
	//!
	RenderingTree::Pointer m_RenderingTree;
};

} //namespace CmguiPlugin{

#endif //_CmguiPluginRenderWidget_H

