/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _ImageToolsPlugin_H
#define _ImageToolsPlugin_H

#include "itProcessorCollective.h"
#include "itWidgetCollective.h"

// CoreLib
#include "coreFrontEndPlugin.h"

/** 
\brief Signal Viewer
\ingroup ImageToolsPlugin
\author Xavi Planes
\date 12 April 2010
*/
class PLUGIN_EXPORT ImageToolsPlugin : public Core::FrontEndPlugin::FrontEndPlugin
{
// TYPE DEFINITIONS
public:
	coreDeclareSmartPointerClassMacro(ImageToolsPlugin, Core::FrontEndPlugin::FrontEndPlugin);

// OPERATIONS
protected:
	//!
	ImageToolsPlugin(void);

	//!
	virtual ~ImageToolsPlugin(void);

private:
	//! Purposely not implemented
	ImageToolsPlugin( const Self& );

	//! Purposely not implemented
	void operator = ( const Self& );

private:
	//! Contains all the processors for the plugin
	itProcessorCollective::Pointer m_Processors;

	//! Contains all the widgets for the plugin
	itWidgetCollective::Pointer m_Widgets;
};

#endif // ImageToolsPlugin_H
