/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _CmguiPluginMacros_H
#define _CmguiPluginMacros_H

extern "C"
{
#include "api/cmiss_command_data.h"
#include "api/cmiss_field.h"
#include "api/cmiss_texture.h"
#include "api/cmiss_scene_viewer.h"
}



#define cmguiCommand(className, methodName, description) \
\
class className : public CmguiPlugin::Command { \
public: \
    className(Cmgui *cmgui) { \
        m_cmgui = cmgui; \
    } \
    \
    void execute() { \
        m_cmgui->methodName(); \
    } \
    \
    std::string getText() { \
        return description; \
    } \
    \
private: \
    Cmgui * m_cmgui; \
};


#endif // _CmguiPluginMacros_H

