/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "TavernaPluginConfigurationPanelWidget.h"
#include "coreKernel.h"
#include "coreSettings.h"
#include "itksys/SystemTools.hxx"

TavernaPlugin::ConfigurationPanelWidget::ConfigurationPanelWidget(  
	wxWindow* parent, int id/*= wxID_ANY*/,
	const wxPoint&  pos /*= wxDefaultPosition*/, 
	const wxSize&  size /*= wxDefaultSize*/, 
	long style/* = 0*/ )
: TavernaPluginConfigurationPanelWidgetUI(parent, id,pos,size,style)
{
}

TavernaPlugin::ConfigurationPanelWidget::~ConfigurationPanelWidget( )
{
	// We don't need to destroy anything because all the child windows 
	// of this wxWindow are destroyed automatically
}

void TavernaPlugin::ConfigurationPanelWidget::OnInit( )
{
}

void TavernaPlugin::ConfigurationPanelWidget::UpdateWidget()
{
	Core::Runtime::Settings::Pointer settings;
	settings = Core::Runtime::Kernel::GetApplicationSettings();
	std::string path;
	if ( settings->GetPluginProperty( "Taverna Plugin", "TavernaPath", path ) )
	{
		m_txtTavernaPath->SetValue( path );
	}

	if ( path.empty() || !itksys::SystemTools::FileExists( path.c_str() )  )
	{
#ifdef _WIN32
		std::list<std::string> defaultPaths;
		defaultPaths.push_back( "C:\\Program Files (x86)\\Taverna Workbench 2.2.0\\taverna.exe" );
		defaultPaths.push_back( "C:\\Program Files\\Taverna Workbench 2.2.0\\taverna.exe" );

		std::list<std::string>::iterator it;
		for ( it = defaultPaths.begin() ; it != defaultPaths.end() ; it++ )
		{
			if ( itksys::SystemTools::FileExists( it->c_str() ) )
			{
				m_txtTavernaPath->SetValue( *it );
				UpdateData();
			}
		}
#endif
	}

	std::string caption;
	if ( settings->GetPluginProperty( "Taverna Plugin", "TavernaCaption", caption ) )
	{
		m_txtTavernaCaption->SetValue( caption );
	}

	if ( caption.empty() )
	{
		m_txtTavernaCaption->SetValue( "Taverna Workbench 2.2.0" );
		UpdateData();
	}

}

void TavernaPlugin::ConfigurationPanelWidget::UpdateData()
{
	Core::Runtime::Settings::Pointer settings;
	settings = Core::Runtime::Kernel::GetApplicationSettings();
	settings->SetPluginProperty( "Taverna Plugin", "TavernaPath", 
		m_txtTavernaPath->GetValue().c_str() );
	settings->SetPluginProperty( "Taverna Plugin", "TavernaCaption", 
		m_txtTavernaCaption->GetValue().c_str() );
}

void TavernaPlugin::ConfigurationPanelWidget::OnBrowse( wxCommandEvent &event )
{
	std::string dataPath;
	wxFileDialog* openFileDialog = 
		new wxFileDialog(this, wxT("Select Taverna executable"), wxT(""), wxT(""), wxT(""), 
		wxFD_OPEN | wxFD_FILE_MUST_EXIST );

	if(openFileDialog->ShowModal() == wxID_OK)
	{
		m_txtTavernaPath->SetValue( openFileDialog->GetPath() );
	}

}
