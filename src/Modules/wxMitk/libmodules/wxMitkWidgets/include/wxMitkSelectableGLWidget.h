/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef wxMitkSelectableGLWidget_H
#define wxMitkSelectableGLWidget_H

#include "wxMitkWidgetsWin32Header.h"
#include "mitkVtkPropRenderer.h"
#include <wx/panel.h>
#include "blMitkCornerAnnotation.h"
#include "itkCommand.h"

class wxColour;
class vtkRenderWindow;

namespace mitk
{

class wxMitkRenderWindow;
class SliceNavigationController;
class CameraRotationController;
class BaseController;
class wxMitkMultiRenderWindow;

/**
The wxMitkSelectableGLWidget is responsible for holding the 
wxMitkRenderWindow and provide easy access to its members.

\note Name of the widget is used to access to the node in the rendering tree.
Be carefull to use a unique name

\ingroup wxMitkWidgets
\author Juan Antonio Moya
\date 18 Sep 2007
*/
class WXMITKWIDGETS_EXPORT wxMitkSelectableGLWidget : public wxPanel
{
public:
	static wxColor RED_COLOR;
	static wxColor GREEN_COLOR;
	static wxColor BLUE_COLOR;
	static wxColor YELLOW_COLOR;
	static wxColor BLACK_COLOR;

public:
	wxMitkSelectableGLWidget(
		wxWindow* parent, 
		wxWindowID id, 
		const wxString& name,
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = 0 );
	virtual ~wxMitkSelectableGLWidget(void);

	mitk::VtkPropRenderer* GetRenderer(void) const;
	wxMitkRenderWindow* GetMitkRenderWindow(void) const;
	vtkRenderWindow* GetVtkRenderWindow(void) const;
	mitk::SliceNavigationController* GetSliceNavigationController(void) const;
	mitk::CameraRotationController* GetCameraRotationController(void) const;
	mitk::BaseController* GetController(void) const;

	void SetFrameBorderColor(wxColour& colour);
	wxColour GetFrameBorderColor();
	void GetColourAsFloat( float colour[3] );

	void RequestUpdate(void);
	void ForceImmediateUpdate(void);

	//! Enable the window (optimize speed)
	bool Enable(bool enable = true );

	//!
	void EnableCornerAnnotations(bool enable);

	//!
	bool GetCornerAnnotationsEnabled();

	//!
	void EnableOrientationAnnotations( bool enable );

	//!
	bool GetOrientationAnnotationsEnabled();

	//!
	void UpdateAnnotations();

	//!
	blMitkCornerAnnotation::Pointer GetCornerAnnotation() const;

	//!
	void UpdateController( 
		mitk::TimeSlicedGeometry::Pointer geometry, 
		bool keepOrientation );

	//!
	template <class T>
	void AddSlicePosObserver( T* object, typename itk::MemberCommand<T>::TConstMemberFunctionPointer f )
	{
		typename itk::MemberCommand<T>::Pointer command;
		command = itk::MemberCommand<T>::New( );
		command->SetCallbackFunction(object, f);
		m_SlicePosObserverTag = GetSliceNavigationController()->GetSlice()->AddObserver(
			itk::ModifiedEvent(), 
			command );
	}


	//!
	void RemoveSlicePosObserver( );

private:
	//!
	void OnModifiedNavigationController( );

	//! Update frame color and node color
	void UpdateColor( );

protected:
	//!
	wxMitkRenderWindow* internalRenderWindow;

	//!
	blMitkCornerAnnotation::Pointer m_CornerAnnotation;

	//!
	unsigned long m_NavigationControllerObserverTag;

	//!
	unsigned long m_SlicePosObserverTag;

};
 
} // namespace mitk


#endif // wxMitkSelectableGLWidget_H
