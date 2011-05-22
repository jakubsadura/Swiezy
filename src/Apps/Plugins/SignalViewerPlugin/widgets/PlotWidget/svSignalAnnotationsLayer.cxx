// Copyright 2006 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "svSignalAnnotationsLayer.h"
#include "wxID.h"

svSignalAnnotationsLayer::svSignalAnnotationsLayer( ) 
{	
	//must be explicitly initialized to null..otherwise in debug mode it is initialized as 0xCDCDCDCD
	m_IntervalBmp=NULL; 
	m_DrawingInterval = false;
}

svSignalAnnotationsLayer::svSignalAnnotationsLayer(
	wxRect rect, 
	const wxBrush* brush ) : mpInfoLayer(rect, brush)
{
	//must be explicitly initialized to null..otherwise in debug mode it is initialized as 0xCDCDCDCD
	m_IntervalBmp=NULL; 
	m_DrawingInterval = false;
}

svSignalAnnotationsLayer::~svSignalAnnotationsLayer()
{
	delete m_IntervalBmp;
}

blSignal::Pointer svSignalAnnotationsLayer::GetSignal() const
{
	return m_Signal;
}

void svSignalAnnotationsLayer::SetSignal( blSignal::Pointer val )
{
	m_Signal = val;
}

void svSignalAnnotationsLayer::Plot( wxDC & dc, mpWindow & w )
{
	if ( visible )
	{
		dc.SetPen( m_pen);

		std::vector< blSignalAnnotation::Pointer > annotations;
		annotations = m_Signal->GetAnnotationsVector();
		for ( unsigned i = 0 ; i < annotations.size() ; i++ )
		{
			bool selected;
			selected = blSignalEventTable::IsEventSelected( 
				annotations[ i ]->GetType() );

			if ( selected )
			{
				PlotAnnotation( dc, w, annotations[ i ] );
			}

		}

		PlotAnnotation( dc, w, m_currAnnotation );
	}
}

void svSignalAnnotationsLayer::StartAddAnnotation( 
	blSignalEvent selectedEvent, 
	int clickedX,
	int clickedY,
	double posX )
{

	// Create the annotation
	m_currAnnotation = blSignalAnnotation::New( );
	m_currAnnotation->SetType( selectedEvent.m_Type );

	blSignal::SampleType yValue = 0;
	unsigned pos;
	bool proceed = m_Signal->FindXValuePosition( posX, pos);
	m_currAnnotation->SetXPos( pos );

	m_currAnnotation->SetValueY( m_Signal->GetValueY( pos ) );
	m_currAnnotation->SetDuration(0);

	m_DrawingInterval = true;

	// Start to draw duration
	if ( !blSignalEventTable::HasDuration( selectedEvent.m_Type ) )
	{	
		EndAddAnnotation( posX );
	}

}

void svSignalAnnotationsLayer::EndAddAnnotation( double posX )
{

	blSignal::SampleType initValueX;
	initValueX = m_Signal->GetValueX( m_currAnnotation->GetXPos() );

	unsigned endPosX;
	bool proceed = m_Signal->FindXValuePosition( posX, endPosX);

	if ( endPosX - m_currAnnotation->GetXPos() > 10  )
	{
		m_currAnnotation->SetDuration( endPosX - m_currAnnotation->GetXPos() );

		m_Signal->AddAnnotation( m_currAnnotation );
	}
	else
	{
		//No duration
		m_currAnnotation->SetDuration(0);

		m_Signal->AddAnnotation( m_currAnnotation );
	}

	m_currAnnotation=NULL;
	m_DrawingInterval=false;
}

void svSignalAnnotationsLayer::OnMouseMove( double posX )
{
	blSignal::SampleType initValueX;
	initValueX = m_Signal->GetValueX( m_currAnnotation->GetXPos() );

	unsigned pos;
	bool proceed = m_Signal->FindXValuePosition( posX, pos);
	m_currAnnotation->SetDuration( pos - m_currAnnotation->GetXPos() );
}

bool svSignalAnnotationsLayer::GetDrawingInterval() const
{
	return m_DrawingInterval;
}

void svSignalAnnotationsLayer::CreateIntervalBmp( wxDC & dc )
{
	wxSize sz=dc.GetSize();
	int wdt=sz.x;
	int hgt=sz.y;
	wdt = std::max( 1, wdt );
	hgt = std::max( 1, hgt );

	// If DC has changed size -> Delete m_IntervalBmp
	if ( m_IntervalBmp!= NULL )
	{
		if ( wdt != m_IntervalBmp->GetWidth( ) || hgt != m_IntervalBmp->GetHeight( ) )
		{
			delete m_IntervalBmp;
			m_IntervalBmp = NULL;
		}
	}

	if(m_IntervalBmp==NULL)
	{
		// Create RED image with alpha channel and DC Size
		wxImage* intervalImg = new wxImage(wdt,hgt,true);
		unsigned char *alphas = (unsigned char *)malloc(wdt*hgt);
		memset(alphas,128,wdt*hgt);
		intervalImg->SetAlpha(alphas);
		intervalImg->SetRGB(wxRect(0,0,wdt,hgt),178,34,34);

		// Create the bitmap
		m_IntervalBmp = new wxBitmap(*intervalImg);

		delete intervalImg;
	}
}

void svSignalAnnotationsLayer::PlotAnnotation( 
	wxDC & dc, 
	mpWindow & w,
	blSignalAnnotation::Pointer annotation )
{
	if ( annotation.IsNull() )
	{
		return;
	}

	wxCoord startPx = m_drawOutsideMargins ? 0 : w.GetMarginLeft();
	wxCoord endPx   = m_drawOutsideMargins ? w.GetScrX() : w.GetScrX() - w.GetMarginRight();
	wxCoord minYpx  = m_drawOutsideMargins ? 0 : w.GetMarginTop();
	wxCoord maxYpx  = m_drawOutsideMargins ? w.GetScrY() : w.GetScrY() - w.GetMarginBottom();

	blSignal::SampleType position = m_Signal->GetValueX( annotation->GetXPos() );
	blSignal::SampleType duration = m_Signal->GetValueX( 
		annotation->GetXPos() + annotation->GetDuration() );

	wxCoord cx = w.x2p( position ); 
	wxCoord cy = w.y2p( annotation->GetValueY() ); 

	if ( cx < startPx || cx > endPx || cy < minYpx || cy > maxYpx )
	{
		return;
	}

	dc.DrawText( wxString::FromAscii(annotation->GetName().c_str() ), cx, cy );


	wxCoord dx=w.x2p( duration )-cx;
	dx = std::max( dx, 1 );
	/*double minY =m_Signal->GetMinYValue(); 
	double maxY =m_Signal->GetMaxYValue(); */
	double minY = w.GetDesiredYmin(); 
	double maxY = w.GetDesiredYmax(); 

	wxCoord mxYc=w.y2p(maxY);
	wxCoord mnYc=w.y2p(minY); 
	int hgt=mxYc-mnYc;
	if (hgt<0) 
		hgt=-hgt;
	int wdt=dx;
	//hgt=100; wdt=100;


	dc.SetBrush(*wxTRANSPARENT_BRUSH);
	wxPen pen2(*wxRED); 
	dc.SetPen(pen2);
	dc.DrawRectangle(cx,mnYc,dx,mxYc-mnYc);

	CreateIntervalBmp( dc );

	// Draw interval
	int imgWdt=m_IntervalBmp->GetWidth();
	int imgHgt=m_IntervalBmp->GetHeight();

	//TODO: handle the case in which windows dimension have changed!
	if((imgWdt>=wdt) && (imgHgt>=hgt) && wdt > 0 && hgt > 0 )
	{
		wxBitmap subRect = m_IntervalBmp->GetSubBitmap(wxRect(0,0,wdt,hgt));
		dc.DrawBitmap(subRect,cx,mxYc,true);
	}
}

blSignalAnnotation::Pointer svSignalAnnotationsLayer::GetCurrAnnotation() const
{
	return m_currAnnotation;
}
