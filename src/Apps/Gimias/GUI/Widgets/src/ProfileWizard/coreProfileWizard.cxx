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

#include <wxUnicode.h>

#include "coreProfileWizard.h"
#include "coreAssert.h"
#include "coreKernel.h"
#include "coreWxMitkGraphicalInterface.h"
#include "coreFrontEndPluginManager.h"
#include "coreReportExceptionMacros.h"

#include "blwxTreeCtrl.h"

using namespace Core::Widgets;

BEGIN_EVENT_TABLE(ProfileWizard, wxWizard)
END_EVENT_TABLE()

//!
ProfileWizard::ProfileWizard(wxFrame* frame)
: wxWizard(frame, wxID_ANY, wxT("Profile Manager"))
{
	try
	{
		// Create the pages
		m_StartPage = new wxWizardPageSimple(this);
		m_PageEnd = new wxWizardPageSimple(this, m_StartPage);
		m_StartPage->SetNext(m_PageEnd);

		m_PluginSelectorWidget = new PluginSelectorWidget(m_PageEnd);
		m_PluginSelectorWidget->UpdateWidget();
		
		// Config the first page
		SetupFirstPage();
		SetupSecondPage();

		// layout
		GetPageAreaSizer()->Add(m_StartPage);
		GetPageAreaSizer()->Add(m_PageEnd);
	}
	coreCatchExceptionsReportAndNoThrowMacro(ProfileWizard::ProfileWizard)
}

//!
ProfileWizard::~ProfileWizard(void)
{

}

//!
void ProfileWizard::SetupFirstPage(void)
{
	try
	{
		std::string textStr = \
			"We are going to spend one minute in configuring your user profile, based on your" \
			" requirements and the tools you need to work with.\n" \
			"Next, the graphical interface will configure itself in order to display only" \
			" the interface elements you select, while hiding the others.\n\n" \
			"Remember that you can call again this wizard whenever you want from the main menu of the application.";
		wxStaticText* lblTextTitle = new wxStaticText(m_StartPage, wxID_ANY, wxT("Welcome to the profile wizard\n\n"));
		wxStaticText* lblTextPh = new wxStaticText(m_StartPage, wxID_ANY, _U(textStr.c_str()), wxDefaultPosition, wxSize(200, 300));

		// Set some style
		wxFont font = lblTextTitle->GetFont();
		font.SetWeight(wxBOLD);
		lblTextTitle->SetFont(font);

		// layout
		wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
		sizer->Add(lblTextTitle, 0, wxEXPAND);
		sizer->Add(lblTextPh, 1, wxEXPAND);
		m_StartPage->SetSizer(sizer);
	}
	coreCatchExceptionsAddTraceAndThrowMacro(ProfileWizard::SetupFirstPage)
}

//!
void ProfileWizard::SetupSecondPage(void)
{
	try
	{
		wxStaticText* lblTextTitle = new wxStaticText(m_PageEnd, wxID_ANY, wxT("Plugin manager\n\n"));
		// Set some style
		wxFont font = lblTextTitle->GetFont();
		font.SetWeight(wxBOLD);
		lblTextTitle->SetFont(font);

		// layout
		wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
		sizer->Add(lblTextTitle, 0, wxEXPAND);
		sizer->Add( m_PluginSelectorWidget, 1, wxEXPAND);
		m_PageEnd->SetSizer(sizer);
	}
	coreCatchExceptionsAddTraceAndThrowMacro(ProfileWizard::SetupSecondPage)
}


/**
 */
Core::Profile::Pointer Core::Widgets::ProfileWizard::GetSelectedProfile() const
{
	return m_PluginSelectorWidget->GetSelectedProfile( );
}

/**
 */
wxWizardPageSimple* Core::Widgets::ProfileWizard::GetStartPage() const
{
	return m_StartPage;
}

wxWizardPageSimple* Core::Widgets::ProfileWizard::GetSecondPage() const
{
	return m_PageEnd;
}

void Core::Widgets::ProfileWizard::UpdateData()
{
	m_PluginSelectorWidget->UpdateData( );
}
