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

#include "SceneViewPlugin.h"
#include "coreException.h"
#include "coreReportExceptionMacros.h"
#include "coreProfile.h"
#include "coreKernel.h"
#include "coreWxMitkGraphicalInterface.h"

using namespace Core::Plugins;
using namespace Core::Widgets;

// Declaration of the plugin
coreBeginDefinePluginMacro(SceneViewPlugin)
	coreDefinePluginAddProfileMacro("Scene View")
coreEndDefinePluginMacro()

/**
* Constructor for class SceneViewPlugin
*/
SceneViewPlugin::SceneViewPlugin(void) 
{
	try
	{
		m_WidgetCollective = svWidgetCollective::New();
		m_WidgetCollective->Init( );

	}
	coreCatchExceptionsReportAndNoThrowMacro(SceneViewPlugin::SceneViewPlugin)
}

/**
* Destructor for class SceneViewPlugin
*/
SceneViewPlugin::~SceneViewPlugin(void)
{
}


