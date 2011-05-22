/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

// For compilers that don't support precompilation, include "wx/wx.h"
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/dir.h"
#include "wxID.h"

#include <blMitkUnicode.h>

#include "ptRadiographVisualizationWidget.h"
#include "coreDataEntityListBrowser.h"
#include "coreDataTreeHelper.h"
#include "coreReportExceptionMacros.h"
#include "coreProcessorWidgetsBuilder.h"

const long wxID_ThumbnailCtrl = wxNewId( );


//!
ptRadiographVisualizationWidget::ptRadiographVisualizationWidget(
	wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style)
: ptRadiographVisualizationWidgetUI(parent, id, pos, size, style)
{
	// Init Processor
	m_DRRProcessor = Core::DRRVisualizationProcessor::New();

	SetName( wxString::FromAscii(m_DRRProcessor->GetName( ).c_str()) );

	m_DRRProcessor->GetOutputDataEntityHolder( 0 )->AddObserver( 
		this, 
		&ptRadiographVisualizationWidget::OnModifiedOutputDataEntity );
}

//!
ptRadiographVisualizationWidget::~ptRadiographVisualizationWidget(void)
{

}

void ptRadiographVisualizationWidget::Append( wxString file, wxString title )
{
	
}

void ptRadiographVisualizationWidget::OnBtnShowProjection(wxCommandEvent &event)
{
	m_DRRProcessor->Update();
}

void ptRadiographVisualizationWidget::OnModifiedOutputDataEntity()
{
	try{

		Core::DataEntity::Pointer inputDataEntity;
		inputDataEntity = m_DRRProcessor->GetInputDataEntity( 0 );

		// Hide input if is different from output and output is not empty
		if ( m_DRRProcessor->GetOutputDataEntity( 0 ).IsNotNull() && 
			m_DRRProcessor->GetOutputDataEntity( 0 ) != inputDataEntity )
		{
			GetRenderingTree()->Show( inputDataEntity, false );
		}

		// Add output to the data list and render it
		// After adding the output, the input will automatically be changed to
		// this one
		Core::DataTreeHelper::PublishOutput( 
			m_DRRProcessor->GetOutputDataEntityHolder( 0 ), 
			GetRenderingTree());
	}
	coreCatchExceptionsLogAndNoThrowMacro( 
		"ptRadiographVisualizationWidget::OnModifiedOutputDataEntity")

}

Core::BaseProcessor::Pointer ptRadiographVisualizationWidget::GetProcessor()
{
	return NULL;
}

