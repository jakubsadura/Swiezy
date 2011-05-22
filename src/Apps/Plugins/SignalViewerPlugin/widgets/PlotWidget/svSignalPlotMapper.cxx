// Copyright 2006 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "svSignalPlotMapper.h"
#include "coreKernel.h"
#include "coreDataEntityHelper.h"
#include "svSignalPlotWindow.h"
#include "svSignalNotebookPage.h"

#include "blSignalCollective.h"

#include "mathplot.h"


svSignalPlotMapper::svSignalPlotMapper()
{
	SetName( "SignalPlotMapper" );
	SetNumberOfInputs( 1 );
	GetInputPort( 0 )->SetName( "Input signal" );
	GetInputPort( 0 )->SetDataEntityType( Core::SignalTypeId );
	SetNumberOfOutputs( 1 );
	GetOutputPort( 0 )->SetName( "Output signal" );
	GetOutputPort( 0 )->SetDataEntityType( Core::SignalTypeId );

	m_ParentWindow = NULL;

	m_ParamHolder = svPlotParametersHolderType::New();
	m_ParamHolder->SetSubject( svPlotParameters::New() );
	m_ParamHolder->AddObserver(
		this,
		&svSignalPlotMapper::OnModifiedParams );
}

/**
*/
svSignalPlotMapper::~svSignalPlotMapper()
{
}


/**
*/
void svSignalPlotMapper::CreateNewPlotWindowVector( )
{	
	blSignalCollective::Pointer signalCollective;
	Core::DataEntityHelper::GetProcessingData(GetInputDataEntityHolder( 0 ), signalCollective);

	int numOfSignals = signalCollective->GetNumberOfSignals();

	std::vector<mpWindow*>::iterator it;
	m_svSignalPlotWindowList.resize(numOfSignals);

	int signalPos = 0;
	for (it = m_svSignalPlotWindowList.begin();
		it != m_svSignalPlotWindowList.end();
		it++)
	{
		blSignal::Pointer signal = signalCollective->GetSignal(signalPos); 

		svSignalPlotWindow* window;
		window = new svSignalPlotWindow(
			GetInputDataEntityHolder( 0 ),
			GetOutputDataEntityHolder( 0 ),
			m_ParentWindow, 
			wxID_ANY );
		window->SetTimeStepHolder( m_timeStepHolder );
		window->Hide(); 

		(*it) = window;
		signalPos++;
	}	

}


/**
*/
void svSignalPlotMapper::Clear()
{
}

/**
*/
void svSignalPlotMapper::PlotSignalValues( )
{
	blSignalCollective::Pointer signalCollective;
	Core::DataEntityHelper::GetProcessingData(GetInputDataEntityHolder( 0 ), signalCollective);
	if (signalCollective.IsNull()) 
		return;

	std::vector<float> x_vector, y_vector;

	// For each svSignalPlotWindow, add the signal data
	int signalPos = 0;
	std::vector<mpWindow*>::iterator it;
	for( it = m_svSignalPlotWindowList.begin();
		it != m_svSignalPlotWindowList.end(); 
		it++)
	{
		svSignalPlotWindow* window;
		window = dynamic_cast< svSignalPlotWindow* > ( (*it) );

		blSignal::Pointer signal = signalCollective->GetSignal(signalPos);
		window->ResetSignals( );
		window->AddSignal( signalPos, signal );

		signalPos++;
	}	
}

void svSignalPlotMapper::SynchronizeAll( bool val )
{
	std::vector<mpWindow*>::iterator it;
	for (it = m_svSignalPlotWindowList.begin();
		it != m_svSignalPlotWindowList.end();
		it++)
	{
		svSignalPlotWindow* signalPlot;
		signalPlot = dynamic_cast<svSignalPlotWindow*> ( *it );
		if ( signalPlot != NULL )
		{
			signalPlot->Synchronize( val );
		}
	}
}

void svSignalPlotMapper::UpdateSignals()
{
	blSignalCollective::Pointer signalCollective;
	Core::DataEntityHelper::GetProcessingData(GetInputDataEntityHolder( 0 ), signalCollective);
	if (signalCollective.IsNull()) 
		return;

	std::vector<mpWindow*>::iterator it;
	int signalPos = 0;
	for (it = m_svSignalPlotWindowList.begin();
		it != m_svSignalPlotWindowList.end();
		it++)
	{
		svSignalPlotWindow* window;
		window = dynamic_cast< svSignalPlotWindow* > ( (*it) );

		if ( signalPos < signalCollective->GetNumberOfSignals() )
		{
			blSignal::Pointer signal = signalCollective->GetSignal(signalPos);
			window->UpdateSignal( signal );
		}

		signalPos++;
	}
}


void svSignalPlotMapper::ZoomInYAll( )
{
	std::vector<mpWindow*>::iterator it;
	for (it = m_svSignalPlotWindowList.begin();
		it != m_svSignalPlotWindowList.end();
		it++)
	{
		svSignalPlotWindow* window;
		window = dynamic_cast< svSignalPlotWindow* > ( (*it) );

		window->ZoomInY();
	}
}

void svSignalPlotMapper::ZoomOutYAll( )
{
	std::vector<mpWindow*>::iterator it;
	for (it = m_svSignalPlotWindowList.begin();
		it != m_svSignalPlotWindowList.end();
		it++)
	{
		svSignalPlotWindow* window;
		window = dynamic_cast< svSignalPlotWindow* > ( (*it) );

		window->ZoomOutY();
	}
}

void svSignalPlotMapper::FitAll()
{
	std::vector<mpWindow*>::iterator it;
	for (it = m_svSignalPlotWindowList.begin();
		it != m_svSignalPlotWindowList.end();
		it++)
	{
		svSignalPlotWindow* window;
		window = dynamic_cast< svSignalPlotWindow* > ( (*it) );

		window->Fit();
	}
}

void svSignalPlotMapper::SetParentWindow( wxWindow* val )
{
	m_ParentWindow = val;
}

void svSignalPlotMapper::Update()
{
	Core::DataEntity::Pointer dataEntity;
	dataEntity = GetInputDataEntityHolder( 0 )->GetSubject( );

	m_svSignalPlotWindowList.resize( 0 );

	if (dataEntity.IsNull())
	{
		return;
	}

	CreateNewPlotWindowVector(  );

	PlotSignalValues( );

}

std::vector<mpWindow*> svSignalPlotMapper::GetsvSignalPlotWindowList() const
{
	return m_svSignalPlotWindowList;
}

void svSignalPlotMapper::SetTimeStepHolder( Core::IntHolderType::Pointer val )
{
	m_timeStepHolder = val;
}

void svSignalPlotMapper::OnModifiedParams()
{
	svPlotParameters::Pointer plotParameters;
	plotParameters = m_ParamHolder->GetSubject( );

	std::vector<mpWindow*>::iterator it;
	for (it = m_svSignalPlotWindowList.begin();
		it != m_svSignalPlotWindowList.end();
		it++)
	{
		svSignalPlotWindow* signalPlot;
		signalPlot = dynamic_cast<svSignalPlotWindow*> ( *it );
		if ( signalPlot != NULL )
		{
			signalPlot->SetWindowOffset( plotParameters->GetWindowOffset( ) );
			signalPlot->SetWindowSize( plotParameters->GetWindowSize( ) );
		}
	}
}

svPlotParametersHolderType::Pointer svSignalPlotMapper::GetParamHolder() const
{
	return m_ParamHolder;
}
