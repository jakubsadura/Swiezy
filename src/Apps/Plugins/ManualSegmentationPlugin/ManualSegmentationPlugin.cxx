/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "ManualSegmentationPlugin.h"

// CoreLib
#include "coreReportExceptionMacros.h"
#include "corePluginMacros.h"
#include "coreProfile.h"

// Declaration of the plugin
coreBeginDefinePluginMacro(ManualSegmentationPlugin)
	coreDefinePluginAddProfileMacro("Manual Segmentation")
coreEndDefinePluginMacro()

ManualSegmentationPlugin::ManualSegmentationPlugin(void) : FrontEndPlugin()
{
	try
	{
		m_Processors = msProcessorCollective::New();

		m_Widgets = msWidgetCollective::New();
		m_Widgets->Init( );
	}
	coreCatchExceptionsReportAndNoThrowMacro(ManualSegmentationPlugin::ManualSegmentationPlugin)
}

ManualSegmentationPlugin::~ManualSegmentationPlugin(void)
{
}
