/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _svPluginProcessorCollective_H
#define _svPluginProcessorCollective_H

#include "coreSmartPointerMacros.h"
#include "coreObject.h"

/**

\ingroup SignalViewerPlugin
\author Xavi Planes
\date 12 April 2010
*/

class svProcessorCollective : public Core::SmartPointerObject
{
public:
	//!
	coreDeclareSmartPointerClassMacro(svProcessorCollective, Core::SmartPointerObject);

private:
	//! The constructor instantiates all the processors and connects them.
	svProcessorCollective();

private:

};

#endif //_svPluginProcessorCollective_H
