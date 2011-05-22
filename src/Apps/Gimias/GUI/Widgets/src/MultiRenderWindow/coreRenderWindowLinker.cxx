/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreRenderWindowLinker.h"
#include "coreMultiRenderWindow.h"

using namespace Core::Widgets;

//!
RenderWindowLinker::RenderWindowLinker( )
{
	m_EnableLink3DViews = false;
	m_EnableLink2DViews = false;
	m_EnableLinkTime = false;
	m_Camera3DHolder = Camera3DHolderType::New();
	m_Camera3DHolder->SetSubject( Camera3D::New() );
	m_Camera3DHolder->GetSubject()->m_initialized = false;
	m_SlicePlaneHolder = SlicePlaneHolderType::New( );
}

//!
RenderWindowLinker::~RenderWindowLinker(void)
{
}


void Core::Widgets::RenderWindowLinker::AddWindow( 
	RenderWindowBase* window )
{
	m_WindowList.push_back( window );
}

bool Core::Widgets::RenderWindowLinker::GetEnableLink3DViews() const
{
	return m_EnableLink3DViews;
}

void Core::Widgets::RenderWindowLinker::SetEnableLink3DViews( bool val )
{
	m_EnableLink3DViews = val;
}

void Core::Widgets::RenderWindowLinker::Update()
{
	if ( m_WindowList.size() == 0 )
	{
		return;
	}

	EnableLink3DViews( );

	EnableLink2DViews( );

	EnableLinkTime( );
}

void Core::Widgets::RenderWindowLinker::RemoveWindow( RenderWindowBase* window )
{
	m_WindowList.remove( window );
}

bool Core::Widgets::RenderWindowLinker::GetEnableLink2DViews() const
{
	return m_EnableLink2DViews;
}

void Core::Widgets::RenderWindowLinker::SetEnableLink2DViews( bool val )
{
	m_EnableLink2DViews = val;
}

void Core::Widgets::RenderWindowLinker::EnableLink3DViews()
{

	std::list<RenderWindowBase*>::iterator it;
	for ( it = m_WindowList.begin() ; 
		it != m_WindowList.end() ; 
		it++ )
	{
		if ( m_EnableLink3DViews )
		{
			(*it)->SetCamera3DHolder( m_Camera3DHolder );
		}
		else
		{
			(*it)->SetCamera3DHolder( NULL );
		}
	}
}

void Core::Widgets::RenderWindowLinker::EnableLink2DViews()
{
	std::list<RenderWindowBase*>::iterator it;
	for ( it = m_WindowList.begin() ; 
		it != m_WindowList.end() ; 
		it++ )
	{

		if ( m_EnableLink2DViews )
		{
			(*it)->SetSlicePlaneHolder( m_SlicePlaneHolder );
		}
		else
		{
			(*it)->SetSlicePlaneHolder( NULL );
		}
	}
}

bool Core::Widgets::RenderWindowLinker::GetEnableLinkTime() const
{
	return m_EnableLinkTime;
}

void Core::Widgets::RenderWindowLinker::SetEnableLinkTime( bool val )
{
	m_EnableLinkTime = val;
}

void Core::Widgets::RenderWindowLinker::EnableLinkTime()
{
	std::list<RenderWindowBase*>::iterator it;
	for ( it = m_WindowList.begin() ; 
		it != m_WindowList.end() ; 
		it++ )
	{
		(*it)->SetEnableTimeUpdates( m_EnableLinkTime );
	}	
}
