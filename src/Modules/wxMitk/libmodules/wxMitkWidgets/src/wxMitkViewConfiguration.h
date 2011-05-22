/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef WXMITKVIEWCONFIGURATION_H
#define WXMITKVIEWCONFIGURATION_H

#include "wxMitkViewConfigurationUI.h"
#include "wxMitkWidgetsWin32Header.h"


namespace mitk
{
class wxMitkSelectableGLWidget;

/**
\author Xavi Planes
\ingroup wxMitkWidgets
\date 10 Jan 2009
*/
class WXMITKWIDGETS_EXPORT wxMitkViewConfiguration : public wxMitkViewConfigurationUI {
public:
	//!
    wxMitkViewConfiguration(wxWindow* parent, int id, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=0);

	//!
	~wxMitkViewConfiguration( );

	//! Set view to manage
	void SetView( mitk::wxMitkSelectableGLWidget* view );
	mitk::wxMitkSelectableGLWidget* GetView( );

	//!
	mitk::SliceNavigationController::ViewDirection GetDirection( );

	//!
	bool GetFlip( );

	//!
	mitk::ScalarType GetAngleOfRotation( );


private:
	//!
	void UpdateWidget( );

    virtual void OnDirection(wxCommandEvent &event);
    virtual void OnFlip(wxCommandEvent &event);
    virtual void OnRotate(wxCommandEvent &event);

protected:

	//!
	mitk::wxMitkSelectableGLWidget* m_View;

	//!
	unsigned long m_ObserverTag;

};

} // namespace mitk

#endif // WXMITKVIEWCONFIGURATION_H
