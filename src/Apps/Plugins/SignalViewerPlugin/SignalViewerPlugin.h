/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _SignalViewerPluginPlugin_H
#define _SignalViewerPluginPlugin_H

#include "svPluginProcessorCollective.h"
#include "svPluginWidgetCollective.h"

// CoreLib
#include "coreFrontEndPlugin.h"

/** 
\brief Signal Viewer
\ingroup SignalViewerPlugin
\author Xavi Planes
\date 12 April 2010
*/
class PLUGIN_EXPORT SignalViewerPlugin : public Core::FrontEndPlugin::FrontEndPlugin
{
// TYPE DEFINITIONS
public:
	coreDeclareSmartPointerClassMacro(SignalViewerPlugin, Core::FrontEndPlugin::FrontEndPlugin);

// OPERATIONS
protected:
	//!
	SignalViewerPlugin(void);

	//!
	virtual ~SignalViewerPlugin(void);

private:
	//! Purposely not implemented
	SignalViewerPlugin( const Self& );

	//! Purposely not implemented
	void operator = ( const Self& );

private:
	//! Contains all the processors for the plugin
	svProcessorCollective::Pointer m_Processors;

	//! Contains all the widgets for the plugin
	svWidgetCollective::Pointer m_Widgets;
};

#endif // SignalViewPlugin_H
