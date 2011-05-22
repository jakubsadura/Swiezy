/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _dynVtkFilterIFace_H
#define _dynVtkFilterIFace_H

#include "dynFilterIFace.h"

/**
Internal interface for VTK Filter

\author Xavi Planes
\date 20 Sept 2010
\ingroup DynLib
*/
class dynVtkFilterIFace : public dynFilterIFace
{
public:
	static dynVtkFilterIFace* New( );
};



typedef dynVtkFilterIFace* (*exNew)();

#endif // _dynVtkFilterIFace_H

