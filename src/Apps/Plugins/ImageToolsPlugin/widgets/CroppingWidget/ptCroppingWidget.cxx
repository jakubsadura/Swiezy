/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "ptCroppingWidget.h"

#include <wx/wupdlock.h>

#include "coreKernel.h"
#include "coreDataTreeHelper.h"
#include "coreDataEntityListBrowser.h"
#include "coreReportExceptionMacros.h"
#include "boost/filesystem.hpp"
#include <blMitkUnicode.h>
#include "coreDataTreeHelper.h"
#include "coreSelectionToolboxWidget.h"
#include "coreBBoxWidget.h"

using namespace mitk;
using namespace Core::Widgets;


BEGIN_EVENT_TABLE(ptCroppingWidget, wxPanel)
	EVT_BUTTON(wxID_btnCrop, ptCroppingWidget::OnButtonCrop)
	EVT_TOGGLEBUTTON(wxID_btnEnableInteraction, ptCroppingWidget::OnButtonShowBox)
END_EVENT_TABLE();


ptCroppingWidget::ptCroppingWidget(
	wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style):
		wxPanel(parent, id, pos, size, wxTAB_TRAVERSAL)
{
	m_ptCroppingWidgetStaticBox = new wxStaticBox(this, -1, wxT("Cropping Tool"));
	m_buttonCrop = new wxButton(this, wxID_btnCrop, wxT("Crop"));
	m_toggleBtnEnable = new wxToggleButton(this, wxID_btnEnableInteraction, wxT("Show Box"));

	m_Processor = Core::CroppingProcessor::New();
	do_layout();
}

void ptCroppingWidget::OnInit( )
{
}

void ptCroppingWidget::OnButtonCrop( wxCommandEvent& event )
{
	Core::Widgets::BBoxWidget* bboxWidget;
	bboxWidget = GetSelectionToolWidget<Core::Widgets::BBoxWidget>( "Bounding Box" );

	itk::ImageRegion<3> region = bboxWidget->GetBox( );

	m_Processor->GetProcessorDataHolder()->GetSubject( )->SetBox( region );
	m_Processor->Update();
}


void ptCroppingWidget::do_layout()
{
    wxBoxSizer* GlobalSizer = new wxBoxSizer(wxVERTICAL);
	wxStaticBoxSizer* ptCroppingWidget = new wxStaticBoxSizer(m_ptCroppingWidgetStaticBox, wxVERTICAL);
	wxBoxSizer* sizer_8 = new wxBoxSizer(wxHORIZONTAL);
	
	sizer_8->Add(m_toggleBtnEnable, 0, 0, 0);
	sizer_8->Add(m_buttonCrop, 1, 0, 0);
	ptCroppingWidget->Add(sizer_8, 0, wxALL|wxALIGN_RIGHT, 2);
	
	GlobalSizer->Add(ptCroppingWidget, 0, wxEXPAND, 0);
	SetSizer(GlobalSizer);
	GlobalSizer->Fit(this);

}

itk::ImageRegion<3> ptCroppingWidget::GetBox()
{
	return m_Processor->GetProcessorDataHolder()->GetSubject()->GetBox();
}

Core::BaseProcessor::Pointer ptCroppingWidget::GetProcessor()
{
	return m_Processor.GetPointer();
}

void ptCroppingWidget::OnButtonShowBox( wxCommandEvent& event )
{
	if ( event.GetInt() )
	{
		wxWindowUpdateLocker noUpdates( GetPluginTab() );

		SetSelectionTool( "Bounding Box" );
		Core::Widgets::BBoxWidget* bboxWidget;
		bboxWidget = GetSelectionToolWidget<Core::Widgets::BBoxWidget>( "Bounding Box" );
		GetPluginTab()->ShowWindow( wxID_SelectionToolboxWidget, true );
		bboxWidget->StartInteractor();
		m_toggleBtnEnable->SetLabel( wxT( "Hide box" ) );
	}
	else
	{
		// Is not possible to use wxID_ANY. It means any window
		SetSelectionTool( "None" );
		GetPluginTab()->ShowWindow( wxID_SelectionToolboxWidget, false );
		m_toggleBtnEnable->SetLabel( wxT( "Show box" ) );
	}
}
