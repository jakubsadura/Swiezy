/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "wxEventHandlerHelper.h"
#include <list>

void wxPopEventHandler(wxWindow* win, wxEvtHandler* handlerToRemove)
{
	std::list<wxEvtHandler*> handlerList;
	wxEvtHandler* handler;

	// Remove all handlers from the window chain and put them in the list
	handlerList.push_back( handler = win->PopEventHandler( ) );
	while ( handler )
	{
		handler = win->PopEventHandler( );
		if ( handler )
		{
			handlerList.push_back( handler );
		}
	}

	// remove the desiered handler from the list
	handlerList.remove( handlerToRemove );

	// Push back all handlers from the list
	std::list<wxEvtHandler*>::reverse_iterator it;
	for ( it = handlerList.rbegin() ; it != handlerList.rend() ; it++ )
	{
		win->PushEventHandler( *it );
	}
}

