/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

// For compilers that don't support precompilation, include "wx/wx.h"
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
       #include <wx/wx.h>
#endif

#include "MITKPlugin.h"

// CoreLib
#include "coreReportExceptionMacros.h"
#include "coreWxMitkGraphicalInterface.h"
#include "corePluginTab.h"

// Declaration of the plugin
coreBeginDefinePluginMacro(MITKPlugin::MITKPlugin)
	coreDefinePluginAddProfileMacro("MITKPlugin")
coreEndDefinePluginMacro()

MITKPlugin::MITKPlugin::MITKPlugin(void) : FrontEndPlugin()
{
	try
	{
		m_Processors = ProcessorCollective::New();
		m_Widgets = WidgetCollective::New();
	}
	coreCatchExceptionsReportAndNoThrowMacro(MITKPlugin::MITKPlugin)
}

MITKPlugin::MITKPlugin::~MITKPlugin(void)
{
}
