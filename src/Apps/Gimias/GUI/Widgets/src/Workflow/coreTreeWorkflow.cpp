/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreTreeWorkflow.h"
#include "coreBaseWindowFactories.h"
#include "coreWxMitkGraphicalInterface.h"
#include "corePluginTabFactory.h"
#include "coreWorkflowTreeItemData.h"

#include "wx/imaglist.h"

#define wxID_AddStep wxID( "wxID_AddStep" )

BEGIN_EVENT_TABLE(Core::Widgets::TreeWorkflow, wxTreeCtrl)
  EVT_MENU		(wxID_AddStep, Core::Widgets::TreeWorkflow::OnMenuAddStep)
  EVT_KEY_DOWN	(Core::Widgets::TreeWorkflow::OnKeyPressEvent)
END_EVENT_TABLE()

const std::string STR_PROCESSING_STEPS = "Processing steps";
const std::string STR_WINDOWS = "Windows";
const std::string STR_ALTERNATIVES = "Alternatives";

/** 
\brief Drop target to visually select the drop item 
\ingroup gmWidgets
\author Xavi Planes
\date 1 June 2010
*/
class DropTarget : public wxTextDropTarget
{
public:
	//!
	DropTarget( Core::Widgets::TreeWorkflow* tree )
	{
		m_Tree = tree;
	}

private:


	//!
	virtual bool OnDropText(wxCoord x, wxCoord y, const wxString& text)
	{
		return true;
	}

	virtual wxDragResult OnDragOver(wxCoord x, wxCoord  y, wxDragResult def)
	{
		// provide visual feedback by selecting the item the mouse is over
		wxTreeItemId item = m_Tree->HitTest( wxPoint( x, y ) );
		wxArrayTreeItemIds selections;
		m_Tree->GetSelections( selections );

		if ( !item.IsOk() )
		{
			m_Tree->UnselectAll();
			return def;
		}

		if ( item == m_Tree->GetRootItem() )
		{
			return def;
		}

		bool itemIsSelected = selections.size( ) && selections[ 0 ] == item;
		if ( itemIsSelected )
		{
			return def;
		}

		if ( m_Tree->GetRestrictedSelection() && 
			 !( m_Tree->IsChildOfRoot( item ) || m_Tree->IsProcessingStep( item ) ) )
		{
			return def;
		}

		m_Tree->SetFocus();
		m_Tree->UnselectAll();
		m_Tree->SelectItem(item);

		return def;

	}

	//!
	Core::Widgets::TreeWorkflow* m_Tree;
};

Core::Widgets::TreeWorkflow::TreeWorkflow( 
	wxWindow* parent, wxWindowID id, const wxPoint& pos /*= wxDefaultPosition*/, 
	const wxSize& size /*= wxDefaultSize*/, long style /*= wxTR_HAS_BUTTONS*/, 
	const wxValidator& validator /*= wxDefaultValidator*/, 
	const wxString& name /*= "treeCtrl"*/ )
	: BaseTreeWorkflow( parent, id, pos, size, style | wxTR_EDIT_LABELS | wxTR_MULTIPLE, validator, name )
{

	wxImageList *images = new wxImageList(20, 18, true);
	images->Add( wxIcon( workflow_xpm ) );
	images->Add( wxIcon( tab_xpm ) );
	images->Add( wxIcon( processing_xpm ) );
	images->Add( wxIcon( folder_xpm ) );
	images->Add( wxIcon( window_xpm ) );
	AssignImageList( images );

	SetDropTarget( new DropTarget( this ) );
	m_RestrictedSelection = false;

	Connect( wxEVT_COMMAND_TREE_ITEM_RIGHT_CLICK, 
		wxTreeEventHandler(TreeWorkflow::OnRightClick ) );
	Connect( wxEVT_COMMAND_TREE_BEGIN_LABEL_EDIT, 
		wxTreeEventHandler(TreeWorkflow::OnBeginLabelEdit ) );
	
}

void Core::Widgets::TreeWorkflow::SetWorkflow( Core::Workflow::Pointer workflow )
{
	m_Workflow = workflow;

	DeleteAllItems();
	if ( workflow.IsNull() )
	{
		return;
	}

	AddRoot(wxT( workflow->GetName() ), IMG_WORKFLOW, -1);

	BaseWindowFactories::Pointer baseWindowFactory;
	baseWindowFactory = Core::Runtime::Kernel::GetGraphicalInterface()->GetBaseWindowFactory( );

	Workflow::StepListType::iterator it;
	for ( it = workflow->GetStepVector( ).begin() ; it != workflow->GetStepVector( ).end( ) ; it++ )
	{
		WorkflowStep::Pointer step = (*it);

		wxTreeItemId stepItemId;
		stepItemId = AppendItem( GetRootItem(), step->GetName(), IMG_TAB );
		wxTreeItemId subStepsItemId;
		subStepsItemId = AppendItem( stepItemId, STR_PROCESSING_STEPS, IMG_FOLDER );
		wxTreeItemId windowsItemId;
		windowsItemId = AppendItem( stepItemId, STR_WINDOWS, IMG_FOLDER );

		Expand( stepItemId );

		// Add sub steps
		WorkflowStep::SubStepListType::iterator it;
		for ( it = step->GetSubStepVector().begin();
			it != step->GetSubStepVector().end();
			it++ )
		{
			Core::WorkflowSubStep::Pointer workflowSubStep = *it;
			wxTreeItemId subStepItemId;
			subStepItemId = AppendWidget( 
				subStepsItemId, workflowSubStep->GetName(), IMG_PROCESSING );

			wxTreeItemId alternativesItemId;
			alternativesItemId = AppendItem( subStepItemId, STR_ALTERNATIVES, IMG_FOLDER );

			// Add alternatives
			WorkflowSubStep::AlternativesListType::iterator it;
			if ( workflowSubStep->GetAlternatives().size() > 1 )
			{
				for ( it = workflowSubStep->GetAlternatives().begin();
					it != workflowSubStep->GetAlternatives().end();
					it++ )
				{
					AppendWidget( alternativesItemId, *it, IMG_PROCESSING );
				}
			}
		}

		// Add windows
		WorkflowStep::WindowListType::iterator itWin;
		for ( itWin = step->GetWindowList().begin();
			itWin != step->GetWindowList().end();
			itWin++ )
		{
			AppendWidget( windowsItemId, (*itWin), IMG_WINDOW );
		}

	}

	Expand( GetRootItem() );
}

void Core::Widgets::TreeWorkflow::AddItems( std::list<std::string> &items )
{
	wxArrayTreeItemIds selections;
	GetSelections( selections );
	if ( selections.size() == 0 )
	{
		return;
	}

	wxTreeItemId destItem = selections[ 0 ];

	BaseWindowFactories::Pointer baseWindowFactory;
	baseWindowFactory = Core::Runtime::Kernel::GetGraphicalInterface()->GetBaseWindowFactory( );

	wxTreeItemId parentItemId = GetItemParent( destItem );
	if ( destItem == GetRootItem() )
	{
		return;
	}

	// Workflow Step
	wxTreeItemId stepItem = GetWorkflowStepItem( destItem );


	// Alternatives
	wxTreeItemId alternativesItemId; 
	if ( GetItemText( destItem ) == STR_ALTERNATIVES )
	{
		alternativesItemId = destItem;
	}
	else if ( GetItemText( parentItemId ) == STR_ALTERNATIVES )
	{
		alternativesItemId = parentItemId;
	}

	if ( alternativesItemId.IsOk() )
	{
		std::list<std::string>::iterator it;
		for ( it = items.begin() ; it != items.end() ; it++ )
		{
			WindowConfig config;
			baseWindowFactory->GetWindowConfig( *it, config );
			if ( config.GetCommandPanel() || config.GetProcessingTool( ) )
			{
				AppendWidget( alternativesItemId, *it, IMG_PROCESSING );
			}
		}
		ExpandAllChildren( alternativesItemId );
	
	}
	// Rest of cases
	else if ( stepItem.IsOk() )
	{
		std::list<std::string>::iterator it;
		for ( it = items.begin() ; it != items.end() ; it++ )
		{
			if ( baseWindowFactory->FindFactory( *it ).IsNull( ) )
			{
				continue;
			}

			WindowConfig config;
			baseWindowFactory->GetWindowConfig( *it, config );

			if ( config.GetCommandPanel() || config.GetProcessingTool( ) || config.GetSelectionTool() )
			{
				wxTreeItemId subStep = AppendWidget( GetSubStepsItem( stepItem ), *it, IMG_PROCESSING );
				AppendItem( subStep, STR_ALTERNATIVES, IMG_FOLDER );
				Expand( GetSubStepsItem( stepItem ) );
			}
			else
			{
				// If item is not present
				if ( !GetChildItem( GetWindowsItem( stepItem ), *it ).IsOk( ) )
				{
					AppendWidget( GetWindowsItem( stepItem ), *it, IMG_WINDOW );
				}
			}
		}
	}


}

wxTreeItemId Core::Widgets::TreeWorkflow::GetChildItem( 
	const wxTreeItemId &stepItemId, const std::string factoryName )
{
	wxTreeItemIdValue cookie;
	wxTreeItemId child = GetFirstChild( stepItemId, cookie );
	while ( child.IsOk() )
	{
		if ( GetItemFactoryName( child ) == factoryName )
		{
			return child;
		}
		child = GetNextChild( stepItemId, cookie );
	}

	return wxTreeItemId();
}

wxTreeItemId Core::Widgets::TreeWorkflow::GetWindowsItem( const wxTreeItemId &stepItemId )
{
	return GetChildItem( stepItemId, STR_WINDOWS );
}

wxTreeItemId Core::Widgets::TreeWorkflow::GetSubStepsItem( const wxTreeItemId &stepItemId )
{
	return GetChildItem( stepItemId, STR_PROCESSING_STEPS );
}

wxTreeItemId Core::Widgets::TreeWorkflow::GetWorkflowStepItem( wxTreeItemId itemId )
{
	while ( itemId.IsOk() )
	{
		if ( GetItemParent( itemId ) == GetRootItem() )
		{
			return itemId;
		}
		itemId = GetItemParent( itemId );
	}

	return wxTreeItemId( );
}

void Core::Widgets::TreeWorkflow::DeleteItems( wxArrayTreeItemIds items )
{
	for ( size_t i = 0 ; i < items.size() ; i++ )
	{
		if ( GetItemText( items[ i ] ) != STR_PROCESSING_STEPS &&
			 GetItemText( items[ i ] ) != STR_WINDOWS &&
			 GetItemText( items[ i ] ) != STR_ALTERNATIVES )
		{
			Delete( items[ i ] );
		}
	}
}

bool Core::Widgets::TreeWorkflow::GetRestrictedSelection() const
{
	return m_RestrictedSelection;
}

void Core::Widgets::TreeWorkflow::SetRestrictedSelection( bool val )
{
	m_RestrictedSelection = val;
}

void Core::Widgets::TreeWorkflow::OnRightClick( wxTreeEvent& event )
{
	if ( event.GetItem() != GetRootItem() )
	{
		return;
	}

	wxMenu* menu = new wxMenu;
	menu->Append(wxID_AddStep, wxT("&Add a step"), wxT("Add a step"));
	PopupMenu(menu);
	delete menu;
}

void Core::Widgets::TreeWorkflow::OnMenuAddStep( wxCommandEvent& event )
{
	wxTreeItemId stepItemId = AppendItem( GetRootItem(), "NewStep", IMG_TAB );
	AppendItem( stepItemId, STR_PROCESSING_STEPS, IMG_FOLDER );
	wxTreeItemId windowsItemId = AppendItem( stepItemId, STR_WINDOWS, IMG_FOLDER );
	ExpandAllChildren( stepItemId );

	// Add common widgets list
	std::list<std::string> windowsList;
	windowsList = Core::Widgets::PluginTabFactory::GetCommonWidgetsList( "NewStep" );
	std::list<std::string>::iterator it;
	for ( it = windowsList.begin( ) ; it != windowsList.end() ; it++ )
	{
		AppendWidget( windowsItemId, *it, IMG_WINDOW );
	}
}

void Core::Widgets::TreeWorkflow::OnBeginLabelEdit( wxTreeEvent& event )
{
	if ( event.GetItem() == GetRootItem() )
	{
		event.Veto();
		return;
	}

	if ( GetItemParent( event.GetItem() ) != GetRootItem() )
	{
		event.Veto();
		return;
	}
	
	event.Skip();
}

void Core::Widgets::TreeWorkflow::OnKeyPressEvent(wxKeyEvent& event)
{
	wxArrayTreeItemIds selections;
	GetSelections( selections );

	if ( event.GetKeyCode() == WXK_F2 && selections.size( ) )
	{
		EditLabel( selections[ 0 ] );
	}
	event.Skip();
}

void Core::Widgets::TreeWorkflow::UpdateWorkflow()
{
	if ( m_Workflow.IsNull() )
	{
		return;
	}

	m_Workflow->Clear( );

	// Root
	m_Workflow->SetName( GetItemText( GetRootItem() ).c_str() );

	// Steps
	wxTreeItemIdValue cookie1;
	wxTreeItemId stepItemId = GetFirstChild( GetRootItem(), cookie1 );
	while ( stepItemId.IsOk() )
	{
		WorkflowStep::Pointer step;
		step = WorkflowStep::New( GetItemText( stepItemId ).c_str() );

		// Sub steps
		wxTreeItemId substepsItemId = GetSubStepsItem( stepItemId );
		wxTreeItemIdValue cookie2;
		wxTreeItemId subStepsChildItemId = GetFirstChild( substepsItemId, cookie2 );
		while ( subStepsChildItemId.IsOk() )
		{
			WorkflowSubStep::Pointer subStep;
			subStep = WorkflowSubStep::New( GetItemFactoryName( subStepsChildItemId ).c_str() );
			step->AddSubStep( subStep );
			
			// Alternatives
			wxTreeItemIdValue cookie3;
			wxTreeItemId alternativesItemId = GetFirstChild( subStepsChildItemId, cookie3 );
			if ( alternativesItemId.IsOk() )
			{
				wxTreeItemIdValue cookie4;
				wxTreeItemId subChild = GetFirstChild( alternativesItemId, cookie4 );
				while ( subChild.IsOk() )
				{
					subStep->AddAlternative( GetItemFactoryName( subChild ).c_str() );
					subChild = GetNextChild( alternativesItemId, cookie4 );
				}
			}

			subStepsChildItemId = GetNextChild( substepsItemId, cookie2 );
		}

		// Windows
		wxTreeItemId windowsItemId = GetWindowsItem( stepItemId );
		
		// Check that working area is ok
		CheckWorkingAreaManager( windowsItemId );

		wxTreeItemIdValue cookie5;
		wxTreeItemId windowsChildItemId = GetFirstChild( windowsItemId, cookie5 );
		while ( windowsChildItemId.IsOk() )
		{
			step->AddWindow( GetItemFactoryName( windowsChildItemId ).c_str() );
			windowsChildItemId = GetNextChild( windowsItemId, cookie5 );
		}


		m_Workflow->AddStep( step );

		stepItemId = GetNextChild( GetRootItem(), cookie1 );
	}

	UpdatePluginNamesList();
}

void Core::Widgets::TreeWorkflow::AddPluginName( 
	const std::string &factoryName, 
	Workflow::PluginNamesListType &pluginNamesList )
{
	BaseWindowFactories::Pointer baseWindowFactory;
	baseWindowFactory = Core::Runtime::Kernel::GetGraphicalInterface()->GetBaseWindowFactory( );

	WindowConfig config;
	baseWindowFactory->GetWindowConfig( factoryName, config );
	if ( config.GetPluginName().empty() )
	{
		return;
	}

	Workflow::PluginNamesListType::iterator it;
	it = std::find( pluginNamesList.begin( ), pluginNamesList.end( ), config.GetPluginName() );
	if ( it != pluginNamesList.end() )
	{
		return;
	}

	pluginNamesList.push_back( config.GetPluginName() );
}

void Core::Widgets::TreeWorkflow::UpdatePluginNamesList()
{
	Workflow::PluginNamesListType pluginNamesList;

	// Update plugin names list
	Workflow::StepListType::iterator it;
	for ( it = m_Workflow->GetStepVector( ).begin() ; it != m_Workflow->GetStepVector( ).end( ) ; it++ )
	{
		WorkflowStep::Pointer step = (*it);
		WorkflowStep::SubStepListType::iterator it;
		for ( it = step->GetSubStepVector().begin();
			it != step->GetSubStepVector().end();
			it++ )
		{
			Core::WorkflowSubStep::Pointer workflowSubStep = *it;
			AddPluginName( workflowSubStep->GetName(), pluginNamesList );

			WorkflowSubStep::AlternativesListType::iterator it;
			if ( workflowSubStep->GetAlternatives().size() > 1 )
			{
				for ( it = workflowSubStep->GetAlternatives().begin();
					it != workflowSubStep->GetAlternatives().end();
					it++ )
				{
					AddPluginName( *it, pluginNamesList );
				}
			}

		}

		WorkflowStep::WindowListType::iterator itWindows;
		for ( itWindows = step->GetWindowList().begin();
			itWindows != step->GetWindowList().end();
			itWindows++ )
		{
			AddPluginName( *itWindows, pluginNamesList );
		}

	}

	m_Workflow->SetPluginNamesList( pluginNamesList );
}

wxTreeItemId  Core::Widgets::TreeWorkflow::AppendWidget( 
	const wxTreeItemId& parent, const wxString& factoryName, int image, bool last )
{
	BaseWindowFactories::Pointer baseWindowFactory;
	baseWindowFactory = Core::Runtime::Kernel::GetGraphicalInterface()->GetBaseWindowFactory( );
	
	WindowConfig config;
	bool factoryFound = baseWindowFactory->GetWindowConfig( factoryName.c_str(), config );
	if ( !factoryFound )
	{
		config.Caption( factoryName.c_str() );
	}

	wxTreeItemId item;
	if ( last )
	{
		item = AppendItem( parent, config.GetCaption(), image, factoryName );
	}
	else
	{
		item = InsertItem( parent, 0, config.GetCaption(), image, factoryName );
	}

	// Check color of font
	if ( !factoryFound )
	{
		wxColour colour = *wxRED;
		SetItemTextColour( item, colour );
	}

	return item;
}

void Core::Widgets::TreeWorkflow::MoveItems( 
	wxArrayTreeItemIds srcItems, wxArrayTreeItemIds destItems )
{
	if ( destItems.size() != 1 || srcItems.size() != 1 )
	{
		return;
	}
	wxTreeItemId srcItem = srcItems[ 0 ];
	wxTreeItemId destItem = destItems[ 0 ];

	bool childsOfRoot = IsChildOfRoot( srcItem ) && IsChildOfRoot( destItem );
	bool processingSteps = IsProcessingStep( srcItem ) && IsProcessingStep( destItem );
	if ( !( childsOfRoot || processingSteps ) )
	{
		return;
	}

	wxTreeItemId newItem = wxTreeCtrl::InsertItem( GetItemParent( srcItem ), destItem, GetItemText( srcItem ) );
	CopyItem( newItem, srcItem );
	CopyChildren( newItem, srcItem );
	Delete( srcItem );
	ExpandAllChildren( newItem );
	UnselectAll( );

}

void Core::Widgets::TreeWorkflow::CopyChildren( 
	wxTreeItemId dest, wxTreeItemId src, bool recursively /*= true */ )
{

	wxTreeItemIdValue cookie;
	wxTreeItemId nextChild = GetFirstChild( src, cookie );
	while ( nextChild.IsOk() )
	{
		wxTreeItemId newChild = AppendItem( dest, GetItemText(nextChild) );
		CopyItem( newChild, nextChild );

		// If the child has children of its own and recursive is
		// True, call CopyChildren on this child
		if ( recursively && ItemHasChildren ( nextChild ) )
		{
			CopyChildren( newChild, nextChild, true);
		}

		// Get the next child
		nextChild = GetNextChild(src, cookie);
	}


}

void Core::Widgets::TreeWorkflow::CopyItem( wxTreeItemId dest, wxTreeItemId src )
{
	SetItemText( dest, GetItemText(src) );
	SetItemImage( dest, GetItemImage(src) );
	SetItemImage( dest, GetItemImage( src, wxTreeItemIcon_Selected), wxTreeItemIcon_Selected );
	SetItemData( dest, new WindowItemData( GetItemFactoryName( src ) ) );
}

bool Core::Widgets::TreeWorkflow::IsProcessingStep( wxTreeItemId item )
{
	if ( !GetItemParent( item ).IsOk() )
	{
		return false;
	}

	return GetItemText( GetItemParent( item ) ) == STR_PROCESSING_STEPS;
}

bool Core::Widgets::TreeWorkflow::IsChildOfRoot( wxTreeItemId item )
{
	if ( !GetItemParent( item ).IsOk() )
	{
		return false;
	}

	return GetItemParent( item ) == GetRootItem();
}

void Core::Widgets::TreeWorkflow::CheckWorkingAreaManager( wxTreeItemId windowsItem )
{

	BaseWindowFactories::Pointer baseWindowFactory;
	baseWindowFactory = Core::Runtime::Kernel::GetGraphicalInterface()->GetBaseWindowFactory( );

	bool isWorkingAreaPresent = false;
	wxTreeItemIdValue cookie5;
	wxTreeItemId windowsChildItemId = GetFirstChild( windowsItem, cookie5 );
	while ( windowsChildItemId.IsOk() )
	{
		WindowConfig config;
		baseWindowFactory->GetWindowConfig( GetItemFactoryName( windowsChildItemId ), config );
		if ( config.GetWorkingArea() )
		{
			isWorkingAreaPresent = true;
		}
		windowsChildItemId = GetNextChild( windowsItem, cookie5 );
	}


	if ( !isWorkingAreaPresent )
	{
		return;
	}

	std::list<std::string> list = baseWindowFactory->GetWindowsList( 
		WIDGET_TYPE_MAX, "", "", wxID_WorkingArea );
	if ( list.empty() )
	{
		return ;
	}
	std::string name = *list.begin();

	if ( !GetChildItem( windowsItem, name ).IsOk( ) )
	{
		AppendWidget( windowsItem, name, IMG_WINDOW, false );
	}
}
