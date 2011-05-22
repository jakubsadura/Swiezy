/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
       #include "wx/wx.h"
#endif

#include <wx/textctrl.h>
#include "coreBaseInputControl.h"
#include "coreAssert.h"
#include "coreStyleManager.h"
#include "coreKernel.h"
#include "coreWxMitkGraphicalInterface.h"
#include "coreDataEntityListBrowser.h"
#include "coreSelectionComboBox.h"


Core::Widgets::BaseInputControl::BaseInputControl(
	wxWindow* parent, 
	wxWindowID id, 
	const wxPoint& pos, 
	const wxSize& size, 
	long style, const 
	wxString& name, 
	bool bAutomaticSelection,
	bool bSetDefaultDataEntity )
	: wxPanel(parent, id, pos, size, style, name)
{
	m_CurrentSelectedDataHolder = NULL;
	m_AutomaticSelection = bAutomaticSelection;
	m_SetDefaultDataEntity = bSetDefaultDataEntity;

	// Create buttons
	SetAutoLayout( true );
	m_StaticBox = new wxStaticBox(this, wxID_ANY, wxT("Input data"));

	
	m_wxListBox = new SelectionComboBox(
		this, 
		wxID_BaseInputControl_ListBox, 
		"",
		wxDefaultPosition,
		wxDefaultSize,
		0,
		NULL,
		wxCB_READONLY,
		wxDefaultValidator,
		"comboBox");
	m_ifaceComboBox = new wxListViewComboPopup();
	m_wxListBox->SetPopupControl(m_ifaceComboBox);
		
	Connect( 
		wxID_BaseInputControl_ListBox, 
		wxEVT_COMMAND_COMBOBOX_SELECTED, 
		wxCommandEventHandler(BaseInputControl::OnAcquireInputListBoxChanged) );

	// Layout the items
	wxStaticBoxSizer* hlayout = new wxStaticBoxSizer(m_StaticBox, wxHORIZONTAL);
	// Proportion 1, EXPAND
	hlayout->Add(m_wxListBox, 1, wxEXPAND );
	SetSizer(hlayout);

	// Now apply the style:
	Core::Runtime::wxMitkGraphicalInterface::Pointer gIface = Core::Runtime::Kernel::GetGraphicalInterface();
	coreAssertMacro(gIface.IsNotNull() && "The graphical interface must be running");
	Core::Runtime::StyleManager::Pointer styleManager = gIface->GetStyleManager();
	coreAssertMacro(styleManager.IsNotNull() && "The style manager must be initialized");
	styleManager->ConfigureAcquireInputControl(this);
}

Core::Widgets::BaseInputControl::~BaseInputControl(void)
{ 
	if ( m_CurrentSelectedDataHolder.IsNotNull() )
	{
		m_CurrentSelectedDataHolder->RemoveObserver(
			this, 
			&BaseInputControl::OnSelectedModified, 
			Core::DH_SUBJECT_MODIFIED_OR_NEW_SUBJECT);
	}

	m_ConnOnAdd.disconnect(); 
	m_ConnOnRem.disconnect();
}


void Core::Widgets::BaseInputControl::SetLabel(std::string label)
{ 
	m_StaticBox->SetLabel(wxString(label.c_str(), wxConvUTF8));
}

void Core::Widgets::BaseInputControl::SetHeaderText(std::string val) 
{ 
	SetLabel( val );
}

void Core::Widgets::BaseInputControl::SetAutomaticSelection( bool bVal )
{
	m_AutomaticSelection = bVal;
	FitInside( );
}


void Core::Widgets::BaseInputControl::OnSelectedModified()
{
	if ( m_AutomaticSelection && 
		 IsEnabled() &&
		 m_CurrentSelectedDataHolder.IsNotNull( ) )
	{
		SetAcquiredInputData( m_CurrentSelectedDataHolder->GetSubject() );
	}
}

bool Core::Widgets::BaseInputControl::Enable(bool enable)
{
#ifdef __WXMAC__
	//this is a trick (for MACOSX) to hide the disabling of the child to the parent window, so that this event function will be called again
	//each time the parent window is enabled/disabled. In Linux, the problem is solved with the OnParentEnable function (that unfortunately
	//is currently not fired neither on mac nor on MSW).
	//On MSW a bugly behavior of the wxWindowMSW::Enable(bool enable) method is actually used to workaround the problem:
	//each wxWindowMSW keeps a list of all disabled children and actually checks if a certain child should be notified of the enabling/disabling
	//event by looking for this child in the list. But, since this list is only created or updated when the parent window is disabled, a parent 
	//that is already disabled when a certain child is disabled, will never be notified of this event (i.e. the disabled_children list won't be updated).
	//This strange behavior can be used to force the calling of this event whenever the parent is enabled/disabled, even if this input control
	//is disabled. Of course, this strange behviour only hides the problem (on MSW), but doesn't really solve it!!!!!
	bool bReturn = true;
#else
	bool bReturn = wxPanel::Enable( enable );
#endif //__WXMAC__

	if ( enable )
	{
		FillListBox( );

		OnSelectedModified( );

		SetDefaultDataEntity( );
	}

	return bReturn;
}

Core::DataEntityHolder::Pointer 
Core::Widgets::BaseInputControl::GetCurrentSelectedDataHolder() const
{ 
	return m_CurrentSelectedDataHolder; 
}

bool Core::Widgets::BaseInputControl::GetDefaultDataEntityFlag() const 
{ 
	return m_SetDefaultDataEntity; 
}

void Core::Widgets::BaseInputControl::SetDefaultDataEntityFlag(bool val) 
{ 
	m_SetDefaultDataEntity = val; 
}

int Core::Widgets::BaseInputControl::FindListBoxItem( 
	Core::DataEntity::Pointer dataEntity )
{
	if ( dataEntity.IsNull() )
	{
		return wxID_ANY;
	}

	for ( int i = 0 ; i < m_ifaceComboBox->GetItemCount( ) ; i++ )
	{
		long data = m_ifaceComboBox->GetItemData( i );
		if ( data == dataEntity->GetId() )
		{
			return i;
		}
	}

	return wxID_ANY;
}

void Core::Widgets::BaseInputControl::UpdateWidget( void )
{
	int id = FindListBoxItem( GetAcquiredInputDataHolder()->GetSubject() );
	m_ifaceComboBox->SetSelection( id );
}

void Core::Widgets::BaseInputControl::OnAcquiredInputModified()
{
	try
	{
		UpdateWidget( );

		// Connect the subject to the subject of the DataEntityList
		// because when the dataEntity is removed from the list, this
		// should propagate the event to the m_AcquiredInputDataHolder
		if ( m_DataEntityList.IsNotNull() && 
			m_AcquiredInputDataHolder->GetSubject().IsNotNull( ) )
		{
			m_DataEntityList->ConnectInputHolder( GetAcquiredInputDataHolder() );
		}

	}
	coreCatchExceptionsLogAndNoThrowMacro(OnAcquiredInputModified)
		// When there are two processors (P1 and P2 ) connected in this way:
		// P2->GetInputDataHolder(i)->ConnectSubject(P1->GetOutPutDataHolder(j))
		// then the acquire input of P2 will look for the id of the output before this is 
		// added to the dataentity list , in order to avoid this we throw an exception 
		// that then we don't catch
}

void Core::Widgets::BaseInputControl::SetDefaultDataEntity()
{
	if ( m_DataEntityList.IsNull() || 
		!m_SetDefaultDataEntity || 
		!IsEnabled() || 
		m_AcquiredInputDataHolder->GetSubject().IsNotNull() ||
		m_ifaceComboBox->GetItemCount() == 0 )
	{
		return;
	}

	long data = m_ifaceComboBox->GetItemData( 0 );
	Core::DataEntity::Pointer dataEntity = m_DataEntityList->GetDataEntity( data );
	SetAcquiredInputData( dataEntity );
}

void Core::Widgets::BaseInputControl::SetDataEntityListBrowser( 
	Core::Widgets::DataEntityListBrowser* dataEntityListBrowser )
{
	if ( dataEntityListBrowser == NULL )
	{
		return;
	}

	BaseInputControl::SetCurrentSelectedDataHolder( 
		dataEntityListBrowser->GetSelectedDataEntityHolder() );

	SetDataEntityList( dataEntityListBrowser->GetDataEntityList() );
	m_DataEntityListBrowser = dataEntityListBrowser;
}


void Core::Widgets::BaseInputControl::SetDataEntityList( 
	Core::DataEntityList::Pointer val )
{
	m_DataEntityList = val;
	m_ConnOnAdd = m_DataEntityList->AddObserverOnAddDataEntity(
		this, 
		&BaseInputControl::OnDataEntityAddedToList);

	m_ConnOnRem = m_DataEntityList->AddObserverOnRemoveDataEntity(
		this, 
		&BaseInputControl::OnDataEntityRemovedFromList);

	FillListBox( );
}

void Core::Widgets::BaseInputControl::OnDataEntityAddedToList( 
	Core::DataEntity::Pointer dataEntity )
{
	FillListBox( );
}

void Core::Widgets::BaseInputControl::OnDataEntityRemovedFromList( 
	Core::DataEntity::Pointer dataEntity )
{
	FillListBox( );
}

void Core::Widgets::BaseInputControl::FillListBox( ) 
{
	m_ifaceComboBox->DeleteAllItems();

	if ( m_DataEntityList.IsNull() )
	{
		return;
	}

	Core::DataEntityList::iterator it;
	it = m_DataEntityList->Begin();
	while ( it != m_DataEntityList->End() ) {

		Core::DataEntity::Pointer dataEntity = m_DataEntityList->Get(it);

		if ( CheckSubjectIsOk( dataEntity ) )
		{
			std::string text;
			text = Core::DataEntityInfoHelper::GetShortDescription(dataEntity);
			m_ifaceComboBox->Append( text, dataEntity->GetId() );
		}

		m_DataEntityList->Next(it);
	}

	SetDefaultDataEntity();

	UpdateWidget( );
}

void Core::Widgets::BaseInputControl::OnAcquireInputListBoxChanged(
	wxCommandEvent& event)
{
	int selectedItem = event.GetInt();
	if ( selectedItem == -1 )
	{
		Core::DataEntity::Pointer de;
		SetAcquiredInputData( de );
		return;
	}
	long data = m_ifaceComboBox->GetItemData( selectedItem );
	if ( data == -1 )
	{
		Core::DataEntity::Pointer de;
		SetAcquiredInputData( de );
		return;
	}

	Core::DataEntity::Pointer dataEntity = m_DataEntityList->GetDataEntity( data );

	SetAcquiredInputData( dataEntity );
}

void Core::Widgets::BaseInputControl::OnFatherModified()
{
	if ( !CheckFatherIsOk( m_AcquiredInputDataHolder->GetSubject( ) ) )
	{
		m_AcquiredInputDataHolder->SetSubject( NULL );
	}
}

void Core::Widgets::BaseInputControl::SetFatherHolder( 
	DataEntityHolder::Pointer val )
{
	if ( m_FatherHolder.IsNotNull() )
	{
		m_FatherHolder->RemoveObserver(
			this, 
			&BaseInputControl::OnFatherModified, 
			Core::DH_NEW_SUBJECT);
	}

	m_FatherHolder = val;

	m_FatherHolder->AddObserver(
		this, 
		&BaseInputControl::OnFatherModified, 
		Core::DH_NEW_SUBJECT);
}


bool Core::Widgets::BaseInputControl::CheckFatherIsOk( 
	const Core::DataEntity::Pointer &subject )
{
	bool isOk = true;

	if ( m_FatherHolder.IsNotNull() && subject.IsNotNull() )
	{
		isOk = m_FatherHolder->GetSubject() == subject->GetFather();
	}

	return isOk;

}

void Core::Widgets::BaseInputControl::SetCurrentSelectedDataHolder(
	DataEntityHolder::Pointer val ) 
{ 
	if ( m_CurrentSelectedDataHolder.IsNotNull( ) )
	{
		m_CurrentSelectedDataHolder->RemoveObserver(
			this, 
			&BaseInputControl::OnSelectedModified, 
			Core::DH_SUBJECT_MODIFIED_OR_NEW_SUBJECT);
	}

	m_CurrentSelectedDataHolder = val;

	// Add observer when the selection changes to automatically 
	// update the m_AcquiredInputDataHolder
	if ( m_CurrentSelectedDataHolder.IsNotNull( ) )
	{
		m_CurrentSelectedDataHolder->AddObserver(
			this, 
			&BaseInputControl::OnSelectedModified, 
			Core::DH_SUBJECT_MODIFIED_OR_NEW_SUBJECT);
	}	

}

void Core::Widgets::BaseInputControl::SetAcquiredInputDataHolder(
	DataEntityHolder::Pointer val)
{ 
	if ( m_AcquiredInputDataHolder.IsNotNull( ) )
	{
		m_AcquiredInputDataHolder->RemoveObserver(
			this, 
			&BaseInputControl::OnAcquiredInputModified ); 
	}

	m_AcquiredInputDataHolder = val; 

	m_AcquiredInputDataHolder->AddObserver(
		this, 
		&BaseInputControl::OnAcquiredInputModified ); 

	// Update selected data entity
	OnSelectedModified();

	// Reset the label
	UpdateWidget( );
};

Core::DataEntityHolder::Pointer 
Core::Widgets::BaseInputControl::GetAcquiredInputDataHolder(void) const
{ 
	return m_AcquiredInputDataHolder; 
}

bool Core::Widgets::BaseInputControl::CheckSubjectIsOk(
	Core::DataEntity::Pointer &subject )
{
	if ( !IsEnabled() )
	{
		return false;
	}

	if ( subject.IsNull( ) )
	{
		return false;
	}

	// Check if is a children of the father
	bool bTypeIsOk = false;
	bTypeIsOk = CheckFatherIsOk( subject );

	return bTypeIsOk;
}

void Core::Widgets::BaseInputControl::OnParentEnable( bool enable )
{
	Enable( enable );
}

void Core::Widgets::BaseInputControl::SetRenderingTree( 
	Core::RenderingTree::Pointer renderingTree )
{
	m_ifaceComboBox->SetRenderingTree( renderingTree );
}

void Core::Widgets::BaseInputControl::SetEnableOpacitySelection( bool enable )
{
	m_ifaceComboBox->SetEnableOpacitySelection( enable );
}

bool Core::Widgets::BaseInputControl::GetEnableOpacitySelection()
{
	return m_ifaceComboBox->GetEnableOpacitySelection();
}

Core::BaseProcessor::Pointer Core::Widgets::BaseInputControl::GetProcessor()
{
	return NULL;
}

void Core::Widgets::BaseInputControl::SetMultiRenderWindow( 
	Core::Widgets::RenderWindowBase* val )
{
	m_ifaceComboBox->SetMultiRenderWindow( val );
}
