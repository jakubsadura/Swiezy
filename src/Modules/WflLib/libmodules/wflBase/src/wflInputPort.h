/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _wflInputPort_H
#define _wflInputPort_H

#include "WflLibWin32Header.h"
#include "blLightObject.h"
#include "wflProperties.h"

/**
Input port for a work flow

Properties:
- "name"
- "depth"
- "granularDepth"
- "annotations"
- "datatype": This can be:
  - image
  - surface mesh

\author Xavi Planes
\date 22 Dec 2010
\ingroup WflLib
*/
class WFLLIB_EXPORT wflInputPort : 
	public blLightObject, 
	public wflProperties
{
public:
	typedef wflInputPort Self;
	typedef blSmartPointer<Self> Pointer;
	blNewMacro( Self );
	
public:

protected:
	//!
	wflInputPort( );

	//!
	~wflInputPort( );

protected:
};

#endif //_wflInputPort_H
