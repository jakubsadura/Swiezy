/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreNetgenProcessor_H
#define _coreNetgenProcessor_H

#include "corePluginMacros.h"
#include "coreDataEntityHolder.h"
#include "coreSmartPointerMacros.h"
#include "coreBaseFilter.h"
#include "coreBaseProcessor.h"

#include "meNGOptimizeFilter.h"

namespace Core{

/**
Close the holes of the surface using meVTKNetgenFilter.

\ingroup MeshEditorPlugin
\author Chiara Riccobene
\date 4 nov 2009
*/

class PLUGIN_EXPORT NetgenProcessor : public Core::BaseProcessor
{
public:
	coreDeclareSmartPointerClassMacro(NetgenProcessor, Core::BaseProcessor);
	//!
	void Update();
	//!
	void SetParams(meMeshParamsOptimizePtr param);
	//!
	meMeshParamsOptimizePtr GetParams( );

private:
	/**
	*/
	NetgenProcessor( );

private:
	//!
	meNGOptimizeFilter::Pointer m_Filter;
};

} // Core

#endif //_coreNetgenProcessor_H
