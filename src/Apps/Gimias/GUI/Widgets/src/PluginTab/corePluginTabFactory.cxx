/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "corePluginTabFactory.h"
#include "coreWxMitkGraphicalInterface.h"
#include "coreDataEntityListBrowser.h"
#include "coreBaseWindowFactories.h"

#include "coreToolbarWorkingArea.h"
#include "coreToolbarWindows.h"
#include "coreToolbarRegisteredWindows.h"
#include "coreToolbarSelectionTools.h"
#include "coreToolbarIO.h"

#include "coreCommandPanel.h"
#include "coreDataEntityListBrowser.h"
#include "coreMovieToolbar.h"
#include "coreDataEntityInformation.h"
#include "coreProcessingToolboxWidget.h"
#include "coreUserHelperWidget.h"
#include "coreVisualProperties.h"
#include "coreDataEntityInformation.h"
#include "coreWorkingAreaConfig.h"
#include "coreSelectionToolboxWidget.h"
#include "coreWorkflowNavigationWidget.h"
#include "CGNSFileReaderWidget.h"
#include "corePluginSelectorWidget.h"
#include "corePluginConfigurationWidget.h"

Core::Widgets::PluginTab* Core::Widgets::PluginTabFactory::Build( 
	const std::string &caption )
{
	Core::Widgets::PluginTab* pluginTab;
	pluginTab = CreatePluginTab( caption );
	CreateWidgets( pluginTab );

	return pluginTab;
}

Core::Widgets::PluginTab* Core::Widgets::PluginTabFactory::CreatePluginTab(
	const std::string &caption )
{
	Core::Runtime::wxMitkGraphicalInterface::Pointer localgIface;
	localgIface = Core::Runtime::Kernel::GetGraphicalInterface();

	Core::Widgets::PluginTab* pluginTab;
	pluginTab = new Core::Widgets::PluginTab(
		localgIface->GetMainWindow(),
		caption );

	return pluginTab;
}

void Core::Widgets::PluginTabFactory::CreateWidgets(
	Core::Widgets::PluginTab* pluginTab )
{
	std::list<std::string> windowsList;
	BaseWindowFactories::Pointer baseWindowFactory;
	baseWindowFactory = Core::Runtime::Kernel::GetGraphicalInterface()->GetBaseWindowFactory( );
	windowsList = baseWindowFactory->GetWindowsList( );
	std::list<std::string>::iterator it;
	for ( it = windowsList.begin( ) ; it != windowsList.end() ; it++ )
	{
		AddWindow( *it, pluginTab );
	}

}

void Core::Widgets::PluginTabFactory::AddWindow(
	const std::string &factoryName,
	Core::Widgets::PluginTab* pluginTab )
{
	BaseWindowFactories::Pointer baseWindowFactory;
	baseWindowFactory = Core::Runtime::Kernel::GetGraphicalInterface()->GetBaseWindowFactory( );

	if ( CheckCreateWindow( factoryName, pluginTab->GetCaption( ) ) )
	{
		BaseWindow *widget = baseWindowFactory->CreateWindow( factoryName, pluginTab );
		Core::WindowConfig config;
		baseWindowFactory->GetWindowConfig( factoryName, config );
		pluginTab->AddWindow( widget, config );
	}
}

bool Core::Widgets::PluginTabFactory::CheckCreateWindow(
	const std::string &factoryName,
	const std::string &tabName )
{
	BaseWindowFactories::Pointer baseWindowFactory;
	baseWindowFactory = Core::Runtime::Kernel::GetGraphicalInterface()->GetBaseWindowFactory( );

	Core::WindowConfig config;
	baseWindowFactory->GetWindowConfig( factoryName, config );
	bool tabPageIsOk = config.GetTabPage( ).empty( ) || config.GetTabPage( ) == tabName;
	bool isPluginTab = config.GetPluginTab();
	return tabPageIsOk && isPluginTab;
}

void Core::Widgets::PluginTabFactory::RegisterDefaultWidgets()
{
	Core::Runtime::wxMitkGraphicalInterface::Pointer gIface;
	gIface = Core::Runtime::Kernel::GetGraphicalInterface();
	Core::Runtime::Settings::Pointer settings;
	settings = Core::Runtime::Kernel::GetApplicationSettings();

	// Default windows
	gIface->RegisterFactory(
		WorkingAreaManager::Factory::NewBase( ),
		WindowConfig( ).Center( ).SetFlag( WIDGET_STATE_CLOSE_BUTTON, false )
		.Id( wxID_WorkingArea ).Caption( "Working area" ).Category( "Windows" ).Show() );

	gIface->RegisterFactory(
		DataEntityListBrowser::Factory::NewBase( ),
		WindowConfig( ).Layer(3).Right().Show()
		.Category( "Windows" ).Id( wxID_DataEntityListBrowser ).Caption( wxT( "Data tree" ) ) );

	gIface->RegisterFactory(
		CommandPanel::Factory::NewBase( ),
		WindowConfig( ).Layer(3).Right().Position(2).Show()
		.Category( "Windows" ).Id( wxID_CommandPanel ).Caption( wxT( "Command panel" ) ) );

	gIface->RegisterFactory(
		WorkflowNavigationWidget::Factory::NewBase( ),
		WindowConfig( ).Layer(3).Right().Position(2)
		.Category( "Windows" ).Id( wxID_WorkflowNavigation ).Caption( wxT( "Workflow Navigation" ) ) );

	gIface->RegisterFactory(
		MovieToolbar::Factory::NewBase( ),
		WindowConfig( ).Layer(0).Bottom()
		.Category( "Windows" ).Id( wxID_MovieToolbar ).Caption( wxT( "Time management" ) ) );

	gIface->RegisterFactory(
		DataEntityInformation::Factory::NewBase( ),
		WindowConfig( ).VerticalLayout()
		.Category( "Windows" ).Id( wxID_DataEntityInformationWidget ).Caption( "Data information" ) );

	gIface->RegisterFactory(
		WorkingAreaConfig::Factory::NewBase( ),
		WindowConfig( ).VerticalLayout()
		.Category( "Windows" ).Id( wxID_WorkingAreaConfig )
		.Caption( "Working Area configuration" ) );

	gIface->RegisterFactory(
		ProcessingToolboxWidget::Factory::NewBase( ),
		WindowConfig( ).VerticalLayout().Category( "Windows" )
		.Id( wxID_ProcessingToolboxWidget ).Caption( "Tools Processor Widget" ) );

	gIface->RegisterFactory(
		SelectionToolboxWidget::Factory::NewBase( ),
		WindowConfig( ).VerticalLayout().Category( "Windows" )
		.Id( wxID_SelectionToolboxWidget ).Caption( "Selection Toolbox" ) );

	gIface->RegisterFactory(
		VisualProperties::Factory::NewBase( ),
		WindowConfig( ).VerticalLayout().Category( "Visualization" )
		.Id( wxID_VisualProperties ).Caption( "VisualProperties" ) );


	// Toolbars
	gIface->RegisterFactory(
		ToolbarIO::Factory::NewBase( ),
		WindowConfig( ).Toolbar( ).Top( ).Show().Caption( "IO Toolbar" ).Id( wxID_IOToolbar ) );

	gIface->RegisterFactory(
		ToolbarWorkingArea::Factory::NewBase( ),
		WindowConfig( ).Toolbar( ).Top( ).Show().Caption( "Working Area Toolbar" ) );

	gIface->RegisterFactory(
		ToolbarWindows::Factory::NewBase( ),
		WindowConfig( ).Toolbar( ).Top( ).Show().Caption( "Windows Toolbar" ) );

	gIface->RegisterFactory(
		ToolbarSelectionTools::Factory::NewBase( ),
		WindowConfig( ).Toolbar( ).Top( ).Show().Caption( "Selection Toolbar" ) );


	// File header dialogs
	gIface->RegisterFactory( 
		CGNSFileReaderWidget::Factory::NewBase( ),
		WindowConfig( ).ReaderHeaderWindow().Caption( "CGNS File Header Reader" ) );

	// Preferences
	gIface->RegisterFactory(
		PluginSelectorWidget::Factory::NewBase( ), 
		Core::WindowConfig( ).Preferences().Caption( wxT("Selected plugins") ) );

	gIface->RegisterFactory(
		PluginConfigurationWidget::Factory::NewBase( ), 
		Core::WindowConfig( ).Preferences().Caption( wxT("Plugin configuration") ) );

}

std::list<std::string> Core::Widgets::PluginTabFactory::GetCommonWidgetsList( 
	const std::string &name )
{
	std::list<std::string> outputWindowsList;

	BaseWindowFactories::Pointer baseWindowFactory;
	baseWindowFactory = Core::Runtime::Kernel::GetGraphicalInterface()->GetBaseWindowFactory( );
	std::list<std::string> windowsList;
	windowsList = baseWindowFactory->GetWindowsList( );
	std::list<std::string>::iterator it;
	for ( it = windowsList.begin( ) ; it != windowsList.end() ; it++ )
	{
		if ( CheckCreateWindow( *it, name ) )
		{
			outputWindowsList.push_back( *it );
		}
	}

	return outputWindowsList;
}

