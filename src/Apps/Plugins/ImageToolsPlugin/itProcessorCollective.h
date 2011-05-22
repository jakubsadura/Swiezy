/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _itProcessorCollective_H
#define _itProcessorCollective_H

#include "coreSmartPointerMacros.h"
#include "coreObject.h"

/**

\ingroup ImageToolsPlugin
\author Xavi Planes
\date 12 April 2010
*/

class itProcessorCollective : public Core::SmartPointerObject
{
public:
	//!
	coreDeclareSmartPointerClassMacro(itProcessorCollective, Core::SmartPointerObject);

private:
	//! The constructor instantiates all the processors and connects them.
	itProcessorCollective();

private:

};

#endif //_svPluginProcessorCollective_H
