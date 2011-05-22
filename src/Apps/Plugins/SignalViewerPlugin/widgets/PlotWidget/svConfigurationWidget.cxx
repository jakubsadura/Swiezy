/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "svConfigurationWidget.h"
#include "svPlotWidget.h"

svConfigurationWidget::svConfigurationWidget(
	wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style):
svConfigurationWidgetUI(parent, id, pos, size, style)
{
	SetName( "Signal Viewer Configuration" );

}


/**
*/
svConfigurationWidget::~svConfigurationWidget()
{
}

void svConfigurationWidget::OnOffset( wxCommandEvent &event )
{
	UpdateData();
}

void svConfigurationWidget::OnSize( wxCommandEvent &event )
{
	UpdateData( );
}

void svConfigurationWidget::SetPlotWidget( svPlotWidget* val )
{
	m_svPlotWidget = val;
	m_svPlotWidget->GetParamHolder( )->AddObserver(
		this,
		&svConfigurationWidget::UpdateWidget );
}

void svConfigurationWidget::UpdateWidget()
{
	svPlotParameters::Pointer plotParameters;
	plotParameters = m_svPlotWidget->GetParamHolder( )->GetSubject( );
	if ( plotParameters.IsNull( ) )
	{
		return;
	}

	m_txtOffset->ChangeValue( wxString::Format("%.2f", plotParameters->GetWindowOffset( ) ) );
	m_txtSize->ChangeValue( wxString::Format("%.2f", plotParameters->GetWindowSize( ) ) );

}

void svConfigurationWidget::UpdateData()
{
	svPlotParameters::Pointer plotParameters;
	plotParameters = m_svPlotWidget->GetParamHolder( )->GetSubject( );
	if ( plotParameters.IsNull( ) )
	{
		return;
	}

	double size;
	m_txtSize->GetValue( ).ToDouble( &size );
	plotParameters->SetWindowSize( size );

	double offset;
	m_txtOffset->GetValue( ).ToDouble( &offset );
	plotParameters->SetWindowOffset( offset );

	m_svPlotWidget->GetParamHolder( )->NotifyObservers( );
}
