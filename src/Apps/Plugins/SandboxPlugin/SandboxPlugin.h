/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _SandboxPlugin_H
#define _SandboxPlugin_H

#include "SandboxPluginProcessorCollective.h"
#include "SandboxPluginWidgetCollective.h"

// CoreLib
#include "coreFrontEndPlugin.h"

namespace SandboxPlugin{

/** 
\brief Creates all objects of the plug-in and connect them.

\note Nobody can get access to this class. This class is only for 
initialization of all components. 

\note Try to make all processors reusable for other plug-ins. Be aware 
of creating a dependency between the processor and any class of the rest 
of the plug-in.

\ingroup SandboxPlugin
\author Jakub Lyko
\date 07 April 2008
*/
class PLUGIN_EXPORT SandboxPlugin : public Core::FrontEndPlugin::FrontEndPlugin
{
// TYPE DEFINITIONS
public:
	coreDeclareSmartPointerClassMacro(SandboxPlugin, Core::FrontEndPlugin::FrontEndPlugin);

// OPERATIONS
protected:
	//!
	SandboxPlugin(void);

	//!
	virtual ~SandboxPlugin(void);

private:
	//! Purposely not implemented
	SandboxPlugin( const Self& );

	//! Purposely not implemented
	void operator = ( const Self& );

private:
	//! Contains all the processors for the plugin
	ProcessorCollective::Pointer m_Processors;

	//! Contains all the widgets for the plugin
	WidgetCollective::Pointer m_Widgets;
};

} // namespace SandboxPlugin

#endif // SandboxPlugin_H
