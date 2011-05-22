/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _medProcessorCollective_H
#define _medProcessorCollective_H

#include "coreSmartPointerMacros.h"
#include "coreObject.h"

/**

\ingroup MeshEditorPlugin
\author Xavi Planes
\date 12 April 2010
*/

class medProcessorCollective : public Core::SmartPointerObject
{
public:
	//!
	coreDeclareSmartPointerClassMacro(medProcessorCollective, Core::SmartPointerObject);

private:
	//! The constructor instantiates all the processors and connects them.
	medProcessorCollective();

private:

};

#endif //_svPluginProcessorCollective_H
