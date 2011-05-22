/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "ImageToolsPlugin.h"

// CoreLib
#include "coreReportExceptionMacros.h"
#include "corePluginMacros.h"
#include "coreProfile.h"

// Declaration of the plugin
coreBeginDefinePluginMacro(ImageToolsPlugin)
	coreDefinePluginAddProfileMacro("Image Tools")
coreEndDefinePluginMacro()

ImageToolsPlugin::ImageToolsPlugin(void) : FrontEndPlugin()
{
	try
	{
		m_Processors = itProcessorCollective::New();

		m_Widgets = itWidgetCollective::New();
		m_Widgets->Init( );
	}
	coreCatchExceptionsReportAndNoThrowMacro(ImageToolsPlugin::ImageToolsPlugin)
}

ImageToolsPlugin::~ImageToolsPlugin(void)
{
}
