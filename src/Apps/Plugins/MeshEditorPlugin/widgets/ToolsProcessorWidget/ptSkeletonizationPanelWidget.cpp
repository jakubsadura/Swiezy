// Copyright 2009 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "ptSkeletonizationPanelWidget.h"

ptSkeletonizationPanelWidget::ptSkeletonizationPanelWidget(
	wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style):
    wxPanel(parent, id, pos, size, wxTAB_TRAVERSAL)
{
    // begin wxGlade: ptSkeletonizationPanelWidget::ptSkeletonizationPanelWidget
    m_GenerateSkeleton_staticbox = new wxStaticBox(this, -1, wxT("Generate Skeleton"));
    m_labelPrecision = new wxStaticText(this, wxID_ANY, wxT("Precision (mm)"));
    m_textPrecision = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    btnSkeletonization = new wxButton(this, wxID_btnSkeletonization, wxT("Skeletonization"));

	m_processor = SkeletonizationProcessor::New();
	m_processor->GetOutputPort( 0 )->SetDataEntityName( "Skeleton" );
	Core::DataEntityType type = Core::DataEntityType( Core::SurfaceMeshTypeId | Core::SkeletonTypeId );
	m_processor->GetOutputPort( 0 )->SetDataEntityType( type );
	m_processor->SetName( "SkeletonizationProcessor" );

    do_layout();
    UpdateWidget();
}


BEGIN_EVENT_TABLE(ptSkeletonizationPanelWidget, wxPanel)
    // begin wxGlade: ptSkeletonizationPanelWidget::event_table
    EVT_BUTTON(wxID_btnSkeletonization, ptSkeletonizationPanelWidget::OnButtonSkeletonization)
    // end wxGlade
END_EVENT_TABLE();


void ptSkeletonizationPanelWidget::OnButtonSkeletonization(wxCommandEvent &event)
{
	UpdateData();
	GetProcessorOutputObserver(0)->SetOpacityInput(true, 0, 0.5);
	UpdateProcessor( );
 }

void ptSkeletonizationPanelWidget::UpdateData()
{
	meMeshSkeletonizationParamsPtr meshSkeletonizationParams = 
		meMeshSkeletonizationParamsPtr( new meMeshSkeletonizationParams );
	
	meshSkeletonizationParams->m_SkeletonizationPrecisionInMm = 
		std::atof(m_textPrecision->GetValue());

	m_processor->GetFilter( )->SetParams(meshSkeletonizationParams);
}

void ptSkeletonizationPanelWidget::UpdateWidget()
{
	meMeshSkeletonizationParamsPtr meshSkeletonizationParams =
		m_processor->GetFilter( )->GetParams();
	
	m_textPrecision->SetValue(wxString::Format("%f", 
		meshSkeletonizationParams->m_SkeletonizationPrecisionInMm) );

}


void ptSkeletonizationPanelWidget::do_layout()
{
    // begin wxGlade: ptSkeletonizationPanelWidget::do_layout
    wxBoxSizer* GlobalSizer = new wxBoxSizer(wxVERTICAL);
    wxStaticBoxSizer* m_GenerateSkeleton = new wxStaticBoxSizer(m_GenerateSkeleton_staticbox, wxVERTICAL);
    wxBoxSizer* sizer_6 = new wxBoxSizer(wxHORIZONTAL);
    sizer_6->Add(m_labelPrecision, 1, wxALIGN_CENTER_VERTICAL, 0);
    sizer_6->Add(m_textPrecision, 0, wxALIGN_CENTER_VERTICAL, 0);
    m_GenerateSkeleton->Add(sizer_6, 0, wxALL|wxEXPAND, 2);
    m_GenerateSkeleton->Add(btnSkeletonization, 0, wxALL|wxALIGN_RIGHT, 2);
    GlobalSizer->Add(m_GenerateSkeleton, 0, wxEXPAND, 0);
    SetSizer(GlobalSizer);
    GlobalSizer->Fit(this);
    // end wxGlade
}


Core::BaseProcessor::Pointer ptSkeletonizationPanelWidget::GetProcessor()
{
	return m_processor.GetPointer();
}

bool ptSkeletonizationPanelWidget::Enable( bool enable /*= true */ )
{
	bool bReturn = wxPanel::Enable( enable );

	try
	{
		const std::string helpStr = \
			"Skeletonization: find the skeleton of a surface." \
			"\n\nUsage: select a precision and run the tool.";
		SetInfoUserHelperWidget( helpStr );
	}
	coreCatchExceptionsReportAndNoThrowMacro("ptSkeletonizationPanelWidget::Enable");

	return bReturn;
}

