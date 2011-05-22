/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "MITKPluginProcessorCollective.h"
#include "coreProcessorFactories.h"

#include "coreMITKSurfaceImpl.h"
#include "coreMITKImageImpl.h"
#include "coreMITKPointSetImpl.h"


MITKPlugin::ProcessorCollective::ProcessorCollective()
{
	Core::DataEntityImplFactory::RegisterFactory( Core::MitkSurfaceImpl::Factory::NewBase() );
	Core::DataEntityImplFactory::RegisterFactory( Core::MitkPointSetImpl::Factory::NewBase() );
	Core::DataEntityImplFactory::RegisterFactory( Core::MitkImageImpl::Factory::NewBase() );
}

