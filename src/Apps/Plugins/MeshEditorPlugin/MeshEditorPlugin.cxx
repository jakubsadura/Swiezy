/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "MeshEditorPlugin.h"

// CoreLib
#include "coreReportExceptionMacros.h"
#include "corePluginMacros.h"
#include "coreProfile.h"

// Declaration of the plugin
coreBeginDefinePluginMacro(MeshEditorPlugin)
	coreDefinePluginAddProfileMacro("Mesh Editor")
coreEndDefinePluginMacro()

MeshEditorPlugin::MeshEditorPlugin(void) : FrontEndPlugin()
{
	try
	{
		m_Processors = medProcessorCollective::New();

		m_Widgets = medWidgetCollective::New();
		m_Widgets->Init( );
	}
	coreCatchExceptionsReportAndNoThrowMacro(MeshEditorPlugin::MeshEditorPlugin)
}

MeshEditorPlugin::~MeshEditorPlugin(void)
{
}
