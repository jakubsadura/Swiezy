/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

// For compilers that don't support precompilation, include "wx/wx.h"
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
       #include <wx/wx.h>
#endif

#include "wxToolBoxControl.h"
#include "wxToolBoxItem.h"
#include "wxToolBoxItemSelectedEvent.h"

BEGIN_EVENT_TABLE(wxToolBoxControl, wxPanel)
	EVT_TBOX_ITEM_SELECTED(wxID_ANY, wxToolBoxControl::OnToggleItem)
END_EVENT_TABLE()

//!
wxToolBoxControl::wxToolBoxControl(wxWindow* parent, wxWindowID id, bool exclusiveMode, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
: wxPanel(parent, id, pos, size, style, name)
{
	m_exclusiveMode = exclusiveMode;

	// Layout
	this->SetAutoLayout(true);
	this->m_internalLayout = new wxBoxSizer(wxVERTICAL);
	this->m_internalLayoutTop = new wxBoxSizer(wxVERTICAL);
	this->m_internalLayoutBottom = new wxBoxSizer(wxVERTICAL);
	this->m_internalLayout->Add(this->m_internalLayoutTop, 1, wxEXPAND);
	this->m_internalLayout->Add(this->m_internalLayoutBottom, 0, wxEXPAND);
	this->SetSizer(this->m_internalLayout);
}

//!
wxToolBoxControl::~wxToolBoxControl(void)
{
	this->m_registeredItems.clear();
}

/**
\brief Adds a widget (wxWindow*) to the toolbox
*/
void wxToolBoxControl::Add(wxToolBoxItem* item)
{
	if(item == NULL)
		return;
	item->Reparent(this);
	this->m_internalLayoutTop->Add(item, 0, wxEXPAND);
	this->m_registeredItems.push_back(item);
}

/**
\brief Removes a widget (wxWindow*) from the toolbox. It does not release memmory for the wxToolBoxItem or
reparents the widget that was holding, so the widget still has the wxToolBoxItem as a parent.
*/
void wxToolBoxControl::Remove(wxToolBoxItem* item)
{
	if(item == NULL)
		return;

	// Erase the widget from the list
	this->m_internalLayout->Detach(item);
	item->Reparent(NULL);
	this->m_registeredItems.remove(item);
}

/**
\brief Clears the toolbox 
It does not manage the memmory for the widgets (wxWindow*) that had mapped or reparent them, so they still are alive 
(and parented to the toolbox - read the destructor of wxToolBoxItem class), and they will be deleted when killing
the toolbox.
This function do releases the memmory for all wxMitkToolBoxItems that were registered to the toolbox.
\sa wxToolBoxItem
*/
void wxToolBoxControl::Clear(void)
{
	while(this->m_registeredItems.size() > 0)
	{
		// Erase the widget from the list
		wxToolBoxItem* item = this->m_registeredItems.front();
		this->m_internalLayoutTop->Detach(item);
		this->m_internalLayoutBottom->Detach(item);
		item->Reparent(NULL);
		this->m_registeredItems.pop_front();
		delete item;
	}
}

//!
wxToolBoxItem* wxToolBoxControl::GetItemByCaption(const wxString& caption)
{
	for(ItemList::iterator it = this->m_registeredItems.begin(); it != this->m_registeredItems.end(); ++it)
	{
		if((*it)->GetCaption().compare(caption) == 0)
			return *it;
	}
	return NULL;
}

//!
wxToolBoxItem* wxToolBoxControl::GetItemByWidget(wxWindow* widget)
{
	for(ItemList::iterator it = this->m_registeredItems.begin(); it != this->m_registeredItems.end(); ++it)
	{
		if((*it)->GetWidget() == widget)
			return *it;
	}
	return NULL;
}

//!
int wxToolBoxControl::GetNumberOfItems(void)
{
	return this->m_registeredItems.size();
}

//!
void wxToolBoxControl::CollapseAll(void)
{
	for(ItemList::iterator it = this->m_registeredItems.begin(); it != this->m_registeredItems.end(); ++it)
	{
		(*it)->Collapse();
	}
}

/**
When exclusive mode is set to true, the user will be able to open just one item at a time. The others
will collapse automatically.
*/
void wxToolBoxControl::SetExclusive(bool exclusiveMode)
{
	this->m_exclusiveMode = exclusiveMode;
}



//!
void wxToolBoxControl::OnToggleItem(wxToolBoxItemSelectedEvent& event)
{
	wxToolBoxItem* item = event.GetSelectedItem();
	if(this->m_exclusiveMode && item != NULL)
	{
		bool wasExpanded = item->IsExpanded();
		CollapseAll();

		if(wasExpanded)
			item->Expand();

		bool nowBottom = false;
		this->m_internalLayoutTop->Clear(false);
		this->m_internalLayoutBottom->Clear(false);
		for(ItemList::iterator it = this->m_registeredItems.begin(); it != this->m_registeredItems.end(); ++it)
		{
			if((*it) == item)
			{
				this->m_internalLayoutTop->Add((*it), 1, wxEXPAND);
				nowBottom = true;
			}
			else
			{
				if(!nowBottom)
					this->m_internalLayoutTop->Add((*it), 0, wxEXPAND);
				else
					this->m_internalLayoutBottom->Add((*it), 0, wxEXPAND);
			}

			// Disable all panel widgets
			(*it)->GetWidget( )->Enable( false );
		}
	}
	
	// 
	OnSelectedItem( item->GetWidget()->GetId() );
	// Enable the expanded one
	item->GetWidget( )->Enable( true );

	// Resize this
	// Cast a resize event
	wxSizeEvent resEvent(this->GetBestSize(), this->GetId());
    resEvent.SetEventObject(this);
	this->GetEventHandler()->ProcessEvent(resEvent);
}

/**
 */
void wxToolBoxControl::OnSelectedItem( int iItemID )
{

}

void wxToolBoxControl::SetEnable( bool bVal )
{
	for(ItemList::iterator it = this->m_registeredItems.begin(); it != this->m_registeredItems.end(); ++it)
	{
		bool isExpanded = (*it)->IsExpanded();
		if ( isExpanded )
		{
			(*it)->GetWidget( )->Enable( bVal );
		}
	}
}

void wxToolBoxControl::SetSelectedItem( wxWindow* widget )
{
	GetItemByWidget( widget )->Toggle();
}

