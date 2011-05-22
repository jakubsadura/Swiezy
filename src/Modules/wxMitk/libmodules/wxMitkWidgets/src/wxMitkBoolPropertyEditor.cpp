/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include <wxMitkBoolPropertyEditor.h>
#include <mitkRenderingManager.h>

BEGIN_EVENT_TABLE(wxMitkBoolPropertyEditor, wxMitkBoolPropertyView)
EVT_CHECKBOX(wxID_Checked,  wxMitkBoolPropertyEditor::onToggle)
END_EVENT_TABLE()

wxMitkBoolPropertyEditor::wxMitkBoolPropertyEditor( const mitk::BoolProperty* property, wxWindow* parent, const char* name )
: wxMitkBoolPropertyView( property, parent, name )
{
	this->Enable( true );
}

wxMitkBoolPropertyEditor::~wxMitkBoolPropertyEditor()
{
}

void wxMitkBoolPropertyEditor::PropertyRemoved()
{
	wxMitkBoolPropertyView::PropertyRemoved(); // inherited function
	this->Enable(false);
}

void wxMitkBoolPropertyEditor::onToggle(wxCommandEvent &event)
{
	if (m_BoolProperty)
	{
		BeginModifyProperty();  // deregister from events
		const_cast<mitk::BoolProperty*>(m_BoolProperty)->SetValue(this->GetValue());
		mitk::RenderingManager::GetInstance()->SetNextLOD( 0 );
		mitk::RenderingManager::GetInstance()->RequestUpdateAll();
		EndModifyProperty();  // again register for events
	}
}
