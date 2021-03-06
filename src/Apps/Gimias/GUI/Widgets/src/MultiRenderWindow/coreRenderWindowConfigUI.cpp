// -*- C++ -*- generated by wxGlade 0.6.3 on Thu Nov 11 08:30:08 2010

#include "coreRenderWindowConfigUI.h"

// begin wxGlade: ::extracode

// end wxGlade


coreRenderWindowConfigUI::coreRenderWindowConfigUI(wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style):
    wxScrolledWindow(parent, id, pos, size, wxTAB_TRAVERSAL)
{
    // begin wxGlade: coreRenderWindowConfigUI::coreRenderWindowConfigUI
    ManageViews_staticbox = new wxStaticBox(this, -1, wxT("Manage Views"));
    const wxString *m_CmbNewView_choices = NULL;
    m_CmbNewView = new wxComboBox(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0, m_CmbNewView_choices, wxCB_DROPDOWN|wxCB_READONLY);
    m_btnAddColView = new wxButton(this, wxID_BTN_ADD_COL_VIEW , wxT("Add Column View"));
    m_btnAddRowView = new wxButton(this, wxID_BTN_ADD_ROW_VIEW, wxT("Add Row View"));
    m_btnRemoveView = new wxButton(this, wxID_BTN_REMOVE_VIEW, wxT("Remove View"));

    set_properties();
    do_layout();
    // end wxGlade
}


BEGIN_EVENT_TABLE(coreRenderWindowConfigUI, wxScrolledWindow)
    // begin wxGlade: coreRenderWindowConfigUI::event_table
    EVT_BUTTON(wxID_BTN_ADD_COL_VIEW , coreRenderWindowConfigUI::OnAddCol)
    EVT_BUTTON(wxID_BTN_ADD_ROW_VIEW, coreRenderWindowConfigUI::OnAddRow)
    EVT_BUTTON(wxID_BTN_REMOVE_VIEW, coreRenderWindowConfigUI::OnRemove)
    // end wxGlade
END_EVENT_TABLE();


void coreRenderWindowConfigUI::OnAddCol(wxCommandEvent &event)
{
    event.Skip();
    wxLogDebug(wxT("Event handler (coreRenderWindowConfigUI::OnAddCol) not implemented yet")); //notify the user that he hasn't implemented the event handler yet
}


void coreRenderWindowConfigUI::OnAddRow(wxCommandEvent &event)
{
    event.Skip();
    wxLogDebug(wxT("Event handler (coreRenderWindowConfigUI::OnAddRow) not implemented yet")); //notify the user that he hasn't implemented the event handler yet
}


void coreRenderWindowConfigUI::OnRemove(wxCommandEvent &event)
{
    event.Skip();
    wxLogDebug(wxT("Event handler (coreRenderWindowConfigUI::OnRemove) not implemented yet")); //notify the user that he hasn't implemented the event handler yet
}


// wxGlade: add coreRenderWindowConfigUI event handlers


void coreRenderWindowConfigUI::set_properties()
{
    // begin wxGlade: coreRenderWindowConfigUI::set_properties
    SetSize(wxSize(293, 336));
    SetScrollRate(10, 10);
    // end wxGlade
}


void coreRenderWindowConfigUI::do_layout()
{
    // begin wxGlade: coreRenderWindowConfigUI::do_layout
    wxBoxSizer* sizer_1 = new wxBoxSizer(wxVERTICAL);
    wxStaticBoxSizer* ManageViews = new wxStaticBoxSizer(ManageViews_staticbox, wxVERTICAL);
    ManageViews->Add(m_CmbNewView, 0, wxEXPAND, 0);
    ManageViews->Add(m_btnAddColView, 0, wxEXPAND, 0);
    ManageViews->Add(m_btnAddRowView, 0, wxEXPAND, 0);
    ManageViews->Add(m_btnRemoveView, 0, wxEXPAND, 0);
    sizer_1->Add(ManageViews, 0, wxALL|wxEXPAND, 5);
    SetSizer(sizer_1);
    // end wxGlade
}

