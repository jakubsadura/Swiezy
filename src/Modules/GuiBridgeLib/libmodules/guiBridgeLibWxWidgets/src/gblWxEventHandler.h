/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef GBLBRIDGEQT_H
#define GBLBRIDGEQT_H

#include "gblBridge.h"
#include "wx/window.h"
#include "CILabNamespaceMacros.h"

CILAB_BEGIN_NAMESPACES(gbl, wx)

/**
See constructor for details.

\author Maarten Nieber
\date 21 mar 2008
*/

class /*GUIBRIDGELIB_EXPORT*/ EvtHandlerConnection : public wxWindow
{
public:
	/**
	Sets _eventHandler as event handler of _eventSender.
	Also, adds this EvtHandlerConnection as child of _eventSender, so the destructor of _eventSender will 
	also delete this EvtHandlerConnection instance.
	*/
	EvtHandlerConnection(wxWindow* _eventSender, wxEvtHandler* _eventHandler);

	/**
	Removes _eventHandler as event handler of _eventSender (see contructor).
	*/
	~EvtHandlerConnection();

private:
	wxEvtHandler* eventHandler;
};

/**
Convenience function for instantiating EvtHandlerConnection.
*/
void SetEvtHandler(wxWindow* _eventSender, wxEvtHandler* _eventHandler);

CILAB_END_NAMESPACES(gbl, wx)

#endif //GBLBRIDGEQT_H
