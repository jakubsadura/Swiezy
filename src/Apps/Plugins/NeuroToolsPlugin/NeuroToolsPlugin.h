/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _NeuroToolsPlugin_H
#define _NeuroToolsPlugin_H

#include "ntProcessorCollective.h"
#include "ntWidgetCollective.h"

// CoreLib
#include "coreFrontEndPlugin.h"

/** 
\brief Signal Viewer
\ingroup NeuroToolsPlugin
\author Xavi Planes
\date 12 April 2010
*/
class PLUGIN_EXPORT NeuroToolsPlugin : public Core::FrontEndPlugin::FrontEndPlugin
{
// TYPE DEFINITIONS
public:
	coreDeclareSmartPointerClassMacro(NeuroToolsPlugin, Core::FrontEndPlugin::FrontEndPlugin);

// OPERATIONS
protected:
	//!
	NeuroToolsPlugin(void);

	//!
	virtual ~NeuroToolsPlugin(void);

private:
	//! Purposely not implemented
	NeuroToolsPlugin( const Self& );

	//! Purposely not implemented
	void operator = ( const Self& );

private:
	//! Contains all the processors for the plugin
	ntProcessorCollective::Pointer m_Processors;

	//! Contains all the widgets for the plugin
	ntWidgetCollective::Pointer m_Widgets;
};

#endif // NeuroToolsPlugin_H
