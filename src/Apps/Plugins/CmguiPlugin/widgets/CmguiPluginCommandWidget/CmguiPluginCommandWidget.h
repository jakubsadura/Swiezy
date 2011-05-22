/*
* Copyright (c) 2010,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _CmguiPluginCommandWidget_H
#define _CmguiPluginCommandWidget_H

#include "wx/treectrl.h"

// CoreLib
#include "coreProcessingWidget.h"

#include "CmguiPluginCmgui.h"
#include "CmguiPluginMacros.h"

extern "C"
{
#include "api/cmiss_command_data.h"
#include "api/cmiss_field.h"
#include "api/cmiss_texture.h"
#include "api/cmiss_scene_viewer.h"
#include "user_interface/message.h"
#include "region/cmiss_region.h"
}


namespace Core{ namespace Widgets {
    class UserHelper;
    class DataEntityListBrowser;
}}

namespace CmguiPlugin{



/**
\brief 
\ingroup CMGUIPlugin
\author Martin Steghöfer
\date Oct 2010
*/
class Command {
public:
    virtual void execute() = 0;
    virtual std::string getText() = 0;
};


/**
\brief 
\ingroup CMGUIPlugin
\author Martin Steghöfer
\date Oct 2010
*/
class ManualCommand : public CmguiPlugin::Command {
public:
    ManualCommand(Cmgui *cmgui, std::string command, std::string description);
    
    void execute();
    
    std::string getText();
    
private:
    Cmgui * m_cmgui;
    std::string m_command;
    std::string m_description;
};


/**
\brief 
\ingroup CMGUIPlugin
\author Martin Steghöfer
\date Oct 2010
*/
class ManualCommandRegion : public CmguiPlugin::Command {
public:
    ManualCommandRegion(Cmgui *cmgui, std::string command, std::string description);
    
    void execute();
    
    std::string getText();
    
private:
    Cmgui * m_cmgui;
    std::string m_command;
    std::string m_description;
};


/**
\brief 
\ingroup CMGUIPlugin
\author Martin Steghöfer
\date Oct 2010
*/
class CommandWidget : public wxScrolledWindow, public Core::Widgets::ProcessingWidget
{

public:
	coreDefineBaseWindowFactory2param(CommandWidget, Cmgui*, std::list<CmguiPlugin::Command*>);

    //!
    CommandWidget(Cmgui* cmgui, std::list<CmguiPlugin::Command*> commands, wxWindow* parent, int id = wxID_ANY, const wxPoint&  pos = wxDefaultPosition, const wxSize&  size = wxDefaultSize, long style = 0);

    //!
    ~CommandWidget();

    //!
    bool Enable(bool enable /*= true */);
    
    void addCommands(std::list<CmguiPlugin::Command*> commands);
    
    virtual Core::BaseProcessor::Pointer GetProcessor() {
        return NULL;
    }

private:
    DECLARE_EVENT_TABLE()
    
    // ************* methods *************

    //! Update GUI from working data
    void UpdateWidget();

    void do_layout();
    
    void OnCommand(wxCommandEvent& event);

    // ************* attributes *************

    //wxListBox* m_listBoxRegions;
    wxPanel* m_notebookPaneRegions;
    wxTreeCtrl *m_treeRegions;
    wxNotebook* m_notebook;
    
    std::map<int, Command*> m_commands;
    std::map<int, wxButton*> m_buttons;

    CmguiPlugin::Cmgui *m_cmgui;
};


} // namespace CmguiPlugin

#endif //_CmguiPluginCommandWidget_H

