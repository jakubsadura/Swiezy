/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

//core
#include "coreFuraPopupWidget.h"
#include "coreKernel.h"
#include "coreReportExceptionMacros.h"


#if defined(WIN32) && defined(USE_GRID_API)
//Fura
#include "FuraConnector.h"
#endif

//
#include <boost/thread/thread.hpp>

using namespace Core::Widgets;

// Event the widget
BEGIN_EVENT_TABLE(FuraPopupWidget, wxDialog)
#ifdef WIN32
	EVT_BUTTON( wxID_OK, FuraPopupWidget::OnOK)
#endif
	EVT_BUTTON (wxID_CANCEL, FuraPopupWidget::OnCancel)
	EVT_BUTTON (wxID_ANY, FuraPopupWidget::OnBrowse)
END_EVENT_TABLE()

//!
FuraPopupWidget::FuraPopupWidget(
wxWindow* parent, int id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
: coreFuraPopupWidgetUI(parent, id, title, pos, size, style)
{
	this->m_Filename = "";
	this->m_FilenamePath = ""; 
}


//!
FuraPopupWidget::~FuraPopupWidget(void)
{
}

/**
*/
#ifdef WIN32
void Core::Widgets::FuraPopupWidget::OnOK( wxCommandEvent& event )
{
	try
	{

		// Fura server host and port
		std::string host = txtCtrl_ServerName->GetValue();
		std::string port = txtCtrl_Port_Number->GetValue();

		// Local paths (input files and results folder)
		std::string definitionFile = txtCtrl_DefinitionFile->GetValue();
		std::string resultsPath = Core::Runtime::Kernel::GetApplicationSettings()->GetApplicationPathSubPath("data");
		std::string module = "ANSYS_Simulation_mod"; //Already configured on the server 
		std::string exGroup= "ansys";

		//TODO: If the browse button is not clicked, it crashes! So a temporal solution was to set
		// the txtCtrl_DefinitionFile as read only
		if (this->m_Filename == "")
			return;

		boost::thread thrd( 
			boost::bind(
				&FuraPopupWidget::ExecuteFuraConnector, this, host, port, resultsPath, module, exGroup) );
	}
	coreCatchExceptionsReportAndNoThrowMacro( FuraPopupWidget::OnOK )

}
#endif

void Core::Widgets::FuraPopupWidget::OnCancel( wxCommandEvent& event )
{
	// What to do if the user press cancel?
	wxDialog::Show( FALSE );
}

void Core::Widgets::FuraPopupWidget::OnBrowse( wxCommandEvent& event )
{
	try
	{	
		Core::Runtime::Settings::Pointer settings;
		settings = Core::Runtime::Kernel::GetApplicationSettings();
		wxFileDialog* openFileDialog = 
			new wxFileDialog(this, wxT("Open an ANSYS Definition file"), wxT(""), wxT(""), wxT(""), 
			wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxMULTIPLE );

		//! by default, all files are .def
		openFileDialog->SetWildcard(  wxT("*.def"));

		// Default value
		openFileDialog->SetDirectory(wxString::FromAscii(settings->GetCurrentDataPath().c_str()));

		if(openFileDialog->ShowModal() == wxID_OK)
		{
			//! Get the filenames and path
			// strange conversion from "wxString" over "const char*" to "std::string" necessary for some compilers
			m_FilenamePath = (const char*)openFileDialog->GetPath().c_str();
			m_Filename = (const char*)openFileDialog->GetFilename().c_str();
			
			//! Print them
			this->txtCtrl_DefinitionFile->SetValue(wxString::FromAscii(m_FilenamePath.c_str()));

		}
	}
	coreCatchExceptionsReportAndNoThrowMacro(Core::Widgets::FuraPopupWidget::OnBrowse)

}

/**
*/
#ifdef WIN32
void Core::Widgets::FuraPopupWidget::ExecuteFuraConnector( 
	std::string host, 
	std::string port, 
	std::string resultsPath, 
	std::string module, 
	std::string exGroup )
{
	// Microsoft Visual Studio 2003 (VC7)
#if defined(_MSC_VER) && (_MSC_VER >= 1300) && (_MSC_VER < 1400) && defined(USE_GRID_API )

	std::cout << "Configuring Task Parameters...Please wait..\n\n" << std::endl;

	try {

		// Only for CDTEAM branch
		GridAPI::Connector::Loadmodule(
			host, port, m_FilenamePath, 
			m_Filename, resultsPath, module, exGroup);

		std::cout << "Task Successfully executed\n";
	}
	catch( ... )
	{
		std::cout << "Task NOT Successfully executed\n";
	}

#else
	std::cout << "Fura is disabled\n";
#endif	

}
#endif

