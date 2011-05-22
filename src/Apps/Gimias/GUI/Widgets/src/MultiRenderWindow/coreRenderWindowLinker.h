/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreRenderWindowLinker_H
#define _coreRenderWindowLinker_H

#include "gmWidgetsWin32Header.h"
#include "coreCamera3D.h"
#include "coreSlicePlane.h"

namespace Core
{
namespace Widgets
{

class vtkCameraCallback;
class RenderWindowBase;

/** 
\brief Link different MultiRenderWindows for 2D or 3D views when the user
interact with the views.

- 2D: Update the 2D slice plane
- 3D: Update the 3D camera

\note When AddWindow( ) or RemoveWindow( ) is called or any parameter 
is changed, you need to call Update( )

\ingroup gmWidgets
\author Xavi Planes
\date 08 Mar 2010
*/
class GMWIDGETS_EXPORT RenderWindowLinker
	: public Core::SmartPointerObject
{
public:
	coreDeclareSmartPointerClassMacro(RenderWindowLinker, Core::SmartPointerObject);

	//!
	void AddWindow( RenderWindowBase* window );

	//!
	void RemoveWindow( RenderWindowBase* window );

	//! 
	bool GetEnableLink3DViews() const;
	void SetEnableLink3DViews(bool val);

	bool GetEnableLink2DViews() const;
	void SetEnableLink2DViews(bool val);

	//!
	bool GetEnableLinkTime() const;
	void SetEnableLinkTime(bool val);

	//!
	void Update( );
private:

	//!
	void EnableLink3DViews( );

	//!
	void EnableLink2DViews( );

	//!
	void EnableLinkTime( );

private:

	//!
	RenderWindowLinker();

	//!
	virtual ~RenderWindowLinker(void);

private:

	//!
	bool m_EnableLink3DViews;

	//!
	bool m_EnableLink2DViews;

	//!
	bool m_EnableLinkTime;

	//!
	std::list<RenderWindowBase*> m_WindowList;

	//!
	Camera3DHolderType::Pointer m_Camera3DHolder;
	
	//!
	SlicePlaneHolderType::Pointer m_SlicePlaneHolder;
};

}
}

#endif // _coreRenderWindowLinker_H
