/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreTreeRegisteredWindows.h"
#include "coreBaseWindowFactories.h"
#include "coreWxMitkGraphicalInterface.h"
#include "wx/imaglist.h"
#include "coreWorkflowTreeItemData.h"

BEGIN_EVENT_TABLE(Core::Widgets::TreeRegisteredWindows, wxTreeCtrl)
END_EVENT_TABLE()

Core::Widgets::TreeRegisteredWindows::TreeRegisteredWindows( 
	wxWindow* parent, wxWindowID id, const wxPoint& pos /*= wxDefaultPosition*/, 
	const wxSize& size /*= wxDefaultSize*/, long style /*= wxTR_HAS_BUTTONS*/, 
	const wxValidator& validator /*= wxDefaultValidator*/, 
	const wxString& name /*= "treeCtrl"*/ )
	: BaseTreeWorkflow( parent, id, pos, size, style, validator, name )
{
	SetWindowStyleFlag( GetWindowStyleFlag() | wxTR_MULTIPLE );

	wxImageList *images = new wxImageList(20, 18, true);
	images->Add( wxIcon( workflow_xpm ) );
	images->Add( wxIcon( tab_xpm ) );
	images->Add( wxIcon( processing_xpm ) );
	images->Add( wxIcon( folder_xpm ) );
	images->Add( wxIcon( window_xpm ) );
	AssignImageList( images );

}


void Core::Widgets::TreeRegisteredWindows::UpdateRegisteredWindows()
{
	DeleteAllItems();
	AddRoot(wxT("Root"), -1, -1);

	BaseWindowFactories::Pointer baseWindowFactory;
	baseWindowFactory = Core::Runtime::Kernel::GetGraphicalInterface()->GetBaseWindowFactory( );

	wxTreeItemId commonItemId = AppendItem( GetRootItem(), "Common", IMG_FOLDER );

	std::list<std::string> windowsList;
	windowsList = baseWindowFactory->GetWindowsList( );
	std::list<std::string>::iterator it;
	for ( it = windowsList.begin( ) ; it != windowsList.end() ; it++ )
	{
		std::string factoryName = *it;
		WindowConfig config;
		baseWindowFactory->GetWindowConfig( factoryName, config );

		bool isProcessingTool = config.GetProcessingTool() || config.GetSelectionTool();
		// These kind of windows are created by other windows
		if ( !config.GetPluginTab() && !isProcessingTool )
		{
			continue;
		}

		std::string pluginName = config.GetPluginName();
		if ( pluginName.empty() )
		{
			pluginName = "Common";
		}

		wxTreeItemId itemId = AddPluginItem( config.GetPluginName() );
		int image = -1;
		if ( config.GetCommandPanel() || config.GetProcessingTool( ) || config.GetSelectionTool() )
		{
			image = IMG_PROCESSING;
		}
		else
		{
			image = IMG_WINDOW;
		}
		AppendItem( itemId, config.GetCaption(), image, factoryName );
	}

	ExpandAll();
	Collapse( commonItemId );
}

wxTreeItemId Core::Widgets::TreeRegisteredWindows::AddPluginItem( std::string name )
{
	wxTreeItemId itemId = GetRootItem();

	if ( name.empty() )
	{
		name = "Common";
	}

	wxTreeItemIdValue cookie;
	bool found = false;
	wxTreeItemId childItemId = GetFirstChild( itemId, cookie );
	while ( childItemId.IsOk() && !found )
	{
		if ( GetItemText( childItemId ) == name )
		{
			found = true;
			itemId = childItemId;
		}
		childItemId = GetNextChild( childItemId, cookie );
	}

	// Add plugin name
	if ( !found )
	{
		itemId = AppendItem( itemId, name, IMG_FOLDER );
	}

	return itemId;
}
