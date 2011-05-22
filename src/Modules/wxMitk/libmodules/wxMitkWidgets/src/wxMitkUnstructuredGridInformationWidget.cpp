/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#include "wxMitkUnstructuredGridInformationWidget.h"

#include "vtkMath.h"

using namespace mitk;


wxMitkUnstructuredGridInformationWidget::wxMitkUnstructuredGridInformationWidget(wxWindow* parent, 
																	   int id, 
																	   const wxPoint& pos, 
																	   const wxSize& size, 
																	   long style):
    wxPanel(parent, id, pos, size, wxTAB_TRAVERSAL)
{
  
	 // begin wxGlade: wxMitkUnstructuredGridInformationWidget::wxMitkUnstructuredGridInformationWidget
    label_1 = new wxStaticText(this, wxID_ANY, wxT("Number of Vertices"));
    m_vertices = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE|wxNO_BORDER);
   // label_2 = new wxStaticText(this, wxID_ANY, wxT("Number of Triangles"));
   // m_triangles = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE|wxNO_BORDER);
	label_7 = new wxStaticText(this, wxID_ANY, wxT("Number of Cells"));
    m_cells = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE|wxNO_BORDER);
    label_3 = new wxStaticText(this, wxID_ANY, wxT("Bounds"));
    label_3_copy_1 = new wxStaticText(this, wxID_ANY, wxT("Min - Max"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
    label_3_copy_2 = new wxStaticText(this, wxID_ANY, wxT("Range"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
    label_4 = new wxStaticText(this, wxID_ANY, wxT("X axis"));
    m_Xminmax = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE|wxNO_BORDER);
    m_Xrange = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE|wxNO_BORDER);
    label_5 = new wxStaticText(this, wxID_ANY, wxT("Y axis"));
    m_Yminmax = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE|wxNO_BORDER);
    m_Yrange = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE|wxNO_BORDER);
    label_6 = new wxStaticText(this, wxID_ANY, wxT("Z axis"));
    m_Zminmax = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE|wxNO_BORDER);
    m_Zrange = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE|wxNO_BORDER);

    set_properties();
    do_layout();
    // end wxGlade
  
}

void wxMitkUnstructuredGridInformationWidget::UpdateData(vtkSmartPointer<vtkUnstructuredGrid> UnstructuredGridInput)
{

if (UnstructuredGridInput == NULL || UnstructuredGridInput->GetNumberOfPoints( ) == 0 )
	{
		this->ResetWidget();
		return;
	}
	
//	m_triangles->SetValue(wxString::Format("%d", UnstructuredGridInput->GetNumberOfTriangles()));
	m_vertices->SetValue(wxString::Format(wxT("%d"), UnstructuredGridInput->GetNumberOfPoints()));
	m_cells->SetValue(wxString::Format(wxT("%d"), UnstructuredGridInput->GetNumberOfCells()));
//	UnstructuredGridInput->GetActualMemorySize();
//  UnstructuredGridInput->GetActiveFieldInformation();
	double *bounds = UnstructuredGridInput->GetBounds();
	double range = 0;
	wxString minmax;
	minmax = wxString::Format(wxT("%.3f"),bounds[0])+ wxT(" - ") + wxString::Format(wxT("%.3f"),bounds[1]);
	range = abs( bounds[1] - bounds[0]) ;
	m_Xminmax->SetValue( minmax );
	m_Xrange->SetValue( wxString::Format(wxT("%.3f"),range));
	minmax = wxString::Format(wxT("%.3f"),bounds[2])+ wxT(" - ") + wxString::Format(wxT("%.3f"),bounds[3]);
	range = abs( bounds[3] - bounds[2]) ;
	m_Yminmax->SetValue( minmax );
	m_Yrange->SetValue( wxString::Format(wxT("%.3f"),range));
	minmax = wxString::Format(wxT("%.3f"),bounds[4])+ wxT(" - ") + wxString::Format(wxT("%.3f"),bounds[5]);
	range = abs( bounds[5] - bounds[4]) ;
	m_Zminmax->SetValue( minmax );
	m_Zrange->SetValue( wxString::Format(wxT("%.3f"),range));
}

void wxMitkUnstructuredGridInformationWidget::ResetWidget()
{		
	//m_triangles->SetValue("");
	m_vertices->SetValue(wxT(""));
	m_cells->SetValue(wxT(""));
	m_Xminmax->SetValue(wxT(""));
	m_Xrange->SetValue(wxT(""));
	m_Yminmax->SetValue(wxT(""));
	m_Yrange->SetValue(wxT(""));
	m_Zminmax->SetValue(wxT(""));
	m_Zrange->SetValue(wxT(""));
}


void wxMitkUnstructuredGridInformationWidget::set_properties()
{
   
}


void wxMitkUnstructuredGridInformationWidget::do_layout()
{
  
    // begin wxGlade: wxMitkUnstructuredGridInformationWidget::do_layout
    wxBoxSizer* sizer_1 = new wxBoxSizer(wxVERTICAL);
    wxGridSizer* grid_sizer_1 = new wxGridSizer(4, 3, 0, 0);
    //wxBoxSizer* sizer_3 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_2 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* sizer_4 = new wxBoxSizer(wxHORIZONTAL);
    sizer_2->Add(label_1, 1, wxEXPAND, 0);
    sizer_2->Add(m_vertices, 1, wxALL|wxEXPAND, 0);
    sizer_1->Add(sizer_2, 0, wxEXPAND, 0);
    //sizer_3->Add(label_2, 1, wxEXPAND, 0);
    //sizer_3->Add(m_triangles, 1, wxEXPAND, 0);
    //sizer_1->Add(sizer_3, 0, wxEXPAND, 0);
	sizer_4->Add(label_7, 1, wxEXPAND, 0);
    sizer_4->Add(m_cells, 1, wxEXPAND, 0);
    sizer_1->Add(sizer_4, 1, wxEXPAND, 0);
    grid_sizer_1->Add(label_3, 1, wxEXPAND, 0);
    grid_sizer_1->Add(label_3_copy_1, 1, wxEXPAND, 0);
    grid_sizer_1->Add(label_3_copy_2, 1, wxEXPAND, 0);
    grid_sizer_1->Add(label_4, 1, wxEXPAND, 0);
    grid_sizer_1->Add(m_Xminmax, 1, wxEXPAND, 0);
    grid_sizer_1->Add(m_Xrange, 1, wxEXPAND, 0);
    grid_sizer_1->Add(label_5, 1, wxEXPAND, 0);
    grid_sizer_1->Add(m_Yminmax, 1, wxEXPAND, 0);
    grid_sizer_1->Add(m_Yrange, 1, wxEXPAND, 0);
    grid_sizer_1->Add(label_6, 1, wxEXPAND, 0);
    grid_sizer_1->Add(m_Zminmax, 1, wxEXPAND, 0);
    grid_sizer_1->Add(m_Zrange, 1, wxEXPAND, 0);
    sizer_1->Add(grid_sizer_1, 2, wxEXPAND, 0);
    SetSizer(sizer_1);
    sizer_1->Fit(this);
    // end wxGlade
  
}

