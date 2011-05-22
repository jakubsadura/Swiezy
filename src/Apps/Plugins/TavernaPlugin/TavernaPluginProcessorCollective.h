/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _TavernaPluginProcessorCollective_H
#define _TavernaPluginProcessorCollective_H

#include "coreSmartPointerMacros.h"
#include "coreObject.h"


namespace TavernaPlugin{

/** 

\ingroup TavernaPlugin
\author Xavi Planes
\date 23 Dec 2010
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

} // namespace TavernaPlugin{

#endif //_TavernaPluginProcessorCollective_H
