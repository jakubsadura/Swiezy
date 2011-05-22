/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "gblWxEventHandler.h"
#include "CILabAssertMacros.h"

using namespace gbl::wx;

EvtHandlerConnection::EvtHandlerConnection(wxWindow* _eventSender, wxEvtHandler* _eventHandler) 
: eventHandler(_eventHandler)
{
	_eventSender->PushEventHandler(_eventHandler);
	_eventSender->AddChild(this);
}

EvtHandlerConnection::~EvtHandlerConnection()
{
	cilabAssertMacro(eventHandler == this->GetParent()->GetEventHandler());
	this->GetParent()->PopEventHandler(true);
}

void gbl::wx::SetEvtHandler(wxWindow* _eventSender, wxEvtHandler* _eventHandler)
{
	new EvtHandlerConnection(_eventSender, _eventHandler);
}
