// Copyright 2009 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "ptVolumeClosingPanelWidget.h"
#include "coreProcessorInputWidget.h"
#include "mitkMaterialProperty.h"
#include "corePointInteractorPointSelect.h"
#include "mitkProperties.h"
#include "coreDataTreeMITKHelper.h"

ptVolumeClosingPanelWidget::ptVolumeClosingPanelWidget(
	wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style):
    wxPanel(parent, id, pos, size, wxTAB_TRAVERSAL)
{
    // begin wxGlade: ptVolumeClosingPanelWidget::ptVolumeClosingPanelWidget
	// begin wxGlade: VolumeClosingPanelWidget::VolumeClosingPanelWidget
	m_GlobalVolumeClosing_staticbox = new wxStaticBox(this, -1, wxT("Global Volume Closing"));
	sizer_2_staticbox = new wxStaticBox(this, -1, wxEmptyString);
	checkbox_closeAll = new wxCheckBox(this, wxID_CHKCLOSEALL, wxT("Close all"));
	btnChoosehole = new wxToggleButton(this, wxID_CHOOSEHOLE, wxT("Choose Hole"));
	checkboxPatchOnly = new wxCheckBox(this, wxID_ANY, wxT(" Patch output only"));
	labelmethod = new wxStaticText(this, wxID_ANY, wxT("Method"));
	const wxString combo_box_1_choices[] = {
		wxT("SingleCenterPoint"),
		wxT("RadialTriangles"),
		wxT("LinearToCenterNR"),
		wxT("LinearToCenter"),
		wxT("SmoothToCenter"),
		wxT("SmoothPatch"),
		wxT("CloseRefinerFairer")
	};
	combo_box_1 = new wxComboBox(this, wxID_COMBOMETHODS, wxT(""), wxDefaultPosition, wxDefaultSize, 7, combo_box_1_choices, wxCB_DROPDOWN);
	m_labelGlobalVolumeClosingDensityFactor = new wxStaticText(this, wxID_ANY, wxT("Density factor"));
	m_textGlobalVolumeClosingDensityFactor = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
	m_labelGlobalVolumeClosingAngle = new wxStaticText(this, wxID_ANY, wxT("Angle"));
	m_textGlobalVolumeClosingAngle = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
	m_labelGlobalVolumeClosingMinimumEdgesSwapped = new wxStaticText(this, wxID_ANY, wxT("Minimum edges swapped"));
	m_textGlobalVolumeClosingMinimumEdgesSwapped = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
	m_labelGlobalVolumeClosingOrder = new wxStaticText(this, wxID_ANY, wxT("Order"));
	m_textGlobalVolumeClosingOrder = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
	btnGlobalVolumeClosing = new wxButton(this, wxID_btnGlobalVolumeClosing, wxT("Close Volume"));



  /* m_processor = VolumeClosingProcessor::New();
   m_processor->SetName( "VolumeClosingProcessor" );
   m_processor->GetOutputPort( 0 )->SetDataEntityName( "Volume Closed" );*/
	m_processor = Core::CloseHolesProcessor::New();

   m_processor->GetInputDataEntityHolder(0)->AddObserver( 
	   this, 
	   &ptVolumeClosingPanelWidget::OnModifiedInputDataEntity );

   m_processor->GetInputDataEntityHolder(2)->AddObserver( 
	   this, 
	   &ptVolumeClosingPanelWidget::OnModifiedSelection );

   m_processor->GetOutputDataEntityHolder(0)->AddObserver( 
	   this, 
	   &ptVolumeClosingPanelWidget::OnModifiedOutputDataEntity );

   do_layout();
   set_properties();
   UpdateWidget();
}


BEGIN_EVENT_TABLE(ptVolumeClosingPanelWidget, wxPanel)
    // begin wxGlade: ptVolumeClosingPanelWidget::event_table
	EVT_TOGGLEBUTTON(wxID_CHOOSEHOLE, ptVolumeClosingPanelWidget::OnChooseHoles)
    EVT_BUTTON(wxID_btnGlobalVolumeClosing, ptVolumeClosingPanelWidget::OnButtonGlobalVolumeClosing)
	EVT_COMBOBOX(wxID_COMBOMETHODS,ptVolumeClosingPanelWidget::OnComboboxChoice)
	EVT_CHECKBOX(wxID_CHKCLOSEALL,ptVolumeClosingPanelWidget::OnComboboxChoice)
    // end wxGlade
END_EVENT_TABLE();


void ptVolumeClosingPanelWidget::UpdateData()
{
	meMeshVolumeClosingParamsPtr meshParamsVolumeClosing =
		meMeshVolumeClosingParamsPtr( new meMeshVolumeClosingParams );
	
	meshParamsVolumeClosing->m_DensityFactor = std::atof(
				m_textGlobalVolumeClosingDensityFactor->GetValue());
	meshParamsVolumeClosing->m_Angle =std::atof(
				m_textGlobalVolumeClosingAngle->GetValue());
	meshParamsVolumeClosing->m_MinimumEdgesSwapped = std::atof(
				m_textGlobalVolumeClosingMinimumEdgesSwapped->GetValue( ));
	meshParamsVolumeClosing->m_Order = std::atof(
				m_textGlobalVolumeClosingOrder->GetValue());

	//m_processor->GetFilter( )->SetParams( meshParamsVolumeClosing);
	m_processor->SetParams(meshParamsVolumeClosing);
	m_processor->SetCloseAll(checkbox_closeAll->GetValue());
	m_processor->SetMethod(combo_box_1->GetSelection());
	m_processor->SetPatchOnly(checkboxPatchOnly->GetValue());
	
}

void ptVolumeClosingPanelWidget::UpdateWidget()
{

	btnChoosehole->Enable(!checkbox_closeAll->GetValue());
	sizer_2->Show(combo_box_1->GetSelection() == 6 );

	if (checkbox_closeAll->GetValue())
		btnGlobalVolumeClosing->SetLabel("Close Volume");
	else
		btnGlobalVolumeClosing->SetLabel("Close Hole");

	meMeshVolumeClosingParamsPtr meshParamsVolumeClosing =
				m_processor->GetParams( );
//		m_processor->GetFilter( )->GetParams( );

	// Global Volume Closing
	m_textGlobalVolumeClosingDensityFactor->SetValue(wxString::Format("%f", 
					meshParamsVolumeClosing->m_DensityFactor));
	m_textGlobalVolumeClosingAngle->SetValue(wxString::Format("%f", 
					meshParamsVolumeClosing->m_Angle ));
	m_textGlobalVolumeClosingMinimumEdgesSwapped->SetValue(wxString::Format("%f",  
					meshParamsVolumeClosing->m_MinimumEdgesSwapped ));
	m_textGlobalVolumeClosingOrder->SetValue( wxString::Format("%f", 
					meshParamsVolumeClosing->m_Order));

	//Fit();
	Layout();

} 

void ptVolumeClosingPanelWidget::OnButtonGlobalVolumeClosing(wxCommandEvent &event)
{
	UpdateData();
	UpdateProcessor( );

	if (btnChoosehole->GetValue())
	{
		btnChoosehole->SetValue(false);
		DisconnectInteractor( );
		Core::DataContainer::Pointer dataContainer = Core::Runtime::Kernel::GetDataContainer();
		Core::DataEntityList::Pointer list = dataContainer->GetDataEntityList();
		if (m_processor->GetInputDataEntity(2 ).IsNotNull())
			list->Remove( m_processor->GetInputDataEntity(2 ) );
		btnChoosehole->SetLabel("Choose Hole");
	}
}

void ptVolumeClosingPanelWidget::do_layout()
{
    // begin wxGlade: ptVolumeClosingPanelWidget::do_layout
	wxBoxSizer* GlobalSizer = new wxBoxSizer(wxVERTICAL);
	wxStaticBoxSizer* m_GlobalVolumeClosing = new wxStaticBoxSizer(m_GlobalVolumeClosing_staticbox, wxVERTICAL);
	sizer_2 = new wxStaticBoxSizer(sizer_2_staticbox, wxVERTICAL);
	wxBoxSizer* sizer_5_copy_2_copy_copy_copy = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* sizer_5_copy_2_copy_copy = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* sizer_5_copy_2_copy = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* sizer_5_copy_2 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* sizer_1 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* sizer_3 = new wxBoxSizer(wxHORIZONTAL);
	sizer_3->Add(checkbox_closeAll, 1, wxALL|wxEXPAND, 0);
	sizer_3->Add(btnChoosehole, 1, wxALIGN_RIGHT, 0);
	m_GlobalVolumeClosing->Add(sizer_3, 0, wxEXPAND, 0);
	m_GlobalVolumeClosing->Add(checkboxPatchOnly, 0, 0, 0);
	m_GlobalVolumeClosing->Add(20, 20, 0, wxEXPAND, 0);
	sizer_1->Add(labelmethod, 1, 0, 0);
	sizer_1->Add(combo_box_1, 1, 0, 0);
	m_GlobalVolumeClosing->Add(sizer_1, 0, wxEXPAND, 0);
	sizer_5_copy_2->Add(m_labelGlobalVolumeClosingDensityFactor, 1, 0, 0);
	sizer_5_copy_2->Add(m_textGlobalVolumeClosingDensityFactor, 0, 0, 0);
	sizer_2->Add(sizer_5_copy_2, 0, wxALL|wxEXPAND, 2);
	sizer_5_copy_2_copy->Add(m_labelGlobalVolumeClosingAngle, 1, 0, 0);
	sizer_5_copy_2_copy->Add(m_textGlobalVolumeClosingAngle, 0, 0, 0);
	sizer_2->Add(sizer_5_copy_2_copy, 0, wxALL|wxEXPAND, 2);
	sizer_5_copy_2_copy_copy->Add(m_labelGlobalVolumeClosingMinimumEdgesSwapped, 1, 0, 0);
	sizer_5_copy_2_copy_copy->Add(m_textGlobalVolumeClosingMinimumEdgesSwapped, 0, 0, 0);
	sizer_2->Add(sizer_5_copy_2_copy_copy, 0, wxALL|wxEXPAND, 2);
	sizer_5_copy_2_copy_copy_copy->Add(m_labelGlobalVolumeClosingOrder, 1, 0, 0);
	sizer_5_copy_2_copy_copy_copy->Add(m_textGlobalVolumeClosingOrder, 0, 0, 0);
	sizer_2->Add(sizer_5_copy_2_copy_copy_copy, 0, wxALL|wxEXPAND, 2);
	m_GlobalVolumeClosing->Add(sizer_2, 1, wxEXPAND, 0);
	m_GlobalVolumeClosing->Add(btnGlobalVolumeClosing, 0, wxALL|wxALIGN_RIGHT, 2);
	GlobalSizer->Add(m_GlobalVolumeClosing, 0, wxEXPAND, 0);
	SetSizer(GlobalSizer);
	GlobalSizer->Fit(this);


    // end wxGlade
}
void ptVolumeClosingPanelWidget::set_properties()
{
	// begin wxGlade: VolumeClosingPanelWidget::set_properties
	checkbox_closeAll->SetValue(1);
	combo_box_1->SetSelection(4);
	// end wxGlade
}

Core::BaseProcessor::Pointer ptVolumeClosingPanelWidget::GetProcessor()
{
	return m_processor.GetPointer();
}

bool ptVolumeClosingPanelWidget::Enable( bool enable /*= true */ )
{
	bool bReturn = wxPanel::Enable( enable );

	try
	{
		const std::string helpStr = \
			"Volume Closing: global volume closing." \
			"\n\nUsage: select a density factor, angle, minimum edges" \
			"\nand order and then run the tool.";
		SetInfoUserHelperWidget( helpStr );
	}
	coreCatchExceptionsReportAndNoThrowMacro("ptVolumeClosingPanelWidget::Enable");

	return bReturn;
}

void ptVolumeClosingPanelWidget::OnChooseHoles(wxCommandEvent &event)
{
	if (m_processor->GetInputDataEntity(0).IsNull() &&
		m_processor->GetOutputDataEntity(0).IsNull())
	{
		btnChoosehole->SetValue(false); 
		return;
	}

	if ( btnChoosehole->GetValue() )
	{
		m_processor->SetInputDataEntity(1,m_processor->GetOutputDataEntity(0));
		ConnectInteractor(  );
		btnChoosehole->SetLabel("Stop Choosing");

	}
	else
	{
		DisconnectInteractor( );
		Core::DataContainer::Pointer dataContainer = Core::Runtime::Kernel::GetDataContainer();
		Core::DataEntityList::Pointer list = dataContainer->GetDataEntityList();
		if (m_processor->GetInputDataEntity(2 ).IsNotNull())
			list->Remove( m_processor->GetInputDataEntity(2 ) );
		btnChoosehole->SetLabel("Choose Hole");
	}
	UpdateWidget();
}

void ptVolumeClosingPanelWidget::OnComboboxChoice( wxCommandEvent &event )
{
	UpdateWidget();
}

void ptVolumeClosingPanelWidget::OnModifiedInputDataEntity()
{
	if (m_processor->GetInputDataEntity(0).IsNotNull())
		m_processor->ComputeHoles();
}

void ptVolumeClosingPanelWidget::OnInit()
{
	GetInputWidget(0)->SetAutomaticSelection(false);
	GetInputWidget(0)->SetDefaultDataEntityFlag(false);
	GetInputWidget(1)->Hide();
	GetInputWidget(1)->SetAutomaticSelection(false);
	GetInputWidget(1)->SetDefaultDataEntityFlag(false);
	GetInputWidget(2)->Hide();
	GetInputWidget(2)->SetAutomaticSelection(false);
	GetInputWidget(2)->SetDefaultDataEntityFlag(false);

	GetProcessorOutputObserver(0)->SetHideInput(false);
	/*GetProcessorOutputObserver(0)->SelectDataEntity(false);
	mitk::FloatProperty::Pointer prop = mitk::FloatProperty::New();
	prop->SetValue(2);
	GetProcessorOutputObserver(0)->AddProperty("line width", prop);*/
	GetProcessorOutputObserver(1)->SetHideInput(false);
	GetProcessorOutputObserver(2)->SetHideInput(false);
	GetProcessorOutputObserver(2)->SelectDataEntity(false);
}

void ptVolumeClosingPanelWidget::OnModifiedOutputDataEntity()
{
	//if (GetRenderingTree()->GetNode(
	//	m_processor->GetOutputDataEntity( 0 )).IsNotNull())
	//{
	//	//thickening of wireframe...
	//	mitk::DataTreeNode::Pointer node = GetRenderingTree()->GetNode(
	//		m_processor->GetOutputDataEntity( 0 ));
	//	mitk::MaterialProperty* material =dynamic_cast<mitk::MaterialProperty* >(
	//		node->GetProperty("material"));
	//	material->SetRepresentation(mitk::MaterialProperty::Wireframe);
	//	material->SetLineWidth(3.0);
	//	node->ReplaceProperty("material", material);

	//	mitk::RenderingManager::GetInstance()->ForceImmediateUpdateAll();
	//}
}

void ptVolumeClosingPanelWidget::ConnectInteractor()
{
	Core::Widgets::LandmarkSelectorWidget* widget;
	widget = GetSelectionToolWidget<Core::Widgets::LandmarkSelectorWidget>( "Landmark selector" );
	if((widget->GetInputDataEntity())!= (m_processor->GetOutputDataEntity(0)))
	{
		this->DisconnectInteractor();	
		//Core::DataContainer::Pointer dataContainer = Core::Runtime::Kernel::GetDataContainer();
		//Core::DataEntityList::Pointer list = dataContainer->GetDataEntityList();
		//Core::DataEntityHolder::Pointer holder;
		//holder = list->GetSelectedDataEntityHolder();
		//holder->SetSubject( m_processor->GetOutputDataEntity(0), true );
		widget->SetInputDataEntity(m_processor->GetOutputDataEntity(0));
	}

	// Connect interactor
	widget->SetAllowedInputDataTypes( 
		Core::SkeletonTypeId );

	widget->SetInteractorType(Core::PointInteractor::POINT_SELECT);
	widget->SetLandmarksName( "ChoosingHole" );
	widget->StartInteractor();

	Core::PointInteractorPointSelect* pointSelectInteractor;
	pointSelectInteractor = static_cast<Core::PointInteractorPointSelect*> (
		widget->GetPointInteractor( ).GetPointer( ));

	if ( pointSelectInteractor )
	{
		Core::DataTreeMITKHelper::ChangeShowLabelsProperty(  
			pointSelectInteractor->GetSelectedPointsDataEntity(),
			GetRenderingTree( ),  
			false );

		m_processor->SetInputDataEntity( 2, 
			pointSelectInteractor->GetSelectedPointsDataEntity() );
	}

	GetMultiRenderWindow()->RequestUpdateAll();

}

void ptVolumeClosingPanelWidget::DisconnectInteractor()
{
	Core::Widgets::LandmarkSelectorWidget* widget;
	widget = GetSelectionToolWidget<Core::Widgets::LandmarkSelectorWidget>( "Landmark selector" );
	if ( widget == NULL )
	{
		return;
	}

	// Disconnect interactor
	widget->StopInteraction();

	widget->SetDefaultAllowedInputDataTypes( );

}

void ptVolumeClosingPanelWidget::OnModifiedSelection()
{
	if (m_processor->GetInputDataEntity(2).IsNotNull())
	{
		m_processor->SelectHole();
		m_processor->SetInputDataEntity(1,
			m_processor->GetOutputDataEntity(2));

		mitk::DataTreeNode::Pointer node;
		boost::any anyData = GetRenderingTree()->GetNode(m_processor->GetOutputDataEntity(0));
		Core::CastAnyProcessingData( anyData, node );
		if ( node.IsNotNull()&&
			m_processor->GetOutputDataEntity(2).IsNotNull())
		{
			GetRenderingTree()->Show(m_processor->GetOutputDataEntity(0),false);
		}

	}
}