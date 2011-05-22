/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
// For compilers that don't support precompilation, include "wx/wx.h"
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "coreBullEyePlotWidget.h"
#include "coreKernel.h"
#include "coreDataEntityHelper.h"
#include "coreMultiRenderWindow.h"
#include "coreWxMitkGraphicalInterface.h"
#include "coreInputControl.h"
#include "coreReportExceptionMacros.h"

#include "vtkLookupTable.h"
#include "vtkStringArray.h"
#include "vtkPointData.h"
#include "vtkProperty2D.h"
#include "vtkTextProperty.h"
#include "vtkFieldData.h"
#include "vtkFloatArray.h"
#include "vtkScalarBarActor.h"
#include "vtkMath.h"
#include "vtkColorTransferFunction.h"

#include "blVtkBullsEyeActor.h"

#include "blSignalCollective.h"
#include "blLookupTables.h"

#include "mitkVtkLayerController.h"
#include "blMitkUnicode.h"

#include "DartBoard.xpm"

using namespace Core::Widgets;

BEGIN_EVENT_TABLE(BullEyePlotWidget, wxPanel)
  EVT_COMBOBOX(wxID_BULLS_EYE_CMB_SELECTION, BullEyePlotWidget::OnComboSelected)
END_EVENT_TABLE()


const long wxID_wxMitkRenderWindow = wxNewId( );
const char* STR_PLOT = "TITLE";
const char* STR_PLOT_UNITS = "UNITS";
//!
BullEyePlotWidget::BullEyePlotWidget(
	wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style)
: coreBullEyePlotWidgetUI(parent, id, pos, size, style)
{
	SetName( "BullEyePlotWidget" );
	//SetBitmap( dartboard_xpm );

	m_wxMitkSelectableGLWidget = new mitk::wxMitkSelectableGLWidget( this, wxID_wxMitkRenderWindow, "BullEyePlotWidget" );
	m_bullsEyeActor = vtkSmartPointer<blVtkBullsEyeActor>::New();

	m_DataObject = vtkSmartPointer<vtkDataObject>::New();

	m_InputDataHolder = Core::DataEntityHolder::New();
	m_InputDataHolder->SetName( _U( GetName() ) + ": Input" );
	m_InputDataHolder->AddObserver( 
		this, 
		&BullEyePlotWidget::OnModifiedInput );

	// Init input control widget
	m_AcquireInputWidget = new Core::Widgets::InputControl(
		this, 
		wxID_ANY,
		true);
	GetSizer()->Insert(0, m_AcquireInputWidget, 0, wxEXPAND | wxALL, 4);
	m_AcquireInputWidget->Hide();

	BuildLUTMap();
}

//!
BullEyePlotWidget::~BullEyePlotWidget(void)
{

}

void BullEyePlotWidget::BuildRenderWindow(  )
{
	// Change m_plot by m_wxMitkSelectableGLWidget
	wxSizer* sizer = m_plot->GetContainingSizer();
	sizer->Detach( m_plot );
	m_plot->Destroy();
	sizer->Add( 
		m_wxMitkSelectableGLWidget, 
		1, 
		wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 
		0);

	sizer->Layout();

	//m_Title->SetLabel( STR_PLOT );
	m_bullsEyeActor->SetTitleVisibility( false );
	m_bullsEyeActor->GetPositionCoordinate()->SetValue(0.05,0.1,0.0);
	m_bullsEyeActor->GetPosition2Coordinate()->SetValue(0.95,0.85,0.0);
	m_bullsEyeActor->GetProperty()->SetColor(1,1,1);
	m_bullsEyeActor->GetTitleTextProperty()->SetColor(1,1,0);
	m_bullsEyeActor->GetLabelTextProperty()->SetColor(1,1,1);
	m_bullsEyeActor->SetInput( m_DataObject );
	m_bullsEyeActor->SetLegendVisibility( true );
	m_bullsEyeActor->GetLegendActor( )->SetTitle( STR_PLOT_UNITS );
	m_bullsEyeActor->SetNumberOfDecimals( 1 );

	vtkRenderer	*renderer;
	renderer = mitk::VtkLayerController::GetInstance(
		m_wxMitkSelectableGLWidget->GetVtkRenderWindow() )->GetSceneRenderer();
	renderer->AddActor( m_bullsEyeActor );
}


void BullEyePlotWidget::SetListBrowser( 
 	Core::Widgets::DataEntityListBrowser* dataEntityListBrowser ) 

{
	BaseWindow::SetListBrowser( dataEntityListBrowser );
	BuildRenderWindow( );

	m_AcquireInputWidget->SetDataEntityListBrowser( dataEntityListBrowser ); 
	m_AcquireInputWidget->SetAcquiredInputDataHolder( 
		m_InputDataHolder,
		Core::SignalTypeId );
}


void BullEyePlotWidget::OnModifiedInput()
{
	if ( !IsEnabled() )
	{
		return;
	}
	try
	{

		// Clean data object
		CleanDataObject();

		if (CheckInputs())
		{
			AddInputSignalsToDataObject( );
		}	
		
		// Update combo box
		UpdateWidgets( );

		// Select the first one
		SetSelectedSignal( 0 );
		
	}
	coreCatchExceptionsReportAndNoThrowMacro( 
		"BullEyePlotWidget" );
}

void Core::Widgets::BullEyePlotWidget::AddInputSignalsToDataObject()
{
	blSignalCollective::Pointer signalCollective;
	Core::DataEntityHelper::GetProcessingData( 
		m_InputDataHolder,
		signalCollective );

	// Find signal with 17 values
	for ( unsigned i = 0 ; i < signalCollective->GetNumberOfSignals(); i++ )
	{
		blSignal::Pointer signal = signalCollective->GetSignal( i );
		AddSignalToDataObject( signal );
	}
}

void Core::Widgets::BullEyePlotWidget::AddSignalToDataObject( blSignal::Pointer signal )
{
	// Convert from signal to vtk
	std::vector<float> yVector = signal->GetYVector( );
	vtkSmartPointer<vtkFloatArray> dataArray;
	dataArray = vtkSmartPointer<vtkFloatArray>::New();
	dataArray->SetNumberOfTuples( yVector.size() );
	dataArray->SetName( signal->GetName().c_str() );
	for ( unsigned i = 0 ; i < yVector.size() ; i++ )
	{
		dataArray->SetValue( i, yVector[ i ] );
	}

	// Update object and bulls eye actor
	dataArray->Modified();
	m_DataObject->GetFieldData()->AddArray( dataArray );
}

void Core::Widgets::BullEyePlotWidget::CleanDataObject()
{
	while ( m_DataObject->GetFieldData()->GetNumberOfArrays() )
	{
		char *arrayName;
		arrayName = m_DataObject->GetFieldData()->GetArray( 0 )->GetName();
		m_DataObject->GetFieldData()->RemoveArray( arrayName );
	}
}

void Core::Widgets::BullEyePlotWidget::RefreshPlot()
{
	m_DataObject->Modified();
	m_wxMitkSelectableGLWidget->ForceImmediateUpdate();
	Refresh();
}

void Core::Widgets::BullEyePlotWidget::OnComboSelected(
	wxCommandEvent& WXUNUSED(event))
{ 
	int sel = m_ComboBoxSelection->GetCurrentSelection();
	if ( sel == -1 || m_DataObject->GetFieldData()->GetNumberOfArrays() == 0 )
	{
		return;
	}

	SetSelectedSignal( sel );
}

void Core::Widgets::BullEyePlotWidget::SetSelectedSignal( int iSignal )
{
	std::string strTitle = STR_PLOT;
	std::string strLegendTitle = STR_PLOT_UNITS;
	std::string strSelectedArray;

	if ( m_InputDataHolder->GetSubject().IsNotNull( ) )
	{
		// Get selected signal
		blSignalCollective::Pointer signalCollective;
		Core::DataEntityHelper::GetProcessingData( 
			m_InputDataHolder,
			signalCollective );

		if ( unsigned( iSignal ) < signalCollective->GetNumberOfSignals() )
		{

			blSignal::Pointer selectedSignal = signalCollective->GetSignal( iSignal );
			strSelectedArray = selectedSignal->GetName();
			strTitle = selectedSignal->GetName();
			strLegendTitle = selectedSignal->GetYUnit();
		}
	}

	// Update controls
	m_ComboBoxSelection->SetSelection( iSignal );
	//m_Title->SetLabel( wxString::Format( "%s", strTitle.c_str() ) );
	//m_Title->GetParent()->FitInside();
	m_bullsEyeActor->GetLegendActor( )->SetTitle( strLegendTitle.c_str() );
	
	// Select data array in bulls eye
	UpdateLUT( );
	m_bullsEyeActor->SetArrayName( strSelectedArray.c_str() );
	RefreshPlot( );
}

void Core::Widgets::BullEyePlotWidget::UpdateLUT()
{
	vtkScalarsToColors *scalarsToColors;
	scalarsToColors = m_LUTMap[ std::string((const char*)m_ComboBoxSelection->GetValue().c_str()) ];
	// If we don't have specific LUT use default one
	if ( scalarsToColors == NULL )
	{
		// Use default with input range
		scalarsToColors = m_LUTMap[ "Default" ];
		m_bullsEyeActor->UseInputArrayScalarRange( true );
	}
	else
	{
		// Use range of the LUT
		if (scalarsToColors == m_LUTMap[ "LAT" ])
			m_bullsEyeActor->UseInputArrayScalarRange( true );
		else
			m_bullsEyeActor->UseInputArrayScalarRange( false );
	}
	m_bullsEyeActor->SetVtkDefaultLut( scalarsToColors );
}

void Core::Widgets::BullEyePlotWidget::BuildLUTMap()
{
	blLookupTables lookupTables;
	vtkSmartPointer<vtkColorTransferFunction> colorTransferFunction;
	vtkLookupTable *lookupTable;

	// "Default"
	vtkSmartPointer<vtkLookupTable> defaultLookupTable(
		lookupTables.BuildLookupTable( blLookupTables::LUT_TYPE_PERCEP_LIN_RAINBOW )
		);
	m_LUTMap[ "Default" ] = defaultLookupTable;

	// "Wall thickness (ED)"
	colorTransferFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
	colorTransferFunction->AddRGBPoint( 20, 1, 0, 0 );
	colorTransferFunction->AddRGBPoint( 14, 1, 0.5, 0.25 );
	colorTransferFunction->AddRGBPoint( 5, 0, 1, 0 );
	colorTransferFunction->Build();
	m_LUTMap[ "Wall thickness (ED)" ] = colorTransferFunction;

	// "Wall thickening (ED-ES)/ED"
	colorTransferFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
	colorTransferFunction->AddRGBPoint( 100, 0, 1, 0 );
	colorTransferFunction->AddRGBPoint( 30, 1, 0.5, 0.25 );
	colorTransferFunction->AddRGBPoint( -50, 1, 0, 0 );
	colorTransferFunction->Build();
	m_LUTMap[ "Wall thickening (ED-ES)/ED" ] = colorTransferFunction;

	// "Longitudinal displacement (ED-ES)"
	lookupTable = lookupTables.BuildLookupTable( 
		blLookupTables::LUT_TYPE_PERCEP_LIN_RAINBOW_INVERTED );
	lookupTable->SetRange( -20, 20 );
	m_LUTMap[ "Longitudinal displacement (ED-ES)" ] = lookupTable;

	// "Regional stroke volume (ED-ES)"
	lookupTable = lookupTables.BuildLookupTable( 
		blLookupTables::LUT_TYPE_RAINBOW_HUE_INVERTED );
	lookupTable->SetRange( 0, 10 );
	m_LUTMap[ "Regional stroke volume (ED-ES)" ] = lookupTable;

	//"LAT"
	lookupTable = lookupTables.BuildLookupTable( 
		blLookupTables::LUT_TYPE_RAINBOW_HUE_INVERTED );
	//lookupTable->SetRange( 0, 10 );
	m_LUTMap[ "LAT" ] = lookupTable;
	
}

void Core::Widgets::BullEyePlotWidget::UpdateWidgets()
{
	m_ComboBoxSelection->Clear();
	for ( int i = 0 ; i < m_DataObject->GetFieldData()->GetNumberOfArrays() ; i++ )
	{
		char *arrayName;
		arrayName = m_DataObject->GetFieldData()->GetArray( i )->GetName();
		m_ComboBoxSelection->Append( wxString::FromAscii(arrayName) );
	}
	
}

bool Core::Widgets::BullEyePlotWidget::CheckInputs()
{
	if ( m_InputDataHolder->GetSubject().IsNull( ) )
	{
		return false;
	}

	blSignalCollective::Pointer signalCollective;
	Core::DataEntityHelper::GetProcessingData( 
		m_InputDataHolder,
		signalCollective );

	bool noValues = true;
	// Find signal with 17 values
	for ( unsigned i = 0 ; i < signalCollective->GetNumberOfSignals(); i++ )
	{
		blSignal::Pointer signal = signalCollective->GetSignal( i );
		if ( signal->GetYVector().size() != 17 )
		{
			noValues = false;
		}
	}

	return noValues;
}

Core::BaseProcessor::Pointer Core::Widgets::BullEyePlotWidget::GetProcessor()
{
	return NULL;
}

void Core::Widgets::BullEyePlotWidget::SetTimeStep( int time )
{
	BaseWindow::SetTimeStep( time );

	if ( !m_chkSynchronize->GetValue() )
	{
		return ;
	}


	if ( time >= 0 && time < m_ComboBoxSelection->GetCount() )
	{
		m_ComboBoxSelection->SetSelection( time );
		SetSelectedSignal( time );
	}

}
