/*
* Copyright (c) 2010,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "CmguiPluginCmgui.h"
#include "coreStringHelper.h"

#include <boost/foreach.hpp>

#include <itksys/SystemTools.hxx>

extern "C" {
#include "api/cmiss_command_data.h"
#include "api/cmiss_element.h"
#include "api/cmiss_field_image.h"
#include "api/cmiss_field.h"
#include "api/cmiss_field_composite.h"
#include "region/cmiss_region.h"
#include "graphics/scene_viewer.h"
#include "graphics/scene.h"
#include "command/cmiss.h"
#include "api/cmiss_field.h"
#include "api/cmiss_scene_viewer.h"
}

#include <iostream>
#include <fstream>
#include <algorithm>


using itksys::SystemTools;
using itksys::SystemTools;


#define max3(x, y, z) (std::max(std::max(x, y), z))


using namespace CmguiPlugin;


Cmgui::Cmgui(void) {
	restart();
    
}

void Cmgui::restart() {
    int cmgui_argc = 2;
    const char **cmgui_argv = (const char**) (new char*[3]);
    *cmgui_argv = strdup("");
    *(cmgui_argv + 1) = strdup("-server");
    *(cmgui_argv + 2) = 0;
    
    m_commandData = Cmiss_command_data_create(cmgui_argc, cmgui_argv, const_cast<char *>("0.0"));
    
    Cmiss_command_data_execute_command(m_commandData, "gfx create texture temp");
    
    m_rootRegion = Cmiss_command_data_get_root_region(m_commandData);
    
    Cmiss_command_data_execute_command(m_commandData, "gfx create material temp normal_mode ambient 0.4 0.2 0.2 diffuse 0.9 0.9 0.7 emission 0 0 0 specular 0.1 0.1 0.1 alpha 0.92 shininess 0.2;");

	// Change to manual to avoid automatic creation of scene objects
	// when a region is added to the tree
	Cmiss_command_data_execute_command(m_commandData, "gfx modify scene default manual_g_element" );
}

    
Cmgui::~Cmgui() {
    Cmiss_command_data_destroy(&m_commandData);
}

    
void Cmgui::startAnimation(int start, int stop, int speed, bool once)
{
    std::stringstream command;
    command << "gfx timekeeper default play minimum ";
    command << start;
    command << " maximum ";
    command << stop;
    command << " speed ";
    command << speed;
    if (once) {
        command << " once";
    }
    Cmiss_command_data_execute_command(m_commandData, command.str().c_str());
}

void Cmgui::playAnimation(bool restart, bool once)
{
    std::stringstream command;
    command << "gfx timekeeper default play";
    if (once) {
        command << " once";
    }
    if (restart) {
        command << " set_time 0";
    }
    Cmiss_command_data_execute_command(m_commandData, command.str().c_str());
}

void Cmgui::stopAnimation()
{
    Cmiss_command_data_execute_command(m_commandData, "gfx timekeeper default stop");
}


void Cmgui::executeCommand(std::string command) {
    Core::StringHelper::setLocale();
    Cmiss_command_data_execute_command(m_commandData, command.c_str());
    Core::StringHelper::restoreLocale();
}

int CmguiPlugin::elementVisitorWrapper(struct Cmiss_element *elem, void *user_data) {
	return ((CmguiElementVisitor*) user_data)->visit(elem);
}

int CmguiPlugin::nodeVisitorWrapper(struct Cmiss_node *node, void *user_data) {
    return ((CmguiNodeVisitor*) user_data)->visit(node);
}

Cmiss_region * CmguiPlugin::Cmgui::GetRootRegion() const
{
	return m_rootRegion;
}

Cmiss_command_data * CmguiPlugin::Cmgui::GetCommandData() const
{
	return m_commandData;
}
