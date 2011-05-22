/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreDynProcessingWidget.h"
#include "coreProcessorInputWidget.h"
#include "coreSelectionToolWidget.h"

#include "dynWxAGUIBuilder.h"
#include "dynModuleHelper.h"

#include <wx/tglbtn.h>
#include "itkImageRegion.h"
#include "coreVTKPolyDataHolder.h"

#define wxID_BTN_REGION wxID("wxID_BTN_REGION")
#define wxID_BTN_POINT wxID("wxID_BTN_POINT")

BEGIN_EVENT_TABLE(Core::Widgets::DynProcessingWidget, wxPanel)
    EVT_BUTTON(wxID_APPLY, Core::Widgets::DynProcessingWidget::OnApply)
	EVT_BUTTON(wxID_SAVE_AS_SCRIPT, Core::Widgets::DynProcessingWidget::OnSaveScript)
	EVT_CHECKBOX(wxID_RUN_AS_COMMAND_LINE, DynProcessingWidget::OnRunAsCommandLine)
	EVT_TOGGLEBUTTON(wxID_BTN_REGION, Core::Widgets::DynProcessingWidget::OnBtnRegion)
	EVT_TOGGLEBUTTON(wxID_BTN_POINT, Core::Widgets::DynProcessingWidget::OnBtnPoint)
END_EVENT_TABLE();

Core::Widgets::DynProcessingWidget::DynProcessingWidget( 
	wxWindow* parent, int id, const wxPoint& pos/*=wxDefaultPosition*/, 
	const wxSize& size/*=wxDefaultSize*/, long style/*=0*/ )
	: wxPanel(parent, id, pos, size, style)
{
	m_Processor = DynProcessor::New();

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(sizer);
	GetSizer( )->Fit(this);
}

void Core::Widgets::DynProcessingWidget::UpdateData()
{
	m_DynPanel->GetUpdater()->UpdateData();
}

void Core::Widgets::DynProcessingWidget::UpdateWidget()
{
	m_DynPanel->GetUpdater()->UpdateWidget();
}

Core::Widgets::DynProcessingWidget::~DynProcessingWidget()
{

}

Core::BaseProcessor::Pointer Core::Widgets::DynProcessingWidget::GetProcessor()
{
	return m_Processor.GetPointer();
}

void Core::Widgets::DynProcessingWidget::SetModule( ModuleDescription* module )
{
	BaseWindow::SetModule( module );

	Core::Runtime::Settings::Pointer settings = Core::Runtime::Kernel::GetApplicationSettings();

	m_Processor = DynProcessor::New();
	m_Processor->SetTemporaryDirectory( settings->GetApplicationPath() );
	m_Processor->SetWorkingDirectory( settings->GetApplicationPath() );
	m_Processor->SetModule( module );
}

void Core::Widgets::DynProcessingWidget::OnApply( wxCommandEvent &event )
{
	try
	{
		UpdateProcessor();
	}
	coreCatchExceptionsReportAndNoThrowMacro( "ProcessingWidget::UpdateProcessor" );
}

void Core::Widgets::DynProcessingWidget::OnInit()
{
	dynWxAGUIBuilder builder;
	builder.SetModule( m_Processor->GetModule() );
	builder.SetParentWindow( this );
	builder.Update();
	m_DynPanel = builder.GetPanel();

	GetSizer( )->Add( m_DynPanel, 0, wxEXPAND );

	AddInteractionButtons( );

	UpdateWidget( );

	// Don't select ouptut by default
	for ( int i = 0 ; i < m_Processor->GetNumberOfOutputs() ; i++ )
	{
		GetProcessorOutputObserver( i )->SelectDataEntity( false );
	}
}

void Core::Widgets::DynProcessingWidget::OnSaveScript( wxCommandEvent &event )
{
	if ( m_Processor->GetModuleExecution( ).IsNull() )
	{
		return;
	}

	try
	{
		m_DynPanel->GetUpdater( )->UpdateData( );

		m_Processor->GetModuleExecution( )->SetSaveScript( true );
		m_Processor->GetModuleExecution( )->SetForceExecutionMode( "CommandLineModule" );
		UpdateProcessor();
	}
	coreCatchExceptionsReportAndNoThrowMacro( "ProcessingWidget::UpdateProcessor" );

	m_Processor->GetModuleExecution( )->SetForceExecutionMode( "UnknownModule" );
	m_Processor->GetModuleExecution( )->SetSaveScript( false );
}

void Core::Widgets::DynProcessingWidget::OnRunAsCommandLine( wxCommandEvent& event )
{
	if ( event.GetInt( ) )
	{
		m_Processor->GetModuleExecution( )->SetForceExecutionMode( "CommandLineModule" );
	}
	else
	{
		m_Processor->GetModuleExecution( )->SetForceExecutionMode( "UnknownModule" );
	}
}

void Core::Widgets::DynProcessingWidget::AddInteractionButtons()
{
	dynWxGUIUpdater::ControlUpdaterMapType updaters;
	updaters = m_DynPanel->GetUpdater()->GetControlUpdaters( );

	dynWxGUIUpdater::ControlUpdaterMapType::iterator it;
	for ( it = updaters.begin() ; it != updaters.end() ; it++ )
	{
		ModuleParameter* param;
		param = dynModuleHelper::FindParameter( m_Processor->GetModule( ), "", 0, it->first );
		if ( !param )
		{
			continue;
		}
		
		if ( param->GetTag() == "region" || param->GetTag() == "point" )
		{
			wxWindow* win = m_DynPanel->FindWindowByName( it->first );
			if ( !win )
			{
				return;
			}

			wxWindowID id;
			if ( param->GetTag() == "region" )
			{
				id = wxID_BTN_REGION;
			}
			else if ( param->GetTag() == "point" )
			{
				id = wxID_BTN_POINT;
			}

			wxSizer* sizer = win->GetContainingSizer( );
			wxToggleButton* button = new wxToggleButton( win->GetParent( ), id, "S", wxDefaultPosition, wxSize( 21, 21 ) );
			sizer->Add( button, wxSizerFlags( ).Proportion( 0 ) );
		}
	}
}

void Core::Widgets::DynProcessingWidget::OnBtnRegion( wxCommandEvent &event )
{
	if ( GetPluginTab() == NULL )
	{
		return;
	}

	if ( event.GetInt( ) )
	{
		StartInteraction( event.GetEventObject(), "Bounding Box" );
	}
	else
	{
		wxWindow* applyWin = m_DynPanel->FindWindowById( wxID_APPLY );
		applyWin->Enable( true );

		SelectionToolWidget* bboxWidget = GetSelectionToolWidget<SelectionToolWidget>( "Bounding Box" );
		itk::ImageRegion<3> region;
		if ( Core::CastAnyProcessingData( bboxWidget->GetData( ), region ) )
		{
			std::ostringstream strRegion;
			strRegion << region.GetIndex( 0 ) << ",";
			strRegion << region.GetIndex( 1 ) << ",";
			strRegion << region.GetIndex( 2 ) << ",";
			strRegion << region.GetSize( 0 ) << ",";
			strRegion << region.GetSize( 1 ) << ",";
			strRegion << region.GetSize( 2 ) << std::ends;
			m_CurrentInteractionControl->SetValue( strRegion.str() );
		}

		bboxWidget->StopInteraction();
	}

}

void Core::Widgets::DynProcessingWidget::OnBtnPoint( wxCommandEvent &event )
{
	if ( GetPluginTab() == NULL )
	{
		return;
	}

	if ( event.GetInt( ) )
	{
		StartInteraction( event.GetEventObject(), "Landmark selector" );
	}
	else
	{
		wxWindow* applyWin = m_DynPanel->FindWindowById( wxID_APPLY );
		applyWin->Enable( true );

		SelectionToolWidget* landmarkWidget = GetSelectionToolWidget<SelectionToolWidget>( "Landmark selector" );
		if ( !landmarkWidget->IsSelectionEnabled() )
		{
			return;
		}

		Core::vtkPolyDataPtr landmarks;
		if ( Core::CastAnyProcessingData( landmarkWidget->GetData( ), landmarks ) )
		{
			std::ostringstream strPoint;
			if ( landmarks && landmarks->GetNumberOfPoints() != 0 )
			{

				ModuleParameter* param;
				param = dynModuleHelper::FindParameter( 
					m_Processor->GetModule( ), "", 0, m_CurrentInteractionControl->GetName().c_str() );

				int xSign = 1, ySign = 1, zSign = 1;
				// Convert from coordinateSystem
				if ( param )
				{
					if ( param->GetCoordinateSystem() == "lps" )
					{
						// nothing
					}else if ( param->GetCoordinateSystem() == "ras" )
					{
						xSign = -1;
						ySign = -1;
						zSign = 1;
					}
				}

				for ( int i = 0 ; i < landmarks->GetNumberOfPoints() ; i++ )
				{
					strPoint << xSign*landmarks->GetPoint( i )[ 0 ] << ",";
					strPoint << ySign*landmarks->GetPoint( i )[ 1 ] << ",";
					strPoint << zSign*landmarks->GetPoint( i )[ 2 ] << ";";
				}
				m_CurrentInteractionControl->SetValue( strPoint.str() );
			}

		}

		Core::DataEntity::Pointer ldDataEntity = landmarkWidget->GetDataEntity();
		landmarkWidget->StopInteraction();
		Core::Runtime::Kernel::GetDataContainer()->GetDataEntityList( )->Remove( ldDataEntity );
	}

}

void Core::Widgets::DynProcessingWidget::StartInteraction( wxObject* object, const std::string &name )
{
	// Find the text control
	wxWindow* win = wxDynamicCast( object, wxWindow);
	wxSizer* sizer = win->GetContainingSizer( );
	wxTextCtrl* textCtrl = NULL;
	wxSizerItemList list =sizer->GetChildren();
	wxSizerItemList::iterator it = list.begin( );
	while ( it != list.end( ) && textCtrl == NULL )
	{
		textCtrl = wxDynamicCast( (*it)->GetWindow( ), wxTextCtrl);
		it++;
	}

	wxWindow* applyWin = m_DynPanel->FindWindowById( wxID_APPLY );
	applyWin->Enable( false );

	// Start interaction
	if ( textCtrl )
	{
		m_CurrentInteractionControl = textCtrl;
		GetSelectionToolWidget<SelectionToolWidget>( name )->StartInteractor();
	}
}
