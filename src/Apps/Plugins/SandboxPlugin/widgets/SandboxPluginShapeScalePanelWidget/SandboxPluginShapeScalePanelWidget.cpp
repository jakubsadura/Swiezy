/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "SandboxPluginShapeScalePanelWidget.h"

// GuiBridgeLib
#include "gblWxBridgeLib.h"
#include "gblWxButtonEventProxy.h"

// Core
#include "coreVTKPolyDataHolder.h"
#include "coreDataEntityHelper.h"
#include "coreUserHelperWidget.h"
#include "coreRenderingTree.h"
#include "coreDataEntityListBrowser.h"
#include "coreReportExceptionMacros.h"

// STD
#include <limits>

// Core
#include "coreDataTreeHelper.h"
#include "coreProcessorWidgetsBuilder.h"
#include "coreProcessorFactories.h"

// Event the widget
BEGIN_EVENT_TABLE(SandboxPlugin::ShapeScalePanelWidget, SandboxPluginShapeScalePanelWidgetUI)
	EVT_BUTTON(wxID_BTN_SCALE, SandboxPlugin::ShapeScalePanelWidget::OnBtnScale)
END_EVENT_TABLE()

SandboxPlugin::ShapeScalePanelWidget::ShapeScalePanelWidget( 
	wxWindow* parent, int id, const wxPoint& pos /*= wxDefaultPosition*/, 
	const wxSize& size /*= wxDefaultSize*/, long style /*= 0*/ )
: SandboxPluginShapeScalePanelWidgetUI(parent, id, pos, size, style )
{
	Core::Runtime::Kernel::GetProcessorFactories()->CreateProcessor<ShapeScaleProcessor>( m_Processor );

	SetName( "SandboxPluginShapeScale" );

	// Each time a text control is modified, this function will be called
	m_changeInWidgetObserver.SetSlotFunction(this, &ShapeScalePanelWidget::UpdateData);
	m_changeInWidgetObserver.Observe(m_textScale);
}

SandboxPlugin::ShapeScalePanelWidget::~ShapeScalePanelWidget( )
{
	// We don't need to destroy anything because all the child windows 
	// of this wxWindow are destroyed automatically
}

void SandboxPlugin::ShapeScalePanelWidget::OnInit(  )
{
	m_Processor->GetParametersHolder()->AddObserver(
		this, &ShapeScalePanelWidget::UpdateWidget);

	UpdateWidget();
}

void SandboxPlugin::ShapeScalePanelWidget::UpdateWidget()
{
	// Check that the working data is initialized
	if( !m_Processor )
	{
		Enable(false);
		return;
	}

	// Disable automatic update of the widget to avoid recursively calls
	m_changeInWidgetObserver.SetEnabled(false);

	float fScale = m_Processor->GetParametersHolder()->GetSubject( );

	// Update each text control with the parameters of the WorkingData
	const int maxNrDecimals = 6;
	gbl::SetNumber(m_textScale, fScale, maxNrDecimals );

	m_changeInWidgetObserver.SetEnabled(true);
	Validate();

	// Update user helper
	UpdateHelperWidget( );
}

bool SandboxPlugin::ShapeScalePanelWidget::Validate()
{
	bool okay = true;

	// Validate each text control. Pending
	return okay;
}

void SandboxPlugin::ShapeScalePanelWidget::UpdateData()
{
	if( !Validate() )
		return;

	float fScale = 0;

	// Update each paramterer of the WorkingData
	fScale = gbl::GetNumber( m_textScale );

	m_Processor->GetParametersHolder()->SetSubject( fScale );
}

void SandboxPlugin::ShapeScalePanelWidget::OnBtnScale(wxCommandEvent& event)
{
	// Catch the exception from the processor and show the message box
	try
	{
		m_Processor->Update( );
	}
	coreCatchExceptionsReportAndNoThrowMacro( "ShapeScalePanelWidget::OnBtnScale" );
}

void SandboxPlugin::ShapeScalePanelWidget::UpdateHelperWidget()
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
		strText = "Please press button";
	}

	GetHelperWidget( )->SetInfo( 
		Core::Widgets::HELPER_INFO_ONLY_TEXT, strText );
}

bool SandboxPlugin::ShapeScalePanelWidget::Enable( bool enable /*= true */ )
{
	bool bReturn = SandboxPluginShapeScalePanelWidgetUI::Enable( enable );

	// If this panel widget is selected -> Update the widget
	if ( enable )
	{
		UpdateWidget();
	}

	return bReturn;
}

void SandboxPlugin::ShapeScalePanelWidget::OnModifiedInput( int num )
{
	UpdateHelperWidget();
}

Core::BaseProcessor::Pointer SandboxPlugin::ShapeScalePanelWidget::GetProcessor()
{
	return m_Processor.GetPointer( );
}

