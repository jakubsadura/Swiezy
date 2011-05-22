/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreOverlayProcessor_H
#define _coreOverlayProcessor_H

#include "corePluginMacros.h"
#include "coreDataEntityHolder.h"
#include "coreSmartPointerMacros.h"
#include "coreBaseProcessor.h"

namespace Core{

/**
Cut the surface using using meRingCutAPI.

\ingroup ImageToolsPlugin
\author Chiara Riccobene
\date 6 nov 2009
*/

class PLUGIN_EXPORT OverlayProcessor : public Core::BaseProcessor
{
public:
	coreDeclareSmartPointerClassMacro(OverlayProcessor, Core::BaseProcessor);

	//!
	void Update();

private:
	//!
	OverlayProcessor( );

private:

};

} // Core

#endif //_coreOverlayProcessor_H
