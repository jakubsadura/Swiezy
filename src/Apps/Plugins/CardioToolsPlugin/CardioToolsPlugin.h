/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _CardioToolsPlugin_H
#define _CardioToolsPlugin_H

#include "ctProcessorCollective.h"
#include "ctWidgetCollective.h"

// CoreLib
#include "coreFrontEndPlugin.h"

/** 
\brief Signal Viewer
\ingroup CardioToolsPlugin
\author Xavi Planes
\date 12 April 2010
*/
class PLUGIN_EXPORT CardioToolsPlugin : public Core::FrontEndPlugin::FrontEndPlugin
{
// TYPE DEFINITIONS
public:
	coreDeclareSmartPointerClassMacro(CardioToolsPlugin, Core::FrontEndPlugin::FrontEndPlugin);

// OPERATIONS
protected:
	//!
	CardioToolsPlugin(void);

	//!
	virtual ~CardioToolsPlugin(void);

private:
	//! Purposely not implemented
	CardioToolsPlugin( const Self& );

	//! Purposely not implemented
	void operator = ( const Self& );

private:
	//! Contains all the processors for the plugin
	ctProcessorCollective::Pointer m_Processors;

	//! Contains all the widgets for the plugin
	ctWidgetCollective::Pointer m_Widgets;
};

#endif // CardioToolsPlugin_H
