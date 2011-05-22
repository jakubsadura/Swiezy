/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "SandboxPluginResamplePanelWidget.h"

// GuiBridgeLib
#include "gblWxBridgeLib.h"
#include "gblWxButtonEventProxy.h"

// Core
#include "coreUserHelperWidget.h"
#include "coreReportExceptionMacros.h"
#include "coreProcessorFactories.h"

// Event the widget
BEGIN_EVENT_TABLE(SandboxPlugin::ResamplePanelWidget, SandboxPluginResamplePanelWidgetUI)
	EVT_BUTTON(wxID_BTN_Resample, SandboxPlugin::ResamplePanelWidget::OnBtnApply)
END_EVENT_TABLE()


SandboxPlugin::ResamplePanelWidget::ResamplePanelWidget( 
	wxWindow* parent, int id, 
	const wxPoint& pos /*= wxDefaultPosition*/, 
	const wxSize& size /*= wxDefaultSize*/, long style /*= 0*/ )
: SandboxPluginResamplePanelWidgetUI(parent, id, pos, size, style)
{
	Core::Runtime::Kernel::GetProcessorFactories()->CreateProcessor<ResampleProcessor>( m_Processor );

	SetName( "Resample an image" );

}

SandboxPlugin::ResamplePanelWidget::~ResamplePanelWidget( )
{
	// We don't need to destroy anything because all the child windows 
	// of this wxWindow are destroyed automatically
}

void SandboxPlugin::ResamplePanelWidget::OnInit( )
{
	UpdateWidget();
}

void SandboxPlugin::ResamplePanelWidget::UpdateWidget()
{
	SandboxPlugin::ResampleProcessor::ScaleTransformType::ScaleType scale;
	scale  = m_Processor->GetScaleTransform( )->GetScale( );

	// Update each text control with the parameters of the WorkingData
	m_SpinCtrlX->SetValue( scale[ 0 ] * 100 );
	m_SpinCtrlY->SetValue( scale[ 1 ] * 100 );
	m_SpinCtrlZ->SetValue( scale[ 2 ] * 100 );

	// Enable Apply button
	bool bInputPointsSelected = CheckInputPointSelected();

	m_btnResample->Enable( bInputPointsSelected );

	UpdateHelperWidget( );

	Validate();
}

void SandboxPlugin::ResamplePanelWidget::UpdateData()
{
	if( !Validate() )
		return;

	SandboxPlugin::ResampleProcessor::ScaleTransformType::ScaleType scale;

	// Update each paramterer of the WorkingData
	scale[ 0 ] = double( m_SpinCtrlX->GetValue( ) ) / 100;
	scale[ 1 ] = double( m_SpinCtrlY->GetValue( ) ) / 100;
	scale[ 2 ] = double( m_SpinCtrlZ->GetValue( ) ) / 100;
	m_Processor->GetScaleTransform( )->SetScale( scale );
}

bool SandboxPlugin::ResamplePanelWidget::Validate()
{
	bool okay = true;

	// Validate each text control. Pending
	return okay;
}


void SandboxPlugin::ResamplePanelWidget::OnBtnApply(wxCommandEvent& event)
{
	// Catch the exception from the processor and show the message box
	try
	{
		// Update the scale values from widget to processor
		UpdateData();

		UpdateProcessor( );
	}
	coreCatchExceptionsReportAndNoThrowMacro( "ResamplePanelWidget::OnBtnApply" );
}

void SandboxPlugin::ResamplePanelWidget::UpdateHelperWidget()
{
	if ( GetHelperWidget( ) == NULL )
	{
		return;
	}

	bool bInputPointsSelected = CheckInputPointSelected( );

	if (m_Processor->GetInputDataEntity( ResampleProcessor::INPUT_IMAGE ).IsNull( ) )
	{
		GetHelperWidget( )->SetInfo( 
			Core::Widgets::HELPER_INFO_ONLY_TEXT, 
			"Please select input data" );
	}
	else if ( !bInputPointsSelected )
	{
		GetHelperWidget( )->SetInfo( 
			Core::Widgets::HELPER_INFO_LEFT_BUTTON, 
			"Select input point using Toolbar button" );
	}
	else
	{
		GetHelperWidget( )->SetInfo( 
			Core::Widgets::HELPER_INFO_ONLY_TEXT, 
			"Please press Apply button" );
	}

}

bool SandboxPlugin::ResamplePanelWidget::Enable( bool enable /*= true */ )
{
	bool bReturn = SandboxPluginResamplePanelWidgetUI::Enable( enable );

	// If this panel widget is selected -> Update the widget
	if ( enable )
	{
		UpdateWidget();
	}

	return bReturn;
}

bool SandboxPlugin::ResamplePanelWidget::CheckInputPointSelected()
{
	// Enable Apply button
	bool bInputPointsSelected = false;
	try{
		Core::vtkPolyDataPtr vtkInputPoint;
		Core::DataEntityHelper::GetProcessingData( 
			m_Processor->GetInputDataEntityHolder( ResampleProcessor::INPUT_POINT ),
			vtkInputPoint );
		bInputPointsSelected = vtkInputPoint->GetNumberOfPoints() != 0;
	}catch(...)
	{

	}

	return bInputPointsSelected;
}

Core::BaseProcessor::Pointer SandboxPlugin::ResamplePanelWidget::GetProcessor()
{
	return m_Processor.GetPointer( );
}

void SandboxPlugin::ResamplePanelWidget::OnModifiedInput( int num )
{
	try
	{
		switch ( num )
		{
		case ResampleProcessor::INPUT_POINT: UpdateWidget(); break;
		case ResampleProcessor::INPUT_IMAGE: UpdateHelperWidget( ); break;
		}
	}
	coreCatchExceptionsReportAndNoThrowMacro( "ResamplePanelWidget::OnModifiedInput" );
}
