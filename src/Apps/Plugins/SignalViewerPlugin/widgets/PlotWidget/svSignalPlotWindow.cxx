// Copyright 2006 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "svSignalPlotWindow.h"
#include "svSignalAnnotationsLayer.h"
#include "wxID.h"
#include <wx/textctrl.h>
#include "blMitkUnicode.h"
#include "coreDataEntityHelper.h"

#include "coreKernel.h"
#include "svSignalDataLayer.h"
#include "svSignalAnnotationDialog.h"
#include "svSignalAnnotationListBox.h"
#include "svSignalFreeAnnotationDialog.h"

#include <blSignalEvent.h>
#include "blSignalCollective.h"

#define wxID_ADD_PLOT_ANNOTATION wxID( "wxID_ADD_PLOT_ANNOTATION" )
#define wxID_ADD_FREE_ANNOTATION wxID( "wxID_ADD_FREE_ANNOTATION" )
#define wxID_SYNCHRONIZE_WITH_DATA wxID( "wxID_SYNCHRONIZE_WITH_DATA")
#define wxID_EDIT_PLOT_ANNOTATION wxID( "wxID_EDIT_PLOT_ANNOTATION" )
#define wxID_ZOOM_IN_Y wxID( "wxID_ZOOM_IN_Y" )
#define wxID_ZOOM_OUT_Y wxID( "wxID_ZOOM_OUT_Y" )

BEGIN_EVENT_TABLE(svSignalPlotWindow, mpWindow)
  EVT_MENU( wxID_ADD_PLOT_ANNOTATION,    svSignalPlotWindow::OnAddPlotAnnotation)
  EVT_MENU( wxID_ADD_FREE_ANNOTATION,    svSignalPlotWindow::OnAddFreeAnnotation)
  EVT_MENU( wxID_SYNCHRONIZE_WITH_DATA, svSignalPlotWindow::OnSyncPlotWithParent)
  EVT_MENU( wxID_ZOOM_IN_Y, svSignalPlotWindow::OnZoomInY)
  EVT_MENU( wxID_ZOOM_OUT_Y, svSignalPlotWindow::OnZoomOutY)
  EVT_LEFT_UP( svSignalPlotWindow::OnMouseLeftRelease)
  EVT_MOTION(svSignalPlotWindow::OnMouseMoved)
END_EVENT_TABLE();



svSignalPlotWindow::svSignalPlotWindow( 
	Core::DataEntityHolder::Pointer inSignalDataHolder,
	Core::DataEntityHolder::Pointer outSignalDataHolder, 
	wxWindow *parent, 
	wxWindowID id ) : mpWindow( parent, id )
{
	SetMinSize( wxDefaultSize );
	SetInitialSize( wxDefaultSize );

	m_InputSignalDataHolder = inSignalDataHolder;
	m_OutputSignalDataHolder = outSignalDataHolder;
	// Eliminate the flicker
	EnableDoubleBuffer( true );

	m_IsSyncEnable = false;

	m_VerticalLine = NULL;

	// Add pop up menu
	wxMenu* popupMenu = GetPopupMenu( );
	popupMenu->Append( 
		wxID_SYNCHRONIZE_WITH_DATA, 
		_("Synchronize with parent data"), 
		_("Synchronize with parent data"),
		wxITEM_CHECK);

	popupMenu->Append( wxID_ZOOM_IN_Y,_("Zoom in Y"), _("Zoom in Y"));
	popupMenu->Append( wxID_ZOOM_OUT_Y,_("Zoom out Y"), _("Zoom out Y"));
	popupMenu->Append( 
		wxID_ADD_PLOT_ANNOTATION,
		_("Add annotation"), 
		_("Add annotation to the current position"));
	popupMenu->Append( 
		wxID_ADD_FREE_ANNOTATION,
		_("Add Free annotation"), 
		_("Add Free annotation to the current position"));

	m_AnnotationDialog = NULL;

	m_WindowOffset = 0;
	m_WindowSize = 10;
}

svSignalPlotWindow::~svSignalPlotWindow()
{
	if ( m_timeStepHolderConnection.connected() )
	{
		m_timeStepHolderConnection.disconnect();
	}
}


void svSignalPlotWindow::SetTimeStepHolder(Core::IntHolderType::Pointer timeStepHolder)
{
	m_timeStepHolder = timeStepHolder;
	m_timeStepHolderConnection = m_timeStepHolder->AddObserver(
		this, 
		&svSignalPlotWindow::VerticalLineTracker );
}	



void svSignalPlotWindow::ResetSignals()
{
	for( svSignalLayersVectorType::iterator it = m_svSignalLayers.begin();
		it != m_svSignalLayers.end(); it++)
	{
		it->DelLayers( this );
	}

	while ( m_svSignalLayers.size() )
	{
		m_svSignalLayers.erase( m_svSignalLayers.begin() );
	}
}

void svSignalPlotWindow::AddSignal( int pos, blSignal::Pointer signal )
{
	svSignalLayer svSignalLayers;
	svSignalLayers.SetSignal( signal );
	svSignalLayers.AddLayers( this );
	if ( signal->GetName( ) != "Time" )
	{
		svSignalLayers.GetsvScaleXLayer( )->SetAutoHide( true );
		svSignalLayers.GetsvScaleYLayer( )->SetAutoHide( true );
	}
	m_svSignalLayers.push_back( svSignalLayers );

	// Compute initial bounding box
	Fit( );
	UpdateView();
}


void svSignalPlotWindow::OnSyncPlotWithParent(wxCommandEvent& event)
{
	try
	{
		Synchronize( GetPopupMenu( )->IsChecked( wxID_SYNCHRONIZE_WITH_DATA ) );
	}
	coreCatchExceptionsReportAndNoThrowMacro( svSignalPlotWindow::OnSyncPlotWithParent )
}

void svSignalPlotWindow::VerticalLineTracker()
{
	try
	{

		// clear all the lines
		if (m_VerticalLine!= NULL)
			this->DelLayer(m_VerticalLine);

		int currentTimeStep = m_timeStepHolder->GetSubject( );

		if (m_InputSignalDataHolder->GetSubject()->GetFather())
		{
			float currentTime = m_InputSignalDataHolder->GetSubject()->GetFather()->GetTimeAtTimeStep(currentTimeStep);		
			m_VerticalLine = new svVerticalLineLayer( currentTime );
			m_VerticalLine->SetDrawOutsideMargins(false);
			this->AddLayer(m_VerticalLine);
		}

		UpdateView();
	}
	coreCatchExceptionsLogAndNoThrowMacro( svSignalPlotWindow::VerticalLineTracker )
}

void svSignalPlotWindow::Sync()
{
	if ( m_InputSignalDataHolder->GetSubject().IsNull() )
	{
		return;
	}

	if ( m_InputSignalDataHolder->GetSubject()->GetFather().IsNull() )
	{
		throw Core::Exceptions::Exception( 
			"svSignalPlotWindow::Sync",
			"Please set a parent data to Synchronize the signal" );
	}

	int numberOfTimeSteps = m_InputSignalDataHolder->GetSubject()->GetFather()->GetNumberOfTimeSteps();
	int currentTimeStep = m_timeStepHolder->GetSubject();
	Core::DataEntity::Pointer fatherData = m_InputSignalDataHolder->GetSubject()->GetFather();

	if ( fatherData.IsNull( ) )
	{
		return;
	}

	double currentImageTime;
	currentImageTime = fatherData->GetTimeAtTimeStep( currentTimeStep );

	// In sec.
	currentImageTime += m_WindowOffset;

	// In sec.
	double windowSize = 10;
	double time1 = currentImageTime - m_WindowSize / 2;
	double time2 = currentImageTime + m_WindowSize / 2;

	blSignalCollective::Pointer signal;
	Core::DataEntityHelper::GetProcessingData(m_InputSignalDataHolder,  signal);

	Fit(time1, time2 , m_desiredYmin, m_desiredYmax );

	// Hide Time label
	if ( m_VerticalLine )
	{
		m_VerticalLine->SetName( "" );
	}
}

bool svSignalPlotWindow::GetIsSyncEnable() const
{
	return m_IsSyncEnable;
}

void svSignalPlotWindow::Synchronize( bool val )
{
	m_IsSyncEnable = val;
	// Reset bounding box
	Fit( );
	UpdateView();
}

void svSignalPlotWindow::OnAddPlotAnnotation( 
	wxCommandEvent& event )
{
	// select the first curve, later, add a mark for every curve 
	svSignalLayersVectorType::iterator it = m_svSignalLayers.begin(); 
	unsigned pos;
	bool proceed = it->GetSignal( )->FindXValuePosition(p2x(m_clickedX), pos);

	// break if there user clicked outisde the range of the graph
	if (!proceed)
	{
		return;
	}

	blSignalEvent selectedEvent;
	bool selected = ShowAnnotationList( selectedEvent );
	if ( !selected )
	{
		return;
	}

	m_svSignalLayers[ 0 ].GetsvSignalAnnotationsLayer( )->StartAddAnnotation( 
		selectedEvent, 
		m_clickedX,
		m_clickedY,
		p2x(m_clickedX) );

	if ( m_svSignalLayers[ 0 ].GetsvSignalAnnotationsLayer( )->GetDrawingInterval() )
	{
		this->SetCursor(wxCURSOR_SIZEWE);
	}
	else
	{
		m_OutputSignalDataHolder->GetSubject()->Modified( );
		m_OutputSignalDataHolder->NotifyObservers( );
	}

	this->Refresh();

}

void svSignalPlotWindow::OnMouseLeftRelease( wxMouseEvent &event )
{
	if ( !m_svSignalLayers[ 0 ].GetsvSignalAnnotationsLayer( )->GetDrawingInterval() )
	{
		event.Skip();
		return;
	}

	m_svSignalLayers[ 0 ].GetsvSignalAnnotationsLayer( )->EndAddAnnotation( 
		p2x( event.GetX() ) );

	// Update ES And ED time steps
	m_OutputSignalDataHolder->GetSubject()->Modified( );
	m_OutputSignalDataHolder->NotifyObservers( );

	this->SetCursor(wxCursor(wxCURSOR_ARROW));
}

void svSignalPlotWindow::OnMouseMoved( wxMouseEvent &event )
{
	if ( m_svSignalLayers.size( ) == 0 || 
		 !m_svSignalLayers[ 0 ].GetsvSignalAnnotationsLayer( )->GetDrawingInterval() )
	{
		event.Skip();
		return;
	}

	m_svSignalLayers[ 0 ].GetsvSignalAnnotationsLayer( )->OnMouseMove( p2x( event.GetX() ) );

	Refresh( );
}

bool svSignalPlotWindow::ShowAnnotationList( blSignalEvent &selectedEvent )
{
	wxPoint screenPoint;
	screenPoint = ClientToScreen( wxPoint(m_clickedX, m_clickedY) );

	m_AnnotationDialog = new svSignalAnnotationDialog( 
		this, 
		wxID_ANY,
		"Select type", 
		screenPoint, 
		wxSize(200,100),
		wxTHICK_FRAME );
	m_AnnotationDialog->SetSize( wxSize( 200, 100 ) );
	int ret = m_AnnotationDialog->ShowModal( );

	bool selected = false;
	if ( ret == wxID_OK )
	{
		selected = m_AnnotationDialog->GetAnnotationList( )->GetSelectedEvent( selectedEvent );
	}

	m_AnnotationDialog->Destroy();
	m_AnnotationDialog= NULL;

	return selected;
}

void svSignalPlotWindow::UpdateSignal( blSignal::Pointer signal )
{
	for( svSignalLayersVectorType::iterator it = m_svSignalLayers.begin();
		it != m_svSignalLayers.end(); it++)
	{
		if ( it->GetSignal() == signal )
		{
			it->UpdateSignal( signal );
			Refresh();
		}
	}

	UpdateView( );
}

void svSignalPlotWindow::OnZoomInY( wxCommandEvent& event )
{
	ZoomInY();
}

void svSignalPlotWindow::OnZoomOutY( wxCommandEvent& event )
{
	ZoomOutY( );
}

void svSignalPlotWindow::ZoomInY()
{
	Fit( 
		m_desiredXmin, 
		m_desiredXmax, 
		m_desiredYmin / zoomIncrementalFactor ,
		m_desiredYmax / zoomIncrementalFactor );
}

void svSignalPlotWindow::ZoomOutY()
{
	Fit( 
		m_desiredXmin, 
		m_desiredXmax, 
		m_desiredYmin * zoomIncrementalFactor ,
		m_desiredYmax * zoomIncrementalFactor );
}

void svSignalPlotWindow::SetWindowOffset( double offset )
{
	m_WindowOffset = offset;
	UpdateView( );
}

void svSignalPlotWindow::SetWindowSize( double size )
{
	m_WindowSize = size;
	UpdateView( );
}

void svSignalPlotWindow::UpdateView()
{
	if ( m_IsSyncEnable )
		Sync();
	else 
	{
		if ( m_VerticalLine )
		{
			m_VerticalLine->SetName( "Time" );
		}
		Fit( m_desiredXmin, 
			m_desiredXmax, 
			m_desiredYmin,
			m_desiredYmax );
	}
}

void svSignalPlotWindow::OnAddFreeAnnotation( wxCommandEvent& event )
{
	// select the first curve, later, add a mark for every curve 
	svSignalLayersVectorType::iterator it = m_svSignalLayers.begin(); 
	unsigned pos;
	bool proceed = it->GetSignal( )->FindXValuePosition(p2x(m_clickedX), pos);

	// break if there user clicked outside the range of the graph
	if (!proceed)
	{
		return;
	}

	blSignalEvent selectedEvent;
	std::string name = AskFreeAnnotationName( );
	if ( name.empty() )
	{
		return;
	}

	m_svSignalLayers[ 0 ].GetsvSignalAnnotationsLayer( )->StartAddAnnotation( 
		selectedEvent, 
		m_clickedX,
		m_clickedY,
		p2x(m_clickedX) );

	m_svSignalLayers[ 0 ].GetsvSignalAnnotationsLayer( )->GetCurrAnnotation( )->SetFreeName( name );

	if ( m_svSignalLayers[ 0 ].GetsvSignalAnnotationsLayer( )->GetDrawingInterval() )
	{
		this->SetCursor(wxCURSOR_SIZEWE);
	}
	else
	{
		m_OutputSignalDataHolder->GetSubject()->Modified( );
		m_OutputSignalDataHolder->NotifyObservers( );
	}

	this->Refresh();
}

std::string svSignalPlotWindow::AskFreeAnnotationName( )
{
	wxPoint screenPoint;
	screenPoint = ClientToScreen( wxPoint(m_clickedX, m_clickedY) );

	std::string name;
	svSignalFreeAnnotationDialog* dialog = new svSignalFreeAnnotationDialog(
		this, 
		wxID_ANY,
		"New annotation", 
		screenPoint, 
		wxSize(200,100),
		wxTHICK_FRAME );
	dialog->SetSize( wxSize( 200, 25 ) );
	int ret = dialog->ShowModal( );
	if ( ret == wxID_OK )
	{
		name = dialog->GetTxtName( )->GetValue();
	}

	dialog->Destroy();

	return name;
}
