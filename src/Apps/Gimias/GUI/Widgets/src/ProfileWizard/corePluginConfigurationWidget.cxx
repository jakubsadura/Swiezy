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

#include "wx/wupdlock.h"

#include <blMitkUnicode.h>

#include "corePluginConfigurationWidget.h"
#include "coreAssert.h"
#include "coreKernel.h"
#include "coreWxMitkGraphicalInterface.h"
#include "coreFrontEndPluginManager.h"
#include "coreReportExceptionMacros.h"

using namespace Core::Widgets;

Core::Widgets::PluginConfigurationWidget::PluginConfigurationWidget( 
	wxWindow* parent, 
	int id /*= wxID_ANY*/, 
	const wxPoint& pos/*=wxDefaultPosition*/, 
	const wxSize& size/*=wxDefaultSize*/, 
	long style/*=0*/ ) : corePluginConfigurationUI( parent, id, pos, size, style )
{
	UpdateWidget();
}

//!
PluginConfigurationWidget::~PluginConfigurationWidget(void)
{

}

void Core::Widgets::PluginConfigurationWidget::OnAddPluginFolder( wxCommandEvent &event )
{
	try
	{

		std::string dirPath(event.GetString().mb_str());

		wxDirDialog* openDirectoryDialog = new wxDirDialog( 
			this, wxT("Open directory"), wxT(""));

		Core::Runtime::Settings::Pointer settings;
		settings = Core::Runtime::Kernel::GetApplicationSettings();
		std::string dataPath = settings->GetPluginsPath();

		openDirectoryDialog->SetPath(_U(dataPath));
		if(openDirectoryDialog->ShowModal() == wxID_OK)
		{
			dirPath = openDirectoryDialog->GetPath().mb_str(wxConvUTF8);
			if ( dirPath.empty() )
			{
				return;
			}

			std::vector<std::string> pluginsFolders = settings->GetPluginsFolders();
			pluginsFolders.push_back( dirPath );
			settings->SetPluginsFolders( pluginsFolders );

			Core::Runtime::wxMitkGraphicalInterface::Pointer graphicalIface;
			graphicalIface = Core::Runtime::Kernel::GetGraphicalInterface();

			Core::Runtime::FrontEndPluginManager::Pointer pManager;
			pManager = graphicalIface->GetFrontEndPluginManager();
			pManager->ScanPluginsFolders( );

			UpdateWidget();
		}
	}
	coreCatchExceptionsReportAndNoThrowMacro(PluginConfigurationWidget::OnAddPluginFolder);

}

void Core::Widgets::PluginConfigurationWidget::OnRemovePluginFolder( wxCommandEvent &event )
{
	try
	{

		if ( m_listBoxPluginFolders->GetSelection( ) == -1 )
		{
			return;
		}

		long item = m_listBoxPluginFolders->GetSelection( );
		wxString selectedFolder = m_listBoxPluginFolders->GetString( item );

		Core::Runtime::Settings::Pointer settings;
		settings = Core::Runtime::Kernel::GetApplicationSettings();

		std::vector<std::string> pluginsFolders = settings->GetPluginsFolders();
		std::vector<std::string>::iterator it;
		for ( it = pluginsFolders.begin() ; it != pluginsFolders.end() ; it++ )
		{
			if ( *it == selectedFolder.c_str() )
			{
				pluginsFolders.erase( it );
				break;
			}
		}
		settings->SetPluginsFolders( pluginsFolders );

		Core::Runtime::wxMitkGraphicalInterface::Pointer graphicalIface;
		graphicalIface = Core::Runtime::Kernel::GetGraphicalInterface();

		Core::Runtime::FrontEndPluginManager::Pointer pManager;
		pManager = graphicalIface->GetFrontEndPluginManager();
		pManager->ScanPluginsFolders( );

		UpdateWidget();
	}
	coreCatchExceptionsReportAndNoThrowMacro(PluginConfigurationWidget::OnAddPluginFolder);
}

void Core::Widgets::PluginConfigurationWidget::UpdateWidget()
{
	wxWindowUpdateLocker lockUpdates( this );
	m_listBoxPluginFolders->Clear( );

	Core::Runtime::Settings::Pointer settings;
	settings = Core::Runtime::Kernel::GetApplicationSettings();

	std::vector<std::string> pluginsFolders = settings->GetPluginsFolders();
	std::vector<std::string>::iterator it;
	for ( it = pluginsFolders.begin() ; it != pluginsFolders.end( ) ; it++ )
	{
		m_listBoxPluginFolders->Append( *it );
	}
}

void Core::Widgets::PluginConfigurationWidget::OnReload( wxCommandEvent &event )
{
	try
	{
		Core::Runtime::wxMitkGraphicalInterface::Pointer graphicalIface;
		graphicalIface = Core::Runtime::Kernel::GetGraphicalInterface();

		Core::Runtime::FrontEndPluginManager::Pointer pManager;
		pManager = graphicalIface->GetFrontEndPluginManager();
		pManager->ScanPluginsFolders( );

		pManager->LoadCommandLinePlugins(
			Core::Runtime::Kernel::GetApplicationSettings()->GetCurrentUserProfile() );
	}
	coreCatchExceptionsReportAndNoThrowMacro(PluginConfigurationWidget::OnReload);
}

void Core::Widgets::PluginConfigurationWidget::UpdateData()
{

}
