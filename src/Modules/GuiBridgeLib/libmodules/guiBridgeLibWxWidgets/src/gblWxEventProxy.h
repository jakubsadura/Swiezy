/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef GBLWXEVENTPROXY_H
#define GBLWXEVENTPROXY_H

#include "CILabNamespaceMacros.h"
#include "gblBridge.h"
#include "wx/wx.h"
#include <vector>

CILAB_BEGIN_NAMESPACES(gbl, wx)

/**
See constructor for details on this class.

\author Maarten Nieber
\date 21 mar 2008
*/

class EventProxy
: public wxEvtHandler
{
public:
	typedef EventProxy Self;
	/**
	When this class receives event with id \a eventId from from \a control, it raises an
	event in \a bridge with id \a eventId.
	In its destructor, the \a control instance will eventually delete this class instance.
	*/
	EventProxy(gbl::Bridge bridge, const std::string& bridgeEventId, wxWindow *control, int wxEventId);
	//! Same as the above constructor, except that all ids in \a wxEventIds are connected.
	EventProxy(gbl::Bridge bridge, const std::string& bridgeEventId, wxWindow *control, std::vector<int> wxEventIds);
	//! Function handling the button push event.
	void OnReceiveWxEvent(wxEvent& event);

private:
	//!
	gbl::Bridge m_Bridge;
	//!
	std::string m_BridgeEventId;
};

CILAB_END_NAMESPACES(gbl, wx)

#endif //GBLWXEVENTPROXY_H
