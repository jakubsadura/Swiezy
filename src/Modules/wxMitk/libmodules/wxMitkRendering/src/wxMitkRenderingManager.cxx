/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#include "wxMitkRenderingManager.h"
#include "wxMitkRenderingRequestEvent.h"
#include "wxMitkAbortEventFilter.h"
#include "wxID.h"

using namespace mitk;

#define ID_wxMitkRenderingManager_TIMER wxID( "ID_wxMitkRenderingManager_TIMER" )

DEFINE_EVENT_TYPE(wxEVT_RESTART_TIMER)

BEGIN_EVENT_TABLE(wxMitkRenderingManager, wxEvtHandler)
    EVT_TIMER   (ID_wxMitkRenderingManager_TIMER, wxMitkRenderingManager::OnTimer)
	EVT_COMMAND	(wxID_ANY, wxEVT_RESTART_TIMER, wxMitkRenderingManager::OnRestartTimer )
END_EVENT_TABLE()

//!
wxMitkRenderingManager::wxMitkRenderingManager() : wxEvtHandler(), RenderingManager()
{
	m_Timer.SetOwner(this, ID_wxMitkRenderingManager_TIMER);
}

//!
wxMitkRenderingManager::~wxMitkRenderingManager()
{
}

//!
void wxMitkRenderingManager::RestartTimer()
{
	wxCommandEvent event( wxEVT_RESTART_TIMER, wxID_ANY );
	event.SetEventObject(this);
	AddPendingEvent(event);
}

//!
void wxMitkRenderingManager::StopTimer()
{
	m_Timer.Stop();
}

/** 
Receives the time elapsed events from wxWidgets timer and calls the Update callback of the 
RenderingManager, triggering the update to all registered renderwindows
*/
void wxMitkRenderingManager::OnTimer(wxTimerEvent& event)
{
	wxMitkRenderingRequestEvent renderingEvent;
	this->AddPendingEvent( renderingEvent );
}

void mitk::wxMitkRenderingManager::GenerateRenderingRequestEvent()
{
	RestartTimer();
}

bool mitk::wxMitkRenderingManager::ProcessEvent(wxEvent& event)
{
	if ( event.GetEventType() == wxEVT_RENDERING_REQUEST )
	{
		// Directly process all pending rendering requests
		this->UpdateCallback();
		return true;
	}
	else
	{
		return wxEvtHandler::ProcessEvent( event );
	}

	return false;
}

void mitk::wxMitkRenderingManager::DoFinishAbortRendering()
{
	wxMitkAbortEventFilter::GetInstance()->IssueQueuedEvents();
}

void mitk::wxMitkRenderingManager::DoMonitorRendering()
{
	wxMitkAbortEventFilter::GetInstance()->ProcessEvents();
}

void mitk::wxMitkRenderingManager::OnRestartTimer( wxCommandEvent& evt )
{
	RestartTimerInternal( );
}

void mitk::wxMitkRenderingManager::RestartTimerInternal( void )
{
	// 5 milliseconds
	m_Timer.Start( 100, wxTIMER_ONE_SHOT);
}
