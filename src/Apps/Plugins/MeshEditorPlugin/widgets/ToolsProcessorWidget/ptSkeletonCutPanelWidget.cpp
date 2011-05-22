// Copyright 2009 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "ptSkeletonCutPanelWidget.h"
#include "mitkProperties.h"
#include "corePointInteractorPointSelect.h"

ptSkeletonCutPanelWidget::ptSkeletonCutPanelWidget(
	wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style):
    wxPanel(parent, id, pos, size, wxTAB_TRAVERSAL)
{
    btnSkeletonCut = new wxButton(this, wxID_btnSkeletonCut, wxT("Split using Plane Cut"));
	m_processor = Core::SkeletonCutProcessor::New();

	m_processor->GetInputDataEntityHolder( 0 )->AddObserver( 
		this, 
		&ptSkeletonCutPanelWidget::OnSelectedInput );

	m_processor->GetInputDataEntityHolder( 1 )->AddObserver( 
		this, 
		&ptSkeletonCutPanelWidget::OnSelectedSkeleton );

	m_processor->GetInputDataEntityHolder( 2 )->AddObserver( 
		this, 
		&ptSkeletonCutPanelWidget::OnSelectedPoint );

	do_layout();
}


BEGIN_EVENT_TABLE(ptSkeletonCutPanelWidget, wxPanel)
    // begin wxGlade: ptSkeletonCutPanelWidget::event_table
    EVT_BUTTON(wxID_btnSkeletonCut, ptSkeletonCutPanelWidget::OnButtonSkeletonCut)
    // end wxGlade
END_EVENT_TABLE();

void ptSkeletonCutPanelWidget::OnButtonSkeletonCut(wxCommandEvent &event)
{
	UpdateProcessor( );
	Enable(false);
}

void ptSkeletonCutPanelWidget::do_layout()
{
    // begin wxGlade: ptSkeletonCutPanelWidget::do_layout
    wxBoxSizer* GlobalSizer = new wxBoxSizer(wxVERTICAL);
    GlobalSizer->Add(btnSkeletonCut, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 2);
    SetSizer(GlobalSizer);
    GlobalSizer->Fit(this);
    // end wxGlade
}

bool ptSkeletonCutPanelWidget::Enable( bool enable /*= true */ )
{
	bool bReturn = wxPanel::Enable( enable );

	try
	{
		const std::string helpStr = \
			"Skeleton Cut tool: cut a surface using a perpandicular" \
			"\nplan to the skeleton." \
			"\n\nUsage: SHIFT + left click on the skeleton.";
		SetInfoUserHelperWidget( helpStr );
		
		Core::Widgets::LandmarkSelectorWidget* widget;
		widget = GetSelectionToolWidget< Core::Widgets::LandmarkSelectorWidget>( "Landmark selector" );
		if ( widget == NULL )
		{
			return bReturn;
		}

		if ( CheckInputs())
		{
			ConnectInteractor();
		}
		else
		{
			DisconnectInteractor();
			Core::DataContainer::Pointer dataContainer = Core::Runtime::Kernel::GetDataContainer();
			Core::DataEntityList::Pointer list = dataContainer->GetDataEntityList();
			if ( m_processor->GetInputDataEntity( 2 ).IsNotNull() )
			{
				list->Remove( m_processor->GetInputDataEntity( 2 ) );
			}
			if ( m_processor->GetOutputDataEntity(1 ).IsNotNull() )
			{
				list->Remove( m_processor->GetOutputDataEntity(1 ) );
			}
		}
	}
	coreCatchExceptionsReportAndNoThrowMacro("RegionGrowWidget::Enable");

	return bReturn;
}

void ptSkeletonCutPanelWidget::OnSelectedPoint()
{
	try
	{
		if ( CheckInputs()  )
		{
			m_processor->UpdatePlane();
		}
	}
	coreCatchExceptionsLogAndNoThrowMacro(
		ptSkeletonCutPanelWidget::OnSelectedPoint)
}

Core::BaseProcessor::Pointer ptSkeletonCutPanelWidget::GetProcessor()
{
	return m_processor.GetPointer();
}

void ptSkeletonCutPanelWidget::OnSelectedSkeleton()
{
	try
	{
		if (CheckInputs())
		{
			ConnectInteractor();
		}

	}
	coreCatchExceptionsReportAndNoThrowMacro(OnSelectedSkeleton)
}

void ptSkeletonCutPanelWidget::OnSelectedInput()
{
	try
	{
		if ( CheckInputs())
		{
			ConnectInteractor();
		}

	}
	coreCatchExceptionsReportAndNoThrowMacro(OnSelectedInput)
}

bool ptSkeletonCutPanelWidget::CheckInputs()
{
	try
	{
		if(!IsEnabled())
			return false;

		m_processor->CheckInputs();
	

		return true;
	}
	catch( ... )
	{
		return false;
	}

	return true;
}

void ptSkeletonCutPanelWidget::ConnectInteractor()
{
	Core::Widgets::LandmarkSelectorWidget* widget;
	widget = GetSelectionToolWidget< Core::Widgets::LandmarkSelectorWidget>( "Landmark selector" );


	// Connect interactor
	widget->SetAllowedInputDataTypes( Core::SkeletonTypeId );
	widget->SetInteractorType( Core::PointInteractor::POINT_SELECT);
	widget->StartInteractor();	


	Core::PointInteractorPointSelect* pointSelectInteractor;
	pointSelectInteractor = static_cast<Core::PointInteractorPointSelect*> (
		widget->GetPointInteractor( ).GetPointer( ));

	m_processor->SetInputDataEntity( 2, 
		pointSelectInteractor->GetSelectedPointsDataEntity() );
	pointSelectInteractor->SetCuttingType(SKELETON_TYPE);
}

void ptSkeletonCutPanelWidget::DisconnectInteractor()
{
	Core::Widgets::LandmarkSelectorWidget* widget;
	widget = GetSelectionToolWidget< Core::Widgets::LandmarkSelectorWidget>( "Landmark selector" );

	// Disconnect interactor
	widget->StopInteraction();

	widget->SetDefaultAllowedInputDataTypes( );

}

void ptSkeletonCutPanelWidget::OnInit()
{
	GetProcessorOutputObserver(1)->SetHideInput(false);
	GetProcessorOutputObserver(1)->SelectDataEntity(false);
	GetProcessorOutputObserver(0)->SelectDataEntity(false);

}
