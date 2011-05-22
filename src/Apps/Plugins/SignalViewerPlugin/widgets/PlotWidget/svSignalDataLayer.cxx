// Copyright 2006 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "svSignalDataLayer.h"


blSignal::Pointer svSignalDataLayer::GetSignal() const
{
	return m_Signal;
}

void svSignalDataLayer::SetSignal( blSignal::Pointer val )
{
	m_Signal = val;
	SetData(val->GetXVector(), val->GetYVector());
	SetContinuity(true);
}

svSignalFunctionLayer::svSignalFunctionLayer() :  mpFXY( wxT(""))
{
	m_drawOutsideMargins = false;
	m_index = 0;
	m_minX  = -1;
	m_maxX  = 1;
	m_minY  = -1;
	m_maxY  = 1;
	m_StepFactor = 1;
}

blSignal::Pointer svSignalFunctionLayer::GetSignal() const
{
	return m_Signal;
}

void svSignalFunctionLayer::SetSignal( blSignal::Pointer val )
{
	m_Signal = val;
	SetContinuity(true);

	if (m_Signal->GetNumberOfRecords()>0)
	{
		m_minX  = m_Signal->GetStartTime();
		m_maxX  = m_Signal->GetStartTime() + m_Signal->GetNumberOfValues() / m_Signal->GetFrameRate();
		m_minY  = m_Signal->GetMinYValue();
		m_maxY  = m_Signal->GetMaxYValue();

		float deltaX = abs(m_maxX - m_minX);  
		float deltaY = abs(m_maxY - m_minY);  
		m_minX = m_minX - 0.1*deltaX;
		m_minY = m_minY - 0.1*deltaY;
		m_maxX = m_maxX + 0.1*deltaX;
		m_maxY = m_maxY + 0.1*deltaY;
	}
	else
	{
		m_minX  = -1;
		m_maxX  = 1;
		m_minY  = -1;
		m_maxY  = 1;
	}
}

double svSignalFunctionLayer::GetMinX()
{
	return m_minX;
}

double svSignalFunctionLayer::GetMaxX()
{
	return m_maxX;
}

double svSignalFunctionLayer::GetMinY()
{
	return m_minY;
}

double svSignalFunctionLayer::GetMaxY()
{
	return m_maxY;
}

void svSignalFunctionLayer::Rewind()
{
	m_index = 0;
}

bool svSignalFunctionLayer::GetNextXY(double & x, double & y)
{
	if ( m_index >= m_Signal->GetNumberOfValues() )
		return FALSE;
	else
	{
		y = m_Signal->GetValueY( m_index );
		x = m_Signal->GetStartTime() + m_index / m_Signal->GetFrameRate();
		m_index+=m_StepFactor;
		return m_index <= m_EndPos;
	}
}

size_t svSignalFunctionLayer::GetNumberOfValues()
{
	return m_Signal->GetNumberOfValues();
}


void svSignalFunctionLayer::SetStepFactor( size_t stepFactor )
{
	m_StepFactor = (stepFactor >= 1)? stepFactor : 1;
}

size_t svSignalFunctionLayer::GetStepFactor( )
{
	return m_StepFactor;
}

void svSignalFunctionLayer::Plot( wxDC & dc, mpWindow & w )
{
	wxCoord startPx = m_drawOutsideMargins ? 0 : w.GetMarginLeft();
	wxCoord endPx   = m_drawOutsideMargins ? w.GetScrX() : w.GetScrX() - w.GetMarginRight();

	double startTime = w.p2x( startPx );
	double endTime = w.p2x( endPx );
	if ( ( endPx - startPx) > 0 )
	{
		m_Signal->FindXValuePosition( startTime, m_StartPos );
		m_Signal->FindXValuePosition( endTime, m_EndPos );
		SetStepFactor( (m_EndPos - m_StartPos) / ( endPx - startPx) / 2 );
	}
	else
	{
		m_StartPos = 0;
		m_EndPos = 0;
	}

	mpFXY::Plot( dc, w );
}

svScaleXLayer::svScaleXLayer( 
	wxString name /*= wxT("X")*/, int flags /*= mpALIGN_CENTER*/, 
	bool ticks /*= true*/, unsigned int type /*= mpX_NORMAL*/ ) 
	: mpScaleX(name, flags, ticks, type)
{
	m_AutoHide = false;
}

void svScaleXLayer::Plot( wxDC & dc, mpWindow & w )
{
	if ( w.GetSize().GetHeight() > 60 || m_AutoHide == false )
	{
		mpScaleX::Plot( dc, w);
	}
}

bool svScaleXLayer::GetAutoHide() const
{
	return m_AutoHide;
}

void svScaleXLayer::SetAutoHide( bool val )
{
	m_AutoHide = val;
}

svScaleYLayer::svScaleYLayer( 
	wxString name /*= wxT("X")*/, int flags /*= mpALIGN_CENTER*/, 
	bool ticks /*= true*/ ) 
		: mpScaleY(name, flags, ticks)
{
	m_AutoHide = false;
}

void svScaleYLayer::Plot( wxDC & dc, mpWindow & w )
{
	if ( w.GetSize().GetHeight() > 60 || m_AutoHide == false )
	{
		mpScaleY::Plot( dc, w);
	}
}

bool svScaleYLayer::GetAutoHide() const
{
	return m_AutoHide;
}

void svScaleYLayer::SetAutoHide( bool val )
{
	m_AutoHide = val;
}
