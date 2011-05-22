/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "gblWxEventProxy.h"
#include "gblWxEventHandler.h"

using namespace gbl::wx;

gbl::wx::EventProxy::EventProxy( gbl::Bridge bridge, const std::string& bridgeEventId, wxWindow *control, int wxEventId )
: m_Bridge(bridge)
, m_BridgeEventId(bridgeEventId)
{
	this->Connect(control->GetId(), wxEventId, wxEventHandler(Self::OnReceiveWxEvent)); 
	SetEvtHandler(control, this);
}

gbl::wx::EventProxy::EventProxy( gbl::Bridge bridge, const std::string& bridgeEventId, wxWindow *control, 
								std::vector<int> wxEventIds )
: m_Bridge(bridge)
, m_BridgeEventId(bridgeEventId)
{
	for( size_t i = 0; i < wxEventIds.size(); ++i )
	{
		this->Connect(control->GetId(), wxEventIds[i], wxEventHandler(Self::OnReceiveWxEvent)); 
	}
	SetEvtHandler(control, this);
}

void gbl::wx::EventProxy::OnReceiveWxEvent( wxEvent& event )
{
	this->m_Bridge.CallEvent(this->m_BridgeEventId);
}
