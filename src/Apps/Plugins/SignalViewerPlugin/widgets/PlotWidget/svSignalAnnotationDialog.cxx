/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "svSignalAnnotationDialog.h"



#define wxID_TEXT_ANNOTATION_CONTROL wxID( "wxID_TEXT_ANNOTATION_CONTROL")

BEGIN_EVENT_TABLE(svSignalAnnotationDialog, wxDialog)
  EVT_KEY_DOWN( svSignalAnnotationDialog::OnKeyDown )
  EVT_LISTBOX( wxID_TEXT_ANNOTATION_CONTROL, svSignalAnnotationDialog::OnEnterTextAnnotation)
END_EVENT_TABLE();

svSignalAnnotationDialog::svSignalAnnotationDialog(
	wxWindow* parent, 
	int id, 
	const wxString& title, 
	const wxPoint& pos, 
	const wxSize& size, 
	long style):
		wxDialog(parent, id, title, pos, size, wxRESIZE_BORDER|wxTHICK_FRAME)
{
    // begin wxGlade: svSignalAnnotationDialog::svSignalAnnotationDialog
	m_AnnotationList = new svSignalAnnotationListBox(this, wxID_TEXT_ANNOTATION_CONTROL);

    set_properties();
    do_layout();
    // end wxGlade
}


void svSignalAnnotationDialog::set_properties()
{
    // begin wxGlade: svSignalAnnotationDialog::set_properties
    SetTitle(wxT("dialog_1"));
    // end wxGlade
}


void svSignalAnnotationDialog::do_layout()
{
    // begin wxGlade: svSignalAnnotationDialog::do_layout
    wxBoxSizer* sizer_1 = new wxBoxSizer(wxVERTICAL);
    sizer_1->Add(m_AnnotationList, 1, wxEXPAND, 0);
    SetSizer(sizer_1);
    sizer_1->Fit(this);
    Layout();
    // end wxGlade
}


svSignalAnnotationListBox* svSignalAnnotationDialog::GetAnnotationList()
{
	return m_AnnotationList;
}

void svSignalAnnotationDialog::OnKeyDown( wxKeyEvent& event )
{
	if ( event.GetKeyCode( ) == WXK_ESCAPE)
	{
		EndModal( wxID_CANCEL );
	}
}


void svSignalAnnotationDialog::OnEnterTextAnnotation(wxCommandEvent& event )
{
	EndModal( wxID_OK );
}
