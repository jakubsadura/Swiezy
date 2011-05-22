/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "SignalViewerPlugin.h"

// CoreLib
#include "coreReportExceptionMacros.h"
#include "corePluginMacros.h"
#include "coreProfile.h"

// Declaration of the plugin
coreBeginDefinePluginMacro(SignalViewerPlugin)
	coreDefinePluginAddProfileMacro("SignalViewer")
coreEndDefinePluginMacro()

SignalViewerPlugin::SignalViewerPlugin(void) : FrontEndPlugin()
{
	try
	{
		m_Processors = svProcessorCollective::New();

		m_Widgets = svWidgetCollective::New();
		m_Widgets->Init( );
	}
	coreCatchExceptionsReportAndNoThrowMacro(SignalViewerPlugin::SignalViewerPlugin)
}

SignalViewerPlugin::~SignalViewerPlugin(void)
{
}
