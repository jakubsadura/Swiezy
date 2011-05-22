/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreRenderWindowBase_H
#define coreRenderWindowBase_H

#include "gmWidgetsWin32Header.h"
#include "coreObject.h"
#include "coreDataEntityHolder.h"
#include "coreRenderingTree.h"
#include "blTagMap.h"
#include "coreBaseWindow.h"
#include "coreCamera3D.h"
#include "coreSlicePlane.h"

namespace Core
{
namespace Widgets
{


/** 
\brief Base class for all RenderWindowBase

You can create a subclass for MITK, CMGUI or other rendering libraries

\ingroup gmWidgets
\author Xavi Planes
\date Oct 2010
*/
class GMWIDGETS_EXPORT RenderWindowBase : public Core::BaseWindow
{
public:
	coreClassNameMacro(RenderWindowBase);

	RenderWindowBase( );
	virtual ~RenderWindowBase(void);

	//!
	virtual void Init(
		Core::DataEntityHolder::Pointer selectedDataEntity,
		bool enablePlanes = true);

	//! Called each time the RenderingTree changes
	virtual void RenderScene(void) = 0;

	//!
	virtual Core::RenderingTree::Pointer GetPrivateRenderingTree() const = 0;

	//! Enable the render windows (optimize speed) and interactors
	virtual bool Enable(bool enable = true ) = 0;

	//!
	virtual std::string GetName( ) = 0;

	//!
	virtual void SetPrivateRenderingTree(Core::RenderingTree::Pointer val);

	//! Redefined
	virtual void GetProperties( blTagMap::Pointer tagMap );

	//! Redefined
	virtual void SetProperties( blTagMap::Pointer tagMap );

	//!
	virtual void SaveImage3DWindow( const std::string& completeFileName );

	//!
	virtual void CenterToPoint( double p[3] );

	//!
	virtual void SetEvtHandlerEnabled( bool enable );

	//!
	virtual void OnDataSelected( Core::DataEntity::Pointer dataEntity );

	//!
	virtual int GetTime( ) = 0;

	//!
	virtual void SetTime( int time ) = 0;

	//!
	virtual void GetTimeRange( int &min, int &max ) = 0;

	//!
	virtual void InitDefaultLayout( );

	//! Update 3D camera when Camera3DHolder changes
	virtual void UpdateCamera3D( Camera3D::Pointer cam );

	//! Update slice planes 
	virtual void UpdateSlicePlane( SlicePlane::Pointer slicePlane );

	//! Update all render windows
	virtual void RequestUpdateAll( ) = 0;

	//!
	bool GetEnableTimeUpdates() const;
	void SetEnableTimeUpdates(bool val);

	//! Set holder that notifies changes on 3D camera
	void SetCamera3DHolder(Camera3DHolderType::Pointer val);
	Camera3DHolderType::Pointer GetCamera3DHolder() const;

	//! Avoid infinite loops
	void Lock3DCamera( bool lock = true );
	bool GetLock3DCamera() const;

	//! Set holder that notifies changes on slice planes
	SlicePlaneHolderType::Pointer GetSlicePlaneHolder() const;
	void SetSlicePlaneHolder(SlicePlaneHolderType::Pointer val);

	//! Avoid infinite loops
	bool GetLockSlicePlanes() const;
	void SetLockSlicePlanes(bool val);

	//! Get position of all slice planes
	virtual void GetSlicePlanesPositions( std::list<SlicePlane::Pointer> &slicePlanesPositions );

private:
	//!
	virtual void SetRenderingTreeInstance(Core::RenderingTree::Pointer val) = 0;

private:
	//! Remove it at destructor
	boost::signals::connection	m_RenderingTreeConnection;

	//! Enable time updates when calling SetTimeStep( )
	bool m_EnableTimeUpdates;

	//!
	Camera3DHolderType::Pointer m_Camera3DHolder;

	//!
	bool m_Lock3DCamera;

	//!
	SlicePlaneHolderType::Pointer m_SlicePlaneHolder;

	//!
	bool m_LockSlicePlanes;
};

typedef DataHolder<RenderWindowBase*> RenderWindowHolderType;

}
}

#endif // coreMultiRenderWindowWidget_H
