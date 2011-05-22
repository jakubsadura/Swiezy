/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "gblWxButtonEventProxy.h"
#include "gblWxEventProxy.h"

using namespace gbl::wx;

void gbl::AddBridgeEvent(gbl::Bridge bridge, const std::string& id, wxButton* button)
{
	new gbl::wx::EventProxy(bridge, id, button, wxEVT_COMMAND_BUTTON_CLICKED);
}
