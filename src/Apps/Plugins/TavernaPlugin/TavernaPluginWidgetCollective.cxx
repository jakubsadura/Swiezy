/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "TavernaPluginWidgetCollective.h"
#include "TavernaPluginWorkflowPanelWidget.h"
#include "TavernaPluginModuleExecutionTaverna.h"
#include "TavernaPluginDataTransferTaverna.h"
#include "TavernaPluginWorkingArea.h"
#include "TavernaWorkingArea.xpm"
#include "TavernaPluginConfigurationPanelWidget.h"
#include "TavernaPluginToolbar.h"
#include "TavernaPluginWorkflowPanelWidgetFactory.h"

TavernaPlugin::WidgetCollective::WidgetCollective( ) 
{
	Core::Runtime::Kernel::GetGraphicalInterface()->CreatePluginTab( "Taverna Plugin" );

	// Register Taverna components
	dynModuleExecution::RegisterImpl( "TavernaModule", ModuleExecutionTaverna::Factory::New( ) );
	Core::DynDataTransferBase::RegisterDataTransfer( "TavernaModule", Core::DynDataTransferBase::Pointer( DataTransferTaverna::New() ) );

	// Scan workflows folder
	WorkflowPanelWidgetFactory::Scan();

	// WorkingArea
#ifdef _WIN32
	Core::Runtime::Kernel::GetGraphicalInterface()->RegisterFactory(
		WorkingArea::Factory::NewBase( ), 
		Core::WindowConfig( ).TabPage( "Taverna Plugin").WorkingArea()
		.Caption( "Taverna working area" )
		.Bitmap(tavernaworkingarea_xpm).Id( wxNewId() ) );
#endif

	// Configuration panel
	Core::Runtime::Kernel::GetGraphicalInterface()->RegisterFactory(
		ConfigurationPanelWidget::Factory::NewBase( ), 
		Core::WindowConfig( ).Preferences().TabPage("Taverna Plugin")
		.Caption( "Taverna configuration" ) );

	Core::Runtime::Kernel::GetGraphicalInterface()->RegisterFactory(
		Toolbar::Factory::NewBase( ),
		Core::WindowConfig( ).Toolbar( ).Top( ).TabPage("Taverna Plugin")
		.Show().Caption( "Taverna Toolbar" ).Id( wxNewId( ) ) );

}

TavernaPlugin::WidgetCollective::~WidgetCollective()
{
	// UnRegister Taverna components
	dynModuleExecution::UnRegisterImpl( "TavernaModule"  );
	Core::DynDataTransferBase::UnRegisterDataTransfer( "TavernaModule" );
}
