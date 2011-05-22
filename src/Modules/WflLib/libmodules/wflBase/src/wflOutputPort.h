/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _wflOutputPort_H
#define _wflOutputPort_H

#include "WflLibWin32Header.h"
#include "blLightObject.h"
#include "wflProperties.h"

/**
Output port for a workflow

Properties:
- "name"
- "annotations"

\author Xavi Planes
\date 22 Dec 2010
\ingroup WflLib
*/
class WFLLIB_EXPORT wflOutputPort : 
	public blLightObject, 
	public wflProperties
{
public:
	typedef wflOutputPort Self;
	typedef blSmartPointer<Self> Pointer;
	blNewMacro( Self );
	
public:

protected:
	//!
	wflOutputPort( );

	//!
	~wflOutputPort( );

protected:
};

#endif //_wflOutputPort_H
