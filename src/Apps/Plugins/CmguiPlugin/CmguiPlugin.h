/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _CmguiPlugin_H
#define _CmguiPlugin_H

#include "CmguiPluginProcessorCollective.h"
#include "CmguiPluginWidgetCollective.h"

#include "CmguiPluginCmgui.h"

// CoreLib
#include "coreFrontEndPlugin.h"

extern "C"
{
#include "api/cmiss_command_data.h"
#include "api/cmiss_field.h"
#include "api/cmiss_texture.h"
#include "api/cmiss_scene_viewer.h"
}

namespace CmguiPlugin{


/**
\brief 
\author Martin Steghöfer
\date Oct 2010
\ingroup CMGUIPlugin
*/
class PLUGIN_EXPORT CmguiPluginMain : public Core::FrontEndPlugin::FrontEndPlugin
{
public:
    coreDeclareSmartPointerClassMacro(CmguiPluginMain, Core::FrontEndPlugin::FrontEndPlugin);

protected:
    //!
    CmguiPluginMain(void);

    //!
    virtual ~CmguiPluginMain(void);

private:
    //! Purposely not implemented
    CmguiPluginMain( const Self& );
    
    //! Purposely not implemented
    void operator = ( const Self& );
    
    Cmgui *m_cmgui;
    
private:
    //! Contains all the processors for the plugin
    ProcessorCollective::Pointer m_Processors;

    //! Contains all the widgets for the plugin
    WidgetCollective::Pointer m_Widgets;
};

} // namespace CmguiPlugin

#endif // CmguiPlugin_H

