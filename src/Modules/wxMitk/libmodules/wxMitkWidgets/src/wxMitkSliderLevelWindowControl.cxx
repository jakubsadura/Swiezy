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

#include "wxMitkSliderLevelWindowControl.h"
#include <mitkLevelWindowManager.h>
#include <mitkDataTree.h>
#include <itkCommand.h>
#include <mitkRenderingManager.h>

using namespace mitk;

BEGIN_EVENT_TABLE(wxMitkSliderLevelWindowControl, wxMitkRangeSliderControl)
	EVT_RANGE_CHANGED(wxID_ANY, wxMitkSliderLevelWindowControl::RangeChangedByUser)
END_EVENT_TABLE()

//!
wxMitkSliderLevelWindowControl::wxMitkSliderLevelWindowControl(wxWindow* parent, wxWindowID id, int level, unsigned int window, int minValue, int maxValue, const wxPoint& pos, const wxSize& size, long style, const wxValidator& validator, const wxString& name)
: wxMitkRangeSliderControl(parent, id, level, window, minValue, maxValue, pos, size, style, validator, name)
{
}

//!
wxMitkSliderLevelWindowControl::~wxMitkSliderLevelWindowControl(void)
{
}

//!
void wxMitkSliderLevelWindowControl::SetDataStorage(mitk::DataStorage* dataStorage)
{
	this->levelWindowManager->SetDataStorage(dataStorage);
}

/**
Assigns a LevelWindowManager to the widget and adds an observer for the LevelWindowProperty, that will be managed by the
OnLevelWindowPropertyModified member function.
\sa wxMitkLineEditLevelWindowControl::OnLevelWindowPropertyModified
*/
void wxMitkSliderLevelWindowControl::SetLevelWindowManager(mitk::LevelWindowManager::Pointer levelWindowManager)
{
	// If an observer was set for the ModifiedEvent of LevelWindowProperty, remove it before re-assigning
	// the LevelWindowManager
	if(this->levelWindowManager.IsNotNull() && this->isObserverTagSet)
	{
		this->levelWindowManager->RemoveObserver(this->ObserverTag);
		isObserverTagSet = false;
	}

	// Assign the new LevelWindowManager
	this->levelWindowManager = levelWindowManager;

	// Set the Observer for the ModifiedEvent of LevelWindowProperty
	if (this->levelWindowManager.IsNotNull())
	{
		itk::ReceptorMemberCommand<wxMitkSliderLevelWindowControl>::Pointer command = itk::ReceptorMemberCommand<wxMitkSliderLevelWindowControl>::New();
		command->SetCallbackFunction(this, &wxMitkSliderLevelWindowControl::OnLevelWindowPropertyModified);
		this->ObserverTag = this->levelWindowManager->AddObserver(itk::ModifiedEvent(), command);
		this->isObserverTagSet = true;
	}
}

/**
This function is responsible for displaying the new size and position of the slider when a itk::ModifiedEvent is 
sent because a modification of the LevelWindowProperty
*/
void wxMitkSliderLevelWindowControl::OnLevelWindowPropertyModified(const itk::EventObject& event)
{
	if(this->levelWindowManager.IsNull())
		return;

	if(this->levelWindowManager->GetLevelWindowProperty().IsNull())
		return;

	mitk::LevelWindow levelWindow(this->levelWindowManager->GetLevelWindow());
	mitk::ScalarType window = levelWindow.GetWindow();
	mitk::ScalarType level = levelWindow.GetLevel();

	// Set the levelwindow values to the slider
	this->ComputeValidRange(level, window, levelWindow.GetRangeMin(), levelWindow.GetRangeMax());
	this->Refresh();
}

//!
void wxMitkSliderLevelWindowControl::RangeChangedByUser(wxMitkRangeChangedEvent& event)
{
	if(this->levelWindowManager.IsNull())
		return;

	if(this->levelWindowManager->GetLevelWindowProperty().IsNull())
		return;

	mitk::LevelWindow levelWindow( this->levelWindowManager->GetLevelWindow());

	// Set the final levelwindow value back to the LevelWindowManager
	levelWindow.SetLevelWindow(this->GetValue(), this->GetRange());
	if ( levelWindow != this->levelWindowManager->GetLevelWindow() )
	{
		this->levelWindowManager->SetLevelWindow(levelWindow);
	}
}




