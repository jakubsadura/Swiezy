/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _svSignalPlotAnnotations_H
#define _svSignalPlotAnnotations_H

// Core
#include "mathplot.h"
#include "blSignal.h"
#include "svSignalPlotWindow.h"
#include "blSignalEvent.h"

/**
Layer to draw annotations of signals

\ingroup SignalViewerPlugin
\author Xavi Planes
\date 21 July 2009 
*/

class svSignalAnnotationsLayer : public mpInfoLayer
{
public:
	//! Default constructor
	svSignalAnnotationsLayer( );

	//!
	svSignalAnnotationsLayer(wxRect rect, const wxBrush* brush = wxTRANSPARENT_BRUSH);
	
	//!
	~svSignalAnnotationsLayer();

	/** Plot method.
	@param dc the device content where to plot
	@param w the window to plot
	@sa mpLayer::Plot */
	virtual void   Plot(wxDC & dc, mpWindow & w);

	//!
	blSignal::Pointer GetSignal() const;

	//!
	void SetSignal(blSignal::Pointer val);

	//!
	void ShowAnnotationList( wxWindow parent );

	//!
	void StartAddAnnotation( 
		blSignalEvent selectedEvent, 
		int clickedX,
		int clickedY,
		double posX );

	//!
	void EndAddAnnotation( double posX );

	//!
	void OnMouseMove( double posX );

	//!
	bool GetDrawingInterval() const;

	//!
	blSignalAnnotation::Pointer GetCurrAnnotation() const;

private:
	//!
	void CreateIntervalBmp( wxDC & dc );

	//!
	void PlotAnnotation( 
		wxDC & dc, 
		mpWindow & w,
		blSignalAnnotation::Pointer annotation );

private:
	
	blSignal::Pointer m_Signal;

	//bmp containing the rectangle for the interval selection
	wxBitmap *m_IntervalBmp;

	//! current selected annotation...must be kept in memory before deciding the duration
	blSignalAnnotation::Pointer m_currAnnotation;

	//! If the draw rectangle button was pressed, the variable goes true
	bool m_DrawingInterval;

};

#endif //_svSignalPlotAnnotations_H
