/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _CmguiPluginCmgui_H
#define _CmguiPluginCmgui_H

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
class PLUGIN_EXPORT Cmgui
{
public:
    Cmgui();
    
    virtual ~Cmgui();

	//!
	Cmiss_region * GetRootRegion() const;

	//!
	Cmiss_command_data * GetCommandData() const;

	//!
	void executeCommand(std::string command);

    void startAnimation(int start, int stop, int speed, bool once = true);
    void playAnimation(bool restart = false, bool once = true);
    void stopAnimation();

private:
    void restart();
private:
	//!
    Cmiss_command_data *m_commandData;

	//!
	Cmiss_region *m_rootRegion;
    
	//!
    static const bool parentRegion = true;
};


class CmguiNodeVisitor {
public:
    virtual int visit(struct Cmiss_node *node) = 0;
};

class CmguiElementVisitor {
public:
	virtual int visit(struct Cmiss_element *elem) = 0;
};

int nodeVisitorWrapper(struct Cmiss_node *node, void *user_data);

int elementVisitorWrapper(struct Cmiss_element *elem, void *user_data);


} // namespace CmguiPluginCmgui

#endif // Cmgui_H

