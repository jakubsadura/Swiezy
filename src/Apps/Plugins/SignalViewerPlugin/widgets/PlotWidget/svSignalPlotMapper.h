/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _svSignalPlotMapper_H
#define _svSignalPlotMapper_H

// Core
#include "coreSmartPointerMacros.h"
#include "coreObject.h"
#include "coreDataEntity.h"
#include "coreDataEntityHolder.h"
#include "coreCommonDataTypes.h"
#include "svSignalPlotWindow.h"

#include "mathplot.h"



/**
\brief Parameters to configure plot widget
\ingroup SignalViewerPlugin
\author Xavi Planes
\date 27 April 2010
*/
class svPlotParameters : public Core::SmartPointerObject
{
public:
	coreDeclareSmartPointerClassMacro( svPlotParameters, Core::SmartPointerObject );
	double GetWindowSize() const { return m_WindowSize; }
	void SetWindowSize(double val) { m_WindowSize = val; }
	double GetWindowOffset() const { return m_WindowOffset; }
	void SetWindowOffset(double val) { m_WindowOffset = val; }
private:
	svPlotParameters( ){
		m_WindowSize = 10;
		m_WindowOffset = 0;
	};
	double m_WindowSize;
	double m_WindowOffset;
};
typedef Core::DataHolder<svPlotParameters::Pointer> svPlotParametersHolderType;

/**
Mediator for the plot widget. 
Holds a list of svSignalPlotWindow and a list of svSignalLayer and decides
on what window will be each curve plotted

Create a new plot window
\param parent The parent ID: should be the plot widget
\param dataEntity used to get the number of leads in the dataEntity in 
order to resize the number of windows
\return The list of windows that will be set in the plot widget

\ingroup SignalViewerPlugin
\author Chiara Riccobene
\date may  2009 
*/

class svSignalPlotMapper : public Core::BaseFilter
{
public:
	//!
	coreDeclareSmartPointerClassMacro(svSignalPlotMapper, Core::SmartPointerObject);
	
	typedef svSignalPlotWindow SignalWindowType;
	typedef std::vector<SignalWindowType*> SignalWindowVectorType;

	//!
	void SetParentWindow(wxWindow* val);

	//!
	void Update( );

	//!
	void Clear();

	//!
	void SynchronizeAll( bool val );

	//!
	void ZoomInYAll( );

	//!
	void ZoomOutYAll( );

	//!
	void FitAll( );

	//!
	std::vector<mpWindow*> GetsvSignalPlotWindowList() const;

	//!
	void UpdateSignals( );

	//!
	void SetTimeStepHolder(Core::IntHolderType::Pointer val);

	//!
	svPlotParametersHolderType::Pointer GetParamHolder() const;

private:
	//! Default constructor
	svSignalPlotMapper();
	
	//!
	~svSignalPlotMapper();

	//! Create the windows
	void CreateNewPlotWindowVector( );

	//! Plot the signals in the previously created window
	void PlotSignalValues( );

	//!
	void OnModifiedParams( );
private:
	
	//! The Strain Plot windows
	std::vector<mpWindow*> m_svSignalPlotWindowList;

	//! Parent window of all windows
	wxWindow* m_ParentWindow;

	//!
	Core::IntHolderType::Pointer m_timeStepHolder;

	//!
	svPlotParametersHolderType::Pointer m_ParamHolder;
};

#endif //_svSignalPlotMapper_H
