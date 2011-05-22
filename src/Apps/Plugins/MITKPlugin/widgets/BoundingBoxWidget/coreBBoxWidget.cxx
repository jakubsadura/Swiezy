/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreBBoxWidget.h"
#include "coreKernel.h"
#include "coreDataTreeHelper.h"
#include "coreDataEntityListBrowser.h"
#include "coreReportExceptionMacros.h"
#include "wx/xrc/xmlres.h"
#include "boost/filesystem.hpp"
#include <wxUnicode.h>
#include "coreDataTreeHelper.h"

using namespace mitk;
using namespace Core::Widgets;


BEGIN_EVENT_TABLE(BBoxWidget, wxPanel)
	EVT_BUTTON(wxID_btnResetBox, BBoxWidget::OnButtonResetBox)
	EVT_BUTTON(wxID_btnExportBox, BBoxWidget::OnButtonExportBox)
END_EVENT_TABLE();


BBoxWidget::BBoxWidget(
	wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style):
		wxPanel(parent, id, pos, size, wxTAB_TRAVERSAL)
{
	//SetLabel( "BBoxWidget" );

	m_ptCroppingWidgetStaticBox = new wxStaticBox(this, -1, wxT("Cropping Tool"));
	m_staticTextX = new wxStaticText(this, wxID_ANY, wxT("X:"));
	m_textCtrlXMin = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
	m_staticTextTo1 = new wxStaticText(this, wxID_ANY, wxT("to"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
	m_textCtrlXMax = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
	m_staticTextX2 = new wxStaticText(this, wxID_ANY, wxT("Y:"));
	m_textCtrlYMin = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
	m_staticTextTo2 = new wxStaticText(this, wxID_ANY, wxT("to"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
	m_textCtrlYMax = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
	m_staticTextX3 = new wxStaticText(this, wxID_ANY, wxT("Z:"));
	m_textCtrlZMin = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
	m_staticTextTo3 = new wxStaticText(this, wxID_ANY, wxT("to"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
	m_textCtrlZMax = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
	m_buttonResetBox = new wxButton(this, wxID_btnResetBox, wxT("Reset"));
	m_buttonExportBox = new wxButton(this, wxID_btnExportBox, wxT("Export"));


	m_Processor = Core::BBoxProcessor::New();
	do_layout();

	// Each time a text control is modified, this function will be called
	changeInWidgetObserver.SetSlotFunction(this, &BBoxWidget::UpdateDataHolders);

	//Compute Centerline
	changeInWidgetObserver.Observe(m_textCtrlXMin);
	changeInWidgetObserver.Observe(m_textCtrlXMax);
	changeInWidgetObserver.Observe(m_textCtrlYMin);
	changeInWidgetObserver.Observe(m_textCtrlYMax);
	changeInWidgetObserver.Observe(m_textCtrlZMin);
	changeInWidgetObserver.Observe(m_textCtrlZMax);

	m_Processor->GetProcessorDataHolder()->AddObserver( 
		this, 
		&BBoxWidget::UpdateWidget );

	m_Processor->GetInputDataEntityHolder(0 )->AddObserver( 
		this, 
		&BBoxWidget::OnModifiedInput );


}

void BBoxWidget::OnInit( )
{
	m_SceneManager = Core::BBoxSceneManager::New(
		m_Processor->GetProcessorDataHolder(),
		GetRenderingTree());
	
	StartInteractor();

	UpdateWidget();

	InitProcessorObservers( true );
	GetProcessorOutputObserver( 1 )->SetHideInput( false );
	GetProcessorOutputObserver( 1 )->PushProperty( blTag::New( "opacity", 0.3f ) );
}

void BBoxWidget::OnButtonResetBox( wxCommandEvent& event )
{
	m_buttonResetBox->Enable(false);
	StopInteraction();
	StartInteractor( );
	m_buttonResetBox->Enable(true);
}

void BBoxWidget::UpdateWidget()
{
	Core::BoundingBox::Pointer parameters;
	parameters = m_Processor->GetProcessorDataHolder()->GetSubject();
	if( parameters.IsNull() || m_SceneManager.IsNull())
		return;

	std::string label = m_SceneManager->IsEnabled() ? "Hide " : "Show ";

	float XMin, XMax, YMin, YMax, ZMin, ZMax;
	changeInWidgetObserver.SetEnabled(false);
	XMin = parameters->GetBox( ).GetIndex(0);
	XMax = parameters->GetBox( ).GetIndex(0) + parameters->GetBox( ).GetSize(0);
	YMin = parameters->GetBox( ).GetIndex(1);
	YMax = parameters->GetBox( ).GetIndex(1) + parameters->GetBox( ).GetSize(1);
	ZMin = parameters->GetBox( ).GetIndex(2);
	ZMax = parameters->GetBox( ).GetIndex(2) + parameters->GetBox( ).GetSize(2);
	m_textCtrlXMin->SetValue( wxString::Format(wxT("%.2f"), XMin ) );
	m_textCtrlXMax->SetValue( wxString::Format(wxT("%.2f"), XMax ) );
	m_textCtrlYMin->SetValue( wxString::Format(wxT("%.2f"), YMin ) );
	m_textCtrlYMax->SetValue( wxString::Format(wxT("%.2f"), YMax ) );
	m_textCtrlZMin->SetValue( wxString::Format(wxT("%.2f"), ZMin ) );
	m_textCtrlZMax->SetValue( wxString::Format(wxT("%.2f"), ZMax ) );

	changeInWidgetObserver.SetEnabled(true);
	Validate();
}

void BBoxWidget::UpdateDataHolders()
{
	if( !Validate() )
		return;

	typedef itk::ImageRegion<3>::IndexValueType ScalarType;
	Core::BoundingBox::Pointer parameters = m_Processor->GetProcessorDataHolder()->GetSubject();

	double XMin, XMax, YMin, YMax, ZMin, ZMax;
	m_textCtrlXMin->GetValue( ).ToDouble( &XMin );
	m_textCtrlXMax->GetValue( ).ToDouble( &XMax );
	m_textCtrlYMin->GetValue( ).ToDouble( &YMin );
	m_textCtrlYMax->GetValue( ).ToDouble( &YMax );
	m_textCtrlZMin->GetValue( ).ToDouble( &ZMin );
	m_textCtrlZMax->GetValue( ).ToDouble( &ZMax );

	itk::ImageRegion<3> box;
	box.SetIndex(0, ScalarType( XMin ));
	box.SetSize( 0, ScalarType( XMax - XMin ) );
	box.SetIndex(1, ScalarType( YMin ));
	box.SetSize( 1, ScalarType( YMax - YMin ) );
	box.SetIndex(2, ScalarType( ZMin ) );
	box.SetSize( 2, ScalarType( ZMax - ZMin ));
	parameters->SetBox( box );

	m_Processor->GetProcessorDataHolder()->NotifyObservers();
}

bool BBoxWidget::Validate()
{
	using boost::get;
	bool okay = true;
	Core::BoundingBox::Pointer parameters;
	parameters = m_Processor->GetProcessorDataHolder()->GetSubject();
	if( parameters.IsNull() )
		return false;

	//okay = validator.IsNumber(GetTextCtrlXMin(), gbl::VC_REAL) && okay;
	//okay = validator.IsNumber(GetTextCtrlXMax(), gbl::VC_REAL) && okay;
	//okay = validator.IsNumber(GetTextCtrlYMin(), gbl::VC_REAL) && okay;
	//okay = validator.IsNumber(GetTextCtrlYMax(), gbl::VC_REAL) && okay;
	//okay = validator.IsNumber(GetTextCtrlZMin(), gbl::VC_REAL) && okay;
	//okay = validator.IsNumber(GetTextCtrlZMax(), gbl::VC_REAL) && okay;
	return okay;

}

bool BBoxWidget::Enable( bool enable /* = true */ )
{
	bool bReturn = wxPanel::Enable( enable );

	if ( !enable )
	{
		StopInteraction();
	}
	
	try
	{
		//const std::string helpStr = \
		//	"Cropping tool: remove of the outer parts of an image." \
		//	"\n\nUsage: CRTL + left click to move the yellow control points to" \
		//	"\nthe desired position, click 'Crop' to crop the data if needed.";
		//SetInfoUserHelperWidget( helpStr );
	}
	coreCatchExceptionsReportAndNoThrowMacro("BBoxWidget::Enable");


	return bReturn;
}


void BBoxWidget::OnModifiedInput()
{
	try{

		if ( !IsEnabled() ||
			m_Processor->GetInputDataEntity(0).IsNull( ) ||
			m_SceneManager.IsNull())
		{
			StopInteraction();
			return;
		}

		// Add the cropping box at the end of the rendering tree
		if ( m_SceneManager->IsEnabled() &&
			 m_Processor->GetInputDataEntity(0).GetPointer( ) != 
			 m_SceneManager->GetInputDataEntity().GetPointer( ) )
		{
			StopInteraction();
			StartInteractor();
		}

		UpdateWidget();
	}
	coreCatchExceptionsLogAndNoThrowMacro( BBoxWidget::OnModifiedInput );
}

void BBoxWidget::StartInteractor()
{
	if ( m_Processor->GetInputDataEntity(0).IsNull( ) )
	{
		return;
	}

	// Reset box before enabling the rendering
	m_Processor->ResetBox();
	m_SceneManager->SetInputDataEntity( m_Processor->GetInputDataEntity(0));
	m_SceneManager->Enable( true );
	UpdateWidget();
}

void BBoxWidget::StopInteraction()
{
	if ( m_Processor.IsNull() || m_SceneManager.IsNull() )
	{
		return;
	}

	m_SceneManager->SetInputDataEntity( NULL );
	m_SceneManager->Enable( false );
	UpdateWidget();
}

void BBoxWidget::do_layout()
{
	// begin wxGlade: TetraGeneratorPanelWidget::do_layout
    wxBoxSizer* GlobalSizer = new wxBoxSizer(wxVERTICAL);
	wxStaticBoxSizer* ptCroppingWidget = new wxStaticBoxSizer(m_ptCroppingWidgetStaticBox, wxVERTICAL);
	wxBoxSizer* sizer_6 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* sizer_5 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* sizer_7 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* sizer_8 = new wxBoxSizer(wxHORIZONTAL);
	
	sizer_5->Add(m_staticTextX, 1, wxALL|wxALIGN_CENTER_HORIZONTAL, 0);
	sizer_5->Add(m_textCtrlXMin, 0, 0, 0);
	sizer_5->Add(m_staticTextTo1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL, 0);
	sizer_5->Add(m_textCtrlXMax, 0, 0, 0);
	ptCroppingWidget->Add(sizer_5, 0, wxALL|wxEXPAND, 2);
	
	sizer_6->Add(m_staticTextX2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL, 0);
	sizer_6->Add(m_textCtrlYMin, 0, 0, 0);
	sizer_6->Add(m_staticTextTo2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL, 0);
	sizer_6->Add(m_textCtrlYMax, 0, 0, 0);
	ptCroppingWidget->Add(sizer_6, 0, wxALL|wxEXPAND, 2);
	
	sizer_7->Add(m_staticTextX3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL, 0);
	sizer_7->Add(m_textCtrlZMin, 0, 0, 0);
	sizer_7->Add(m_staticTextTo3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL, 0);
	sizer_7->Add(m_textCtrlZMax, 0, 0, 0);
	ptCroppingWidget->Add(sizer_7, 0, wxALL|wxEXPAND, 2);
	
	sizer_8->Add(m_buttonExportBox, 0, 0, 0);
	sizer_8->Add(m_buttonResetBox, 0, 0, 0);
	ptCroppingWidget->Add(sizer_8, 0, wxALL|wxALIGN_RIGHT, 2);
	
	GlobalSizer->Add(ptCroppingWidget, 0, wxEXPAND, 0);
	SetSizer(GlobalSizer);
	GlobalSizer->Fit(this);
	// end wxGlade
}

itk::ImageRegion<3> BBoxWidget::GetBox()
{
	return m_Processor->GetProcessorDataHolder()->GetSubject()->GetBox();
}

Core::BaseProcessor::Pointer BBoxWidget::GetProcessor()
{
	return m_Processor.GetPointer();
}

void BBoxWidget::UpdateData()
{
}

bool Core::Widgets::BBoxWidget::IsSelectionEnabled()
{
	return m_SceneManager->IsEnabled();
}

void Core::Widgets::BBoxWidget::OnButtonExportBox( wxCommandEvent& event )
{
	m_Processor->ExportBBox();

}

boost::any Core::Widgets::BBoxWidget::GetData()
{
	return GetBox( );
}

Core::DataEntity::Pointer Core::Widgets::BBoxWidget::GetDataEntity()
{
	return NULL;
}
