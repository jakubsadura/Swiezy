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

#include <blMitkUnicode.h>

#include "coreImportConfigurationWizard.h"
#include "coreAssert.h"
#include "coreKernel.h"
#include "coreWxMitkGraphicalInterface.h"
#include "coreFrontEndPluginManager.h"
#include "coreReportExceptionMacros.h"
#include "coreWorkflowManager.h"

#include "blwxTreeCtrl.h"

using namespace Core::Widgets;

#define wxID_chkList (1 + wxID_HIGHEST)

BEGIN_EVENT_TABLE(ImportConfigurationWizard, wxWizard)
  EVT_WIZARD_PAGE_CHANGED(wxID_ANY, ImportConfigurationWizard::OnPageChanged)
END_EVENT_TABLE()

//!
ImportConfigurationWizard::ImportConfigurationWizard(wxFrame* frame)
: wxWizard(frame, wxID_ANY, wxT("Import Configuration Wizard"))
{
	try
	{
		// Create the first page
		m_StartPage = new wxWizardPageSimple(this);
		SetupFirstPage();
		GetPageAreaSizer()->Add(m_StartPage);

		// Create second page
		m_SecondPage = new wxWizardPageSimple(this, m_StartPage);
		m_StartPage->SetNext(m_SecondPage);
		SetupSecondPage();
		GetPageAreaSizer()->Add(m_SecondPage);

		// Create third page
		m_EndPage = new wxWizardPageSimple(this, m_SecondPage);
		m_SecondPage->SetNext(m_EndPage);
		SetupEndPage();
		GetPageAreaSizer()->Add(m_EndPage);

		m_ImportConfigurationSuccess = true;
	}
	coreCatchExceptionsReportAndNoThrowMacro(ImportConfigurationWizard::ImportConfigurationWizard)
}

//!
ImportConfigurationWizard::~ImportConfigurationWizard(void)
{

}

//!
void ImportConfigurationWizard::SetupFirstPage(void)
{
	try
	{
		wxStaticText* lblTextTitle = new wxStaticText(m_StartPage, wxID_ANY, wxT("Welcome to the import configuration wizard\n\n"));
		wxFont font = lblTextTitle->GetFont();
		font.SetWeight(wxBOLD);
		lblTextTitle->SetFont(font);

		wxStaticText* lblTextPh = new wxStaticText(m_StartPage, wxID_ANY, _U(""), wxDefaultPosition, wxSize(200, 300));

		Core::Runtime::Settings::Pointer settings;
		settings = Core::Runtime::Kernel::GetApplicationSettings();
		std::string textStr;

		// If configuration was active from last time -> error
		if ( settings->GetImportConfiguration() )
		{
			textStr = \
				"This wizard has failed last time it was executed.\n" \
				"Is recommended to choose another configuration.\n";
			wxFont font = lblTextPh->GetFont();
			font.SetWeight(wxBOLD);
			lblTextPh->SetFont(font);
		}
		else
		{
			textStr = \
				"This wizard will scan for previous GIMIAS configurations and " \
				" will allow you to select the one you want to import.\n";
		}
		lblTextPh->SetLabel( textStr );

		// layout
		wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
		sizer->Add(lblTextTitle, 0, wxEXPAND);
		sizer->Add(lblTextPh, 1, wxEXPAND);
		m_StartPage->SetSizer(sizer);
	}
	coreCatchExceptionsAddTraceAndThrowMacro(ImportConfigurationWizard::SetupFirstPage)
}

//!
void ImportConfigurationWizard::SetupSecondPage(void)
{
	try
	{
		wxStaticText* lblTextTitle = new wxStaticText(m_SecondPage, wxID_ANY, wxT("Please select the configuration to import\n\n"));
		// Set some style
		wxFont font = lblTextTitle->GetFont();
		font.SetWeight(wxBOLD);
		lblTextTitle->SetFont(font);

		const wxString *m_List_choices = NULL;
		m_ConfigurationList = new wxListBox(m_SecondPage, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, m_List_choices, 0);

		// layout
		wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
		sizer->Add(lblTextTitle, 0, wxEXPAND);
		sizer->Add(m_ConfigurationList, 1, wxEXPAND);
		m_SecondPage->SetSizer(sizer);

	}
	coreCatchExceptionsAddTraceAndThrowMacro(ImportConfigurationWizard::SetupSecondPage)
}

void Core::Widgets::ImportConfigurationWizard::SetupEndPage()
{

	// Title
	m_lblTextTitleEndPage = new wxStaticText(m_EndPage, wxID_ANY, wxT("Finished configuration import\n\n"));
	wxFont font = m_lblTextTitleEndPage->GetFont();
	font.SetWeight(wxBOLD);
	m_lblTextTitleEndPage->SetFont(font);

	m_lblTextLabelEndPage = new wxStaticText(m_EndPage, wxID_ANY, _U(""), wxDefaultPosition, wxSize(200, 300));

	// layout
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(m_lblTextTitleEndPage, 0, wxEXPAND);
	sizer->Add(m_lblTextLabelEndPage, 1, wxEXPAND);
	m_EndPage->SetSizer(sizer);
}

bool Core::Widgets::ImportConfigurationWizard::ScanPreviousConfigurations()
{
	bool success = false;

	m_ConfigurationMap.clear();

	Core::Runtime::Settings::Pointer settings;
	settings = Core::Runtime::Kernel::GetApplicationSettings();
	std::string projectHomePath;

	// Scan Project home path for versions < 1.3.0
	projectHomePath = Core::IO::DirectoryHelper::GetHomePath() + Core::IO::SlashChar + ".gimias";
	Core::IO::Directory::Pointer directory = Core::IO::Directory::New();
	directory->SetDirNameFullPath( projectHomePath );
	if ( directory->Exists() )
	{
		m_ConfigurationMap[ "v1.2.0 (or older)" ] = projectHomePath;
		success = true;
	}

	// Scan Current .gimias folder
	projectHomePath = Core::IO::DirectoryHelper::GetApplicationData() + Core::IO::SlashChar + settings->GetProjectHomeFolderName( );
	directory->SetDirNameFullPath( projectHomePath );
	if ( directory->Exists() )
	{
		Core::IO::FileNameList contents;
		contents = directory->GetContents();
		Core::IO::FileNameList::iterator it;
		for ( it = contents.begin() ; it != contents.end() ; it++ )
		{
			Core::IO::Directory::Pointer subDirectory = Core::IO::Directory::New();
			subDirectory->SetDirNameFullPath( *it );
			if ( subDirectory->GetDirName( ) != settings->GetKernelVersion() )
			{
				m_ConfigurationMap[ subDirectory->GetDirName( ) ] = *it;
			}
			success = true;
		}
	}

	// Update configuration list
	m_ConfigurationList->Clear();
	std::map<std::string,std::string>::iterator it;
	for ( it = m_ConfigurationMap.begin() ; it != m_ConfigurationMap.end() ; it++ )
	{
		m_ConfigurationList->Append( it->first );
	}
	if ( m_ConfigurationList->GetCount() )
	{
		m_ConfigurationList->Select( m_ConfigurationList->GetCount() - 1 );
	}

	return success;
}

wxWizardPageSimple* Core::Widgets::ImportConfigurationWizard::GetStartPage() const
{
	return m_StartPage;
}

wxWizardPageSimple* Core::Widgets::ImportConfigurationWizard::GetSecondPage() const
{
	return m_SecondPage;
}

void Core::Widgets::ImportConfigurationWizard::OnPageChanged( wxWizardEvent& event )
{
	if ( event.GetPage() == m_SecondPage )
	{
		ScanPreviousConfigurations( );
	}
	else if ( event.GetPage() == m_EndPage )
	{
		// Label
		std::string textStr;
		if ( ImportPreviousConfiguration( ) )
		{
			textStr = "Configuration successfully imported.\n";
		}
		else
		{
			textStr = "Failed configuration import.\n";
		}
		m_lblTextLabelEndPage->SetLabel( textStr );
	}
}

bool Core::Widgets::ImportConfigurationWizard::ImportPreviousConfiguration()
{
	if ( m_ConfigurationList->GetSelection() == -1 )
	{
		return false;
	}

	std::string projectPath = m_ConfigurationMap[ m_ConfigurationList->GetStringSelection( ).c_str() ];

	try
	{
		Core::Runtime::Settings::Pointer settings;
		settings = Core::Runtime::Kernel::GetApplicationSettings();

		// Set import configuration to true
		settings->SetImportConfiguration( true );
		settings->SaveSettings();

		// Load settings
		m_ImportConfigurationSuccess = settings->LoadSettings( projectPath.c_str() );

		if ( m_ImportConfigurationSuccess )
		{
			Core::Runtime::wxMitkGraphicalInterface::Pointer graphicalIface;
			graphicalIface = Core::Runtime::Kernel::GetGraphicalInterface();

			graphicalIface->GetWorkingAreaStorage()->ScanDirectory( projectPath );
			graphicalIface->GetWorkingAreaStorage()->SaveAll();

			Core::Runtime::Kernel::GetWorkflowManager()->ScanDirectory( projectPath );
			Core::Runtime::Kernel::GetWorkflowManager()->SaveAll( );
		}

		// If was correctly import, set it to false
		if ( m_ImportConfigurationSuccess )
		{
			settings->SetImportConfiguration( false );
			settings->SaveSettings();
		}
	}
	catch( ... )
	{
		m_ImportConfigurationSuccess = false;
	}

	return m_ImportConfigurationSuccess;
}

