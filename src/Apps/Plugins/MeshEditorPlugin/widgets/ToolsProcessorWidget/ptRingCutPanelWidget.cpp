// Copyright 2009 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "ptRingCutPanelWidget.h"
#include "mitkProperties.h"
#include "blMitkUnicode.h"
#include "coreDataContainer.h"
#include "mitkMaterialProperty.h"
#include "coreKernel.h"
#include "corePointInteractorPointSelect.h"
#include "coreDataTreeMITKHelper.h"

ptRingCutPanelWidget::ptRingCutPanelWidget(wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style):
    wxPanel(parent, id, pos, size, wxTAB_TRAVERSAL)
{
    btnRingCut = new wxButton(this, wxID_btnRingCut, wxT("Split using Ring Cut"));
	btnChooseSurface = new wxButton(this, wxID_btnChooseSurface, wxT("Keep surface"));
	cmbSelectColor = new wxComboBox (this, wxID_cmbSelectColor, wxT("Choose surface"));
	toggleBtnEnable = new wxToggleButton(this, wxID_btnEnable, wxT("Enable Interactor"));
	cmbSelectColor->Append(wxT("Orange"));
	cmbSelectColor->Append(wxT("Cyan"));
	cmbSelectColor->Append(wxT("Both"));
	cmbSelectColor->SetSelection(0);
	btnRingCut->Disable();
	btnChooseSurface->Disable();
	m_oldSurfaceColor = NULL;
	m_processor = Core::RingCutProcessor::New();

	m_processor->GetInputDataEntityHolder( 0 )->AddObserver( 
		this, 
		&ptRingCutPanelWidget::OnSelectedInput );

	m_processor->GetInputDataEntityHolder( 1 )->AddObserver( 
		this, 
		&ptRingCutPanelWidget::OnSelectedPoint );

	m_interactionEnabled = false;

	do_layout();
	UpdateWidget();
}
ptRingCutPanelWidget::~ptRingCutPanelWidget()
{
	if(m_oldSurfaceColor!=NULL)
	{
		delete m_oldSurfaceColor;
		m_oldSurfaceColor = NULL;
	}
}

BEGIN_EVENT_TABLE(ptRingCutPanelWidget, wxPanel)
    // begin wxGlade: ptRingCutPanelWidget::event_table
    EVT_BUTTON(wxID_btnRingCut, ptRingCutPanelWidget::OnButtonRingCut)
	EVT_BUTTON(wxID_btnChooseSurface, ptRingCutPanelWidget::OnButtonChooseSurfaces)
	EVT_TOGGLEBUTTON(wxID_btnEnable, ptRingCutPanelWidget::OnButtonEnableInteraction)
    // end wxGlade
END_EVENT_TABLE();

void ptRingCutPanelWidget::OnButtonRingCut(wxCommandEvent &event)
{
	UpdateProcessor( );
	btnRingCut->Disable();
	btnChooseSurface->Enable();
	m_interactionEnabled = false;
	DisconnectInteractor();
	UpdateWidget();
	Core::DataContainer::Pointer dataContainer = Core::Runtime::Kernel::GetDataContainer();
	Core::DataEntityList::Pointer list = dataContainer->GetDataEntityList();
	list->Remove( m_processor->GetInputDataEntity(1 ) );
	list->Remove( m_processor->GetOutputDataEntity(2 ) );


	mitk::DataTreeNode::Pointer nodeComponent1;
	boost::any anyData = GetRenderingTree()->GetNode(m_processor->GetOutputDataEntityHolder(0)->GetSubject());
	Core::CastAnyProcessingData( anyData, nodeComponent1 );
	if(m_oldSurfaceColor!=NULL)
	{
		delete m_oldSurfaceColor;
		m_oldSurfaceColor = NULL;
	}

	m_oldSurfaceColor = ChangeNodeColor (nodeComponent1, 0);
	mitk::DataTreeNode::Pointer nodeComponent2;
	anyData = GetRenderingTree()->GetNode(m_processor->GetOutputDataEntityHolder(1)->GetSubject());
	Core::CastAnyProcessingData( anyData, nodeComponent2 );
	ChangeNodeColor (nodeComponent2, 1);
}

void ptRingCutPanelWidget::do_layout()
{
    // begin wxGlade: ptRingCutPanelWidget::do_layout
    wxBoxSizer* GlobalSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* sizer_1 = new wxBoxSizer(wxHORIZONTAL);
	sizer_1->Add(toggleBtnEnable, 1, wxALIGN_LEFT, 0);
	sizer_1->Add(btnRingCut, 0, wxALL|wxALIGN_RIGHT, 0);
	sizer_1->Add(btnChooseSurface, 0, wxALL|wxALIGN_RIGHT, 0);
	sizer_1->Add(cmbSelectColor, 0, wxALL|wxALIGN_RIGHT, 0);
    GlobalSizer->Add(sizer_1, 0, wxALL|wxALIGN_RIGHT, 0);
	SetSizer(GlobalSizer);
    GlobalSizer->Fit(this);
    // end wxGlade
}


bool ptRingCutPanelWidget::Enable( bool enable /*= true */ )
{
	bool bReturn = wxPanel::Enable( enable );

	try
	{
		const std::string helpStr = \
			"Ring Cut tool: cut a surface using a ring." \
			"\n\nUsage: SHIFT + left button click to select a point where" \
			"\nthe ring shoud be.";
		SetInfoUserHelperWidget( helpStr );

		if ( CheckInputs()&&
			m_interactionEnabled)
		{
			ConnectInteractor();
		}
		else
		{
			DisconnectInteractor();
			Core::DataContainer::Pointer dataContainer = Core::Runtime::Kernel::GetDataContainer();
			Core::DataEntityList::Pointer list = dataContainer->GetDataEntityList();
			if (m_processor->GetInputDataEntity(1 ).IsNotNull())
				list->Remove( m_processor->GetInputDataEntity(1 ) );
			if (m_processor->GetOutputDataEntity(2 ).IsNotNull())
				list->Remove( m_processor->GetOutputDataEntity(2 ) );
		}
	}
	coreCatchExceptionsReportAndNoThrowMacro("ptRingCutPanelWidget::Enable");

	return bReturn;
}

void ptRingCutPanelWidget::OnSelectedPoint()
{
	Core::Runtime::Kernel::GetApplicationRuntime( )->SetAppState( 
		Core::Runtime::APP_STATE_PROCESSING );

	try
	{
		if ( CheckInputs()&&
			m_interactionEnabled )
		{
			m_processor->ComputePlaneUsingRingCut();	
		}
	}
	coreCatchExceptionsReportAndNoThrowMacro(
		ptRingCutPanelWidget::OnSelectedPoint)

	Core::Runtime::Kernel::GetApplicationRuntime( )->SetAppState( 
		Core::Runtime::APP_STATE_IDLE );
}

Core::BaseProcessor::Pointer ptRingCutPanelWidget::GetProcessor()
{
	return m_processor.GetPointer();
}

void ptRingCutPanelWidget::OnSelectedInput()
{
	try
	{
		//if(m_processor->GetInputDataEntity(0).IsNull()==NULL)
		//{
		//	DoCancel();
		//	UpdateWidget();
		//	return;
		//}

		//if ( CheckInputs() &&
		//	m_interactionEnabled )
		//{
		//	ConnectInteractor();			
		//}
	}
	coreCatchExceptionsLogAndNoThrowMacro(
		ptRingCutPanelWidget::OnSelectedInput)
}

bool ptRingCutPanelWidget::CheckInputs()
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

void ptRingCutPanelWidget::ConnectInteractor()
{
	Core::Widgets::LandmarkSelectorWidget* widget;
	widget = GetSelectionToolWidget< Core::Widgets::LandmarkSelectorWidget>( "Landmark selector" );
	if((widget->GetInputDataEntity())!= (m_processor->GetInputDataEntity(0)))
	{
		this->DisconnectInteractor();	
		Core::DataContainer::Pointer dataContainer = Core::Runtime::Kernel::GetDataContainer();
		Core::DataEntityList::Pointer list = dataContainer->GetDataEntityList();
		Core::DataEntityHolder::Pointer holder;
		holder = list->GetSelectedDataEntityHolder();
		holder->SetSubject( m_processor->GetInputDataEntity(0), true );
		//GetLandmarkSelectorWidget()->SetInputDataEntity(m_processor->GetInputDataEntity(0));
	}

	// Connect interactor
	widget->SetAllowedInputDataTypes( Core::SurfaceMeshTypeId );

	widget->SetInteractorType(Core::PointInteractor::POINT_SELECT);
	widget->SetLandmarksName( "RingCut point" );
	widget->StartInteractor();

	Core::PointInteractorPointSelect* pointSelectInteractor;
	pointSelectInteractor = static_cast<Core::PointInteractorPointSelect*> (
		widget->GetPointInteractor( ).GetPointer( ));

	m_processor->SetInputDataEntity( 1, 
		pointSelectInteractor->GetSelectedPointsDataEntity() );

	Core::DataTreeMITKHelper::ChangeShowLabelsProperty(  
		pointSelectInteractor->GetSelectedPointsDataEntity(),
		GetRenderingTree( ),  
		false );

	// Disable
	blMITKUtils::SetScalarMode( pointSelectInteractor->GetSelectedDataEntityNode(), "", 2 );
}

void ptRingCutPanelWidget::DisconnectInteractor()
{
	Core::Widgets::LandmarkSelectorWidget* widget;
	widget = GetSelectionToolWidget< Core::Widgets::LandmarkSelectorWidget>( "Landmark selector" );
	if ( widget == NULL )
	{
		return;
	}

	// Disconnect interactor
	widget->StopInteraction();

	widget->SetDefaultAllowedInputDataTypes( );

}

void ptRingCutPanelWidget::OnInit()
{
	GetProcessorOutputObserver(2)->SetHideInput(false);
	//GetProcessorOutputObserver(2)->AddProperty("line width", 
	//	new mitk::FloatProperty(1));
	GetProcessorOutputObserver(1)->SelectDataEntity(false);
	GetProcessorOutputObserver(0)->SelectDataEntity(false);
	GetProcessorOutputObserver(2)->SelectDataEntity(false);

}

void ptRingCutPanelWidget::DoEnableNewCut()
{
	ConnectInteractor(  );
	btnRingCut->Enable();
	m_interactionEnabled = true;
}


void ptRingCutPanelWidget::DoCancel()
{
	DisconnectInteractor( );
	m_interactionEnabled = false;
	Core::DataContainer::Pointer dataContainer = Core::Runtime::Kernel::GetDataContainer();
	Core::DataEntityList::Pointer list = dataContainer->GetDataEntityList();
	if (m_processor->GetInputDataEntity(1 ).IsNotNull())
		list->Remove( m_processor->GetInputDataEntity(1 ) );
	if (m_processor->GetOutputDataEntity(2 ).IsNotNull())
		list->Remove( m_processor->GetOutputDataEntity(2 ) );
	btnRingCut->Disable();
}

void ptRingCutPanelWidget::OnButtonEnableInteraction( wxCommandEvent &event )
{
	if (m_processor->GetInputDataEntity(0).IsNull())
		return;

	if ( toggleBtnEnable->GetValue() )
	{
		DoEnableNewCut();
	}
	else
	{
		DoCancel();	
	}
	UpdateWidget();
}

void ptRingCutPanelWidget::UpdateWidget()
{
	toggleBtnEnable->SetValue( m_interactionEnabled );
	std::string label = m_interactionEnabled ? "Cancel " : "New Cut ";
	toggleBtnEnable->SetLabel(_U(label));
}

void ptRingCutPanelWidget::OnButtonChooseSurfaces( wxCommandEvent &event )
{
	btnChooseSurface->Disable();
	if (m_processor->GetOutputDataEntityHolder(0).IsNull())
		return;

	if (m_processor->GetOutputDataEntityHolder(1).IsNull())
		return;
	

	mitk::DataTreeNode::Pointer nodeComponent1;
	boost::any anyData = GetRenderingTree()->GetNode( m_processor->GetOutputDataEntity(0) );
	Core::CastAnyProcessingData( anyData, nodeComponent1 );
	ChangeNodeColor (nodeComponent1, 2);

	mitk::DataTreeNode::Pointer nodeComponent2;
	anyData = GetRenderingTree()->GetNode( m_processor->GetOutputDataEntity(1) );
	Core::CastAnyProcessingData( anyData, nodeComponent2 );
	ChangeNodeColor (nodeComponent2, 2);
	
	if (cmbSelectColor->GetSelection() == 2)
		return;

	int whichSurfaceToDelete = 1 - cmbSelectColor->GetSelection();
	Core::DataContainer::Pointer dataContainer = Core::Runtime::Kernel::GetDataContainer();
	Core::DataEntityList::Pointer list = dataContainer->GetDataEntityList();
	list->Remove( m_processor->GetOutputDataEntity(whichSurfaceToDelete));

	Core::DataEntityHolder::Pointer holder;
	holder = list->GetSelectedDataEntityHolder();
	holder->SetSubject( m_processor->GetOutputDataEntity(1-whichSurfaceToDelete), true );
}



double *ptRingCutPanelWidget::ChangeNodeColor (mitk::DataTreeNode::Pointer node, int colorIndex)
{
	double orange [3] = {0.93, 0.63, 0.45} ;
	double cyan [3] = {0.3, 0.53, 0.85};
	double *oldColor = new double [3];
	if(node.IsNotNull())
	{
		double *currentColor = NULL;
		switch (colorIndex)
		{
			case 0: 
				currentColor = orange;
				break;
			case 1:
				currentColor = cyan;
				break;

			default:
				if(m_oldSurfaceColor!=NULL)
					currentColor = m_oldSurfaceColor;
				break;
		}		
			
		if(currentColor!=NULL)
		{

			mitk::MaterialProperty::Pointer oldMaterialProperty = static_cast<mitk::MaterialProperty *>(node->GetProperty("material"));
			if(oldMaterialProperty.IsNotNull())
			{
				oldColor [0] = oldMaterialProperty->GetDiffuseColor()[0];
				oldColor [1] = oldMaterialProperty->GetDiffuseColor()[1];
				oldColor [2] = oldMaterialProperty->GetDiffuseColor()[2];

			}

			mitk::MaterialProperty::Pointer materialProperty = mitk::MaterialProperty::New( currentColor[0], currentColor[1], currentColor[2], 1.0, node );
			materialProperty->SetDiffuseColor(currentColor[0], currentColor[1], currentColor[2]);
			node->ReplaceProperty("material", materialProperty);

			// Disable
			blMITKUtils::SetScalarMode( node, "", 2 );

			GetMultiRenderWindow()->RequestUpdateAll();
		}
	}
	return oldColor;

}
