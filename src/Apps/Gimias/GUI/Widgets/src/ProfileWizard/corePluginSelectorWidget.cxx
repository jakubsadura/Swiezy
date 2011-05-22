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

#include <blMitkUnicode.h>

#include "corePluginSelectorWidget.h"
#include "coreAssert.h"
#include "coreKernel.h"
#include "coreWxMitkGraphicalInterface.h"
#include "coreFrontEndPluginManager.h"
#include "coreReportExceptionMacros.h"

#include "blwxTreeCtrl.h"

using namespace Core::Widgets;

#define wxID_chkList (1 + wxID_HIGHEST)

BEGIN_EVENT_TABLE(PluginSelectorWidget, wxPanel)
	EVT_TREE_STATE_IMAGE_CLICK( wxID_chkList, PluginSelectorWidget::OnCheckBoxItemToggle)
END_EVENT_TABLE()

Core::Widgets::PluginSelectorWidget::PluginSelectorWidget( 
	wxWindow* parent, 
	int id /*= wxID_ANY*/, 
	const wxPoint& pos/*=wxDefaultPosition*/, 
	const wxSize& size/*=wxDefaultSize*/, 
	long style/*=0*/ ) : wxPanel( parent, id, pos, size, style )
{
	//!
	m_SelectedProfile = Core::Profile::New();

	m_ChkTree  = new blwxTreeCtrl(this,  
		wxID_chkList, 
		wxDefaultPosition, 
		wxSize(200, 300), 
		wxTR_HAS_BUTTONS|wxTR_LINES_AT_ROOT|wxTR_DEFAULT_STYLE|wxSUNKEN_BORDER
		);


	do_layout( );

	m_Modified = false;
}

//!
PluginSelectorWidget::~PluginSelectorWidget(void)
{

}

//!
void PluginSelectorWidget::do_layout(void)
{
	// layout
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(m_ChkTree, wxSizerFlags( ).Proportion(1).Expand().Border(wxALL,5) );
	SetSizer(sizer);
}



/** Fills the second page of the profile wizard with a list of checkboxes.
The list is populated with all the available profiles specified at the Settings class, and retrieved 
from it by GetCurrentUserProfile().
*/
void PluginSelectorWidget::SetupProfileCheckboxes(void)
{
	Core::Profile::Pointer availableProfiles;
	Core::Profile::ProfileSet availableSet;
	Core::Profile::Pointer userProfile;

	try
	{
		Core::Runtime::Settings::Pointer settings;
		settings = Core::Runtime::Kernel::GetApplicationSettings();
		
		Core::Runtime::wxMitkGraphicalInterface::Pointer graphicalIface;
		graphicalIface = Core::Runtime::Kernel::GetGraphicalInterface();

		Core::Runtime::FrontEndPluginManager::Pointer pManager;
		pManager = graphicalIface->GetFrontEndPluginManager();

		userProfile = settings->GetCurrentUserProfile();

		m_ChkTree->DeleteAllItems( );
		m_ChkTree->AddRoot( "Plugins" );
		m_ChkTree->SetItemImage(m_ChkTree->GetRootItem(), 1);

		// Create a checkbox with the text of each available profile type
		wxTreeItemId parentItem;
		availableProfiles = pManager->GetAvailableProfiles();
		coreAssertMacro(availableProfiles.IsNotNull());
		availableSet = availableProfiles->GetProfileAsStringSet();
		parentItem = m_ChkTree->AppendItem(m_ChkTree->GetRootItem( ), _U( "GIMIAS") );
		m_ChkTree->SetItemImage(parentItem, 1);
		UpdateTree( parentItem, availableSet, userProfile );

		availableProfiles = pManager->GetCLPProfiles();
		coreAssertMacro(availableProfiles.IsNotNull());
		availableSet = availableProfiles->GetProfileAsStringSet();
		parentItem = m_ChkTree->AppendItem(m_ChkTree->GetRootItem( ), _U( "Command Line") );
		m_ChkTree->SetItemImage(parentItem, 1);
		UpdateTree( parentItem, availableSet, userProfile );

		m_ChkTree->ExpandAll( );

		m_ChkTree->SetScrollPos(wxVERTICAL,0); 

	}
	coreCatchExceptionsAddTraceAndThrowMacro(PluginSelectorWidget::SetupProfileCheckboxes)
}

//!
void PluginSelectorWidget::OnCheckBoxItemToggle(wxTreeEvent& event)
{
	wxTreeItemId clickedItemId = event.GetItem();

	int state = m_ChkTree->GetItemImage( clickedItemId );

	// If it's a category item -> Apply to all children
	if ( m_ChkTree->GetItemParent( clickedItemId ) == m_ChkTree->GetRootItem() )
	{
		wxTreeItemIdValue cookie;
		wxTreeItemId itemId = m_ChkTree->GetFirstChild( clickedItemId, cookie );
		while( itemId.IsOk() )
		{
			m_ChkTree->SetItemImage( itemId, state );
			itemId = m_ChkTree->GetNextChild( clickedItemId, cookie);
		}
	}

	UpdateProfile( m_ChkTree->GetRootItem() );

	m_Modified = true;

	event.Skip();
}

/**
 */
Core::Profile::Pointer Core::Widgets::PluginSelectorWidget::GetSelectedProfile() const
{
	return m_SelectedProfile;
}

/**
 */
void Core::Widgets::PluginSelectorWidget::UpdateTree(
	wxTreeItemId parentItem,
	Core::Profile::ProfileSet availableSet,
	Core::Profile::Pointer userProfile )
{
	for(Profile::ProfileSet::const_iterator it = availableSet.begin(); it != availableSet.end(); ++it)
	{
		wxTreeItemId  item = m_ChkTree->AppendItem( parentItem, _U(*it));

		// Check the checkbox if the user profile matches that profile from the plugins 
		if( userProfile.IsNotNull() && userProfile->ProfileHasType(*it))
		{
			m_ChkTree->SetItemImage(item, 2);
			m_SelectedProfile->AddToProfile(*it);
		}
		else
		{
			m_ChkTree->SetItemImage(item, 1);
			m_SelectedProfile->RemoveFromProfile(*it);
		}
	}

}

void Core::Widgets::PluginSelectorWidget::UpdateProfile( wxTreeItemId item )
{
	wxTreeItemIdValue cookie;
	wxTreeItemId itemId = m_ChkTree->GetFirstChild( item, cookie );
	while( itemId.IsOk() )
	{
		if ( m_ChkTree->GetItemParent( itemId ) != m_ChkTree->GetRootItem() )
		{
			std::string itemTxt = _U(m_ChkTree->GetItemText(itemId));
			int state = m_ChkTree->GetItemImage( itemId );

			if ( state == 2 )
				m_SelectedProfile->AddToProfile(itemTxt);
			else
				m_SelectedProfile->RemoveFromProfile(itemTxt);
		}

		UpdateProfile( itemId );
		itemId = m_ChkTree->GetNextChild( item, cookie);
	}
}

void Core::Widgets::PluginSelectorWidget::UpdateData()
{
	if ( m_Modified )
	{
		Core::Runtime::Settings::Pointer settings;
		settings = Core::Runtime::Kernel::GetApplicationSettings();

		// It was not canceled, so assign the profile to the current user profile
		settings->GetCurrentUserProfile()->ClearProfile();
		settings->GetCurrentUserProfile()->MergeProfiles( GetSelectedProfile() );
		settings->SaveSettings();

		// Show a confirmation for restart
		std::string message;
		if(settings->IsFirstTimeStart())
		{
			message = \
				"The application will restart in order.\n" \
				"to apply the selected configuration.\n" \
				"Remember that you can run again this wizard\n" \
				"at any time from the application menu.\n"
				"Exit now?";
		}
		else
		{
			message = \
				"The application needs to restart in order.\n" \
				"to apply the new configuration.\n" \
				"All unsaved data will be lost if you proceed.\n" \
				"Make sure to save all your data before\n" \
				"continuing.\n" \
				"Exit now?";
		}

		wxMessageDialog* question = new wxMessageDialog(
			this, _U(message.c_str()), _U( "Exit application?" ),
			wxYES_NO | wxICON_QUESTION | wxSTAY_ON_TOP);
		if ( question->ShowModal() == wxID_YES)
		{
			Core::Runtime::wxMitkGraphicalInterface::Pointer graphicalIface;
			graphicalIface = Core::Runtime::Kernel::GetGraphicalInterface();
			if(graphicalIface.IsNotNull())
				graphicalIface->LogMessage("Restarting application now...");
			settings->SetFirstTimeStart(false);
			settings->SetPerspective( Core::Runtime::PERSPECTIVE_PLUGIN );
			Core::Runtime::Environment::Pointer runtime;
			runtime = Core::Runtime::Kernel::GetApplicationRuntime();
			coreAssertMacro(runtime.IsNotNull());
			runtime->RestartApplication();
		}

	}
}

void Core::Widgets::PluginSelectorWidget::UpdateWidget()
{
	// Fills the page with one checkbox for each available profile, and checks them for user selected profiles
	SetupProfileCheckboxes();
}
