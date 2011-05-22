/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "TavernaPluginWorkflowPanelWidgetFactory.h"
#include "coreSettings.h"
#include "wflWorkflowFactory.h"
#include "coreDirectory.h"
#include "blTextUtils.h"

const std::string STR_WORKFLOW_FOLDER = "Workflows";

Core::BaseWindowFactory::Pointer 
TavernaPlugin::WorkflowPanelWidgetFactory::NewBase( const wflWorkflow::Pointer workflow )
{
	WorkflowPanelWidgetFactory::Pointer factory = New( );
	factory->SetWorkflow( workflow );
	return factory.GetPointer( );
}

Core::BaseWindow* TavernaPlugin::WorkflowPanelWidgetFactory::CreateWindow()
{
	WorkflowPanelWidget* window = new WorkflowPanelWidget( 
		m_Workflow, GetParent(), GetWindowId( ), GetPosition( ), GetSize( ) );
	window->SetFactoryName( GetName( ) );
	if ( !GetWindowName( ).empty( ) ){ 
		wxWindowBase* windowBase = window; 
		windowBase->SetName( GetWindowName( ) ); 
	} 
	return window;
}

std::string TavernaPlugin::WorkflowPanelWidgetFactory::GetName() const
{
	return m_Workflow->GetProperty( "name" );
}

wflWorkflow::Pointer TavernaPlugin::WorkflowPanelWidgetFactory::GetWorkflow() const
{
	return m_Workflow;
}

void TavernaPlugin::WorkflowPanelWidgetFactory::SetWorkflow( wflWorkflow::Pointer val )
{
	m_Workflow = val;
}

void TavernaPlugin::WorkflowPanelWidgetFactory::Scan()
{
	Core::Runtime::Settings::Pointer settings;
	settings = Core::Runtime::Kernel::GetApplicationSettings();

	// Scan workflows dir
	wflWorkflowFactory::Pointer factory = wflWorkflowFactory::New( );
	factory->SetScanDirectory( settings->GetProjectHomePath() + Core::IO::SlashChar + STR_WORKFLOW_FOLDER );
	factory->Update();
	wflWorkflowFactory::WorkflowMapType workflows = factory->GetWorkflows();

	// For each workflow -> register a factory
	wflWorkflowFactory::WorkflowMapType::iterator it;
	for ( it = workflows.begin() ; it != workflows.end() ; it++ )
	{
		wflWorkflow::Pointer workflow = it->second;

		Core::Runtime::wxMitkGraphicalInterface::Pointer graphicalIface;
		graphicalIface = Core::Runtime::Kernel::GetGraphicalInterface();

        // Replace "_" by " " because in GTK "_" is not valid for menu items
        std::string name = workflow->GetProperty( "name" );
        blTextUtils::StrSub( name, "_", " " );

		std::list<std::string> windows;
		windows = graphicalIface->GetBaseWindowFactory( )->GetWindowsList( 
			WIDGET_TYPE_MAX, name );

		if ( windows.empty() )
		{
			// Panel widgets
			Core::WindowConfig config;
			config.ProcessorObservers().Category( "Taverna Workflow" );
			Core::Runtime::Kernel::GetGraphicalInterface()->RegisterFactory(
				NewBase( workflow ), 
				config.ProcessingTool().ProcessorObservers().Caption( name ) );
		}
		else
		{
			Core::BaseWindowFactory::Pointer factory;
			factory = graphicalIface->GetBaseWindowFactory( )->FindFactory( *windows.begin() );
			((WorkflowPanelWidgetFactory*) factory.GetPointer())->SetWorkflow( workflow );
		}

	}
}