/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreWorkingAreaManager.h"
#include "coreRenderWindowContainer.h"
#include "coreBaseWindow.h"
#include "coreBaseWindowFactories.h"
#include "corePluginTab.h"
#include "coreWxMitkGraphicalInterface.h"

#include <wx/wupdlock.h>

Core::Widgets::WorkingAreaManager::WorkingAreaManager(
	wxWindow* parent, wxWindowID id, const wxPoint& pos /*= wxDefaultPosition*/, 
	const wxSize& size /*= wxDefaultSize*/, long style /*= 0*/,
	const wxString&  name /*= wxPanelNameStr */) 
	: wxWidgetStackControl( parent, id, pos, size, style, name )
{
	m_ActiveMultiRenderWindowHolder = RenderWindowHolderType::New( );
	m_ActiveMultiRenderWindowHolder->SetSubject( NULL );
	m_ActiveWorkingAreaHolder = IntHolderType::New();
	m_ActiveWorkingAreaHolder->SetSubject( -1 );
}

Core::Widgets::WorkingAreaManager::~WorkingAreaManager()
{

}

void Core::Widgets::WorkingAreaManager::AddWorkingArea( wxWindow* aWorkingArea )
{
	std::map<int, wxWindow*>::iterator it = m_WorkingAreaMap.find( aWorkingArea->GetId() );
	if ( it != m_WorkingAreaMap.end( ) )
	{
		return ;
	}

	Add( aWorkingArea );
	m_WorkingAreaMap[ aWorkingArea->GetId() ] = aWorkingArea;
	aWorkingArea->Enable( false );

	// Add observer to the selected RenderWindowBase
	Core::Widgets::RenderWindowContainer* multiRenderWindowContainer = NULL;
	multiRenderWindowContainer =
		dynamic_cast<Core::Widgets::RenderWindowContainer*> ( aWorkingArea );
	if ( multiRenderWindowContainer )
	{
		multiRenderWindowContainer->SetActiveWindowHolder( m_ActiveMultiRenderWindowHolder );
	}
}

void Core::Widgets::WorkingAreaManager::SetActiveWorkingArea( int ID )
{
	if ( GetActiveWorkingAreaID() == ID )
	{
		return;
	}

	wxWindowUpdateLocker noUpdates( this );

	std::map<int, wxWindow*>::iterator it = m_WorkingAreaMap.find( ID );
	if ( it != m_WorkingAreaMap.end( ) )
	{
		// Disable the old one
		if ( GetActiveWorkingArea() )
		{
			GetActiveWorkingArea()->Enable( false );
		}

		// Set selected working area. This statement should be called
		// before Enable because Enable will call SetActiveWindow and this
		// will change the active RenderWindowBase (m_ActiveMultiRenderWindowHolder)
		// but the m_ActiveWorkingAreaHolder will be the old one
		m_ActiveWorkingAreaHolder->SetSubject( ID );

		// Enable the new one
		m_WorkingAreaMap[ ID ]->Enable( true );
		Raise( m_WorkingAreaMap[ ID ] );

		wxSizeEvent resEvent(this->GetBestSize(), this->GetId());
		resEvent.SetEventObject(this);
		this->GetEventHandler()->ProcessEvent(resEvent);
	}
	else
	{
		m_ActiveWorkingAreaHolder->SetSubject( -1 );
		// Remove references to active RenderWindowBase of current working area
		m_ActiveMultiRenderWindowHolder->SetSubject( NULL );
	}


}

wxWindow* Core::Widgets::WorkingAreaManager::GetWorkingArea( int ID )
{
	std::map<int, wxWindow*>::iterator it = m_WorkingAreaMap.find( ID );
	if ( it != m_WorkingAreaMap.end( ) )
	{
		return m_WorkingAreaMap[ ID ];
	}
	return NULL;
}

wxWindow* Core::Widgets::WorkingAreaManager::GetWorkingArea( 
	const wxString &name )
{
	std::map<int, wxWindow*>::iterator it;
	for ( it = m_WorkingAreaMap.begin( ) ; 
		it != m_WorkingAreaMap.end( ); 
		it++ )
	{
		if ( name == it->second->GetName( ) )
		{
			return it->second;
		}
	}
	return NULL;
}

int Core::Widgets::WorkingAreaManager::GetActiveWorkingAreaID()
{
	return m_ActiveWorkingAreaHolder->GetSubject();
}

void Core::Widgets::WorkingAreaManager::RemoveWorkingArea( int ID )
{
	// If it's the current displayed, unselect it
	if ( ID == GetActiveWorkingAreaID( ) )
	{
		SetActiveWorkingArea( -1 );
	}

	wxWindow* window = GetWorkingArea( ID );
	if ( window )
	{
		wxWindow* window = GetWorkingArea( ID );
		m_WorkingAreaMap.erase( ID );
		Remove( window );
	}
}

wxWindow* Core::Widgets::WorkingAreaManager::GetActiveWorkingArea()
{
	return GetWorkingArea( GetActiveWorkingAreaID() );
}

Core::Widgets::RenderWindowBase* 
Core::Widgets::WorkingAreaManager::GetActiveMultiRenderWindow() const
{
	return m_ActiveMultiRenderWindowHolder->GetSubject();
}

blTagMap::Pointer
Core::Widgets::WorkingAreaManager::GetActiveWindowMetadata()
{
	blTagMap::Pointer metadata;
	Core::Widgets::RenderWindowBase* multiRenderWindow = NULL;
	multiRenderWindow = GetActiveMultiRenderWindow( );
	if ( multiRenderWindow != NULL )
	{
		metadata = multiRenderWindow->GetMetadata( );
	}
	return metadata;
}


void Core::Widgets::WorkingAreaManager::AddObserverOnActiveWindow( 
	Core::BaseWindow* baseWindow )
{
	m_ActiveMultiRenderWindowHolder->AddObserver1( 
		baseWindow, 
		&Core::BaseWindow::SetMultiRenderWindow );
}

void Core::Widgets::WorkingAreaManager::RemoveObserverOnActiveWindow( Core::BaseWindow* baseWindow )
{
	m_ActiveMultiRenderWindowHolder->RemoveObserver1( 
		baseWindow, 
		&Core::BaseWindow::SetMultiRenderWindow,
		Core::DH_NEW_SUBJECT );
}

Core::Widgets::RenderWindowHolderType::Pointer 
Core::Widgets::WorkingAreaManager::GetActiveMultiRenderWindowHolder() const
{
	return m_ActiveMultiRenderWindowHolder;
}

bool Core::Widgets::WorkingAreaManager::Enable( bool enable )
{
	// Don't call enable to the parent class because this will
	// enable all child windows
	// bool val = wxWidgetStackControl::Enable( enable );

	if ( enable && GetActiveWorkingArea() )
	{
		GetActiveWorkingArea()->Enable( true );
	}
	else
	{
		std::map<int, wxWindow*>::iterator it;
		for ( it = m_WorkingAreaMap.begin( ) ; 
			  it != m_WorkingAreaMap.end( ); 
			  it++ )
		{
			it->second->Enable( false );
		}

	}

	return true;
}

Core::IntHolderType::Pointer 
Core::Widgets::WorkingAreaManager::GetActiveWorkingAreaHolder() const
{
	return m_ActiveWorkingAreaHolder;
}

Core::BaseProcessor::Pointer Core::Widgets::WorkingAreaManager::GetProcessor()
{
	return NULL;
}

std::map<int, wxWindow*> Core::Widgets::WorkingAreaManager::GetWorkingAreaMap() const
{
	return m_WorkingAreaMap;
}
