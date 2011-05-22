/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "SandboxPluginSubtractPanelWidget.h"

// GuiBridgeLib
#include "gblWxBridgeLib.h"
#include "gblWxButtonEventProxy.h"
// Core
#include "coreDataEntityHelper.h"
#include "coreUserHelperWidget.h"
#include "coreDataEntityListBrowser.h"
#include "coreReportExceptionMacros.h"
#include "coreProcessorInputWidget.h"
// Core
#include "coreDataTreeHelper.h"
#include "coreProcessorFactories.h"

// Event the widget
BEGIN_EVENT_TABLE(SandboxPlugin::SubtractPanelWidget, SandboxPluginSubtractPanelWidgetUI)
	EVT_BUTTON(wxID_BTN_SUBTRACT, SandboxPlugin::SubtractPanelWidget::OnBtnApply)
END_EVENT_TABLE()

SandboxPlugin::SubtractPanelWidget::SubtractPanelWidget( 
	wxWindow* parent, int id,const wxPoint& pos /*= wxDefaultPosition*/, 
	const wxSize& size /*= wxDefaultSize*/, long style /*= 0*/ )
: SandboxPluginSubtractPanelWidgetUI(parent, id, pos, size, style)
{
	Core::Runtime::Kernel::GetProcessorFactories()->CreateProcessor<SubtractProcessor>( m_Processor );

	SetName( "Subtract images" );
	
}

SandboxPlugin::SubtractPanelWidget::~SubtractPanelWidget( )
{
	// We don't need to destroy anything because all the child windows 
	// of this wxWindow are destroyed automatically
}

void SandboxPlugin::SubtractPanelWidget::OnInit(  )
{
	GetInputWidget( 0 )->SetAutomaticSelection( false );
	GetInputWidget( 1 )->SetAutomaticSelection( false );

	UpdateWidget();
}

void SandboxPlugin::SubtractPanelWidget::UpdateWidget()
{
	// Check that the working data is initialized
	if( !m_Processor )
	{
		Enable(false);
		return;
	}

	// Disable automatic update of the widget to avoid recursively calls
	Validate();
}

bool SandboxPlugin::SubtractPanelWidget::Validate()
{
	bool okay = true;

	// Validate each text control. Pending
	return okay;
}

void SandboxPlugin::SubtractPanelWidget::OnBtnApply(wxCommandEvent& event)
{
	// Catch the exception from the processor and show the message box
	try
	{
		m_Processor->Update( );
	}
	coreCatchExceptionsReportAndNoThrowMacro( "SubtractPanelWidget::OnBtnApply" );
}

void SandboxPlugin::SubtractPanelWidget::UpdateHelperWidget()
{
	if ( GetHelperWidget( ) == NULL )
	{
		return;
	}

	std::string strText = "";
	if (m_Processor->GetInputDataEntity( 0 ).IsNull( ) )
	{
		strText = "Please select input data";
	}
	else
	{
		strText = "Please press button Apply";
	}

	GetHelperWidget( )->SetInfo( 
		Core::Widgets::HELPER_INFO_ONLY_TEXT, strText );
}

bool SandboxPlugin::SubtractPanelWidget::Enable( bool enable /*= true */ )
{
	bool bReturn = SandboxPluginSubtractPanelWidgetUI::Enable( enable );

	// If this panel widget is selected -> Update the widget
	if ( enable )
	{
		UpdateWidget();
	}

	return bReturn;
}

Core::BaseProcessor::Pointer SandboxPlugin::SubtractPanelWidget::GetProcessor()
{
	return m_Processor.GetPointer( );
}

void SandboxPlugin::SubtractPanelWidget::OnModifiedInput( int num )
{
	UpdateHelperWidget();
}

void SandboxPlugin::SubtractPanelWidget::UpdateData()
{

}
