// Copyright 2006 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "svSignalAnnotationListBox.h"
#include "svSignalEventData.h"
#include "wxID.h"
#include <wx/textctrl.h>
#include "blMitkUnicode.h"

#include <blSignalEvent.h>
#include "blSignalCollective.h"


BEGIN_EVENT_TABLE(svSignalAnnotationListBox, wxListBox)
  EVT_KEY_DOWN( svSignalAnnotationListBox::OnKeyDown )
END_EVENT_TABLE();

void svSignalAnnotationListBox::OnKeyDown( wxKeyEvent& event )
{
	GetParent()->GetEventHandler()->AddPendingEvent( event );
}

svSignalAnnotationListBox::svSignalAnnotationListBox( 
	wxWindow *parent, 
	wxWindowID id, 
	const wxPoint& pos /*= wxDefaultPosition*/, 
	const wxSize& size /*= wxDefaultSize*/, 
	int n /*= 0*/, 
	const wxString choices[] /*= NULL*/, 
	long style /*= 0*/, 
	const wxValidator& validator /*= wxDefaultValidator*/, 
	const wxString& name /*= wxListBoxNameStr*/ ) 
	: wxListBox( parent, id, pos, size, n, choices, style, validator, name )
{

	FillEvents( );
}

void svSignalAnnotationListBox::FillEvents()
{
	std::vector<blSignalEvent> events = blSignalEventTable::GetAllEvents();
	std::vector<blSignalEvent>::iterator iterator = events.begin();
	while (iterator != events.end())
	{
		if ( blSignalEventTable::IsEventSelected( iterator->m_Type ) )
		{
			Append( 
				_U(iterator->m_Description.c_str()), 
				new svSignalEventData( *iterator ) );
		}
		iterator++;
	}

}

bool svSignalAnnotationListBox::GetSelectedEvent( blSignalEvent &event )
{
	int selection = GetSelection();
	if ( selection == -1 )
	{
		return false;
	}

	svSignalEventData *item;
	item = dynamic_cast<svSignalEventData*> ( GetClientObject( selection ) );
	if ( item == NULL )
	{
		return false;
	}

	event = item->GetSignalEvent();
	return true;
}
