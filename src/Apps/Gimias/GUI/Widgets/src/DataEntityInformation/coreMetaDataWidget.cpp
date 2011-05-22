/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#include "coreMetadataWidget.h"
#include "coreDataEntityMetadata.h"
#include "coreDataEntityInfoHelper.h"

#include "wxUnicode.h"

Core::Widgets::MetadataWidget::MetadataWidget(wxWindow* parent, 
								int id, 
								const wxPoint& pos, 
								const wxSize& size, 
								long style):
    wxPanel(parent, id, pos, size, wxTAB_TRAVERSAL)
{
	m_label_NrOfTimeStep = new wxStaticText(this, wxID_ANY, wxT("Number of Time Steps"));
	m_txt_numberTimeSteps = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxNO_BORDER);
	m_label_EDFlag = new wxStaticText(this, wxID_ANY, wxT("ED Flag (time step num)"));
	m_txtCtrl_EDFlag = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxNO_BORDER);
	m_label_ESFlag = new wxStaticText(this, wxID_ANY, wxT("ES Flag (time step num)"));
	m_txtCtrl_ESFlag = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxNO_BORDER);
	m_label_mod = new wxStaticText(this, wxID_ANY, wxT("Image modality"));
	m_comboBox_modality = new wxComboBox(this, wxID_ANY, wxT("Image modality"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_DROPDOWN|wxCB_SIMPLE|wxCB_READONLY);

	// definition of variables
	m_changeInWidgetObserver.SetSlotFunction(this, &MetadataWidget::UpdateData);
	m_changeInWidgetObserver.Observe(m_txtCtrl_EDFlag);
	m_changeInWidgetObserver.Observe(m_txtCtrl_ESFlag);
	m_changeInWidgetObserver.Observe(m_comboBox_modality);
		
	this->m_dataEntityHolder = Core::DataEntityHolder::New();

	set_properties();
	do_layout();

}

void Core::Widgets::MetadataWidget::UpdateData()
{
	if(m_dataEntity.IsNull())
		 return;
	
	Core::DataEntityMetadata::Pointer meta = m_dataEntity->GetMetadata();

	int nTimeSteps=m_dataEntity->GetNumberOfTimeSteps();
	long EDvalue=0, ESvalue=0;
	wxString strED=wxString::Format(wxT("%s"), m_txtCtrl_EDFlag->GetValue().mb_str());
	wxString strES=wxString::Format(wxT("%s"), m_txtCtrl_ESFlag->GetValue().mb_str());
	if(strED.ToLong(&EDvalue,10) && (EDvalue>0) && (EDvalue<=nTimeSteps))
	{
		meta->SetEDTimeStep(EDvalue-1);
	}
	else
	{
		meta->SetEDTimeStep( -1 );
		m_txtCtrl_EDFlag->SetValue("");
	}

	if(strES.ToLong(&ESvalue,10) && (ESvalue>0) && (ESvalue<=nTimeSteps))
	{
		meta->SetESTimeStep(ESvalue-1);
	}
	else
	{
		meta->SetESTimeStep( -1 );
		m_txtCtrl_ESFlag->SetValue("");
	}
	//get modality from combo box
	std::string modStr = _U(m_comboBox_modality->GetValue());
	ModalityType mod = DataEntityInfoHelper::GetModalityType (modStr);
	meta->SetModality (mod);

}

void Core::Widgets::MetadataWidget::UpdateWidget(bool bResetModality /*=true*/)
{		
	if(m_dataEntity.IsNull())
		return;

	Core::DataEntityMetadata::Pointer meta = m_dataEntity->GetMetadata();
	int numTimeSteps = m_dataEntity->GetNumberOfTimeSteps();

	if (numTimeSteps > 1)
	{
		//show widget controls for time steps number, ED,ES flags and values
		m_txt_numberTimeSteps->Show();
		m_label_NrOfTimeStep->Show();
		m_label_EDFlag->Show();
		m_txtCtrl_EDFlag->Show();
		m_label_ESFlag->Show();
		m_txtCtrl_ESFlag->Show();

		m_txt_numberTimeSteps->SetValue( wxString::Format(wxT("%d"),
			m_dataEntity->GetNumberOfTimeSteps()));

		if ( meta->GetEDTimeStep() == "")
			m_txtCtrl_EDFlag->SetValue(wxString::FromAscii(meta->GetEDTimeStep().c_str()));
		else
			m_txtCtrl_EDFlag->SetValue(wxString::Format(wxT("%d"),
				std::atoi(meta->GetEDTimeStep().c_str())+1));
		if ( meta->GetESTimeStep() == "")
			m_txtCtrl_ESFlag->SetValue(meta->GetESTimeStep());
		else
			m_txtCtrl_ESFlag->SetValue(wxString::Format(wxT("%d"),
				std::atoi(meta->GetESTimeStep().c_str())+1));
	}
	else
	{
		//hide widget controls for time steps number, ED,ES flags and values, when 
		m_txt_numberTimeSteps->Hide();
		m_label_NrOfTimeStep->Hide();
		m_label_EDFlag->Hide();
		m_txtCtrl_EDFlag->Hide();
		m_label_ESFlag->Hide();
		m_txtCtrl_ESFlag->Hide();
	}

	if(!bResetModality)
		return;

	//update modality combo box
	m_comboBox_modality->Clear();
	//Fill in the list
	Core::DataEntityInfoHelper::ModalityList list = Core::DataEntityInfoHelper::GetAvailableModalityTypes();
	for(Core::DataEntityInfoHelper::ModalityList::iterator it = list.begin();
		it != list.end();
		++it)
		{
			std::string modalityStr = Core::DataEntityInfoHelper::GetModalityTypeAsString(*it);
			m_comboBox_modality->Append(_U(modalityStr.c_str()));
		}
	//set modality on combo box
	Core::ModalityType mod = m_dataEntity->GetMetadata()->GetModality();
	std::string modString = DataEntityInfoHelper::GetModalityTypeAsString (mod);
	m_comboBox_modality->SetValue (modString);
}

void Core::Widgets::MetadataWidget::SetDataEntity( Core::DataEntity::Pointer data)
{
	m_dataEntity = data;
	this->m_dataEntityHolder->SetSubject(m_dataEntity);
}

void Core::Widgets::MetadataWidget::set_properties()
{
    
}


void Core::Widgets::MetadataWidget::do_layout()
{
	// layout them
	wxBoxSizer* vlayout = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* sizer_1 = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* sizer_2 = new wxBoxSizer(wxVERTICAL);
	//image modality
	wxBoxSizer* sizer_9 = new wxBoxSizer(wxHORIZONTAL);
	//Timesteps and ES,ED flags
	wxBoxSizer* sizer_6 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* sizer_7 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* sizer_8 = new wxBoxSizer(wxHORIZONTAL);
	
	//Sizer for image modality
	sizer_9->Add (m_label_mod, 1, wxEXPAND, 0);
	sizer_9->Add (m_comboBox_modality, 1, wxEXPAND, 0);
	sizer_1->Add (sizer_9, 0, wxEXPAND, 0);
	
	//Sizers for timesteps and ES,ED flags
	sizer_6->Add(m_label_NrOfTimeStep, 1, wxEXPAND, 0);
	sizer_6->Add(m_txt_numberTimeSteps, 1, wxEXPAND, 0);
	sizer_2->Add(sizer_6, 0, wxEXPAND, 0);
	sizer_7->Add(m_label_EDFlag, 1, wxEXPAND, 0);
	sizer_7->Add(m_txtCtrl_EDFlag, 1, wxEXPAND, 0);
	sizer_2->Add(sizer_7, 0, wxEXPAND, 0);
	sizer_8->Add(m_label_ESFlag, 1, wxEXPAND, 0);
	sizer_8->Add(m_txtCtrl_ESFlag, 1, wxEXPAND, 0);
	sizer_2->Add(sizer_8, 0, wxEXPAND, 0);
	vlayout->Add(sizer_1, 0, wxEXPAND | wxALL, 4);
	vlayout->Add(sizer_2, 0, wxEXPAND | wxALL, 4);
	SetSizer(vlayout);
	vlayout->Fit(this);
  
}