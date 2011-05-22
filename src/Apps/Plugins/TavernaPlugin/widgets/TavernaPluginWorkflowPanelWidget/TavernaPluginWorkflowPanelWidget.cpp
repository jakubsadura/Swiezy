/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "TavernaPluginWorkflowPanelWidget.h"
#include "TavernaPluginModuleConverter.h"

// Core
#include "coreSettings.h"


TavernaPlugin::WorkflowPanelWidget::WorkflowPanelWidget(  
	wflWorkflow::Pointer workflow,
	wxWindow* parent, int id/*= wxID_ANY*/,
	const wxPoint&  pos /*= wxDefaultPosition*/, 
	const wxSize&  size /*= wxDefaultSize*/, 
	long style/* = 0*/ )
: TavernaPluginWorkflowPanelWidgetUI(parent, id,pos,size,style)
{
	// Convert from Workflow to ModuleDescription
	ModuleConverter::Pointer converter = ModuleConverter::New();
	converter->SetWorkflow( workflow );
	converter->Update( );
	m_Module = converter->GetModule();

	// Create DynProcessor
	Core::Runtime::Settings::Pointer settings;
	settings = Core::Runtime::Kernel::GetApplicationSettings();

	m_Processor = Core::DynProcessor::New( );
	m_Processor->SetTemporaryDirectory( settings->GetApplicationPath() );
	m_Processor->SetWorkingDirectory( settings->GetApplicationPath() );
	m_Processor->SetModule( &m_Module );
}

TavernaPlugin::WorkflowPanelWidget::~WorkflowPanelWidget( )
{
	// We don't need to destroy anything because all the child windows 
	// of this wxWindow are destroyed automatically
}

void TavernaPlugin::WorkflowPanelWidget::OnInit( )
{
	// Don't select ouptut by default
	for ( int i = 0 ; i < m_Processor->GetNumberOfOutputs() ; i++ )
	{
		GetProcessorOutputObserver( i )->SelectDataEntity( false );
	}
}

void TavernaPlugin::WorkflowPanelWidget::UpdateWidget()
{

}

void TavernaPlugin::WorkflowPanelWidget::UpdateData()
{
	// Set parameters to processor. Pending
}

void TavernaPlugin::WorkflowPanelWidget::OnBtnApply(wxCommandEvent& event)
{
	UpdateData();

	UpdateProcessor( );
}

Core::BaseProcessor::Pointer TavernaPlugin::WorkflowPanelWidget::GetProcessor()
{
	return m_Processor.GetPointer( );
}
