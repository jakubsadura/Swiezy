/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef wxMitkRenderingManager_H
#define wxMitkRenderingManager_H

#include "wxMitkRenderingWin32Header.h"
#include <mitkRenderingManager.h>
#include <wx/event.h>
#include <wx/timer.h>

namespace mitk
{
/**
\brief wxWidgets specific implementation of mitk::RenderingManager.

This implementation uses a wxTimer object to realize the RenderWindow
update timing. The execution of pending updates is controlled by the timer.

\ingroup wxMitkRendering
\author Juan Antonio Moya
\date 20 Sep 2007
 */
class WXMITKRENDERING_EXPORT wxMitkRenderingManager 
	: public wxEvtHandler, public RenderingManager
{

public:
	mitkClassMacro( wxMitkRenderingManager, mitk::RenderingManager );
	itkFactorylessNewMacro(Self);

	//!
	void OnTimer(wxTimerEvent& event);

	//!
	void GenerateRenderingRequestEvent( );

	//!
	bool ProcessEvent(wxEvent& event);

	//!
	virtual void DoMonitorRendering();

	//!
	virtual void DoFinishAbortRendering();

protected:
	wxMitkRenderingManager(void);
	virtual ~wxMitkRenderingManager(void);

	void RestartTimer(void);
	void StopTimer(void);

	void OnRestartTimer(wxCommandEvent& evt);

	void RestartTimerInternal(void);

	wxTimer m_Timer;

	DECLARE_EVENT_TABLE();
};

} // namespace mitk

#endif // wxMitkRenderingManager_H
