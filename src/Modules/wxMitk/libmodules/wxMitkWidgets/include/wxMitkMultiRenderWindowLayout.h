/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _wxMitkMultiRenderWindowLayoutLayout_H
#define _wxMitkMultiRenderWindowLayoutLayout_H

#include "wxMitkWidgetsWin32Header.h"
#include "mitkSliceNavigationController.h"

#include "mitkDataStorage.h"

#include <wx/sizer.h>

namespace mitk
{
class wxMitkSelectableGLWidget;
class wxMitkLevelWindowWidget;

//!
enum LayoutConfiguration { 
	NoneLayout, 
	Default2x2, 
	Left2DRight3D, 
	Single, 
	Single2D_X,
	Single2D_Y,
	Single2D_Z,
	Single3D, 
	Only2D,
	Default2x2_YXZ3D,
	Default2x2_XYZ3D
};

//!
enum RenderWindowType {ThreeD_Win, X_Win, Y_Win, Z_Win} ;


/*
\brief Sizer for MultiRenderWindow. 
It contains:
- One 3D view widget
- Several Slices view (2D views) widgets
- One LevelWidget

\ingroup wxMitkWidgets
\author Xavi Planes
\date 23 Feb 2010
*/
class WXMITKWIDGETS_EXPORT wxMitkMultiRenderWindowLayout : public wxBoxSizer
{
public:
	typedef std::list<wxMitkSelectableGLWidget*> WidgetListType;
public:
	//!
	wxMitkMultiRenderWindowLayout();

	//!
	virtual ~wxMitkMultiRenderWindowLayout(void);

	//!
	void SetLevelWindowWidget(wxMitkLevelWindowWidget* val);

	//!
	void SetViews(std::list<wxMitkSelectableGLWidget*> val);

	//! Set current layout
	void SetCurrentLayout( 
		LayoutConfiguration layout, 
		wxMitkSelectableGLWidget* widget = NULL );

	//!
	mitk::LayoutConfiguration GetCurrentLayout() const;

	//!
	bool RestoreLayout( );

	//!
	void BackupLayout( );

	//!
	WidgetListType GetSliceViews( );

	//!
	WidgetListType GetSliceViews( 
		mitk::SliceNavigationController::ViewDirection direction );

	//!
	wxMitkSelectableGLWidget* Get3DView( );

	//!
	wxMitkSelectableGLWidget* GetXView( );

	//!
	wxMitkSelectableGLWidget* GetYView( );

	//!
	wxMitkSelectableGLWidget* GetZView( );

	//!
	void ShowLevelWindow(bool enable);

	//!
	void ShowAllViews( bool show = true );

	//!
	wxMitkSelectableGLWidget* GetShowWidget( );

protected:
	

private:

	void ChangeLayoutToDefault(void);
	void ChangeLayoutTo2x2_YXZ3D(void);
	void ChangeLayoutTo2x2_XYZ3D(void);
	void ChangeLayoutTo2DImagesLeft(void);
	void ChangeLayoutToSingle( wxMitkSelectableGLWidget* widget );
	void ChangeLayoutToOnly2D(void);
	void ChangeLayoutToNone();

private:
	//! 3D, X, Y, Z
	WidgetListType m_Views;

	//!
	wxMitkLevelWindowWidget* m_LevelWindowWidget;

	//! Current layout
	LayoutConfiguration m_CurrentLayout;
	
	//! If the user pressed double click
	LayoutConfiguration m_BackupCurrentLayout;
};
 
} // namespace mitk


#endif // _wxMitkMultiRenderWindowLayoutLayout_H
