/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreDoubleBananaProcessor_H
#define _coreDoubleBananaProcessor_H

#include "corePluginMacros.h"
#include "coreDataEntityHolder.h"
#include "coreSmartPointerMacros.h"
#include "coreBaseFilter.h"
#include "coreBaseProcessor.h"

namespace Core{

/**
Propagates time information from signal to connected data entity (3d +t).

\ingroup SignalViewerPlugin
\author Chiara Riccobene
\date 9 dec 2009
*/

class PLUGIN_EXPORT DoubleBananaProcessor : public Core::BaseProcessor
{
public:
	coreDeclareSmartPointerClassMacro(DoubleBananaProcessor, Core::BaseProcessor);
	//!
	void Update();
private:
	/**
	*/
	DoubleBananaProcessor( );	

};

} // Core

#endif //_coreDoubleBananaProcessor_H
