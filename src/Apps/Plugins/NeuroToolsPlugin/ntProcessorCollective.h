/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _ntProcessorCollective_H
#define _ntProcessorCollective_H

#include "coreSmartPointerMacros.h"
#include "coreObject.h"

/**

\ingroup NeuroToolsPlugin
\author Xavi Planes
\date 12 April 2010
*/

class ntProcessorCollective : public Core::SmartPointerObject
{
public:
	//!
	coreDeclareSmartPointerClassMacro(ntProcessorCollective, Core::SmartPointerObject);

private:
	//! The constructor instantiates all the processors and connects them.
	ntProcessorCollective();

private:

};

#endif //_svPluginProcessorCollective_H
