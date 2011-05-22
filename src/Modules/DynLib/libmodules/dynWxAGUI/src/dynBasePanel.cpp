/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "dynBasePanel.h"
#include "ModuleDescriptionUtilities.h"
#include "blTextUtils.h"

#include <wx/wupdlock.h>

BEGIN_EVENT_TABLE(dynBasePanel, dynBasePanelUI)
  EVT_COLLAPSIBLEPANE_CHANGED( wxID_ANY, dynBasePanel::OnCollapsiblePanel )
  EVT_BUTTON(wxID_BROWSE_BUTTON, dynBasePanel::OnBrowseBtn)
  EVT_BUTTON(wxID_RESET_BUTTON, dynBasePanel::OnResetBtn)
END_EVENT_TABLE()

dynBasePanel::dynBasePanel(wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style):
    dynBasePanelUI(parent, id, pos, size, style)
{
	m_Module = NULL;
	m_Updater = dynWxGUIUpdater::New( );
}

void dynBasePanel::OnApply( wxCommandEvent &event )
{
	m_Updater->UpdateData();
	event.Skip();
}

ModuleDescription* dynBasePanel::GetModule() const
{
	return m_Module;
}

void dynBasePanel::SetModule( ModuleDescription* val )
{
	m_DefaultModule = val;
	m_Module = val;
	m_Updater->SetModule( m_Module );
	m_Updater->UpdateWidget();
}

dynWxGUIUpdater::Pointer dynBasePanel::GetUpdater() const
{
	return m_Updater;
}

void dynBasePanel::SetUpdater( dynWxGUIUpdater::Pointer val )
{
	m_Updater = val;
}

void dynBasePanel::OnCollapsiblePanel( wxCollapsiblePaneEvent &event )
{
	wxWindow* parent = this;

	// Get the parent of the parent or the parent
	if ( parent->GetParent() )
	{
		parent = parent->GetParent();
	}
	if ( parent->GetParent() )
	{
		parent = parent->GetParent();
	}

	wxWindowUpdateLocker noUpdates( this );

	// Resize parent
	// Cast a resize event
	wxSizeEvent resEvent(GetParent()->GetBestSize(), GetParent()->GetId());
	resEvent.SetEventObject(GetParent());
	GetParent()->GetEventHandler()->ProcessEvent(resEvent);

	parent->FitInside();

	parent->Refresh();


}

void dynBasePanel::OnBrowseBtn(wxCommandEvent &event)
{
	wxWindow* ctrlWin = wxDynamicCast( event.GetEventObject(), wxWindow );

	// Find parameter and control
	std::string paramName = ctrlWin->GetName().c_str();
	size_t pos = paramName.rfind( "Button" );
	if ( pos == std::string::npos )
	{
		return;
	}
	paramName = paramName.substr( 0, pos );
	ModuleParameter* param = findModuleParameter( m_Module, "", 0, paramName );
	wxTextCtrl* textCtrl = wxDynamicCast( FindWindow( paramName ), wxTextCtrl );
	if ( textCtrl == NULL )
	{
		return;
	}

	if ( param->GetTag() == "file" )
	{
		// Open file dialog
		std::string dataPath;
		wxFileDialog* openFileDialog = 
			new wxFileDialog(this, wxT("Open a data file"), wxT(""), wxT(""), wxT(""), 
			wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxMULTIPLE );

		std::string wildcard = param->GetFileExtensionsAsString();
		blTextUtils::StrSub( wildcard, ".", "*." );
		blTextUtils::StrSub( wildcard, ",", ";" );
		openFileDialog->SetWildcard( wxString::FromUTF8( wildcard.c_str() ) );

		if(openFileDialog->ShowModal() == wxID_OK)
		{
			textCtrl->SetValue( openFileDialog->GetPath() );
		}
	}
	else if ( param->GetTag() == "directory" )
	{
		wxDirDialog* openDirectoryDialog = new wxDirDialog( 
			this, wxT("Directory"), wxT(""));

		if(openDirectoryDialog->ShowModal() == wxID_OK)
		{
			textCtrl->SetValue( openDirectoryDialog->GetPath() );
		}
	}

	event.Skip();
}

void dynBasePanel::OnResetBtn( wxCommandEvent &event )
{
	// iterate over each parameter group
	std::vector<ModuleParameterGroup>::const_iterator pgbeginit
		= m_DefaultModule->GetParameterGroups().begin();
	std::vector<ModuleParameterGroup>::const_iterator pgendit
		= m_DefaultModule->GetParameterGroups().end();
	std::vector<ModuleParameterGroup>::const_iterator pgit;

	for (pgit = pgbeginit; pgit != pgendit; ++pgit)
	{
		// iterate over each parameter in this group
		std::vector<ModuleParameter>::const_iterator pbeginit
			= (*pgit).GetParameters().begin();
		std::vector<ModuleParameter>::const_iterator pendit
			= (*pgit).GetParameters().end();
		std::vector<ModuleParameter>::const_iterator pit;

		for (pit = pbeginit; pit != pendit; ++pit)
		{
			if (   (*pit).GetTag() != "image"
				&& (*pit).GetTag() != "geometry"
				&& (*pit).GetTag() != "transform"
				&& (*pit).GetTag() != "table"
				&& (*pit).GetTag() != "measurement" )
			{
				m_Module->SetParameterDefaultValue( (*pit).GetName(), (*pit).GetDefault() );
			}
		}
	}

	GetUpdater()->UpdateWidget();
}
