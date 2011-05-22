/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _svSignalDataLayer_H
#define _svSignalDataLayer_H

// Core
#include "mathplot.h"
#include "blSignal.h"

/**
The rendering data of a blSignal

\ingroup SignalViewerPlugin
\author Xavi Planes
\date 21 July 2009 
*/
class svSignalDataLayer : public mpFXYVector
{

public:
	//!
	svSignalDataLayer() :  mpFXYVector( wxT("")) { m_drawOutsideMargins = false; }

	blSignal::Pointer GetSignal() const;
	void SetSignal(blSignal::Pointer val);

private:
	//!
	blSignal::Pointer m_Signal;
};



/**
The rendering data of a blSignal

\ingroup SignalViewerPlugin
\author Xavi Planes
\date 21 July 2009 
*/
class svSignalFunctionLayer : public mpFXY
{

public:
	//!
	svSignalFunctionLayer();

	blSignal::Pointer GetSignal() const;
	void SetSignal(blSignal::Pointer val);

	/** Get inclusive left border of bounding box.
	@return Value
	*/
	double GetMinX();

	/** Get inclusive right border of bounding box.
	@return Value
	*/
	double GetMaxX();

	/** Get inclusive bottom border of bounding box.
	@return Value
	*/
	double GetMinY();

	/** Get inclusive top border of bounding box.
	@return Value
	*/
	double GetMaxY();

	//!
	size_t GetNumberOfValues( );

	/**
	*/
	void SetStepFactor( size_t stepFactor );
	size_t GetStepFactor( );

	//!
	void Plot(wxDC & dc, mpWindow & w);

protected:
	/** Rewind value enumeration with mpFXY::GetNextXY.
	Overrided in this implementation.
	*/
	void Rewind();

	/** Get locus value for next N.
	Overrided in this implementation.
	@param x Returns X value
	@param y Returns Y value
	*/
	bool GetNextXY(double & x, double & y);

private:
	//!
	blSignal::Pointer m_Signal;

	double m_minX;      //!< Global layer bounding box, left border incl.
	double m_maxX;      //!< Global layer bounding box, right border incl.
	double m_minY;      //!< Global layer bounding box, bottom border incl.
	double m_maxY;      //!< Global layer bounding box, top border incl.

	//! The internal counter for the "GetNextXY" interface
	size_t              m_index;

	//! For sub-sampling
	size_t m_StepFactor;

	//! Start position in the vector when calling Plot
	unsigned m_StartPos;

	//! End position in the vector when calling Plot
	unsigned m_EndPos;
};

/**
Expanded with automatic visible/invisible depending on the size of the window

\ingroup SignalViewerPlugin
\author Xavi Planes
\date 21 July 2009 
*/
class svScaleXLayer : public mpScaleX
{
public:
	//!
	svScaleXLayer(wxString name = wxT("X"), int flags = mpALIGN_CENTER, bool ticks = true, unsigned int type = mpX_NORMAL);

	//!
	void Plot(wxDC & dc, mpWindow & w);

	//!
	bool GetAutoHide() const;
	void SetAutoHide(bool val);

private:
	//! Auto hide the layer when the height of the window < 40 pixels
	bool m_AutoHide;
};

/**
Expanded with automatic visible/invisible depending on the size of the window

\ingroup SignalViewerPlugin
\author Xavi Planes
\date 21 July 2009 
*/
class svScaleYLayer : public mpScaleY
{
public:
	//!
	svScaleYLayer(wxString name = wxT("X"), int flags = mpALIGN_CENTER, bool ticks = true);

	//!
	void Plot(wxDC & dc, mpWindow & w);

	//!
	bool GetAutoHide() const;
	void SetAutoHide(bool val);

private:
	//! Auto hide the layer when the height of the window < 40 pixels
	bool m_AutoHide;
};

#endif //_svSignalDataLayer_H
