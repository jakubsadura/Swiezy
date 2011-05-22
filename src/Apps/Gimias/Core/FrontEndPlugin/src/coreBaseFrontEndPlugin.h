/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#ifndef coreBaseFrontEndPlugin_H
#define coreBaseFrontEndPlugin_H

#include "gmFrontEndPluginWin32Header.h"
#include "coreObject.h"

// Forward declarations
namespace Core
{
	namespace Widgets 
	{
		class PluginTab;
	}
}

namespace Core{
namespace FrontEndPlugin{

	/** 
	\brief Base class for the FrontEndPlugin

	FrontEndPlugin is inside the gmWidgets and we don't want to create
	a dependency

	\ingroup gmKernel
	\author Xavi Planes
	\date 19 Jun 2009
	*/
	class BaseFrontEndPlugin : public Core::SmartPointerObject
	{
	public:
		//!
		coreDeclareSmartPointerTypesMacro(
			BaseFrontEndPlugin, 
			Core::SmartPointerObject);
		coreClassNameMacro(BaseFrontEndPlugin);
	};
}
}

#endif

