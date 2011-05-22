/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef GBLEVENTPROXY_H
#define GBLEVENTPROXY_H

#include "CILabNamespaceMacros.h"
#include "gblBridge.h"
#include <string>

CILAB_BEGIN_NAMESPACE(gbl)

/**
This is the base class for event proxies. It stores a pointer to the bridge, and the id of the bridge event that must be called.
\author Maarten Nieber
\date 2 Sep 2007
*/

class GUIBRIDGELIB_EXPORT EventProxy
{
public:
	//! Stores \a _bridge pointer and \a _eventId so that we can call this event in CallEvent.
	EventProxy(Bridge bridge, const std::string& eventId);
	//!
	virtual ~EventProxy();

	//! Executes the event in the bridge.
	void CallEvent();

private:
	//!
	Bridge bridge;
	//!
	std::string eventId;
};

CILAB_END_NAMESPACE(gbl)

#endif //GBLEVENTPROXY_H
