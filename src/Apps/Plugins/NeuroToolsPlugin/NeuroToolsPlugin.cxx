/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "NeuroToolsPlugin.h"

// CoreLib
#include "coreReportExceptionMacros.h"
#include "corePluginMacros.h"
#include "coreProfile.h"

// Declaration of the plugin
coreBeginDefinePluginMacro(NeuroToolsPlugin)
	coreDefinePluginAddProfileMacro("Neuro Tools")
coreEndDefinePluginMacro()

NeuroToolsPlugin::NeuroToolsPlugin(void) : FrontEndPlugin()
{
	try
	{
		m_Processors = ntProcessorCollective::New();

		m_Widgets = ntWidgetCollective::New();
		m_Widgets->Init( );
	}
	coreCatchExceptionsReportAndNoThrowMacro(NeuroToolsPlugin::NeuroToolsPlugin)
}

NeuroToolsPlugin::~NeuroToolsPlugin(void)
{
}
