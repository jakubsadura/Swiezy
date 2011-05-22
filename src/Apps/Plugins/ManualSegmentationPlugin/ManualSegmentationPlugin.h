/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _ManualSegmentationPlugin_H
#define _ManualSegmentationPlugin_H

#include "msProcessorCollective.h"
#include "msWidgetCollective.h"

// CoreLib
#include "coreFrontEndPlugin.h"

/** 
\brief Manual Segmentation Plugin
\ingroup ManualSegmentationPlugin
\author Xavi Planes
\date 12 April 2010
*/
class PLUGIN_EXPORT ManualSegmentationPlugin : public Core::FrontEndPlugin::FrontEndPlugin
{
// TYPE DEFINITIONS
public:
	coreDeclareSmartPointerClassMacro(ManualSegmentationPlugin, Core::FrontEndPlugin::FrontEndPlugin);

// OPERATIONS
protected:
	//!
	ManualSegmentationPlugin(void);

	//!
	virtual ~ManualSegmentationPlugin(void);

private:
	//! Purposely not implemented
	ManualSegmentationPlugin( const Self& );

	//! Purposely not implemented
	void operator = ( const Self& );

private:
	//! Contains all the processors for the plugin
	msProcessorCollective::Pointer m_Processors;

	//! Contains all the widgets for the plugin
	msWidgetCollective::Pointer m_Widgets;
};

#endif // ManualSegmentationPlugin_H
