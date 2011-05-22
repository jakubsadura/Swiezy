/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "gblEventProxy.h"

gbl::EventProxy::EventProxy(Bridge _bridge, const std::string& _eventId)
: eventId(_eventId)
{
	this->bridge.ShallowCopyFrom(_bridge);
}

gbl::EventProxy::~EventProxy()
{

}

void gbl::EventProxy::CallEvent()
{
	this->bridge.CallEvent(this->eventId);
}
