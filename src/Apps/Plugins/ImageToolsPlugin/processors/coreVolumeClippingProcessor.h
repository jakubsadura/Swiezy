/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreVolumeClippingProcessor_H
#define _coreVolumeClippingProcessor_H

#include "corePluginMacros.h"
#include "coreDataEntityHolder.h"
#include "coreSmartPointerMacros.h"
#include "coreBaseFilter.h"
#include "coreBaseProcessor.h"

//#include "meVTKVolumeClippingFilter.h"

namespace Core{

/**
Close the holes of the surface using meVTKVolumeClippingFilter.

\ingroup ImageToolsPlugin
\author Chiara Riccobene
\date 4 nov 2009
*/

class PLUGIN_EXPORT VolumeClippingProcessor : public Core::BaseProcessor
{
public:
	coreDeclareSmartPointerClassMacro(VolumeClippingProcessor, Core::BaseProcessor);
	//!
	void Update();

private:
	/**
	*/
	VolumeClippingProcessor( );
};

} // Core

#endif //_coreVolumeClippingProcessor_H
