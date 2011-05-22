// Copyright 2006 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "svSignalLayer.h"

#include "svSignalDataLayer.h"
#include "svSignalAnnotationsLayer.h"


blSignal::Pointer svSignalLayer::GetSignal() const
{
	svSignalDataLayer* svSignalLayer = dynamic_cast<svSignalDataLayer*> (m_svSignalLayer);
	svSignalFunctionLayer* signalFunctionLayer = dynamic_cast<svSignalFunctionLayer*> (m_svSignalLayer);

	if ( svSignalLayer != NULL )
	{
		return svSignalLayer->GetSignal();
	}
	else if ( signalFunctionLayer != NULL )
	{
		return signalFunctionLayer->GetSignal();
	}

	return NULL;
}

void svSignalLayer::SetSignal( blSignal::Pointer signal )
{
	// If the signal has X values -> Create a XYVector layer
	// else, create a single X->Y layer
	if ( !signal->IsEquidistant() )
	{
		svSignalDataLayer* svSignalLayer = new svSignalDataLayer();
		svSignalLayer->SetSignal( signal );
		m_svSignalLayer = svSignalLayer;
	}
	else
	{
		svSignalFunctionLayer* svSignalLayer = new svSignalFunctionLayer();
		svSignalLayer->SetSignal( signal );
		m_svSignalLayer = svSignalLayer;
	}
	m_svSignalLayer->SetName( wxString::FromAscii(signal->GetName().c_str()));
	// Name is in the Legend
	m_svSignalLayer->ShowName( false );

	m_svSignalAnnotationsLayer = new svSignalAnnotationsLayer();
	m_svSignalAnnotationsLayer->SetSignal( signal );
	m_svSignalAnnotationsLayer->SetVisible(true);

	m_LegendLayer = new mpInfoLegend( wxRect(0,00,20,10), wxWHITE_BRUSH);
	m_LegendLayer->SetVisible(true);

	m_ls_x_axis = new svScaleXLayer(wxString::FromAscii(signal->GetXUnit().c_str()), mpALIGN_BORDER_BOTTOM, false);
	wxFont myFont = wxFont(8, wxDEFAULT, wxNORMAL, wxNORMAL, 0);
	m_ls_x_axis->SetFont(myFont);

	m_ls_y_axis = new svScaleYLayer(wxString::FromAscii(signal->GetYUnit().c_str()), mpALIGN_LEFT, false);
	m_ls_y_axis->SetFont(myFont);

}

void svSignalLayer::AddLayers( mpWindow *window )
{
	window->AddLayer( m_svSignalLayer );
	window->AddLayer( m_svSignalAnnotationsLayer );
	window->AddLayer( m_LegendLayer);
	window->AddLayer( m_ls_x_axis );
	window->AddLayer( m_ls_y_axis );
}

void svSignalLayer::DelLayers( mpWindow *window )
{
	window->DelLayer( m_svSignalLayer );
	window->DelLayer( m_svSignalAnnotationsLayer );
	window->DelLayer( m_LegendLayer );
	window->DelLayer( m_ls_x_axis );
	window->DelLayer( m_ls_y_axis );
}

svSignalAnnotationsLayer* svSignalLayer::GetsvSignalAnnotationsLayer() const
{
	return m_svSignalAnnotationsLayer;
}

void svSignalLayer::UpdateSignal( blSignal::Pointer signal )
{
	svSignalDataLayer* svSignalLayer = dynamic_cast<svSignalDataLayer*> (m_svSignalLayer);
	svSignalFunctionLayer* signalFunctionLayer = dynamic_cast<svSignalFunctionLayer*> (m_svSignalLayer);

	if ( svSignalLayer != NULL )
	{
		svSignalLayer->SetSignal( signal );
	}
	else if ( signalFunctionLayer != NULL )
	{
		signalFunctionLayer->SetSignal( signal );
	}
}

svScaleXLayer* svSignalLayer::GetsvScaleXLayer() const
{
	return m_ls_x_axis;
}

svScaleYLayer* svSignalLayer::GetsvScaleYLayer() const
{
	return m_ls_y_axis;
}
