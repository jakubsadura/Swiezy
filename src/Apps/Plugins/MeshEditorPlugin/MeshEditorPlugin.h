/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _MeshEditorPlugin_H
#define _MeshEditorPlugin_H

#include "medProcessorCollective.h"
#include "medWidgetCollective.h"

// CoreLib
#include "coreFrontEndPlugin.h"

/** 
\brief Signal Viewer
\ingroup MeshEditorPlugin
\author Xavi Planes
\date 12 April 2010
*/
class PLUGIN_EXPORT MeshEditorPlugin : public Core::FrontEndPlugin::FrontEndPlugin
{
// TYPE DEFINITIONS
public:
	coreDeclareSmartPointerClassMacro(MeshEditorPlugin, Core::FrontEndPlugin::FrontEndPlugin);

// OPERATIONS
protected:
	//!
	MeshEditorPlugin(void);

	//!
	virtual ~MeshEditorPlugin(void);

private:
	//! Purposely not implemented
	MeshEditorPlugin( const Self& );

	//! Purposely not implemented
	void operator = ( const Self& );

private:
	//! Contains all the processors for the plugin
	medProcessorCollective::Pointer m_Processors;

	//! Contains all the widgets for the plugin
	medWidgetCollective::Pointer m_Widgets;
};

#endif // MeshEditorPlugin_H
