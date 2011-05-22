/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _TavernaPlugin_H
#define _TavernaPlugin_H

#include "TavernaPluginProcessorCollective.h"
#include "TavernaPluginWidgetCollective.h"

// CoreLib
#include "coreFrontEndPlugin.h"

namespace TavernaPlugin{

/** 

\ingroup TavernaPlugin
\author Xavi Planes
\date 23 Dec 2010
*/
class PLUGIN_EXPORT TavernaPlugin : public Core::FrontEndPlugin::FrontEndPlugin
{
// TYPE DEFINITIONS
public:
	coreDeclareSmartPointerClassMacro(TavernaPlugin, Core::FrontEndPlugin::FrontEndPlugin);

// OPERATIONS
protected:
	//!
	TavernaPlugin(void);

	//!
	virtual ~TavernaPlugin(void);

private:
	//! Purposely not implemented
	TavernaPlugin( const Self& );

	//! Purposely not implemented
	void operator = ( const Self& );

private:
	//! Contains all the processors for the plugin
	ProcessorCollective::Pointer m_Processors;

	//! Contains all the widgets for the plugin
	WidgetCollective::Pointer m_Widgets;
};

} // namespace TavernaPlugin

#endif // TavernaPlugin_H
