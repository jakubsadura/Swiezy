/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef GBLWXBUTTONEVENTPROXY_H
#define GBLWXBUTTONEVENTPROXY_H

#include "wx/button.h"
#include "gblBridge.h"

namespace gbl
{
	//! Convenience function that instantiates ButtonEventProxy.
	void AddBridgeEvent(gbl::Bridge bridge, const std::string& id, wxButton* button);
}

#endif //GBLWXBUTTONEVENTPROXY_H
