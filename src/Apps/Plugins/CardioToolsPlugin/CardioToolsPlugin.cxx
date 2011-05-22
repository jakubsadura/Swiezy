/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "CardioToolsPlugin.h"

// CoreLib
#include "coreReportExceptionMacros.h"
#include "corePluginMacros.h"
#include "coreProfile.h"

// Declaration of the plugin
coreBeginDefinePluginMacro(CardioToolsPlugin)
	coreDefinePluginAddProfileMacro("Cardio Tools")
coreEndDefinePluginMacro()

CardioToolsPlugin::CardioToolsPlugin(void) : FrontEndPlugin()
{
	try
	{
		m_Processors = ctProcessorCollective::New();

		m_Widgets = ctWidgetCollective::New();
		m_Widgets->Init( );
	}
	coreCatchExceptionsReportAndNoThrowMacro(CardioToolsPlugin::CardioToolsPlugin)
}

CardioToolsPlugin::~CardioToolsPlugin(void)
{
}
