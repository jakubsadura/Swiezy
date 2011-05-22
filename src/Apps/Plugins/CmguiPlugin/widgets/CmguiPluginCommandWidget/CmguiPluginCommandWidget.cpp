/*
* Copyright (c) 2010,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "CmguiPluginCommandWidget.h"

#include "boost/algorithm/string.hpp"
#include "boost/foreach.hpp"

// STD
#include <cstdio>
#include <limits>

// Cmgui
extern "C"
{
#include "api/cmiss_command_data.h"
#include "api/cmiss_field_image.h"
#include "api/cmiss_field.h"
#include "api/cmiss_field_composite.h"
#include "api/cmiss_field.h"
#include "api/cmiss_scene_viewer.h"
#include "region/cmiss_region.h"
}


using namespace CmguiPlugin;


// Event the widget
BEGIN_EVENT_TABLE(CommandWidget, wxScrolledWindow)
END_EVENT_TABLE()


CommandWidget::CommandWidget(Cmgui* cmgui, std::list<CmguiPlugin::Command*> commands, wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style)
: wxScrolledWindow(parent, id, pos, size, style)
{
    m_cmgui = cmgui;
    
    wxString name = wxT("CMGUI Commands");
    SetLabel(name);
    SetName(name);
    
    addCommands(commands); // includes a call to do_layout()
}


CommandWidget::~CommandWidget()
{
    // We don't need to destroy anything because all the child windows
    // of this wxWindow are destroyed automatically
}


void CommandWidget::do_layout()
{
    wxGridSizer* sizer = new wxGridSizer(3, 3);
    //sizer_1->Add(m_notebook, 1, wxEXPAND, 0);
    for (std::map<int, wxButton*>::iterator buttonMapEntry = m_buttons.begin(); buttonMapEntry != m_buttons.end(); buttonMapEntry++) {
        sizer->Add(buttonMapEntry->second, 1, wxEXPAND, 0);
    }
    SetSizer(sizer);
    sizer->Fit(this);
    Layout();
}



void CommandWidget::UpdateWidget() {
}

void CommandWidget::OnCommand(wxCommandEvent& event)
{
    Command *command = m_commands[event.GetId()];
    if (command != NULL) {
        command->execute();
    }
}

bool CommandWidget::Enable(bool enable)
{
    bool bReturn = wxScrolledWindow::Enable(enable);
    
    // If this panel widget is selected -> Update the widget
    if (enable) {
        UpdateWidget();
    }
    
    return bReturn;
}

void CommandWidget::addCommands(std::list<CmguiPlugin::Command*> commands) {
    BOOST_FOREACH(CmguiPlugin::Command *command, commands) {
        // create a new ID for the button using a unique string (including the pointer to the command)
        std::stringstream ssPointer;
        ssPointer << command;
        std::string idString = std::string("CMGUIPluginCommand ") + std::string(ssPointer.str());
        int id = wxID(idString);
        //std::cout << "ID: " << idString << std::endl;
        
        // create the button
        wxButton *button = new wxButton(this, id, command->getText());
        m_commands[id] = command;
        m_buttons[id] = button;
        
        // register the command event handler
        Connect(id, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CommandWidget::OnCommand));
        
    }
    
    do_layout();
    
}


ManualCommand::ManualCommand(CmguiPlugin::Cmgui *cmgui, std::string command, std::string description) {
    m_cmgui = cmgui;
    m_command = command;
    m_description = description;
}
    
void ManualCommand::execute() {
    m_cmgui->executeCommand(m_command);
}
    
std::string ManualCommand::getText() {
    return m_description;
}

ManualCommandRegion::ManualCommandRegion(CmguiPlugin::Cmgui *cmgui, std::string command, std::string description) {
    m_cmgui = cmgui;
    m_command = command;
    m_description = description;
}
    
void ManualCommandRegion::execute() {
	m_cmgui->executeCommand(m_command);
}
    
std::string ManualCommandRegion::getText() {
    return m_description;
}
    

