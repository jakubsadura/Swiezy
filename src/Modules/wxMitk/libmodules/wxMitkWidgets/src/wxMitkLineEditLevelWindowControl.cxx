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
 #include <wx/textctrl.h>

#include "wxMitkLineEditLevelWindowControl.h"
#include "wxMitkLevelWindowHelper.h"
#include <mitkLevelWindowManager.h>
#include <mitkDataTree.h>
#include <itkCommand.h>
#include <mitkRenderingManager.h>
#include <sstream>
#include <wxUnicode.h>
#include "mitkNodePredicateDataType.h"

using namespace mitk;

#define ID_wxTextCtrl_Level (1 + wxID_HIGHEST)
#define ID_wxTextCtrl_Window (2 + wxID_HIGHEST)


BEGIN_EVENT_TABLE(wxMitkLineEditLevelWindowControl, wxPanel)
	EVT_TEXT_ENTER(ID_wxTextCtrl_Level, wxMitkLineEditLevelWindowControl::OnLevelValueChanged)
	EVT_TEXT_ENTER(ID_wxTextCtrl_Window, wxMitkLineEditLevelWindowControl::OnWindowValueChanged)
END_EVENT_TABLE()

//!
wxMitkLineEditLevelWindowControl::wxMitkLineEditLevelWindowControl(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
: wxPanel(parent, id, pos, size, style, name), isObserverTagSet(false)
{
	isObserverTagSet = false;
	ObserverTag = 0;
	m_LevelWindowProperty = NULL;
	m_IsPropertyModifiedTagSet = false;

	// Create the level and window text input widgets
	this->txtLevel = new wxTextCtrl(this, ID_wxTextCtrl_Level, wxT("0"), wxDefaultPosition, wxSize(40, 16), wxWANTS_CHARS);
	this->txtWindow = new wxTextCtrl(this, ID_wxTextCtrl_Window, wxT("0"), wxDefaultPosition, wxSize(40, 16), wxWANTS_CHARS);

	// Create the vertical layout
	wxBoxSizer* boxSizer = new wxBoxSizer(wxVERTICAL);
	boxSizer->Add(this->txtLevel, 0);
	boxSizer->Add(this->txtWindow, 0);
	this->SetSizer(boxSizer);
	this->SetAutoLayout(true);
}

//!
wxMitkLineEditLevelWindowControl::~wxMitkLineEditLevelWindowControl(void)
{
	SetLevelWindowProperty( NULL );
}

//!
void wxMitkLineEditLevelWindowControl::SetDataStorage(mitk::DataStorage* dataStorage)
{
	this->levelWindowManager->SetDataStorage(dataStorage);
}

/**
Assigns a LevelWindowManager to the widget and adds an observer for the LevelWindowProperty, that will be managed by the
OnLevelWindowPropertyModified member function.
\sa wxMitkLineEditLevelWindowControl::OnLevelWindowPropertyModified
*/
void wxMitkLineEditLevelWindowControl::SetLevelWindowManager(mitk::LevelWindowManager::Pointer levelWindowManager)
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
		itk::ReceptorMemberCommand<wxMitkLineEditLevelWindowControl>::Pointer command = itk::ReceptorMemberCommand<wxMitkLineEditLevelWindowControl>::New();
		command->SetCallbackFunction(this, &wxMitkLineEditLevelWindowControl::OnLevelWindowPropertyModified);
		this->ObserverTag = this->levelWindowManager->AddObserver(itk::ModifiedEvent(), command);
		this->isObserverTagSet = true;
	}
}

/**
This function is responsible for displaying the new values of the levelWindow when a itk::ModifiedEvent is sent because
a modification on the LevelWindowProperty
*/
void wxMitkLineEditLevelWindowControl::OnLevelWindowPropertyModified(const itk::EventObject& event)
{
	if(this->levelWindowManager.IsNull())
		return;
	if(this->levelWindowManager->GetLevelWindowProperty().IsNull())
	{
		SetLevelWindowProperty( NULL );
		return;
	}

	mitk::LevelWindow levelWindow(this->levelWindowManager->GetLevelWindow());

	// Change "activeImage" property to be used by interactor
	mitk::DataStorage::SetOfObjects::ConstPointer allImageNodes;
	allImageNodes = levelWindowManager->GetDataStorage()->GetSubset(mitk::NodePredicateDataType::New( "Image") );
	for ( unsigned int i = 0; i < allImageNodes->size() ; i++ )
	{
		bool isActiveImage = false;
		bool propFound = allImageNodes->at( i )->GetBoolProperty( "activeImage", isActiveImage );
		if ( allImageNodes->at( i )->GetData( ) == levelWindowManager->GetCurrentImage() )
		{
			if ( !propFound || isActiveImage == false )
			{
				allImageNodes->at( i )->SetBoolProperty( "activeImage", true );
			}
		}
		else if ( propFound && isActiveImage == true )
		{
			allImageNodes->at( i )->SetBoolProperty( "activeImage", false );
		}
	}

	SetLevelWindowProperty( levelWindowManager->GetLevelWindowProperty() );

	float levelValue = static_cast<float>(levelWindow.GetLevel());
	float windowValue = static_cast<float>(levelWindow.GetWindow());
	
	// Set the levelwindow values to the text boxes
	std::ostringstream oss;
	oss << levelValue;
	this->txtLevel->ChangeValue(_U(oss.str()));
	oss.str("");
	oss << windowValue;
	this->txtWindow->ChangeValue(_U(oss.str()));

	mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

/**
When user changes the text value in Level text box, notify it to mitk and change the LevelWindow accordingly
*/
void wxMitkLineEditLevelWindowControl::OnLevelValueChanged(wxCommandEvent& WXUNUSED(event))
{
	if(this->levelWindowManager.IsNull())
		return;

	if(this->levelWindowManager->GetLevelWindowProperty().IsNull())
		return;

	mitk::LevelWindow levelWindow(this->levelWindowManager->GetLevelWindow());
	float levelValue;
	float windowValue = static_cast<float>(levelWindow.GetWindow());
	float rangeMax = static_cast<float>(levelWindow.GetRangeMax());
	float rangeMin = static_cast<float>(levelWindow.GetRangeMin());
	wxString levelValueStr = this->txtLevel->GetValue();
	levelValue = std::atof(levelValueStr);

	// Set the final levelwindow value back to the text box
	std::ostringstream oss;
	oss << levelValue;
	this->txtLevel->ChangeValue(_U(oss.str()));
	// Set the final levelwindow value back to the LevelWindowManager
	levelWindow.SetLevelWindow(levelValue, windowValue);
	this->levelWindowManager->SetLevelWindow(levelWindow);
}

/**
When user changes the text value in Window text box, notify it to mitk and change the LevelWindow accordingly
*/
void wxMitkLineEditLevelWindowControl::OnWindowValueChanged(wxCommandEvent& WXUNUSED(event))
{
	if(this->levelWindowManager.IsNull())
		return;

	if(this->levelWindowManager->GetLevelWindowProperty().IsNull())
		return;

	mitk::LevelWindow levelWindow(levelWindowManager->GetLevelWindow());
	float windowValue;
	float levelValue = static_cast<float>(levelWindow.GetLevel());
	float rangeMax = static_cast<float>(levelWindow.GetRangeMax());
	float rangeMin = static_cast<float>(levelWindow.GetRangeMin());
	wxString windowValueStr = this->txtWindow->GetValue();
	windowValue = std::atof(windowValueStr);

	// Set the final levelwindow value back to the text box
	std::ostringstream oss;
	oss << windowValue;
	this->txtWindow->ChangeValue(_U(oss.str()));
	
	// Set the final levelwindow value back to the LevelWindowManager
	levelWindow.SetLevelWindow(levelValue, windowValue);
	this->levelWindowManager->SetLevelWindow(levelWindow);
}

void mitk::wxMitkLineEditLevelWindowControl::SetLevelWindowProperty( 
	mitk::LevelWindowProperty::Pointer levelWindowProperty )
{
	if ( m_LevelWindowProperty.IsNotNull( ) && levelWindowProperty == m_LevelWindowProperty )
	{
		return;
	}

	if (m_IsPropertyModifiedTagSet)  // remove listener for old property
	{
		m_LevelWindowProperty->RemoveObserver(m_PropertyModifiedTag);
		m_IsPropertyModifiedTagSet = false;
	}
	
	m_LevelWindowProperty = levelWindowProperty;

	if ( m_LevelWindowProperty.IsNotNull() )
	{
		itk::ReceptorMemberCommand<wxMitkLineEditLevelWindowControl>::Pointer command;
		command = itk::ReceptorMemberCommand<wxMitkLineEditLevelWindowControl>::New();  // register listener for new property
		command->SetCallbackFunction(this, &wxMitkLineEditLevelWindowControl::OnPropertyModified);
		m_PropertyModifiedTag = m_LevelWindowProperty->AddObserver( itk::ModifiedEvent(), command );
		m_IsPropertyModifiedTagSet = true;
	}
}

void mitk::wxMitkLineEditLevelWindowControl::OnPropertyModified( const itk::EventObject& e )
{
	if(this->levelWindowManager.IsNull())
		return;

	if(this->levelWindowManager->GetLevelWindowProperty().IsNull())
		return;

	this->levelWindowManager->Modified();
}
