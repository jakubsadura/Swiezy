/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _svSignalEventData_H
#define _svSignalEventData_H

#include <wx/listbox.h>

#include "blSignalEvent.h"


/**
Client data of list box or combo box for a blSignalEvent

\ingroup SignalViewerPlugin
\author Xavi Planes
\date 25 Jan 2010 
*/
class svSignalEventData : public wxClientData
{
public:
	//!
	svSignalEventData( blSignalEvent& event );

	//!
	blSignalEvent &GetSignalEvent( );

protected:
	//! 
	blSignalEvent m_SignalEvent;
};

#endif //_svSignalEventData_H
