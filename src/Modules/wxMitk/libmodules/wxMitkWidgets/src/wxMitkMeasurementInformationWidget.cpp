/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#include "wxMitkMeasurementInformationWidget.h"

#include "vtkMath.h"

using namespace mitk;


wxMitkMeasurementInformationWidget::wxMitkMeasurementInformationWidget(wxWindow* parent, 
																	   int id, 
																	   const wxPoint& pos, 
																	   const wxSize& size, 
																	   long style):
    wxPanel(parent, id, pos, size, wxTAB_TRAVERSAL)
{
  
	sizer_2_staticbox = new wxStaticBox(this, -1, wxT("Static dimensional properties"));
    label_9 = new wxStaticText(this, wxID_ANY, wxT("Axis"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
    label_6 = new wxStaticText(this, wxID_ANY, wxT("FirstPoint"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
	label_7 = new wxStaticText(this, wxID_ANY, wxT("SecondPoint"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
    label_3 = new wxStaticText(this, wxID_ANY, wxT("X"));
    m_point1_X = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE|wxNO_BORDER);
    m_point2_X = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE|wxNO_BORDER);
    label_4 = new wxStaticText(this, wxID_ANY, wxT("Y"));
    m_point1_Y = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE|wxNO_BORDER);
    m_point2_Y = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE|wxNO_BORDER);
    label_5 = new wxStaticText(this, wxID_ANY, wxT("Z"));
    m_point1_Z = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE|wxNO_BORDER);
    m_point2_Z = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE|wxNO_BORDER);
    label_1 = new wxStaticText(this, wxID_ANY, wxT("Distance"));
    m_measure = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE|wxNO_BORDER);
   
	m_UpdateWidgetCommand = vtkUpdateWidgetCommand::New();
	m_UpdateWidgetCommand->SetWidget( this );
	m_MeasurementObserverTag = 0;

    set_properties();
    do_layout();
  
}

mitk::wxMitkMeasurementInformationWidget::~wxMitkMeasurementInformationWidget()
{
	if ( m_MeasurementInput )
	{
		m_MeasurementInput->RemoveObserver( m_MeasurementObserverTag );
	}
}

void mitk::wxMitkMeasurementInformationWidget::Execute( 
	vtkObject* caller, 
	unsigned long eventId, 
	void* callData )
{
	UpdateData( m_MeasurementInput );
}

void mitk::wxMitkMeasurementInformationWidget::UpdateWidget()
{

	// If input is NULL -> Reset widget
	if ( m_MeasurementInput == NULL || m_MeasurementInput->GetNumberOfPoints() < 2 )
	{
		this->ResetWidget();
		return;
	}

	double distance = 0.0;
	double coord1[3] = {0.0,0.0,0.0};
	double coord2[3] = {0.0,0.0,0.0};

	wxTextCtrl *point1[3] ={m_point1_X, m_point1_Y, m_point1_Z};
	wxTextCtrl *point2[3]={m_point2_X, m_point2_Y, m_point2_Z};

	//Bucle x,y,z
	for (int i=0;i<3;i++)
	{
		//Getpoint1 from vtk
		point1[i]->SetValue(wxString::Format(wxT("%.1f"), 
			m_MeasurementInput->GetPoint( 0 )[i] ));
		coord1[i] = m_MeasurementInput->GetPoint( 0 )[i];

		//Getpoint2 from vtk
		point2[i]->SetValue(wxString::Format(wxT("%.1f"), 
			m_MeasurementInput->GetPoint( 1 )[i] ));
		coord2[i] = m_MeasurementInput->GetPoint( 1 )[i];
	}

	distance = vtkMath::Distance2BetweenPoints(coord1, coord2);
	m_measure->SetValue(wxString::Format(wxT("%.2f"), sqrt(distance) ));
}

void wxMitkMeasurementInformationWidget::UpdateData(
	vtkSmartPointer<vtkPolyData> measurementInput)
{

	if ( m_MeasurementInput )
	{
		m_MeasurementInput->RemoveObserver( m_MeasurementObserverTag );
	}
	m_MeasurementInput = measurementInput;

	// If input is NULL -> Reset widget
	if ( measurementInput == NULL )
	{
		this->ResetWidget();
		return;
	}

	// Add observer to receive notifications about new added points
	m_MeasurementObserverTag = m_MeasurementInput->AddObserver( 
		vtkCommand::ModifiedEvent, 
		m_UpdateWidgetCommand );

	UpdateWidget();

}

void wxMitkMeasurementInformationWidget::ResetWidget()
{		
				
	wxTextCtrl *point1[3] ={m_point1_X, m_point1_Y, m_point1_Z};
	wxTextCtrl *point2[3]={m_point2_X, m_point2_Y, m_point2_Z};

	for (int i=0;i<3;i++)
		{
			point1[i]->SetValue(wxString::Format(wxT("%.1f"), 0.0 ) );
			point2[i]->SetValue(wxString::Format(wxT("%.1f"), 0.0 ) );
		}
		m_measure->SetValue(wxT(""));
}


void wxMitkMeasurementInformationWidget::set_properties()
{
    
    m_point1_X->SetMinSize(wxSize(50, 17));
    m_point2_X->SetMinSize(wxSize(50, 17));
    m_point1_Y->SetMinSize(wxSize(50, 17));
    m_point2_Y->SetMinSize(wxSize(50, 17));
    m_point1_Z->SetMinSize(wxSize(50, 17));
    m_point2_Z->SetMinSize(wxSize(50, 17));
}


void wxMitkMeasurementInformationWidget::do_layout()
{
  
    wxBoxSizer* sizer_1 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_4 = new wxBoxSizer(wxHORIZONTAL);
    wxStaticBoxSizer* sizer_2 = new wxStaticBoxSizer(sizer_2_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_sizer_1 = new wxFlexGridSizer(4, 3, 5, 15);
    grid_sizer_1->Add(label_9, 0, wxEXPAND, 0);
    grid_sizer_1->Add(label_6, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	grid_sizer_1->Add(label_7, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
     grid_sizer_1->Add(label_3, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    grid_sizer_1->Add(m_point1_X, 0, wxEXPAND, 0);
    grid_sizer_1->Add(m_point2_X, 0, wxEXPAND, 0);
    grid_sizer_1->Add(label_4, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    grid_sizer_1->Add(m_point1_Y, 0, wxEXPAND, 0);
    grid_sizer_1->Add(m_point2_Y, 0, wxEXPAND, 0);
    grid_sizer_1->Add(label_5, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    grid_sizer_1->Add(m_point1_Z, 0, wxEXPAND, 0);
    grid_sizer_1->Add(m_point2_Z, 0, wxEXPAND, 0);
    sizer_2->Add(grid_sizer_1, 1, wxEXPAND, 0);
	sizer_4->Add(label_1, 1, wxEXPAND, 0);
    sizer_4->Add(m_measure, 1, wxALL|wxEXPAND, 0);
	sizer_2->Add(sizer_4);
    sizer_1->Add(sizer_2, 0, wxEXPAND, 0);
    //sizer_1->Add(sizer_4, 0, wxEXPAND, 0);
    SetSizer(sizer_1);
    sizer_1->Fit(this);
  
}

vtkUpdateWidgetCommand * mitk::vtkUpdateWidgetCommand::New()
{
	return new vtkUpdateWidgetCommand;
}

void mitk::vtkUpdateWidgetCommand::SetWidget( wxMitkMeasurementInformationWidget* widget )
{
	m_widget = widget;
}

void mitk::vtkUpdateWidgetCommand::Execute( vtkObject* caller, unsigned long eventId, void* callData )
{
	m_widget->UpdateWidget( );
}

