// Copyright 2009 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "ptTetraGeneratorPanelWidget.h"


ptTetraGeneratorPanelWidget::ptTetraGeneratorPanelWidget(
	wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style):
    wxPanel(parent, id, pos, size, wxTAB_TRAVERSAL)
{
    // begin wxGlade: ptTetraGeneratorPanelWidget::ptTetraGeneratorPanelWidget
    m_TetraGenerator_staticbox = new wxStaticBox(this, -1, wxT("Tetra Generator"));
	const wxString m_typeRadiobox_choices[] = {
		wxT("Tetgen"),
		wxT("Netgen")
	};
	m_typeRadiobox = new wxRadioBox(this, wxID_RADIOBOX_TETRA, wxT("Tetra Type"), wxDefaultPosition, wxDefaultSize, 2, m_typeRadiobox_choices, 0, wxRA_SPECIFY_COLS);

	m_Tetgen_staticbox = new wxStaticBox(this, -1, wxT("Tetgen Params"));
    m_labelTetraGeneratorMaxVolume = new wxStaticText(this, wxID_ANY, wxT("Max. Volume"));
    m_textTetraGeneratorMaxVolume = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    m_labelTetraGeneratorAspectRatio = new wxStaticText(this, wxID_ANY, wxT("Aspect Ratio"));
    m_textTetraGeneratorAspectRatio = new wxTextCtrl(this, wxID_ANY, wxEmptyString);

    btnTetraGenerator = new wxButton(this, wxID_btnTetraGenerator, wxT("Generate Tetra"));

	m_GlobalOptimization_staticbox = new wxStaticBox(this, -1, wxT("Netgen Params"));
	m_labelMaxMeshSize = new wxStaticText(this, wxID_ANY, wxT("Max Mesh size"));
	m_textMaxMeshSize = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
	m_labelElementsPerCurvatureRadius = new wxStaticText(this, wxID_ANY, wxT("Elements Per Curvature Radius"));
	m_textElementsPerCurvatureRadius = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
	m_labelElementsPerEdge = new wxStaticText(this, wxID_ANY, wxT("Elements Per Edge"));
	m_textElementsPerEdge = new wxTextCtrl(this, wxID_ANY, wxEmptyString);

	m_processor = Core::TetraGenerationProcessor::New();
    do_layout();
	UpdateWidget();
	UpdateLayout();
}


BEGIN_EVENT_TABLE(ptTetraGeneratorPanelWidget, wxPanel)
    // begin wxGlade: ptTetraGeneratorPanelWidget::event_table
	EVT_RADIOBOX(wxID_RADIOBOX_TETRA, ptTetraGeneratorPanelWidget::OnTypeTetra)
    EVT_BUTTON(wxID_btnTetraGenerator, ptTetraGeneratorPanelWidget::OnButtonTetraGenerator)
    // end wxGlade
END_EVENT_TABLE();


void ptTetraGeneratorPanelWidget::OnButtonTetraGenerator(wxCommandEvent &event)
{
	UpdateData();
	try
	{
		UpdateProcessor( );
	}
	coreCatchExceptionsReportAndNoThrowMacro("ptTetraGeneratorPanelWidget::OnButtonTetraGenerator"); 
}




void ptTetraGeneratorPanelWidget::UpdateData()
{
	meMeshTetraGenerationParamsPtr meshTetraGenerationParams =
		meMeshTetraGenerationParamsPtr ( new meMeshTetraGenerationParams );
	meshTetraGenerationParams->m_MaxVolume = std::atof(m_textTetraGeneratorMaxVolume->GetValue());
	meshTetraGenerationParams->m_AspectRatio = std::atof(m_textTetraGeneratorAspectRatio->GetValue());
	
	meMeshParamsOptimizePtr meshNetgenParams =
	meMeshParamsOptimizePtr( new meMeshParamsOptimize );

	meshNetgenParams->m_curvaturesafety =std::atof(m_textElementsPerCurvatureRadius->GetValue( ));
	meshNetgenParams->m_maxh = std::atof(m_textMaxMeshSize->GetValue());
	meshNetgenParams->m_segmentsperedge = std::atof(m_textElementsPerEdge->GetValue());
	
	m_processor->SetParams(meshTetraGenerationParams);
	m_processor->SetNetgenParams(meshNetgenParams);
	m_processor->SetFlag((bool)m_typeRadiobox->GetSelection());
}

void ptTetraGeneratorPanelWidget::UpdateWidget()
{
	meMeshTetraGenerationParamsPtr meshTetraGenerationParams = 
		m_processor->GetParams( );
	m_textTetraGeneratorMaxVolume->SetValue(
		wxString::Format("%f", meshTetraGenerationParams->m_MaxVolume));
	m_textTetraGeneratorAspectRatio->SetValue(
		wxString::Format("%f", meshTetraGenerationParams->m_AspectRatio));


	meMeshParamsOptimizePtr meshNetgenParams =
		m_processor->GetNetgenParams( );

	m_textElementsPerCurvatureRadius->SetValue(wxString::Format("%f", 
		meshNetgenParams->m_curvaturesafety) );
	m_textMaxMeshSize->SetValue(wxString::Format("%f", 
		meshNetgenParams->m_maxh) );
	m_textElementsPerEdge->SetValue(wxString::Format("%f", 
		meshNetgenParams->m_segmentsperedge) );

}


void ptTetraGeneratorPanelWidget::do_layout()
{
    // begin wxGlade: ptTetraGeneratorPanelWidget::do_layout
    wxBoxSizer* GlobalSizer = new wxBoxSizer(wxVERTICAL);
    m_TetraGenerator = new wxStaticBoxSizer(m_TetraGenerator_staticbox, wxVERTICAL);
	m_TetraGenerator->Add(m_typeRadiobox, 0, wxEXPAND, 0);
	
	m_Tetgen = new wxStaticBoxSizer(m_Tetgen_staticbox, wxVERTICAL);
    wxBoxSizer* sizer_6 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_5 = new wxBoxSizer(wxHORIZONTAL);
	sizer_5->Add(m_labelTetraGeneratorMaxVolume, 1, 0, 0);
    sizer_5->Add(m_textTetraGeneratorMaxVolume, 0, 0, 0);
    m_Tetgen ->Add(sizer_5, 0, wxALL|wxEXPAND, 2);
    sizer_6->Add(m_labelTetraGeneratorAspectRatio, 1, 0, 0);
    sizer_6->Add(m_textTetraGeneratorAspectRatio, 0, 0, 0);
    m_Tetgen ->Add(sizer_6, 0, wxALL|wxEXPAND, 2);
   
	m_GlobalOptimization = new wxStaticBoxSizer(m_GlobalOptimization_staticbox, wxVERTICAL);
	wxBoxSizer* sizer_5_copy_1_copy = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* sizer_5_copy_copy = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* sizer_5_copy_3 = new wxBoxSizer(wxHORIZONTAL);
	sizer_5_copy_3->Add(m_labelMaxMeshSize, 1, 0, 0);
	sizer_5_copy_3->Add(m_textMaxMeshSize, 0, 0, 0);
	m_GlobalOptimization->Add(sizer_5_copy_3, 0, wxALL|wxEXPAND, 2);
	sizer_5_copy_copy->Add(m_labelElementsPerCurvatureRadius, 1, 0, 0);
	sizer_5_copy_copy->Add(m_textElementsPerCurvatureRadius, 0, 0, 0);
	m_GlobalOptimization->Add(sizer_5_copy_copy, 0, wxALL|wxEXPAND, 2);
	sizer_5_copy_1_copy->Add(m_labelElementsPerEdge, 1, 0, 0);
	sizer_5_copy_1_copy->Add(m_textElementsPerEdge, 0, 0, 0);
	m_GlobalOptimization->Add(sizer_5_copy_1_copy, 0, wxALL|wxEXPAND, 2);

	m_TetraGenerator->Add(m_Tetgen, 0, wxALL|wxEXPAND, 2);
	m_TetraGenerator->Add(m_GlobalOptimization, 0, wxALL|wxEXPAND, 2);
	m_TetraGenerator->Add(btnTetraGenerator, 0, wxALL|wxALIGN_RIGHT, 2);
	GlobalSizer->Add(m_TetraGenerator, 0, wxEXPAND, 0);
    SetSizer(GlobalSizer);
    GlobalSizer->Fit(this);
    // end wxGlade
}


Core::BaseProcessor::Pointer ptTetraGeneratorPanelWidget::GetProcessor()
{
	return m_processor.GetPointer();
}

bool ptTetraGeneratorPanelWidget::Enable( bool enable /*= true */ )
{
	bool bReturn = wxPanel::Enable( enable );

	try
	{
		const std::string helpStr = \
			"Tetra Generation: generate a tetra mesh from a surface." \
			"\n\nUsage: define the maximum volume and the aspect ration and" \
			"\nthen run the tool.";
		SetInfoUserHelperWidget( helpStr );
	}
	coreCatchExceptionsReportAndNoThrowMacro("ptTetraGeneratorPanelWidget::Enable");

	return bReturn;
}

void ptTetraGeneratorPanelWidget::set_properties()
{
	 m_typeRadiobox->SetSelection(0);
}

void ptTetraGeneratorPanelWidget::OnTypeTetra(wxCommandEvent &event)
{
	UpdateData();
	UpdateLayout();
}

void ptTetraGeneratorPanelWidget::UpdateLayout()
{
		m_GlobalOptimization->Show(m_typeRadiobox->GetSelection() == 1);
		m_Tetgen->Show(m_typeRadiobox->GetSelection() == 0);
		Layout();
}