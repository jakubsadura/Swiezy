/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _MultiRenderWindowEventHandler_H
#define _MultiRenderWindowEventHandler_H

#define wxID_CurrentMultiView wxID( "wxID_CurrentMultiView" )
#define wxID_ViewLinkPlanes wxID("wxID_ViewLinkPlanes")
#define wxID_ViewShowLabels wxID("wxID_ViewShowLabels")
#define wxID_ShowAnnotationCubeMenuItem	wxID("wxID_ShowAnnotationCubeMenuItem")
#define wxID_ShowCornerAnnotationsMenuItem		wxID("ShowAnnotationMenuItem")
#define wxID_LockAxis wxID( "wxID_LockAxis" )
#define wxID_LevelWindowInteractor wxID( "wxID_LevelWindowInteractor" )

#define wxID_ViewLayout wxID("wxID_ViewLayout")
#define wxID_ViewLayout2DViewsLeft wxID("wxID_ViewLayout2DViewsLeft")
#define wxID_ViewLayoutDefault wxID("wxID_ViewLayoutDefault")
#define wxID_ViewLayoutOnly2DViews wxID("wxID_ViewLayoutOnly2DViews")
#define wxID_ViewLayoutXView wxID("wxID_ViewLayoutXView")
#define wxID_ViewLayoutYView wxID("wxID_ViewLayoutYView")
#define wxID_ViewLayoutZView wxID("wxID_ViewLayoutZView")
#define wxID_ViewLayoutBig3DView wxID("wxID_ViewLayoutBig3DView")
#define wxID_ViewLayoutDefault2x2_YXZ3D wxID("wxID_ViewLayoutDefault2x2_YXZ3D")
#define wxID_ViewLayoutDefault2x2_XYZ3D wxID("wxID_ViewLayoutDefault2x2_XYZ3D")


namespace Core{
namespace Widgets{

class MultiRenderWindowMITK;
class MultiRenderWindowState;

/** 
Handler for MultiRenderWindow

Each time an event is received, the handler should:
- Change state of MultiRenderWiudow (for example EnableAxis( ) )
- UpdateMenu( ) (in case the user presses the toolbar tool)
- Skip event (to call the toolbar handler)

\ingroup gmWidgets
\author Xavi Planes
\date 11 June 2010
*/
class MultiRenderWindowEventHandler : public wxEvtHandler
{
public:
	MultiRenderWindowEventHandler( MultiRenderWindowMITK* renderWindow );

	//!
	void SetEvtHandlerEnabled(bool enabled);

protected:
	DECLARE_EVENT_TABLE( );

	//!
	void AppendMenuItems( );

	//!
	void UpdateMenu( );

	//!
	wxMenu* GetViewMenu( );

	void OnMenuEnableAxis(wxCommandEvent& event);
	void OnMenuShowAnnotationCubeMenuItem(wxCommandEvent& event);
	void OnMenuShowAnnotationMenuItem(wxCommandEvent& event);
	void OnMenuViewLinkPlanesItem( wxCommandEvent& event );
	void OnMenuViewShowLabelsItem( wxCommandEvent& event );
	void OnMenuViewLayout( wxCommandEvent& event );
	void OnMenuEnableLevelWindowInteractor(wxCommandEvent& event);

private:
	//!
	MultiRenderWindowMITK* m_RenderWindow;
};

} // Widgets
} // Core

#endif //_MultiRenderWindowEventHandler_H
