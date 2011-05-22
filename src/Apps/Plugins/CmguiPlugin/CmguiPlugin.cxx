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

#include "CmguiPlugin.h"
#include "CmguiPluginCommandWidget.h"

// CoreLib
#include "coreReportExceptionMacros.h"
#include "coreWxMitkGraphicalInterface.h"

extern "C" {
#include "api/cmiss_command_data.h"
#include "api/cmiss_field_image.h"
#include "api/cmiss_field.h"
#include "api/cmiss_field_composite.h"
#include "api/cmiss_region.h"
#include "api/cmiss_field.h"
#include "api/cmiss_scene_viewer.h"
}


// Declaration of the plugin
coreBeginDefinePluginMacro(CmguiPlugin::CmguiPluginMain)
    coreDefinePluginAddProfileMacro("Cmgui")
coreEndDefinePluginMacro()


using namespace CmguiPlugin;


CmguiPluginMain::CmguiPluginMain(void) : FrontEndPlugin() {
    try
    {
        // startup cmgui
        m_cmgui = new Cmgui();
        
        // create collectives
        m_Processors = ProcessorCollective::New(m_cmgui);
        m_Widgets = WidgetCollective::New(m_cmgui);
        
    }
    coreCatchExceptionsReportAndNoThrowMacro(CmguiPlugin::CmguiPlugin)
}


CmguiPluginMain::~CmguiPluginMain(void) {
    delete m_cmgui;
}

