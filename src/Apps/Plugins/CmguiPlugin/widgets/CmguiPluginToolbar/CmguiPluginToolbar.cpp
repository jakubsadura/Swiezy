/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "CmguiPluginToolbar.h"
#include "CmguiPluginCommandlineWidget.h"
#include "CmguiPluginWidgetCollective.h"

#include "coreKernel.h"
#include "coreWxMitkGraphicalInterface.h"
#include "coreFrontEndPluginManager.h"
#include "coreDirectory.h"

BEGIN_EVENT_TABLE(CmguiPlugin::Toolbar, wxToolBar)
END_EVENT_TABLE()

IMPLEMENT_CLASS( CmguiPlugin::wxComItem, wxObject )

CmguiPlugin::Toolbar::Toolbar(
	wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style, const wxString& name):
    wxToolBar(parent, id, pos, size, style, name)
{
	BuildToolbarPluginPath( );

	ScanToolbarPluginPath( );

	Realize();

	// Connect handler
	Connect(
		wxID_ANY,
		wxEVT_COMMAND_TOOL_CLICKED,
		wxCommandEventHandler(Toolbar::OnClickedTool)
		);

}

void CmguiPlugin::Toolbar::BuildToolbarPluginPath()
{
	Core::Runtime::wxMitkGraphicalInterface::Pointer graphicalIface;
	graphicalIface = Core::Runtime::Kernel::GetGraphicalInterface();

	Core::Runtime::FrontEndPluginLoader::Pointer pluginLoader;
	pluginLoader = graphicalIface->GetFrontEndPluginManager( )->GetPluginLoader( "CmguiPlugin" );

	m_PluginToolbarPath = pluginLoader->GetLibraryPath() + Core::IO::SlashChar + "Toolbar";
}

void CmguiPlugin::Toolbar::ScanToolbarPluginPath()
{
	Core::IO::Directory::Pointer workflowsDir = Core::IO::Directory::New();
	workflowsDir->SetDirNameFullPath( m_PluginToolbarPath );

	Core::IO::FileNameList list;
	list = workflowsDir->GetContents( );

	Core::IO::FileNameList::iterator it;
	for ( it = list.begin() ; it != list.end() ; it++ )
	{
		LoadTool( *it );
	}
}

void CmguiPlugin::Toolbar::LoadTool( std::string path )
{
	if ( itksys::SystemTools::GetFilenameExtension( path ) != ".com" )
	{
		return;
	}

	Core::IO::File::Pointer file = Core::IO::File::New( );
	file->SetFileNameFullPath( path );
	std::string iconFilename;
	iconFilename = itksys::SystemTools::GetFilenamePath( path );
	iconFilename += "/" + itksys::SystemTools::GetFilenameWithoutExtension( path );
	iconFilename += ".png";
	if ( !itksys::SystemTools::FileExists( iconFilename.c_str() ) )
	{
		std::cerr << "Cannot find bitmap " << iconFilename << std::endl;
		return;
	}

	wxBitmap bitmap( iconFilename, wxBITMAP_TYPE_PNG );
	if ( !bitmap.IsOk() )
	{
		std::cerr << "Cannot load bitmap " << iconFilename << std::endl;
		return;
	}

	std::string filename = itksys::SystemTools::GetFilenameWithoutExtension( path );
	AddTool( wxID_ANY, _T( filename.c_str() ),
		bitmap, wxNullBitmap, wxITEM_NORMAL,
		_T( filename.c_str() ), _T( filename.c_str() ), new wxComItem( path ) );
}

void CmguiPlugin::Toolbar::OnClickedTool( wxCommandEvent& event )
{
	wxObject *object = GetToolClientData( event.GetId() );

	wxComItem* item = wxDynamicCast( object, wxComItem );

	CommandlineWidget* coommandlineWidget;
	GetPluginTab( )->GetWidget( wxID_CmguiCommandline, coommandlineWidget );
	if ( !coommandlineWidget )
	{
		return;
	}

	std::ifstream file( item->m_FileName.c_str(), std::ifstream::in);
	while (file.good()) 
	{
		std::string command;
		getline(file, command);
		coommandlineWidget->execute( command );
	}

	GetMultiRenderWindow()->RequestUpdateAll();
}
