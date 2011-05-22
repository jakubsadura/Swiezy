/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreSlicePlane_H
#define coreSlicePlane_H

#include "gmWidgetsWin32Header.h"
#include "coreObject.h"

namespace Core
{

/** 
Slice Plane information to pass to other windows
\ingroup gmWidgets
\author Xavi Planes
\date Nov 2010
*/
class SlicePlane : public Core::SmartPointerObject
{
public:
	coreDeclareSmartPointerClassMacro(SlicePlane, Core::SmartPointerObject);

	enum PlaneOrientation { Transversal, Sagittal, Frontal };

protected:
	SlicePlane( ) 
	{
		m_Orientation = Transversal;
		m_Id = 0;
		m_Pos = -1;
	}
	~SlicePlane( ) {}

public:
	//!
	PlaneOrientation m_Orientation;
	//!
	int m_Id;
	//!
	int m_Pos;
};

typedef Core::DataHolder<SlicePlane::Pointer> SlicePlaneHolderType;

}

#endif // coreSlicePlane_H
