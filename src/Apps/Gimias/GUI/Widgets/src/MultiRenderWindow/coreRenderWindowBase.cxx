/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreMultiRenderWindow.h"
#include "coreDataEntity.h"
#include "coreAssert.h"
#include "coreKernel.h"
#include "coreSettings.h"
#include "coreException.h"
#include "coreReportExceptionMacros.h"

using namespace Core::Widgets;

//!
RenderWindowBase::RenderWindowBase()
{
	// Setup the logo
	Core::Runtime::Settings::Pointer settings = Core::Runtime::Kernel::GetApplicationSettings();
	coreAssertMacro(settings.IsNotNull() && "The Settings Manager must have been initialized");

	m_EnableTimeUpdates = false;
	m_Lock3DCamera = false;
	m_LockSlicePlanes = false;
}

//!
RenderWindowBase::~RenderWindowBase(void)
{
	if ( m_RenderingTreeConnection.connected() )
	{
		m_RenderingTreeConnection.disconnect();
	}
}

void Core::Widgets::RenderWindowBase::SetPrivateRenderingTree( 
	Core::RenderingTree::Pointer val )
{
	m_RenderingTreeConnection.disconnect( );

	SetRenderingTreeInstance( val );

	if ( GetPrivateRenderingTree( ).IsNotNull() )
	{
		m_RenderingTreeConnection = GetPrivateRenderingTree( )->AddObserverOnModified<RenderWindowBase>(
			this, 
			&RenderWindowBase::RenderScene);
	}
}

void Core::Widgets::RenderWindowBase::SaveImage3DWindow( const std::string& completeFileName )
{
	
}

void Core::Widgets::RenderWindowBase::CenterToPoint( double p[3] )
{

}

void Core::Widgets::RenderWindowBase::SetEvtHandlerEnabled( bool enable )
{

}

void Core::Widgets::RenderWindowBase::OnDataSelected( Core::DataEntity::Pointer dataEntity )
{

}

void Core::Widgets::RenderWindowBase::GetProperties( blTagMap::Pointer tagMap )
{

}

void Core::Widgets::RenderWindowBase::SetProperties( blTagMap::Pointer tagMap )
{

}

void Core::Widgets::RenderWindowBase::Init( Core::DataEntityHolder::Pointer selectedDataEntity, bool enablePlanes /*= true*/ )
{

}
bool Core::Widgets::RenderWindowBase::GetEnableTimeUpdates() const
{
	return m_EnableTimeUpdates;
}

void Core::Widgets::RenderWindowBase::SetEnableTimeUpdates( bool val )
{
	m_EnableTimeUpdates = val;
}

void Core::Widgets::RenderWindowBase::SetCamera3DHolder( Camera3DHolderType::Pointer val )
{
	if ( m_Camera3DHolder.IsNotNull() )
	{
		m_Camera3DHolder->RemoveObserver1<RenderWindowBase>( this, &RenderWindowBase::UpdateCamera3D );
	}

	m_Camera3DHolder = val;

	if ( m_Camera3DHolder.IsNotNull() )
	{
		m_Camera3DHolder->AddObserver1<RenderWindowBase>( this, &RenderWindowBase::UpdateCamera3D );
	}
}

Core::Camera3DHolderType::Pointer 
Core::Widgets::RenderWindowBase::GetCamera3DHolder() const
{
	return m_Camera3DHolder;
}

void Core::Widgets::RenderWindowBase::UpdateCamera3D( Camera3D::Pointer cam )
{
	
}

void Core::Widgets::RenderWindowBase::Lock3DCamera( bool lock /*= true */ )
{
	m_Lock3DCamera = lock;
}

bool Core::Widgets::RenderWindowBase::GetLock3DCamera() const
{
	return m_Lock3DCamera;
}

Core::SlicePlaneHolderType::Pointer 
Core::Widgets::RenderWindowBase::GetSlicePlaneHolder() const
{
	return m_SlicePlaneHolder;
}

void Core::Widgets::RenderWindowBase::SetSlicePlaneHolder( 
	SlicePlaneHolderType::Pointer val )
{
	if ( m_SlicePlaneHolder.IsNotNull() )
	{
		m_SlicePlaneHolder->RemoveObserver1<RenderWindowBase>( this, &RenderWindowBase::UpdateSlicePlane );
	}

	m_SlicePlaneHolder = val;

	if ( m_SlicePlaneHolder.IsNotNull() )
	{
		m_SlicePlaneHolder->AddObserver1<RenderWindowBase>( this, &RenderWindowBase::UpdateSlicePlane );
	}
}

bool Core::Widgets::RenderWindowBase::GetLockSlicePlanes() const
{
	return m_LockSlicePlanes;
}

void Core::Widgets::RenderWindowBase::SetLockSlicePlanes( bool val )
{
	m_LockSlicePlanes = val;
}

void Core::Widgets::RenderWindowBase::UpdateSlicePlane( 
	SlicePlane::Pointer slicePlane )
{

}

void Core::Widgets::RenderWindowBase::InitDefaultLayout()
{

}

void Core::Widgets::RenderWindowBase::GetSlicePlanesPositions( 
	std::list<SlicePlane::Pointer> &slicePlanesPositions )
{

}
