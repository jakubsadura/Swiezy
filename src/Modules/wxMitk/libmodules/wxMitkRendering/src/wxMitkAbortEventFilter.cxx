/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
// For compilers that don't support precompilation, include "wx/wx.h"
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
       #include <wx/wx.h>
#endif

#include "wxMitkAbortEventFilter.h"
#include <mitkRenderingManager.h>
#include "wxMitkVTKWindow.h"
#include "wxMitkRenderingRequestEvent.h"

#define wxFilterAllowsToProcessEvent -1
#define wxFilterBlocksEvent 0

using namespace mitk;

// Static member declarations
wxMitkAbortEventFilter::Pointer wxMitkAbortEventFilter::filterInstance = NULL;


//!
wxMitkAbortEventFilter::wxMitkAbortEventFilter(void)
{
	m_LODRendererAtButtonPress = NULL;
	m_ButtonPressed = false;
	m_MouseMoved = false;
}

//!
wxMitkAbortEventFilter::~wxMitkAbortEventFilter(void)
{
}

//!
wxMitkAbortEventFilter* wxMitkAbortEventFilter::GetInstance(void)
{
	if(wxMitkAbortEventFilter::filterInstance.IsNull())
	{
		wxMitkAbortEventFilter::filterInstance = wxMitkAbortEventFilter::New();
	}
	return wxMitkAbortEventFilter::filterInstance;
}

/**
This routine is the one responsible to filter the events and to decide for 
each one, if the rendering has to be aborted and which level of detail 
is applied for the next render.
*/
int wxMitkAbortEventFilter::eventFilter(wxEvent& event)
{

	// Extract renderer (if event has been invoked on a RenderWindow)
	bool isLODRenderer = false;
	mitk::BaseRenderer *renderer = NULL;
	wxObject* object = event.GetEventObject( );
	wxMitkVTKWindow *widget = dynamic_cast< wxMitkVTKWindow * >( object );
	if (widget != NULL )
	{
		renderer = mitk::BaseRenderer::GetInstance( widget->GetRenderWindow() );
		if ( renderer && renderer->GetNumberOfVisibleLODEnabledMappers() > 0 )
		{
			isLODRenderer = true;
		}
		else
		{
			// Only LOD enabled renderers are considered
			renderer = NULL;
		}

	}


	int type = event.GetEventType();
	wxMouseEvent *mEvt = dynamic_cast<wxMouseEvent*>(&event);

	// This code is never executed because in wxWidgets, the
	// main GUI thread blocks all other events until the ProcessEvent finishes
	if (mitk::RenderingManager::GetInstance()->IsRendering() )
	{
		// If mouse push, tell RenderingManager to abort rendering
		if(type == wxEVT_LEFT_DOWN || type == wxEVT_MIDDLE_DOWN || type == wxEVT_RIGHT_DOWN)
		{
			// Let only the first (RenderWindow specific) click event affect
			// the LOD process (Qt issues multiple events on mouse click, but the
			// RenderWindow specific one is issued first)
			if ( !m_ButtonPressed )
			{
				m_ButtonPressed = true;

				// Abort current rendering (for all rendering windows!)
				mitk::RenderingManager::GetInstance()->AbortRendering();

				// Block LOD increase until mouse button is released
				mitk::RenderingManager::GetInstance()->LODIncreaseBlockedOn();

				// Store renderer (if LOD-enabled), otherwise renderer will be NULL
				m_LODRendererAtButtonPress = renderer;
			}

			// Store renderer if it is LOD enabled for mouse release
			if ( isLODRenderer )
			{
				m_LODRendererAtButtonPress = renderer;
			}

			m_EventQueue.push( ObjectEventPair( object, event.Clone()) );
			return wxFilterBlocksEvent;
		}

		// If mouse drags, tell RenderingManager to abort rendering and reset LOD
		// If mouse just moves, do nothing
		else if( mEvt != NULL && mEvt->Dragging() )
		{
			if ( isLODRenderer && mitk::RenderingManager::GetInstance()->GetNextLOD( m_LODRendererAtButtonPress ) != 0 )
			{
				mitk::RenderingManager::GetInstance()->AbortRendering();
			}

			return wxFilterBlocksEvent;
		}


		// Mouse release do not has to abort rendering
		else if(type == wxEVT_LEFT_UP || type == wxEVT_MIDDLE_UP || type == wxEVT_RIGHT_UP)
		{
			if ( m_ButtonPressed )
			{
				m_ButtonPressed = false;

				mitk::RenderingManager::GetInstance()->LODIncreaseBlockedOff();
			}

			m_EventQueue.push( ObjectEventPair( object, event.Clone()) );
			return wxFilterBlocksEvent;
		}

		// Wheel
		else if(type == wxEVT_MOUSEWHEEL )
		{
			mitk::RenderingManager::GetInstance()->AbortRendering();

			m_EventQueue.push( ObjectEventPair( object, event.Clone()) );
			return wxFilterBlocksEvent;
		}

		else if(type == wxEVT_KEY_DOWN )
		{ 
			mitk::RenderingManager::GetInstance()->AbortRendering();

			m_EventQueue.push( ObjectEventPair( object, event.Clone()) );
			return wxFilterBlocksEvent;
		}

		//case QEvent::ChildAdded: //change Layout (Big3D, 2D images up, etc.)	 
		//	{	 
		//		QChildEvent* ce = ( QChildEvent* )( event );	 
		//		QChildEvent* newEvent = new QChildEvent(	 
		//			QEvent::ChildAdded, ce->child() );	 
		//		m_EventQueue.push( ObjectEventPair(GuardedObject( object ), newEvent) );
		//		return true;	 
		//	}	 

		//case QEvent::ChildRemoved: //change Layout (Big3D, 2D images up, etc.)	 
		//	{	 
		//		QChildEvent* ce = ( QChildEvent* )( event );	 
		//		QChildEvent* newEvent = new QChildEvent(	 
		//			QEvent::ChildRemoved, ce->child() );	 
		//		m_EventQueue.push( ObjectEventPair(GuardedObject( object ), newEvent) );
		//		return true;	 
		//	}	 

		//case QEvent::Show:	 
		//	{	 
		//		QShowEvent* newEvent = new QShowEvent();	 
		//		m_EventQueue.push( ObjectEventPair(GuardedObject( object ), newEvent) );
		//		return true;	 
		//	}	 

		//case QEvent::Hide:	 
		//	{	 
		//		QHideEvent* newEvent = new QHideEvent();	 
		//		m_EventQueue.push( ObjectEventPair(GuardedObject( object ), newEvent) );
		//		return true;
		//	}

		//case QEvent::Close:
		//	{
		//		QCloseEvent* newEvent = new QCloseEvent();
		//		m_EventQueue.push( ObjectEventPair(GuardedObject( object ), newEvent) );
		//		return true;
		//	}

		//case QEvent::ContextMenu:
		//	{
		//		QContextMenuEvent *cme = ( QContextMenuEvent * )( event );
		//		QContextMenuEvent *newEvent = new QContextMenuEvent(
		//			cme->reason(), cme->pos(), cme->globalPos() );
		//		m_EventQueue.push( ObjectEventPair(GuardedObject( object ), newEvent) );
		//		return true;
		//	}

		//case QEvent::FocusIn:
		//	{
		//		QFocusEvent *fe = (QFocusEvent *)( event );
		//		QFocusEvent *newEvent = new QFocusEvent( QEvent::FocusIn, fe->reason() );
		//		m_EventQueue.push( ObjectEventPair(GuardedObject( object ), newEvent) );
		//		return true;
		//	}

		//case QEvent::FocusOut:
		//	{
		//		QFocusEvent *fe = (QFocusEvent *)( event );
		//		QFocusEvent *newEvent = new QFocusEvent( QEvent::FocusOut, fe->reason() );
		//		m_EventQueue.push( ObjectEventPair(GuardedObject( object ), newEvent) );
		//		return true;
		//	}

		//case QEvent::Resize: 	 
		//	{ 	 
		//		QResizeEvent *re = (QResizeEvent *)( event );
		//		QResizeEvent *newEvent = new QResizeEvent(
		//			re->size(), re->oldSize() );
		//		m_EventQueue.push( ObjectEventPair(GuardedObject( object ), newEvent) );
		//		return true; 	 
		//	}

		else if(type == wxEVT_TIMER )
		{ 
			m_EventQueue.push( ObjectEventPair( object, event.Clone()) );
			return wxFilterBlocksEvent;
		}
		else 
		{
			m_EventQueue.push( ObjectEventPair( object, event.Clone()) );
			return wxFilterBlocksEvent;
		}
	}
	else
	{
		// If mouse push, tell RenderingManager to abort rendering
		if(type == wxEVT_LEFT_DOWN || type == wxEVT_MIDDLE_DOWN || type == wxEVT_RIGHT_DOWN)
		{
			// Let only the first (RenderWindow specific) click event affect
			// the LOD process (Qt issues multiple events on mouse click, but the
			// RenderWindow specific one is issued first)
			if ( !m_ButtonPressed )
			{
				m_ButtonPressed = true;
				mitk::RenderingManager::GetInstance()->LODIncreaseBlockedOn();

				m_LODRendererAtButtonPress = renderer;
			}

			// Store renderer if it is LOD enabled for mouse release
			if ( isLODRenderer )
			{
				m_LODRendererAtButtonPress = renderer;
			}

			return wxFilterAllowsToProcessEvent;
		}

		// Moving
		else if ( mEvt != NULL && mEvt->Moving( ) )
		{
			// Nothing to do in this case
			return wxFilterAllowsToProcessEvent;
		}

		else if ( type == wxEVT_MOUSEWHEEL )
		{
			//mitk::RenderingManager::GetInstance()->RequestUpdateAll(
			//  mitk::RenderingManager::REQUEST_UPDATE_3DWINDOWS );
			return wxFilterAllowsToProcessEvent;
		}     

		// Mouse release do not has to abort rendering
		if(type == wxEVT_LEFT_UP || type == wxEVT_MIDDLE_UP || type == wxEVT_RIGHT_UP)
		{
			if ( m_ButtonPressed )
			{
				m_ButtonPressed = false;
				mitk::RenderingManager::GetInstance()->LODIncreaseBlockedOff();

				if ( m_LODRendererAtButtonPress != NULL )
				{
					mitk::RenderingManager::GetInstance()->RequestUpdate(
						m_LODRendererAtButtonPress->GetRenderWindow() );
				}
				else
				{
					mitk::RenderingManager::GetInstance()->RequestUpdateAll(
						mitk::RenderingManager::REQUEST_UPDATE_3DWINDOWS );
				}
			}
			return wxFilterAllowsToProcessEvent;
		}
		else
		{
			return wxFilterAllowsToProcessEvent;
		}
	}

	// Any other event is not to be blocked
	return wxFilterAllowsToProcessEvent;
}



void mitk::wxMitkAbortEventFilter::ProcessEvents()
{

}

void mitk::wxMitkAbortEventFilter::IssueQueuedEvents()
{
	bool renderingRequested = false;

	while ( !m_EventQueue.empty() )
	{
		ObjectEventPair eventPair = m_EventQueue.front();
		if ( eventPair.first )
		{
			if ( eventPair.second->GetEventType() == wxEVT_RENDERING_REQUEST )
			{
				// Rendering requests are issued only after all other events, to
				// ensure that they are executed BEFORE rendering, to avoid endless
				// loops
				renderingRequested = true;
			}
			else
			{
				wxTheApp->AddPendingEvent( *eventPair.second );
			}
		}
		delete eventPair.second;
		m_EventQueue.pop();
	}

	// If a rendering request has been posed, issue it now at the end
	if ( renderingRequested )
	{
		wxMitkRenderingRequestEvent event;
		wxTheApp->AddPendingEvent( event );
	}
}
