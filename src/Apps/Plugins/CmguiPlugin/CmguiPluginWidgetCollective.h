/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _CmguiPluginWidgetCollective_H
#define _CmguiPluginWidgetCollective_H

#include "coreFrontEndPlugin.h"
#include "coreSmartPointerMacros.h"
#include "coreObject.h"
#include "coreWidgetCollective.h"

#include "CmguiPluginCmgui.h"

#define wxID_CmguiToolbar wxID( "wxID_CmguiToolbar" )
#define wxID_CmguiCommandline wxID( "wxID_CmguiCommandline" )

namespace CmguiPlugin {

class RenderWidget;
class CommandlineWidget;
class InfoWidget;
class CommandWidget;

class Command;

/**
\brief 
\author Martin Steghöfer
\date Oct 2010
\ingroup CMGUIPlugin
*/
class WidgetCollective : public Core::WidgetCollective
{
public:
    //!
    coreDeclareSmartPointerClassMacro1Param(
        WidgetCollective, 
        Core::WidgetCollective,
        CmguiPlugin::Cmgui*
        );
/*
    void Init(
        CmguiPlugin::ProcessorCollective::Pointer processors, 
        Core::FrontEndPlugin::FrontEndPlugin::Pointer frontEndPlugin,
        std::list<CmguiPlugin::Command*> commands);
    
    const RenderWidget * GetRenderWidget();
    
    CommandWidget * GetCommandWidget();*/

private:
    /** The constructor instantiates all the widgets and connects them to
    the processors.
    It is private because clients must use the New function, which returns
    a smart pointer, to instantiate this class.
    */
    WidgetCollective(CmguiPlugin::Cmgui *cmgui);
    
/*
    //! 
    void EnablePluginTabWidgets();

    //! See m_RenderWidget.
    void CreateRenderWidget();
    
    void CreateCommandlineWidget();
    
    void CreateInfoWidget();
    
    void CreateCommandWidget(std::list<CmguiPlugin::Command*> commands);*/

private:
    
    CmguiPlugin::Cmgui *m_cmgui;
    /*
    //! 
    ProcessorCollective::Pointer m_Processors;

    //! Holds the widget for rendering
    RenderWidget* m_RenderWidget;
    
    CommandlineWidget* m_CommandlineWidget;
    
    InfoWidget* m_InfoWidget;
    
    CommandWidget *m_CommandWidget;*/
};

} // namespace CmguiPlugin

#endif //_CmguiPluginWidgetCollective_H
