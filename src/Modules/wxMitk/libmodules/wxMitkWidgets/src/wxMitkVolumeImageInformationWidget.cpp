/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#include "wxMitkVolumeImageInformationWidget.h"

using namespace mitk;


wxMitkVolumeImageInformationWidget::wxMitkVolumeImageInformationWidget(wxWindow* parent, 
																	   int id, 
																	   const wxPoint& pos, 
																	   const wxSize& size, 
																	   long style):
    wxPanel(parent, id, pos, size, wxTAB_TRAVERSAL)
{
  
    sizer_2_staticbox = new wxStaticBox(this, -1, wxT("Static dimensional properties"));
    label_9 = new wxStaticText(this, wxID_ANY, wxT("Axis"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
    label_6 = new wxStaticText(this, wxID_ANY, wxT("Slices"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
    label_7 = new wxStaticText(this, wxID_ANY, wxT("Size (mm)"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
    label_8 = new wxStaticText(this, wxID_ANY, wxT("Origin"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
    label_3 = new wxStaticText(this, wxID_ANY, wxT("X"));
    m_dimension_X = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE|wxNO_BORDER);
    m_size_X = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE|wxNO_BORDER);
    m_origin_X = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE|wxNO_BORDER);
    label_4 = new wxStaticText(this, wxID_ANY, wxT("Y"));
    m_dimension_Y = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE|wxNO_BORDER);
    m_size_Y = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE|wxNO_BORDER);
    m_origin_Y = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE|wxNO_BORDER);
    label_5 = new wxStaticText(this, wxID_ANY, wxT("Z"));
    m_dimension_Z = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE|wxNO_BORDER);
    m_size_Z = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE|wxNO_BORDER);
    m_origin_Z = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE|wxNO_BORDER);
    label_10 = new wxStaticText(this, wxID_ANY, wxT("Data Type"));
    m_txt_dataType = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE|wxNO_BORDER);

    set_properties();
    do_layout();
  
}

void wxMitkVolumeImageInformationWidget::UpdateData(vtkSmartPointer<vtkImageData> imageInput)
{

	if (imageInput == NULL )
	{
		this->ResetWidget();
		return;
	}
	int fdimension = 0;
	double fsize = 0.0;
	double forigin = 0.0;

	wxTextCtrl *origin[3] ={m_origin_X, m_origin_Y, m_origin_Z};
	wxTextCtrl *dimension[3]={m_dimension_X, m_dimension_Y, m_dimension_Z};
	wxTextCtrl *size[3]={m_size_X, m_size_Y, m_size_Z};

	//Bucle x,y,z
	for (int i=0;i<3;i++)
	{
		const int maxNrDecimals = 2;
		//GetDimensions from vtk
		fdimension = imageInput->GetDimensions()[i];	
		dimension[i]->SetValue(wxString::Format(wxT("%d"), fdimension ) );

		//GetSpacing from vtk
		fsize = imageInput->GetSpacing()[i];	
		size[i]->SetValue(wxString::Format(wxT("%.4f"), fsize ) );
		//GetOrigin from vtk
		forigin = imageInput->GetOrigin()[i];
		origin[i]->SetValue(wxString::Format(wxT("%.2f"), forigin ) );
	}

	//GetScalarTypeAsString from vtk
	const char *fType = imageInput->GetScalarTypeAsString();
	//GetScalarSize from vtk
	int fSizeByte = imageInput->GetScalarSize();

	//Add fType and fSizeByte
	char fdataType[25];
	sprintf(fdataType, "%s - %d byte",fType,fSizeByte);
	m_txt_dataType->SetValue(fdataType);

}

void wxMitkVolumeImageInformationWidget::ResetWidget()
{		
				
	wxTextCtrl *origin[3] ={m_origin_X, m_origin_Y, m_origin_Z};
	wxTextCtrl *dimension[3]={m_dimension_X, m_dimension_Y, m_dimension_Z};
	wxTextCtrl *size[3]={m_size_X, m_size_Y, m_size_Z};

		for (int i=0;i<3;i++)
		{
			origin[i]->SetValue(wxString::Format(wxString("%d", wxConvUTF8), 0 ) );
			dimension[i]->SetValue(wxString::Format(wxString("%d", wxConvUTF8), 0 ) );
			size[i]->SetValue(wxString::Format(wxString("%d", wxConvUTF8), 0 ) );
		}
		m_txt_dataType->SetValue(wxT(""));
}


void wxMitkVolumeImageInformationWidget::set_properties()
{
    
    m_dimension_X->SetMinSize(wxSize(50, 17));
    m_size_X->SetMinSize(wxSize(50, 17));
    m_origin_X->SetMinSize(wxSize(50, 17));
    m_dimension_Y->SetMinSize(wxSize(50, 17));
    m_size_Y->SetMinSize(wxSize(50, 17));
    m_origin_Y->SetMinSize(wxSize(50, 17));
    m_dimension_Z->SetMinSize(wxSize(50, 17));
    m_size_Z->SetMinSize(wxSize(50, 17));
    m_origin_Z->SetMinSize(wxSize(50, 17));

}


void wxMitkVolumeImageInformationWidget::do_layout()
{
  
    wxBoxSizer* sizer_1 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_3 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_5 = new wxBoxSizer(wxHORIZONTAL);
    wxStaticBoxSizer* sizer_2 = new wxStaticBoxSizer(sizer_2_staticbox, wxHORIZONTAL);
    wxFlexGridSizer* grid_sizer_1 = new wxFlexGridSizer(4, 4, 5, 15);
    grid_sizer_1->Add(label_9, 0, wxEXPAND, 0);
    grid_sizer_1->Add(label_6, 0, wxEXPAND, 0);
    grid_sizer_1->Add(label_7, 0, wxEXPAND, 0);
    grid_sizer_1->Add(label_8, 0, wxEXPAND, 0);
    grid_sizer_1->Add(label_3, 0, wxEXPAND, 0);
    grid_sizer_1->Add(m_dimension_X, 0, wxEXPAND, 0);
    grid_sizer_1->Add(m_size_X, 0, wxEXPAND, 0);
    grid_sizer_1->Add(m_origin_X, 0, wxEXPAND, 0);
    grid_sizer_1->Add(label_4, 0, wxEXPAND, 0);
    grid_sizer_1->Add(m_dimension_Y, 0, wxEXPAND, 0);
    grid_sizer_1->Add(m_size_Y, 0, wxEXPAND, 0);
    grid_sizer_1->Add(m_origin_Y, 0, wxEXPAND, 0);
    grid_sizer_1->Add(label_5, 0, wxEXPAND, 0);
    grid_sizer_1->Add(m_dimension_Z, 0, wxEXPAND, 0);
    grid_sizer_1->Add(m_size_Z, 0, wxEXPAND, 0);
    grid_sizer_1->Add(m_origin_Z, 0, wxEXPAND, 0);
    sizer_2->Add(grid_sizer_1, 1, wxEXPAND, 2);
    sizer_1->Add(sizer_2, 0,wxALL| wxEXPAND, 2);
    sizer_5->Add(label_10, 1, wxALL|wxEXPAND, 2);
    sizer_5->Add(m_txt_dataType, 1, wxEXPAND, 0);
    sizer_3->Add(sizer_5, 0, wxALL|wxEXPAND, 2);
    sizer_1->Add(sizer_3, 0, wxALL|wxEXPAND, 2);
    SetSizer(sizer_1);
    sizer_1->Fit(this);
  
}

