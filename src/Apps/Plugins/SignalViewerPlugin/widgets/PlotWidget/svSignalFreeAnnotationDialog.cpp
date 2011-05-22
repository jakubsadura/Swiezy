/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "svSignalFreeAnnotationDialog.h"

/**
Text control that captures ESC key and closes a dialog

\ingroup SignalViewerPlugin
\author Xavi Planes
\date 9 June 2010 
*/
class AnnotationTextCtrl : public wxTextCtrl
{
public:
	AnnotationTextCtrl(wxWindow* parent, wxWindowID id, const wxString&  value = "", const wxPoint&  pos = wxDefaultPosition, const wxSize&  size = wxDefaultSize, long style = 0, const wxValidator&  validator = wxDefaultValidator, const wxString&  name = wxTextCtrlNameStr)
		: wxTextCtrl( parent, id, value, pos, size, style, validator, name)
	{

	} 
	void OnKeyDown( wxKeyEvent& event )
	{
		if ( event.GetKeyCode( ) == WXK_ESCAPE)
		{
			m_Dialog->EndModal( wxID_CANCEL );
		}

		event.Skip( );
	}

	wxDialog* m_Dialog;

	DECLARE_EVENT_TABLE( );

};

BEGIN_EVENT_TABLE(AnnotationTextCtrl, wxTextCtrl)
  EVT_KEY_DOWN( AnnotationTextCtrl::OnKeyDown )
END_EVENT_TABLE();


svSignalFreeAnnotationDialog::svSignalFreeAnnotationDialog(wxWindow* parent, int id, const wxString& title, const wxPoint& pos, const wxSize& size, long style):
    svSignalFreeAnnotationDialogUI(parent, id, title, pos, size, style)
{
	// Add wxTE_PROCESS_ENTER style to the text control to process ENTER key
	AnnotationTextCtrl* txtName = new AnnotationTextCtrl(this, wxID_ANNOTATION_NAME, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	txtName->m_Dialog = this;
	GetSizer()->Replace( m_txtName, txtName );
	GetSizer()->Layout( );
	m_txtName->Destroy();
	m_txtName = txtName;
}

wxTextCtrl* svSignalFreeAnnotationDialog::GetTxtName() const
{
	return m_txtName;
}

void svSignalFreeAnnotationDialog::OnTextEnter( wxCommandEvent &event )
{
	EndModal( wxID_OK );
}
