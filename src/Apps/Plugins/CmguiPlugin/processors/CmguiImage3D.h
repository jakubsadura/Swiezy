/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _CmguiImage3D_H
#define _CmguiImage3D_H


extern "C"
{
#include "api/cmiss_region.h"
}

namespace CmguiPlugin{

/**
\author Xavi Planes
\date Nov 2010
\ingroup CMGUIPlugin
*/
class CmguiImage2D : public Core::SmartPointerObject
{
public:
	coreDeclareSmartPointerClassMacro(CmguiImage2D, Core::SmartPointerObject);
	//! Image data and dimensions
	struct Cmgui_image * m_CmguiImage;
	//! Region coordinates
	Cmiss_region_id m_Region;

	CmguiImage2D( )
	{
		m_CmguiImage = 0;
		m_Region = 0;
	}
};


/**
\author Xavi Planes
\date Nov 2010
\ingroup CMGUIPlugin
*/
class CmguiImage3D : public Core::SmartPointerObject
{
public:
	coreDeclareSmartPointerClassMacro(CmguiImage3D, Core::SmartPointerObject);
	//! 2D Slices
	std::vector<CmguiImage2D::Pointer> m_Slices;
	//! Parent Region
	Cmiss_region_id m_Region;

	CmguiImage3D( )
	{
		m_Region = 0;
	}
};

}

#endif //_CmguiImage3D_H
