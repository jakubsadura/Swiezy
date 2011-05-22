/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _ctProcessorCollective_H
#define _ctProcessorCollective_H

#include "coreSmartPointerMacros.h"
#include "coreObject.h"

/**

\ingroup CardioToolsPlugin
\author Xavi Planes
\date 12 April 2010
*/

class ctProcessorCollective : public Core::SmartPointerObject
{
public:
	//!
	coreDeclareSmartPointerClassMacro(ctProcessorCollective, Core::SmartPointerObject);

private:
	//! The constructor instantiates all the processors and connects them.
	ctProcessorCollective();

private:

};

#endif //_svPluginProcessorCollective_H
