/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _MITKPlugin_H
#define _MITKPlugin_H

#include "MITKPluginProcessorCollective.h"
#include "MITKPluginWidgetCollective.h"

// CoreLib
#include "coreFrontEndPlugin.h"

namespace MITKPlugin{

/** 
\brief Creates all objects of the plug-in and connect them.

\ingroup MITKPlugin
\author Xavi Planes
\date Nov 2010
*/
class PLUGIN_EXPORT MITKPlugin : public Core::FrontEndPlugin::FrontEndPlugin
{
// TYPE DEFINITIONS
public:
	coreDeclareSmartPointerClassMacro(MITKPlugin, Core::FrontEndPlugin::FrontEndPlugin);

// OPERATIONS
protected:
	//!
	MITKPlugin(void);

	//!
	virtual ~MITKPlugin(void);

private:
	//! Purposely not implemented
	MITKPlugin( const Self& );

	//! Purposely not implemented
	void operator = ( const Self& );

private:
	//! Contains all the processors for the plugin
	ProcessorCollective::Pointer m_Processors;

	//! Contains all the widgets for the plugin
	WidgetCollective::Pointer m_Widgets;
};

} // namespace MITKPlugin

#endif // MITKPlugin_H
