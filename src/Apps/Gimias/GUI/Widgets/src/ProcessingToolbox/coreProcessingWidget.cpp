// Copyright 2009 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "coreProcessingWidget.h"
#include "coreSelectionToolboxWidget.h"
#include "corePluginTab.h"

Core::Widgets::ProcessingWidget::ProcessingWidget()
{
}

void Core::Widgets::ProcessingWidget::UpdateData()
{
}

void Core::Widgets::ProcessingWidget::UpdateWidget()
{
}

Core::Widgets::ProcessingWidget::~ProcessingWidget()
{

}

void Core::Widgets::ProcessingWidget::UpdateProcessor()
{
	// Set state to processing (dialog box)
	Core::Runtime::Kernel::GetApplicationRuntime( )->SetAppState( 
		Core::Runtime::APP_STATE_PROCESSING );

	try
	{
		GetProcessor()->Update( );
	}
	coreCatchExceptionsReportAndNoThrowMacro( "ProcessingWidget::UpdateProcessor" );

	Core::Runtime::Kernel::GetApplicationRuntime( )->SetAppState( 
		Core::Runtime::APP_STATE_IDLE );
}

void Core::Widgets::ProcessingWidget::SetInfoUserHelperWidget( std::string text )
{
	if ( GetHelperWidget() == NULL )
	{
		return;
	}

	GetHelperWidget()->SetInfo( 
		Core::Widgets::HELPER_INFO_ONLY_TEXT, text );
}

void Core::Widgets::ProcessingWidget::SetSelectionTool(
	wxWindowID id )
{
	if ( GetPluginTab() == NULL )
	{
		return;
	}

	SelectionToolboxWidget* selectionToolboxWidget;
	GetPluginTab()->GetWidget( wxID_SelectionToolboxWidget, selectionToolboxWidget );
	selectionToolboxWidget->SetToolByID( id );
	//GetPluginTab()->ShowWindow( wxID_SelectionToolboxWidget, true );
}

void Core::Widgets::ProcessingWidget::SetSelectionTool( const std::string &caption )
{
	if ( GetPluginTab() == NULL )
	{
		return;
	}

	SelectionToolboxWidget* selectionToolboxWidget;
	GetPluginTab()->GetWidget( wxID_SelectionToolboxWidget, selectionToolboxWidget );
	selectionToolboxWidget->SetToolByName( caption );
}

