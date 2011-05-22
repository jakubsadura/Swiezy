/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef wxMitkMultiRenderWindow_H
#define wxMitkMultiRenderWindow_H

#include "wxMitkWidgetsWin32Header.h"

#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/bmpbuttn.h>
#include <string>

#include "vtkCornerAnnotation.h"
#include <vtkRenderer.h>

// Templates are not forward-declared:
#include <itkLightObject.h>   // SmartPointer template
#include <mitkDataTree.h>     // DataTreeIteratorBase template
#include "mitkSliceNavigationController.h"
#include "blMitkCornerAnnotation.h"
#include "wxMitkMultiRenderWindowLayout.h"

class wxMouseEvent;
class wxMitkMultiRenderWindowImpl;
class vtkPolyData;

namespace blMitk{ class SlicesRotator;};

namespace mitk
{
class wxMitkLevelWindowWidget;
class wxMitkSelectableGLWidget;
class wxMitkRenderWindow;
class SliceNavigationController;
class DisplayVectorInteractor;
class LogoRendering;
class SlicesRotator;
class SlicesSwiveller;
class DataStorage;


/*
\brief wxWidgets specific implementation of mitk::QmitkStdMultiView.

The MultiRenderWindow is responsible for holding some wxRenderWindow and 
manage the m_CurrentLayout so as to display
many different views over the same data.

\sa wxMitkm_LevelWindowWidget
\sa wxMitkSelectableGLWidget

\ingroup wxMitkWidgets
\author Juan Antonio Moya
\date 17 Oct 2007
*/
class WXMITKWIDGETS_EXPORT wxMitkMultiRenderWindow : public wxPanel
{
public:
	typedef std::list<wxMitkSelectableGLWidget*> WidgetListType;

public:
	enum PlaneInteractionMode{ SlicePlanes = 0, RotatePlanes, SwivelPlanes };

	//!
	wxMitkMultiRenderWindow(
		wxWindow* parent, 
		wxWindowID id, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = 0, 
		const wxString& name = wxT("wxMitkMultiRenderWindow"));
	virtual ~wxMitkMultiRenderWindow(void);

	//!
	virtual void SetDataStorage(mitk::DataStorage* storage);

	//!
	mitk::DataStorage::Pointer GetDataStorage() const;

	//!
	bool UpdateViewControllers( );

	//!
	void ReInitializeViewControllers(  );

	//!
	void EnableDepartmentLogo(bool enable = true);

	//!
	bool IsDepartmentLogoEnabled();

	//!
	void SetDepartmentLogoPath(const std::string& fileNameFullPath);

	//!
	void EnableNavigationController(bool enable = true);

	//!
	void SetNavigationInteractionMode(PlaneInteractionMode mode);

	//!
	void EnableMoveAndZoomInteractor(bool enable = true);

	//!
	void EnableWindowLevelInteractor(bool enable = true);
	
	//!
	void EnableStandardLevelWindow(bool enable = true);

	/**
	\brief Enable the annotated cube for the 3D view. 
	\note You should call this function after EnableDepartmentLogo( )
	*/
	virtual void EnableAnnotatedCube(bool enable);

	/**
	\brief Enable the corner annotations
	\note You should call this function after EnableDepartmentLogo( )
	*/
	virtual void EnableCornerAnnotations(bool enable);

	//! Orientation letters
	virtual void EnableOrientationAnnotations(bool enable);

	//!
	bool GetShowCornerAnnotations();

	//!
	mitk::SlicesRotator* GetSlicesRotator(void) const;

	//!
	mitk::SlicesSwiveller* GetSlicesSwiveller(void) const;

	//!
	mitk::SliceNavigationController* GetTimeNavigationController(void) const;

	//!
	mitk::DisplayVectorInteractor* GetMoveAndZoomInteractor(void) const;

	/**
	Sets the plane interaction to link the movements of the planes. So if 
	linked, when dragging the mouse over a 2D 
	view it will rotate the 2 planes at once.
	*/
	virtual void SetPlaneRotationLinked(bool link);

	//!
	void LockPlaneSlicing(bool locked = true);

	//!
	void LockPlaneRotation(bool locked = true);

	//! Select slice by point of all 2D views
	void SelectSliceByPoint( mitk::Point3D point );

	//! Get the intersection point of all 2D views
	bool GetIntersectionPoint( mitk::Point3D &axisCenter );

	//!
	void SaveImage3DWindow(std::string imageName);

	//!
	void EnableMouseWheel( bool enable = true );

	//! Enable the render windows (optimize speed)
	bool Enable(bool enable = true );

	//! Resets camera in all windows and fits the bounding box of the world inside the view
	void FitScene(void);

	//! Tells the rendering manager to request update on the render windows managed by this widget
	void RequestUpdate(void);

	//! Tells the rendering manager to force inmediate update on the render windows managed by this widget
	void ForceImmediateUpdate(void);

	void UpdateNavigationControllers(
		mitk::TimeSlicedGeometry::Pointer geometry, 
		bool keepOrientation,
		bool centerPos );

	//! Write a background text in window window at corner corner
	void SetCornerAnotation(vtkRenderer* textRenderer, RenderWindowType  window);

	//! Set current layout
	virtual void SetCurrentLayout( 
		LayoutConfiguration layout,
		wxMitkSelectableGLWidget* widget = NULL );
	
	//!
	wxMitkMultiRenderWindowLayout* GetLayout( );

	//!
	wxMitkLevelWindowWidget*  GetLevelWindowWidget();

	//!
	wxMitkSelectableGLWidget* GetX();

	//!
	wxMitkSelectableGLWidget* GetY();

	//!
	wxMitkSelectableGLWidget* GetZ();

	//!
	wxMitkSelectableGLWidget* Get3D();

	//!
	WidgetListType GetWidgets() const;

	//!
	void SetWidgets( const std::list<wxMitkSelectableGLWidget*> &widgets );

	//!
	void AddWidget( wxMitkSelectableGLWidget* widget );

	//!
	void RemoveWidget( wxMitkSelectableGLWidget* widget );

protected:
	

private:
	//!
	void OnDblClick( wxMouseEvent& event );

	//!
	void UpdateTimeController(
		mitk::TimeSlicedGeometry::Pointer geometry );

	//!
	void InitCubeAnnotations();

	//!
	void InitInteractors( );

	//! Connect interactors to this widget
	void ConnectInteractors( wxMitkSelectableGLWidget* widget );

	//! Disconnect interactors to this widget
	void DisconnectInteractors( wxMitkSelectableGLWidget* widget );

	//!
	void AddSlicePlanesObervers( );

	//!
	void RemoveSlicePlanesObervers( );

	//!
	virtual void OnModifiedSlicePlane( const itk::Object* o, const itk::EventObject &event );

private:
	//! Use of PImpl pattern to avoid recompilation when headers files changed
	wxMitkMultiRenderWindowImpl *m_pImpl;

	//!
	WidgetListType m_Widgets;

	//!
	wxMitkLevelWindowWidget* m_LevelWindowWidget;

	//!
	PlaneInteractionMode m_PlaneMode;

	//! keep position at initialization
	bool m_bKeep2DViewPosition;

	//!
	mitk::TimeSlicedGeometry::Pointer m_OldGeometry;

	//!
	mitk::DataStorage::Pointer m_DataStorage;

	DECLARE_EVENT_TABLE();
};
 
} // namespace mitk


#endif // wxMitkRenderWindow_H
