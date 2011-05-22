/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "SandboxPluginProcessorCollective.h"
#include "coreProcessorFactories.h"

SandboxPlugin::ProcessorCollective::ProcessorCollective()
{
	Core::ProcessorFactories::Pointer factories;
	factories = Core::Runtime::Kernel::GetProcessorFactories();
	factories->RegisterFactory( ShapeScaleProcessor::Factory::NewBase( ) );
	factories->RegisterFactory( SubtractProcessor::Factory::NewBase( ) );
	factories->RegisterFactory( ResampleProcessor::Factory::NewBase( ) );
}

