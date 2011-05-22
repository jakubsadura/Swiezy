/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

// For compilers that don't support precompilation, include "wx/wx.h"
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "coreMissingDataEntityFieldsWizard.h"
#include "coreAssert.h"
#include "coreKernel.h"
#include "coreReportExceptionMacros.h"
#include "coreDataEntityInfoHelper.h"
#include "coreDataEntity.h"
#include "wxID.h"

#include <wxUnicode.h>

using namespace Core::Widgets;

#define wxID_listOfModalities wxID("listOfModalities")

BEGIN_EVENT_TABLE(MissingDataEntityFieldsWizard, wxWizard)
	EVT_LISTBOX(wxID_listOfModalities, MissingDataEntityFieldsWizard::OnModalityItemSelected)
END_EVENT_TABLE()

//!
MissingDataEntityFieldsWizard::MissingDataEntityFieldsWizard(wxFrame* frame, Core::DataEntity::Pointer dataEntity)
: wxWizard(frame, wxID_ANY, wxT("Fill in missing information")), 
m_ModalityPage(NULL)
{
	try
	{
		// If modality is missing, create a page so as to ask for it
		if(	dataEntity->IsImage() && 
			dataEntity->GetMetadata( )->GetModality() == Core::UnknownModality)
		{
			this->m_ModalityPage = new wxWizardPageSimple(this);
			this->m_SelectedModality = dataEntity->GetMetadata( )->GetModality();

			// Config the first page
			SetupModalityPage();

			// layout
			GetPageAreaSizer()->Add(m_ModalityPage);
		}
		
	}
	coreCatchExceptionsReportAndNoThrowMacro(MissingDataEntityFieldsWizard::MissingDataEntityFieldsWizard)
}

wxWizardPageSimple* MissingDataEntityFieldsWizard::GetStartPage(void)
{
	return this->m_ModalityPage;
}

//!
MissingDataEntityFieldsWizard::~MissingDataEntityFieldsWizard(void)
{

}

//!
void MissingDataEntityFieldsWizard::SetupModalityPage(void)
{
	try
	{
		std::string textStr = \
			"Please provide the modality of the image.\n" \
			"The modality will be required for some algorithms to perform correctly\n"\
			"or to allow them to take certain decisions with the input data.\n";
		
		wxStaticText* lblTextPh = new wxStaticText(this->m_ModalityPage, wxID_ANY, _U(textStr.c_str()));
		wxStaticText* lblTextTitle = new wxStaticText(this->m_ModalityPage, wxID_ANY, wxT("Set modality\n\n"));
		this->m_ListModality = new wxListBox(this->m_ModalityPage, wxID_listOfModalities, wxDefaultPosition, wxSize(100, 100), 0, NULL, wxLB_SINGLE);

		// Fill in the list
		Core::DataEntityInfoHelper::ModalityList list = Core::DataEntityInfoHelper::GetAvailableModalityTypes();
		for(Core::DataEntityInfoHelper::ModalityList::iterator it = list.begin();
			it != list.end();
			++it)
		{
			std::string modalityStr = Core::DataEntityInfoHelper::GetModalityTypeAsString(*it);
			int k = this->m_ListModality->Append(_U(modalityStr.c_str()));
			this->m_ModalityMap[k] = (*it); 
			if(this->m_SelectedModality == (*it))
				this->m_ListModality->SetSelection(k);
		}
		
		// Set some style
		wxFont font = lblTextTitle->GetFont();
		font.SetWeight(wxBOLD);
		lblTextTitle->SetFont(font);

		// layout
		wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
		sizer->Add(lblTextTitle);
		sizer->Add(lblTextPh);
		sizer->Add(this->m_ListModality, 1, wxEXPAND | wxALL, 4);
		this->m_ModalityPage->SetSizer(sizer);
	}
	coreCatchExceptionsAddTraceAndThrowMacro(MissingDataEntityFieldsWizard::SetupModalityPage)
}

//!
void MissingDataEntityFieldsWizard::OnModalityItemSelected(wxCommandEvent& event)
{
	int index = event.GetSelection();
	if(this->m_ListModality->IsSelected(index))
		this->m_SelectedModality = this->m_ModalityMap[index];
	else
		this->m_SelectedModality = Core::UnknownModality;
}

Core::ModalityType Core::Widgets::MissingDataEntityFieldsWizard::GetSelectedModality() const
{
	return m_SelectedModality;
}
