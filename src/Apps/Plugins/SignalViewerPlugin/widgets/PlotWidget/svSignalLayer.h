/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _svSignalLayer_H
#define _svSignalLayer_H

// Core
#include "mathplot.h"
#include "blSignal.h"

class svSignalAnnotationsLayer;
class svScaleXLayer;
class svScaleYLayer;

/**
Signal and annotations layer for each blSignal

\ingroup SignalViewerPlugin
\author Xavi Planes
\date 14 Jan 2010
*/
class svSignalLayer
{
public:
	//! Return the signal
	blSignal::Pointer GetSignal() const;

	//! Set a new signal and create the layers
	void SetSignal( blSignal::Pointer signal );

	//! Add layers to the window
	void AddLayers( mpWindow *window );

	//! Remove layers from the window
	void DelLayers( mpWindow *window );

	//!
	void UpdateSignal( blSignal::Pointer signal );

	//!
	svSignalAnnotationsLayer* GetsvSignalAnnotationsLayer() const;

	//!
	svScaleXLayer* GetsvScaleXLayer() const;

	//!
	svScaleYLayer* GetsvScaleYLayer() const;

protected:
	//!
	mpLayer* m_svSignalLayer;

	//!
	svSignalAnnotationsLayer* m_svSignalAnnotationsLayer;

	//!
	mpInfoLegend* m_LegendLayer;

	//!
	svScaleXLayer* m_ls_x_axis;

	//!
	svScaleYLayer* m_ls_y_axis;
};

#endif //_svSignalLayer_H
