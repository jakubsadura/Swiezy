/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "medProcessorCollective.h"
#include "coreNetgenTimeStepImpl.h"

medProcessorCollective::medProcessorCollective()
{
	Core::DataEntityImplFactory::RegisterFactory(Core::NetgenImpl::Factory::NewBase() );
}

