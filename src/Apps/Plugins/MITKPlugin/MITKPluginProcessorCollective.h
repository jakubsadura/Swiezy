/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _MITKPluginProcessorCollective_H
#define _MITKPluginProcessorCollective_H

#include "coreSmartPointerMacros.h"
#include "coreObject.h"

namespace MITKPlugin{

/**
This class instantiates all processors used in the plugin and registers them.

\ingroup MITKPlugin
\author Xavi Planes
\date Nov 2010
*/

class ProcessorCollective : public Core::SmartPointerObject
{
public:
	//!
	coreDeclareSmartPointerClassMacro(ProcessorCollective, Core::SmartPointerObject);

private:
	//! The constructor instantiates all the processors and connects them.
	ProcessorCollective();

};

} // namespace MITKPlugin{

#endif //_MITKPluginProcessorCollective_H
