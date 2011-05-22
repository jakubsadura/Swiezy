/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreCamera3D_H
#define coreCamera3D_H

#include "gmWidgetsWin32Header.h"
#include "coreObject.h"

namespace Core
{

/** 
3D Camera information to pass to other windows
\ingroup gmWidgets
\author Xavi Planes
\date Nov 2010
*/
class Camera3D : public Core::SmartPointerObject
{
public:
	coreDeclareSmartPointerClassMacro(Camera3D, Core::SmartPointerObject);

protected:
	Camera3D( ) 
	{
		m_eyex = 0; m_eyey = 0; m_eyez = 0;
		m_lookatx = 0; m_lookaty = 0; m_lookatz = 0;
		m_upx = 0; m_upy = 0; m_upz = 0;
		m_view_angle = 0;
		m_near_clip_plane = m_far_clip_plane = 0;
		m_initialized = false;
	}
	~Camera3D( ) {}

public:
	double m_eyex, m_eyey, m_eyez;
	double m_lookatx, m_lookaty, m_lookatz;
	double m_upx, m_upy, m_upz;
	double m_view_angle;
	double m_near_clip_plane;
	double m_far_clip_plane;
	bool m_initialized;
};

typedef Core::DataHolder<Camera3D::Pointer> Camera3DHolderType;

}

#endif // coreCamera3D_H
