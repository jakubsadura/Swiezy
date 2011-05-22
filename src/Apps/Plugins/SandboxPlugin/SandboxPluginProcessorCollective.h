/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _SandboxPluginProcessorCollective_H
#define _SandboxPluginProcessorCollective_H

#include "coreSmartPointerMacros.h"
#include "coreObject.h"

#include "SandboxPluginShapeScaleProcessor.h"
#include "SandboxPluginSubtractProcessor.h"
#include "SandboxPluginResampleProcessor.h"

namespace SandboxPlugin{

/**
This class instantiates all processors used in the plugin and registers them.

\ingroup SandboxPlugin
\author Maarten Nieber
\date 18 jun 2008
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

} // namespace SandboxPlugin{

#endif //_SandboxPluginProcessorCollective_H
