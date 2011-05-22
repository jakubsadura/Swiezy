/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreSignalESEDPropagationProcessor_H
#define _coreSignalESEDPropagationProcessor_H

#include "gmProcessorsWin32Header.h"
#include "coreDataEntityHolder.h"
#include "coreSmartPointerMacros.h"
#include "coreBaseFilter.h"
#include "coreBaseProcessor.h"

namespace Core{

/**
Propagates ES/ED from signal to father data entity (3d +t).

\ingroup SignalViewerPlugin
\author Xavi Planes
\date 14 Jan 2009
*/

class SignalESEDPropagationProcessor : public Core::BaseProcessor
{
public:
	coreDeclareSmartPointerClassMacro(SignalESEDPropagationProcessor, Core::BaseProcessor);
	//!
	void Update();
	
private:
	/**
	*/
	SignalESEDPropagationProcessor( );	
};

} // Core

#endif //_coreSignalESEDPropagationProcessor_H
