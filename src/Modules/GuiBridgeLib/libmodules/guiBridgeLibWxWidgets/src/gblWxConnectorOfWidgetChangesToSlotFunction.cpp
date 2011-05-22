/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "gblWxConnectorOfWidgetChangesToSlotFunction.h"
#include "CILabAssertMacros.h"
#include "wx/bmpcbox.h"

using namespace gbl::wx;

class ConnectorOfWidgetChangesToSlotFunction::Handler : public wxEvtHandler
{
public:
	Handler(ConnectorOfWidgetChangesToSlotFunction* callBack)
	{
		this->m_CallBack = callBack;
	}
	//! Receive a command event from one of the attached wxWidget controls.
	void ReceiveWxEvent(wxEvent &event)
	{
		this->m_CallBack->ReceiveWxEvent(event);
	}

private:
	ConnectorOfWidgetChangesToSlotFunction* m_CallBack;
};

ConnectorOfWidgetChangesToSlotFunction::ConnectorOfWidgetChangesToSlotFunction()
: m_BlockSignalFlag(new bool)
{
	*this->m_BlockSignalFlag = false;
}

void ConnectorOfWidgetChangesToSlotFunction::Observe( wxTextCtrl* textCtrl )
{
	Handler* handler = this->CreateHandler();
	handler->Connect(textCtrl->GetId(), wxEVT_COMMAND_TEXT_ENTER, 
		wxEventHandler(ConnectorOfWidgetChangesToSlotFunction::Handler::ReceiveWxEvent)); 
	handler->Connect(textCtrl->GetId(), wxEVT_KILL_FOCUS, 
		wxEventHandler(ConnectorOfWidgetChangesToSlotFunction::Handler::ReceiveWxEvent)); 
	this->Add(textCtrl, handler);
}

void ConnectorOfWidgetChangesToSlotFunction::Observe( wxRadioButton* radioButton )
{
	Handler* handler = this->CreateHandler();
	handler->Connect(radioButton->GetId(), wxEVT_COMMAND_RADIOBUTTON_SELECTED, 
		wxEventHandler(ConnectorOfWidgetChangesToSlotFunction::Handler::ReceiveWxEvent)); 
	this->Add(radioButton, handler);
}

void ConnectorOfWidgetChangesToSlotFunction::Observe( wxToggleButton* toggleButton )
{
	Handler* handler = this->CreateHandler();
	handler->Connect(toggleButton->GetId(), wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, 
		wxEventHandler(ConnectorOfWidgetChangesToSlotFunction::Handler::ReceiveWxEvent)); 
	this->Add(toggleButton, handler);
}

void ConnectorOfWidgetChangesToSlotFunction::Observe( wxCheckBox* checkBox )
{
	Handler* handler = this->CreateHandler();
	handler->Connect(checkBox->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
		wxEventHandler(ConnectorOfWidgetChangesToSlotFunction::Handler::ReceiveWxEvent)); 
	this->Add(checkBox, handler);
}

void gbl::wx::ConnectorOfWidgetChangesToSlotFunction::Observe( wxListBox* listBox )
{
	Handler* handler = this->CreateHandler();
	handler->Connect(listBox->GetId(), wxEVT_COMMAND_LISTBOX_SELECTED, 
		wxEventHandler(ConnectorOfWidgetChangesToSlotFunction::Handler::ReceiveWxEvent)); 
	this->Add(listBox, handler);
}

void gbl::wx::ConnectorOfWidgetChangesToSlotFunction::Observe( wxComboBox* comboBox )
{
	Handler* handler = this->CreateHandler();
	handler->Connect(comboBox->GetId(), wxEVT_COMMAND_COMBOBOX_SELECTED, 
		wxEventHandler(ConnectorOfWidgetChangesToSlotFunction::Handler::ReceiveWxEvent)); 
	this->Add(comboBox, handler);
}

void gbl::wx::ConnectorOfWidgetChangesToSlotFunction::Observe( wxBitmapComboBox* bitmapComboBox )
{
	Handler* handler = this->CreateHandler();
	handler->Connect(bitmapComboBox->GetId(), wxEVT_COMMAND_COMBOBOX_SELECTED, 
		wxEventHandler(ConnectorOfWidgetChangesToSlotFunction::Handler::ReceiveWxEvent)); 
	this->Add(bitmapComboBox, handler);
}

void gbl::wx::ConnectorOfWidgetChangesToSlotFunction::Observe( wxChoice* choice )
{
	Handler* handler = this->CreateHandler();
	handler->Connect(choice->GetId(), wxEVT_COMMAND_CHOICE_SELECTED, 
		wxEventHandler(ConnectorOfWidgetChangesToSlotFunction::Handler::ReceiveWxEvent)); 
	this->Add(choice, handler);
}

void gbl::wx::ConnectorOfWidgetChangesToSlotFunction::Observe(wxChoicebook* choice)
{
	Handler* handler = this->CreateHandler();
	handler->Connect(choice->GetId(), wxEVT_COMMAND_CHOICEBOOK_PAGE_CHANGED, 
		wxEventHandler(ConnectorOfWidgetChangesToSlotFunction::Handler::ReceiveWxEvent)); 
	this->Add(choice, handler);
}

void ConnectorOfWidgetChangesToSlotFunction::SetEnabled( bool _enabled )
{
	*(this->m_BlockSignalFlag) = !_enabled;
}

bool ConnectorOfWidgetChangesToSlotFunction::GetEnabled() const
{
	return !(*(this->m_BlockSignalFlag));
}

void gbl::wx::ConnectorOfWidgetChangesToSlotFunction::ReceiveWxEvent( wxEvent& event )
{
	if( *(this->m_BlockSignalFlag) == false )
		this->m_SlotFunction();
	event.Skip();
}

void gbl::wx::ConnectorOfWidgetChangesToSlotFunction::Add( wxWindow *control, 
	ConnectorOfWidgetChangesToSlotFunction::Handler* handler )
{
	if( m_Controls.find(control) == m_Controls.end() )
	{
		control->PushEventHandler(handler);
		m_Controls[control] = handler;
	}
}

void gbl::wx::ConnectorOfWidgetChangesToSlotFunction::Remove( wxWindow *control )
{
	Control2Handler::iterator it = m_Controls.find(control);
	if( it != m_Controls.end() )
	{
		bool removed = control->RemoveEventHandler(it->second);
		cilabAssertMacro(removed);
		delete it->second;
		this->m_Controls.erase(it);
	}
}

gbl::wx::ConnectorOfWidgetChangesToSlotFunction::~ConnectorOfWidgetChangesToSlotFunction()
{
	while( !this->m_Controls.empty() )
	{
		this->Remove(this->m_Controls.begin()->first);
	}
}

ConnectorOfWidgetChangesToSlotFunction::Handler* gbl::wx::ConnectorOfWidgetChangesToSlotFunction::CreateHandler()
{
	Handler* result = new Handler(this);
	return result;
}
