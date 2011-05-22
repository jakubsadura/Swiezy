/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _wxMitkMultiRenderWindowConfig_H
#define _wxMitkMultiRenderWindowConfig_H

#include "wxMitkMultiRenderWindowConfigUI.h"
#include "wxMitkWidgetsWin32Header.h"


namespace mitk
{
class wxMitkViewConfiguration;
class wxMitkMultiRenderWindow;

/**
\author Xavi Planes
\ingroup wxMitkWidgets
\date 10 Jan 2009
*/
class WXMITKWIDGETS_EXPORT wxMitkMultiRenderWindowConfig : 
	public wxMitkMultiRenderWindowConfigUI {
public:
	//!
    wxMitkMultiRenderWindowConfig(wxWindow* parent, int id, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=0);

	//! Set view to manage
	void SetView( mitk::wxMitkMultiRenderWindow* view );

private:
	//!
	void UpdateWidget( );

	//!
	bool ApplyConfigToAllSlices( bool direction, bool flip, bool rotation );

	//!
	void SetActiveWidget( int i );

	//!
	void SetActiveWidget( std::string name );

	virtual void OnDirection(wxCommandEvent &event);
	virtual void OnFlip(wxCommandEvent &event);
	virtual void OnRotate(wxCommandEvent &event);
	virtual void OnApplyAll(wxCommandEvent &event);
	virtual void OnSelectedView(wxCommandEvent &event);
	virtual void OnNumberOfSlices(wxSpinEvent &event);

	DECLARE_EVENT_TABLE( );

protected:
	mitk::wxMitkMultiRenderWindow* m_View;

};

} // namespace mitk

#endif // _wxMitkMultiRenderWindowConfig_H
