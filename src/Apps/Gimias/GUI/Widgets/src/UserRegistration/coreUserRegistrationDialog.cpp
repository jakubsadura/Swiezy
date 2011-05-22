/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreUserRegistrationDialog.h"
#include "GIMIASLogo.xpm"

#include "wx/url.h"

#define SOCKET_ID wxID( "SOCKET_ID" )

BEGIN_EVENT_TABLE(Core::Widgets::UserRegistrationDialog, coreUserRegistrationDialogUI)
	EVT_SOCKET(SOCKET_ID,     Core::Widgets::UserRegistrationDialog::OnSocketEvent)
END_EVENT_TABLE();

Core::Widgets::UserRegistrationDialog::UserRegistrationDialog(wxWindow* parent, int id, const wxString& title, const wxPoint& pos, const wxSize& size, long style):
    coreUserRegistrationDialogUI(parent, id, title, pos, size, style)
{
	m_bmpGIMIAS->SetBitmap( logo_xpm );

	// Create the socket
	m_sock = new wxSocketClient();

	// Setup the event handler and subscribe to most events
	m_sock->SetEventHandler(*this, SOCKET_ID);
	m_sock->SetNotify(wxSOCKET_CONNECTION_FLAG |
		wxSOCKET_INPUT_FLAG |
		wxSOCKET_LOST_FLAG);
	m_sock->Notify(true);
}

void Core::Widgets::UserRegistrationDialog::OnRegister( wxCommandEvent &event )
{
	try
	{
		if ( m_FirstName->IsEmpty() )
		{
			throw Core::Exceptions::Exception( 
				"UserRegistrationDialog::OnRegister", 
				"First name cannot be empty"  );
		}

		if ( m_LastName->IsEmpty() )
		{
			throw Core::Exceptions::Exception( 
				"UserRegistrationDialog::OnRegister", 
				"Last name cannot be empty"  );
		}

		if ( m_EmailAddress->IsEmpty() )
		{
			throw Core::Exceptions::Exception( 
				"UserRegistrationDialog::OnRegister", 
				"Email address cannot be empty"  );
		}

		if ( m_Company->IsEmpty() )
		{
			throw Core::Exceptions::Exception( 
				"UserRegistrationDialog::OnRegister", 
				"Institution/Company cannot be empty"  );
		}

		wxString hostname = "www.gimias.org";
		wxIPV4address addr;
		addr.Hostname(hostname);
		addr.Service(80);

		m_sock->Connect(addr, false);
		m_sock->WaitOnConnect(3);
		if (!m_sock->IsConnected())
		{
			throw Core::Exceptions::Exception( 
				"UserRegistrationDialog::OnRegister", 
				wxString::Format( "Can't connect to www.gimias.org", hostname) );
		}

		wxString msg;
		msg = "/images/UserRegistration.php?";
		msg += "FirstName=" + m_FirstName->GetValue();
		msg += "&LastName=" + m_LastName->GetValue();
		msg += "&EmailAddress=" + m_EmailAddress->GetValue();
		m_chkNewsletter->GetValue() ? msg += "&Newsletter=Yes" : msg += "&Newsletter=No";
		msg += "&Company=" + m_Company->GetValue();
		msg += "&ResearchField=" + m_ResearchField->GetValue();
		msg += "&UsingGIMIAS=" + m_UsingGIMIAS->GetValue();

		wxURI uri( msg );

		msg = "GET " + uri.GetPath() + "?" + uri.GetQuery() + " HTTP/1.1\nHost: " + hostname + "\n\n";
		m_sock->Write(msg.GetData(), msg.Length());
		if ( m_sock->Error() )
		{
			throw Core::Exceptions::Exception( 
				"UserRegistrationDialog::OnRegister", 
				wxString::Format( "Error connecting to %s", hostname ) );
		}

		m_sock->SetFlags(wxSOCKET_WAITALL);
		wxChar* buffer = new wxChar[30];
		m_sock->Read(buffer, 30);
		buffer[29] = 0;
		wxString returnMessage = buffer;
		delete buffer;

		if ( !returnMessage.Contains( "200 OK" ) )
		{
			throw Core::Exceptions::Exception( 
				"UserRegistrationDialog::OnRegister", 
				wxString::Format( "www.gimias.org returned error %s", returnMessage )  );
		}

		wxMessageBox( _("Your information has been sent successfully"), "User registration" );
		Core::Runtime::Settings::Pointer settings = Core::Runtime::Kernel::GetApplicationSettings();
		settings->SetShowRegistrationForm( false );
		settings->SetUserRegistered( true );
		wxCloseEvent evt(wxEVT_CLOSE_WINDOW, wxID_ANY);
		GetEventHandler()->AddPendingEvent( evt );
	}
	coreCatchExceptionsReportAndNoThrowMacro( UserRegistrationDialog::OnRegister )

	SetFocus();
}

void Core::Widgets::UserRegistrationDialog::OnRemindMe( wxCommandEvent &event )
{
	Core::Runtime::Settings::Pointer settings = Core::Runtime::Kernel::GetApplicationSettings();
	settings->SetShowRegistrationForm( true );
	settings->SetUserRegistered( false );
	wxCloseEvent evt(wxEVT_CLOSE_WINDOW, wxID_ANY);
	GetEventHandler()->AddPendingEvent( evt );
}

void Core::Widgets::UserRegistrationDialog::OnDoNotAsk( wxCommandEvent &event )
{
	Core::Runtime::Settings::Pointer settings = Core::Runtime::Kernel::GetApplicationSettings();
	settings->SetShowRegistrationForm( false );
	settings->SetUserRegistered( false );
	wxCloseEvent evt(wxEVT_CLOSE_WINDOW, wxID_ANY);
	GetEventHandler()->AddPendingEvent( evt );
}

void Core::Widgets::UserRegistrationDialog::OnSocketEvent( wxSocketEvent& event )
{
}

Core::Widgets::UserRegistrationDialog::~UserRegistrationDialog()
{
	// No delayed deletion here, as the frame is dying anyway
	delete m_sock;
}
