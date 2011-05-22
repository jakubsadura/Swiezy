/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _svSignalPlotWindow_H
#define _svSignalPlotWindow_H

// Core
#include "mathplot.h"
#include "blSignal.h"
#include "blSignalEvent.h"
#include "coreDataEntityHolder.h"
#include "coreCommonDataTypes.h"
#include "svSignalLayer.h"

class svSignalAnnotationsLayer;
class svSignalAnnotationDialog;

/**
Vertical line layer for time position

\ingroup SignalViewerPlugin
\author Martin Bianculli
\date 21 July 2009 
*/
class svVerticalLineLayer : public mpFY
{

public:
	svVerticalLineLayer(double x) : mpFY(wxT("Time")) {this->m_X = x;}
	virtual double GetX( double y ) { return this->m_X; }
	//void SetX(double x){this->m_X = x;}
	virtual double GetMinX() { return this->m_X; }
	virtual double GetMaxX() { return this->m_X; }
	virtual double GetMinY() { return 0; }
	virtual double GetMaxY() { return 0; }

private:
	double m_X;
};

/**
Plot window to manage blSignal objects

\ingroup SignalViewerPlugin
\author Xavi Planes
\date 21 July 2009 
*/
class svSignalPlotWindow : public mpWindow
{
public:
	typedef std::vector<svSignalLayer> svSignalLayersVectorType;

public:
	//! Default constructor
	/*!
	 \param signalDataHolder The holder of the signal. 
	*/
	svSignalPlotWindow(
		Core::DataEntityHolder::Pointer inSignalDataHolder, 
		Core::DataEntityHolder::Pointer outSignalDataHolder, 
		wxWindow *parent, 
		wxWindowID id);

	void SetTimeStepHolder(Core::IntHolderType::Pointer timeStepHolder);
	
	//!
	~svSignalPlotWindow();

	//!
	void AddSignal( int pos, blSignal::Pointer signal );

	//! Update data of a already used signal
	void UpdateSignal( blSignal::Pointer signal );

	//!
	void ResetSignals( );

	//!
	void OnMouseLeftRelease(wxMouseEvent &event);
	
	//!
	void OnMouseMoved(wxMouseEvent &event);

	//!
	void OnSyncPlotWithParent(wxCommandEvent& event);

	void VerticalLineTracker();

	//!
	bool GetIsSyncEnable() const;
	void Synchronize(bool val);

	//!
	void ZoomInY();

	//!
	void ZoomOutY();


	//!
	void SetWindowOffset( double offset );

	//!
	void SetWindowSize( double size );

private:
	
	DECLARE_EVENT_TABLE()

	void OnAddPlotAnnotation(wxCommandEvent& event );

	void OnAddFreeAnnotation(wxCommandEvent& event );
	
	void OnZoomInY( wxCommandEvent& event );

	void OnZoomOutY( wxCommandEvent& event );

	// Synchronize current time step with plot
	void Sync();

	//!
	void StartDrawDuration( );

	//!
	void EndDrawDuration( int releaseX, int releaseY );

	//!
	bool ShowAnnotationList( blSignalEvent &selectedEvent );

	//!
	std::string AskFreeAnnotationName( );

	//!
	void UpdateView( );

private:

	//! Curves
	svSignalLayersVectorType m_svSignalLayers;

	//! 
	Core::DataEntityHolder::Pointer m_InputSignalDataHolder;

	//! To notify observers of Data entity list that the signal has changed
	Core::DataEntityHolder::Pointer m_OutputSignalDataHolder;

	//! Holds the current time step, used to synchronize with data when needed
	Core::IntHolderType::Pointer m_timeStepHolder;

	//!
	boost::signals::connection m_timeStepHolderConnection;

	//! If true, synchronize the plot window time step with the data
	bool m_IsSyncEnable;

	//!
	svVerticalLineLayer* m_VerticalLine;

	//! An input for storing the annotation text inserted by the user
	svSignalAnnotationDialog* m_AnnotationDialog; 

	//! In sec.
	double m_WindowOffset;

	//! In sec.
	double m_WindowSize;
};

#endif //_svSignalPlotWindow_H
