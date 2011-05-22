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

#include <wxUnicode.h>

#include "corePointsTableWidget.h"
#include "coreKernel.h"
#include "coreDataEntityHelper.h"
#include "coreMultiRenderWindow.h"
#include "coreWxMitkGraphicalInterface.h"
#include "coreReportExceptionMacros.h"
#include "coreVTKPolyDataHolder.h"

#include "vtkStringArray.h"
#include "vtkPointData.h"

using namespace Core::Widgets;
using namespace mitk;

// Event the widget
BEGIN_EVENT_TABLE(PointsTableWidget, wxPanel)
	EVT_LIST_ITEM_SELECTED( wxID_LIST_CONTROL_POINTS, PointsTableWidget::OnItemSelected)
END_EVENT_TABLE()

//!
PointsTableWidget::PointsTableWidget(
	wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style)
: corePointsTableWidgetUI(parent, id, pos, size, style)
{
	Core::Runtime::wxMitkGraphicalInterface::Pointer graphicalIface;
	graphicalIface = Core::Runtime::Kernel::GetGraphicalInterface();
	coreAssertMacro(graphicalIface.IsNotNull());
	Core::Runtime::StyleManager::Pointer styleManager = graphicalIface->GetStyleManager();
	coreAssertMacro(styleManager.IsNotNull());
	styleManager->ConfigureProgressPanel(this);

	m_SelectedItemHolder = SelectedItemHolderType::New();
	m_SelectedItemHolder->SetSubject( -1 );

	m_MultiRenderWindow = NULL;
}

//!
PointsTableWidget::~PointsTableWidget(void)
{
	if ( m_timeStepHolder.IsNotNull() )
	{
		m_timeStepHolder->RemoveObserver( 
			this, 
			&PointsTableWidget::OnModifiedInput );
	}

	m_InputDataHolderConnection.disconnect();
}

void PointsTableWidget::SetInput( Core::DataEntityHolder::Pointer data )
{
	m_InputDataHolderConnection.disconnect();
	m_InputDataHolder = data;
	m_InputDataHolderConnection = m_InputDataHolder->AddObserver( 
		this, 
		&PointsTableWidget::OnModifiedInput );
}

void PointsTableWidget::OnModifiedInput()
{
	try{

		if ( !IsEnabled() )
		{
			return;
		}

		UpdateListControl();
	}
	coreCatchExceptionsLogAndNoThrowMacro( PointsTableWidget::OnModifiedInput );
}

void PointsTableWidget::SetUpColumns()
{
	if ( m_InputDataHolder.IsNull( ) || m_InputDataHolder->GetSubject( ).IsNull( ) )
	{
		return;
	}

	// Get processing data
	Core::vtkPolyDataPtr polyData;
	Core::DataEntityHelper::GetProcessingData( 
		m_InputDataHolder, polyData, 
		m_timeStepHolder->GetSubject( ) );
	vtkStringArray *stringData = NULL;
	vtkAbstractArray *abstractData = NULL;
	vtkPointData *pd = polyData->GetPointData();
	if ( !pd )
	{
		return;
	}

	// Resize the columns
	int iNumArrays = 0;
	for ( int arrayNum = 0 ; arrayNum < pd->GetNumberOfArrays(); arrayNum++ )
	{
		abstractData = pd->GetAbstractArray(arrayNum);
		stringData = vtkStringArray::SafeDownCast(abstractData);
		if ( stringData )
		{
			iNumArrays++;
		}
	}

	int iColumnCount = 0;
	if ( m_list_ctrl->GetColumnCount() != iNumArrays + 1 )
	{
		m_list_ctrl->ClearAll();

		wxListItem itemCol;
		itemCol.SetText( wxT("Id") );
		m_list_ctrl->InsertColumn( iColumnCount++, itemCol );
		for ( int i = 0 ; i < iNumArrays ; i++ )
		{
			itemCol.SetText(_U(""));
			m_list_ctrl->InsertColumn( iColumnCount++, itemCol );
		}
	}

	// Find all string arrays
	iColumnCount = 0;
	for ( int arrayNum = 0 ; arrayNum < pd->GetNumberOfArrays(); arrayNum++ )
	{
		abstractData = pd->GetAbstractArray(arrayNum);
		stringData = vtkStringArray::SafeDownCast(abstractData);
		if ( stringData )
		{
			wxListItem item;
			item.SetText( _U(stringData->GetName()) );
			m_list_ctrl->SetColumn( iColumnCount + 1, item );
			iColumnCount++;
		}
	}

}

void PointsTableWidget::FillItems()
{
	if ( m_InputDataHolder.IsNull( ) || m_InputDataHolder->GetSubject( ).IsNull( ) )
	{
		return;
	}

	Core::vtkPolyDataPtr polyData;
	Core::DataEntityHelper::GetProcessingData( 
		m_InputDataHolder, 
		polyData, 
		m_timeStepHolder->GetSubject( ) );

	// Hide the control
	m_list_ctrl->Hide( );

	if ( m_list_ctrl->GetItemCount() != polyData->GetNumberOfPoints() )
	{
		m_list_ctrl->DeleteAllItems();
		for ( int i = 0 ; i < polyData->GetNumberOfPoints() ; i++ )
		{
			// ID
			m_list_ctrl->InsertItem( i, wxString::Format(_T("%d"), i) );
		}
	}

	for ( int i = 0 ; i < polyData->GetNumberOfPoints() ; i++ )
	{
		// Rest of vtkStringArray
		int iColumnCount = 1;
		vtkStringArray *stringData = NULL;
		vtkAbstractArray *abstractData = NULL;
		vtkPointData *pd = polyData->GetPointData();
		for ( int arrayNum = 0 ; arrayNum < pd->GetNumberOfArrays(); arrayNum++ )
		{
			abstractData = pd->GetAbstractArray(arrayNum);
			stringData = vtkStringArray::SafeDownCast(abstractData);
			if ( stringData && i < stringData->GetNumberOfValues() )
			{
				m_list_ctrl->SetItem( i, iColumnCount++, wxString::FromAscii(stringData->GetValue( i )) );
			}
		}
	}

	// Configure the width
	for ( int i = 0 ; i < m_list_ctrl->GetColumnCount( ) ; i++ )
	{
		m_list_ctrl->SetColumnWidth( i, wxLIST_AUTOSIZE );
		int iColumnWidthAutoSize = m_list_ctrl->GetColumnWidth( i );
		m_list_ctrl->SetColumnWidth( i, wxLIST_AUTOSIZE_USEHEADER );
		int iColumnWidthAutoSizeHeader = m_list_ctrl->GetColumnWidth( i );
		if ( iColumnWidthAutoSizeHeader < iColumnWidthAutoSize )
		{
			m_list_ctrl->SetColumnWidth( i, wxLIST_AUTOSIZE );
		}
	}

	// show the control
	m_list_ctrl->Show( );
}

/** 
*/
void PointsTableWidget::OnItemSelected(wxListEvent& event)
{
	m_SelectedItemHolder->SetSubject( event.GetIndex(), true );

	CenterViewsToLandmark( );
}

PointsTableWidget::SelectedItemHolderType::Pointer 
PointsTableWidget::GetSelectedItemHolder() const
{
	return m_SelectedItemHolder;
}


void Core::Widgets::PointsTableWidget::SetMultiRenderWindow( Core::Widgets::RenderWindowBase* val )
{
	m_MultiRenderWindow = val;
}


void Core::Widgets::PointsTableWidget::CenterViewsToLandmark( )
{

	try
	{

		int iItemSelected = GetSelectedItemHolder( )->GetSubject();
		if ( iItemSelected != -1 && m_MultiRenderWindow )
		{
			Core::vtkPolyDataPtr polyData;
			Core::DataEntityHelper::GetProcessingData( m_InputDataHolder, polyData, m_timeStepHolder->GetSubject( ) );
			m_MultiRenderWindow->CenterToPoint( polyData->GetPoint( iItemSelected ) );
		}
	}
	coreCatchExceptionsLogAndNoThrowMacro( PointsTableWidget::CenterViewsToLandmark );
}

void Core::Widgets::PointsTableWidget::SetTimeStepHolder( 
	Core::IntHolderType::Pointer timeStepHolder )
{
	if ( m_timeStepHolder.IsNotNull() )
	{
		m_timeStepHolder->RemoveObserver( 
			this, 
			&PointsTableWidget::OnModifiedInput );
	}
	m_timeStepHolder = timeStepHolder;
	m_timeStepHolder->AddObserver( 
		this, 
		&PointsTableWidget::OnModifiedInput );
}

bool Core::Widgets::PointsTableWidget::Enable( bool enable /*= true */ )
{
	bool bReturn = corePointsTableWidgetUI::Enable( enable );

	if ( enable )
	{
		OnModifiedInput( );
	}

	return bReturn;
}

void Core::Widgets::PointsTableWidget::UpdateListControl()
{
	if ( m_InputDataHolder.IsNull( ) || m_InputDataHolder->GetSubject( ).IsNull( ) )
	{
		m_list_ctrl->ClearAll();
		return;
	}

	SetUpColumns( );

	FillItems( );
}



