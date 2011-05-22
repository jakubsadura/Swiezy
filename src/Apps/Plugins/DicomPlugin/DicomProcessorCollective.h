/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _DicomProcessorCollective_H
#define _DicomProcessorCollective_H

#include "coreSmartPointerMacros.h"
#include "coreObject.h"

#include "DicomDataSetProcessor.h"

namespace DicomPlugin{

/**

\ingroup DicomPlugin
\author XaviPlanes
\date 24 july 2009
*/

class ProcessorCollective : public Core::SmartPointerObject
{
public:
	//!
	coreDeclareSmartPointerClassMacro(ProcessorCollective, Core::SmartPointerObject);

private:
	//! The constructor instantiates all the processors and connects them.
	ProcessorCollective();

private:

};

} // namespace DicomPlugin

#endif //_DicomProcessorCollective_H
