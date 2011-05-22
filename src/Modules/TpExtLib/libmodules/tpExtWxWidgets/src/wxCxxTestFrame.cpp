/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "wxCxxTestFrame.h"

wxCxxTestFrame::wxCxxTestFrame( 
							   wxWindow* parent, 
							   wxWindowID id, 
							   const wxString& title,
							   const wxPoint& pos,
							   const wxSize& size
) : wxFrame(parent, id, title, pos, size)
{
	this->layoutOfClientWidgets = new wxBoxSizer(wxVERTICAL);
	this->layout = new wxBoxSizer(wxVERTICAL);
	this->SetAutoLayout(true);
	this->layout->Add(this->layoutOfClientWidgets, wxEXPAND, wxEXPAND, 0);
	this->buttonClose = new wxButton(this, wxID("buttonClose"), wxT("Close"));
	this->layout->Add(this->buttonClose, 0, wxEXPAND | wxALIGN_BOTTOM | wxALIGN_CENTER_HORIZONTAL, 0);
	this->buttonDebugEvent = new wxButton(this, wxID("buttonDebugEvent"), wxT("Send debug event"));
	this->layout->Add(this->buttonDebugEvent, 0, wxEXPAND | wxALIGN_BOTTOM | wxALIGN_CENTER_HORIZONTAL, 0);
	this->Connect(buttonClose->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxEventHandler(Self::OnButtonClose)); 
	this->Connect(buttonDebugEvent->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxEventHandler(Self::OnButtonDebugEvent)); 
	this->Connect(wxID_ANY, wxEVT_CLOSE_WINDOW, wxEventHandler(Self::OnButtonClose)); 
	this->SetSizer(this->layout);
}

void wxCxxTestFrame::StartMainLoop()
{
	wxTheApp->SetTopWindow(this);
	wxTheApp->SetExitOnFrameDelete(false);
	this->Show();
	wxTheApp->MainLoop();
}

void wxCxxTestFrame::OnButtonClose( wxEvent& event )
{
	wxTheApp->SetTopWindow(NULL);
	wxTheApp->ExitMainLoop();
}

void wxCxxTestFrame::OnButtonDebugEvent( wxEvent& event )
{
	if( this->m_Callback )
	{
		this->m_Callback();
	}
}

wxSizer* wxCxxTestFrame::GetLayout() const
{
	return layoutOfClientWidgets;
}

void wxCxxTestFrame::SetCallback( CallbackType callback )
{
	this->m_Callback = callback;
}
