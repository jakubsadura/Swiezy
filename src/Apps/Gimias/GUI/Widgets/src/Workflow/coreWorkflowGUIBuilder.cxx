/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreWorkflowGUIBuilder.h"
#include "coreWxMitkGraphicalInterface.h"
#include "corePluginTabFactory.h"
#include "coreWorkflowNavigationWidget.h"


wxAuiNotebook* Core::Widgets::WorkflowGUIBuilder::Build( 
	Workflow::Pointer workflow )
{

	// Create a default worklow
	if ( workflow.IsNull() )
	{
		return NULL;
	}

	// Create GUI
	wxAuiNotebook* notebook = new wxAuiNotebook(
		Runtime::Kernel::GetGraphicalInterface()->GetMainWindow( ), 
		wxNewId(), wxDefaultPosition, wxDefaultSize, 
		wxAUI_NB_TOP | wxAUI_NB_TAB_SPLIT | wxAUI_NB_TAB_MOVE | wxAUI_NB_SCROLL_BUTTONS );

	wxFont font = *wxNORMAL_FONT;
	font.SetPointSize( 16 );
	notebook->SetFont( font );
	notebook->SetName( workflow->GetName( ) );

	BaseWindowFactories::Pointer baseWindowFactory;
	baseWindowFactory = Core::Runtime::Kernel::GetGraphicalInterface()->GetBaseWindowFactory( );

	bool success = true;

	// Create tab pages
	Workflow::StepListType::iterator it;
	for ( it = workflow->GetStepVector( ).begin() ; it != workflow->GetStepVector( ).end( ) ; it++ )
	{
		// Create plugin tab
		WorkflowStep::Pointer step = (*it);
		PluginTab* tabPage;
		tabPage = PluginTabFactory::CreatePluginTab( step->GetName( ) );
		tabPage->Reparent( notebook );
		notebook->AddPage(tabPage, _U(tabPage->GetCaption().c_str()), false);

		// Add other widgets to plugin tab
		WorkflowStep::WindowListType::iterator it;
		for ( it = step->GetWindowList().begin();
			  it != step->GetWindowList().end();
			  it++ )
		{
			// If factoryName is not registered -> continue
			try
			{
				BaseWindow *baseWindow = baseWindowFactory->CreateWindow( (*it), tabPage );
				// Show WorkflowNavigationWidget and hide comand panel by default
				WindowConfig config;
				baseWindowFactory->GetWindowConfig( (*it), config );
				if ( config.GetId() == wxID_WorkflowNavigation )
				{
					config.SetFlag( WIDGET_STATE_SHOW, true );
				}
				else if ( config.GetId() == wxID_CommandPanel )
				{
					config.SetFlag( WIDGET_STATE_SHOW, false );
				}
				// Add window to plugin tab page
				tabPage->AddWindow( baseWindow, config );
			}
			catch(Core::Exceptions::Exception& e) 
			{
				std::cerr << e.what( );
				e.AddTrace( "WorkflowGUIBuilder::Build" );
				Core::LogException(e);
				success = false;
			}
		}

		// Add CommandPanel widgets to WorkflowNavigationWidget
		WorkflowNavigationWidget* navigation;
		tabPage->GetWidget( wxID_WorkflowNavigation, navigation );
		if ( navigation )
		{
			navigation->SetWorkflowStep( step );
		}


		tabPage->BackupLayoutConfiguration();

		try
		{
			tabPage->SetProperties( step->GetProperties() );
		}
		coreCatchExceptionsReportAndNoThrowMacro( WorkflowGUIBuilder::Build )

	}

	if ( !success )
	{
		Core::Runtime::wxMitkGraphicalInterface::Pointer graphicalIface = 
			Core::Runtime::Kernel::GetGraphicalInterface();
		std::string message = "There are some missing components for this workflow.\n";
		message += "We recommend you to check the components using the Workflow Editor";
		graphicalIface->ReportMessage(  message );
	}

	return notebook;
}
