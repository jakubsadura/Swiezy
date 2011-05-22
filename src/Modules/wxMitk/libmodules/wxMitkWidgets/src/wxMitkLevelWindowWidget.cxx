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

#include "wxMitkLevelWindowWidget.h"
#include "wxMitkLineEditLevelWindowControl.h"
#include "wxMitkSliderLevelWindowControl.h"
#include <mitkLevelWindowManager.h>
#include <mitkDataTree.h>

using namespace mitk;

#define ID_wxMitkLineEditLevelWindowControl (1 + wxID_HIGHEST)
#define ID_wxMitkSliderLevelWindowControl (2 + wxID_HIGHEST)

//!
wxMitkLevelWindowWidget::wxMitkLevelWindowWidget(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
: wxPanel(parent, id, pos, size, style, name)
{
	this->SetId(wxID_ANY);

	// Hide it by default
	this->Show(false);

	// Create the widgets
	this->lineEdit = new wxMitkLineEditLevelWindowControl(this, ID_wxMitkLineEditLevelWindowControl);
	this->slider = new wxMitkSliderLevelWindowControl(this, ID_wxMitkSliderLevelWindowControl);

	// Layout them vertically
	wxBoxSizer* boxSizer = new wxBoxSizer(wxVERTICAL);
	boxSizer->Add(this->slider, 1);
	boxSizer->Add(this->lineEdit, 0);
	this->SetSizer(boxSizer);
	this->SetAutoLayout(true);
	
	// Attach a LevelWindowManager and set it to the widgets
	this->SetLevelWindowManager(mitk::LevelWindowManager::New());
}

//!
wxMitkLevelWindowWidget::~wxMitkLevelWindowWidget(void)
{
}

//!
void wxMitkLevelWindowWidget::SetDataStorage(mitk::DataStorage* dataStorage)
{
	this->levelWindowManager->SetDataStorage(dataStorage);
}

itk::SmartPointer<mitk::LevelWindowManager> wxMitkLevelWindowWidget::GetLevelWindowManager()
{
	return this->levelWindowManager;
}

//!
void wxMitkLevelWindowWidget::SetLevelWindowManager(mitk::LevelWindowManager::Pointer levelWindowManager)
{
	// Assign the new LevelWindowManager so as to keep it alive
	this->levelWindowManager = levelWindowManager;
	// Assign it to the widgets
	this->lineEdit->SetLevelWindowManager(this->levelWindowManager);
	this->slider->SetLevelWindowManager(this->levelWindowManager);
}




