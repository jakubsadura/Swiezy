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

#include "coreDataEntityListBrowser.h"
#include "coreAssert.h"
#include "coreDataEntityList.txx"
#include "coreReportExceptionMacros.h"
#include "coreDataEntityInfoHelper.h"
#include "coreDataContainer.h"
#include "coreKernel.h"
#include "coreDataEntityHelper.h"
#include <wx/treectrl.h>
#include <wxUnicode.h>
#include "wxID.h"
#include "coreDataEntityTreeCtrl.h"
#include "coreMultiRenderWindow.h"
#include "coreWxMitkGraphicalInterface.h"

#include "itkCommand.h"

#include "DataTree.xpm"

using namespace Core::Widgets;

#define wxID_TreeControl	(1 + wxID_HIGHEST)
#define wxID_RenderButton	(2 + wxID_HIGHEST)
#define wxID_UnloadButton	(3 + wxID_HIGHEST)
#define wxID_RemoveDataEntity		wxID("wxID_RemoveDataEntity")

BEGIN_EVENT_TABLE(DataEntityListBrowser, wxPanel)
	EVT_TREE_SEL_CHANGED(wxID_TreeControl, DataEntityListBrowser::OnItemSelectedFromList)
	EVT_BUTTON(wxID_RenderButton, DataEntityListBrowser::OnButtonRenderDataEntity)
	EVT_BUTTON(wxID_UnloadButton, DataEntityListBrowser::OnButtonRemoveDataEntity)
	EVT_TREE_END_LABEL_EDIT(wxID_TreeControl,DataEntityListBrowser::EndLabelEdit )
	EVT_MENU	(wxID_RemoveDataEntity, DataEntityListBrowser::OnRemoveDataEntity)
	EVT_TREE_ITEM_RIGHT_CLICK( wxID_TreeControl, DataEntityListBrowser::OnRightMouseButtonClick)
	EVT_TREE_STATE_IMAGE_CLICK( wxID_TreeControl, DataEntityListBrowser::OnItemStateClick)
	EVT_TREE_END_DRAG( wxID_TreeControl, DataEntityListBrowser::OnItemEndDrag)
	EVT_KEY_DOWN(DataEntityListBrowser::OnKeyPressEvent)
END_EVENT_TABLE()

const wxColor selectedInputColor = wxColor( 224, 224, 255 );
const wxColor unselectedInputColor = wxColor( 255, 255, 255 );




///
//!
DataEntityListBrowser::DataEntityListBrowser(
	wxWindow* parent, 
	wxWindowID id, 
	const wxPoint& pos, 
	const wxSize& size, 
	long style, 
	const wxString& name)
: wxPanel(parent, id, pos, size, style, name)
{
	SetBitmap( datatree_xpm );

	// Create the widgets
	m_TreeControl = new Core::Widgets::DataEntityTreeCtrl(
		this, 
		wxID_TreeControl, 
		wxDefaultPosition, 
		wxDefaultSize, 
		wxTR_HAS_BUTTONS|wxTR_LINES_AT_ROOT|wxTR_DEFAULT_STYLE|wxSUNKEN_BORDER|wxTR_EDIT_LABELS|wxTR_FULL_ROW_HIGHLIGHT);
	m_UnloadButton = new wxButton(this, wxID_UnloadButton, wxT("Unload selected"));
	m_RightClickDataEntityID = 0;

	// Layout the widgets
	wxBoxSizer* hlayout = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* vlayout = new wxBoxSizer(wxVERTICAL);
	hlayout->Add(m_UnloadButton, 0, wxEXPAND);
	vlayout->Add(m_TreeControl, 1, wxEXPAND);
	vlayout->Add(hlayout, 0, wxEXPAND);

	// apply the layout 
	SetSizer(vlayout);

	m_disableSelection = false;

	Core::Runtime::wxMitkGraphicalInterface::Pointer gIface;
	gIface = Core::Runtime::Kernel::GetGraphicalInterface();
	Core::Runtime::StyleManager::Pointer styleManager = gIface->GetStyleManager();
	styleManager->ConfigurePageContainer( this );

	InitConnections( );
}

//!
DataEntityListBrowser::~DataEntityListBrowser(void)
{
	m_RenderingTreeObserver.disconnect( );
	m_RenderingTreeObserverAnyNode.disconnect( );

	if(m_ConnOnAdd.connected())
		m_ConnOnAdd.disconnect(); 
	if(m_ConnOnRemove.connected())
		m_ConnOnRemove.disconnect();
	m_SelectedDataEntityHolder->RemoveObserver(
		this, 
		&Core::Widgets::DataEntityListBrowser::HighlightCurrentSelected, 
		Core::DH_SUBJECT_MODIFIED_OR_NEW_SUBJECT);
}

void DataEntityListBrowser::InitConnections( )
{
	Core::DataEntityList::Pointer dataEntityList;
	dataEntityList = Core::Runtime::Kernel::GetDataContainer( )->GetDataEntityList( );

	Core::DataEntityHolder::Pointer currentProcessing;
	currentProcessing = dataEntityList->GetSelectedDataEntityHolder( );

	m_SelectedDataEntityHolder = currentProcessing;
	m_DataEntityList = dataEntityList;

	m_TreeControl->SetDataEntityList( m_DataEntityList );

	m_ConnOnAdd = m_DataEntityList->AddObserverOnAddDataEntity<DataEntityListBrowser>(
		this, 
		&DataEntityListBrowser::OnDataEntityAddedToList);
	m_ConnOnRemove = m_DataEntityList->AddObserverOnRemoveDataEntity<DataEntityListBrowser>(
		this, 
		&DataEntityListBrowser::OnDataEntityRemovedFromList);

	// Highlight the current selected data entity
	m_SelectedDataEntityHolder->AddObserver(
		this, 
		&Core::Widgets::DataEntityListBrowser::HighlightCurrentSelected, 
		Core::DH_SUBJECT_MODIFIED_OR_NEW_SUBJECT);
	//m_SelectedDataEntityPositionInList= 0;

	//m_MeshCreationProcessor = Core::MeshCreationProcessor::New();
}



/**
Envent handler fired when the user selected an item in the list of this 
browser. It changes the current 
selected DataEntity in the Core::DataContainer, so every observer of the 
current selected data entity will
get noticed by observing its holder.
\sa DataContainer
*/
void DataEntityListBrowser::OnItemSelectedFromList(wxTreeEvent& event)
{
	if ( m_disableSelection )
	{
		return;
	}

	try
	{
		wxTreeItemId item = event.GetItem();
		Core::DataEntity::Pointer dataEntity = GetDataEntity( item );
		m_SelectedDataEntityHolder->SetSubject(dataEntity);

		if ( GetMultiRenderWindow() != NULL )
		{
			GetMultiRenderWindow()->OnDataSelected( dataEntity );
		}
	}
	coreCatchExceptionsReportAndNoThrowMacro(
		DataEntityListBrowser::OnItemSelectedFromList)
}

Core::DataEntity::Pointer DataEntityListBrowser::GetDataEntity( 
	wxTreeItemId &item )
{
	return m_TreeControl->GetItemDataEntity( item );
}

/**
Envent handler fired when the user deselects an item in the list of this browser. 
It changes the current selected DataEntity in the Core::DataContainer, 
so every observer of the 
current selected data entity will get noticed by observing its holder.
\sa DataContainer
*/
void DataEntityListBrowser::OnItemUnSelectedFromList(wxListEvent& event)
{
	try
	{
		m_SelectedDataEntityHolder->SetSubject(NULL);
	}
	coreCatchExceptionsReportAndNoThrowMacro(
		DataEntityListBrowser::OnItemUnSelectedFromList)
}

/**
Slot function observing the OnAdded signal emitted by the DataEntityList 
when a DataEntity is added to it.
It will add also the dataEntity to the list of this browser.
*/
void DataEntityListBrowser::OnDataEntityAddedToList(
	Core::DataEntity::Pointer dataEntity)
{
	try
	{
		if(dataEntity.IsNotNull())
			AddDataEntityToList(dataEntity);
	}
	coreCatchExceptionsReportAndNoThrowMacro(
		DataEntityListBrowser::OnDataEntityAddedToList)
}

/**
Slot function observing the OnRemove signal emitted by the DataEntityList 
when a DataEntity is removed from it.
It will remove the dataEntity from the list of this browser.
*/
void DataEntityListBrowser::OnDataEntityRemovedFromList(
	Core::DataEntity::Pointer dataEntity)
{
	try
	{
		if(dataEntity.IsNotNull())
			RemoveDataEntityFromList(dataEntity);
	}
	coreCatchExceptionsReportAndNoThrowMacro(
		DataEntityListBrowser::OnDataEntityRemovedFromList)
}

/**
Adds an item (row) to the list of the browser describing the data entity
*/
void DataEntityListBrowser::AddDataEntityToList(
	Core::DataEntity::Pointer dataEntity)
{
	coreAssertMacro(dataEntity.IsNotNull() 
		&& "You may not have a NULL data entity in the data entity list");
	
	try
	{
		wxTreeItemId k = m_TreeControl->AppendItem( dataEntity );

		bool OK = k.IsOk();
		if ( OK == false )
		{
			Core::Exceptions::CannotInsertDataEntityInfoException e(
				"DataEntityListBrowser::AddDataEntityToList");
			throw e;
		}

		UpdateRenderingTreeState( dataEntity );

		// Keep the previous selected data entity. When appending a new 
		// data entity to the tree, it automatically selects the data entity. 
		// Whe change the selection to keep the previous selected data entity
		HighlightCurrentSelected();

		/* Add an observer in order to update all the data entity list of 
		all the plugins when a data entity is renamed
		*/
		dataEntity->GetMetadata( )->AddObserverOnChangedTag<DataEntityListBrowser>(
			this, 
			&DataEntityListBrowser::OnRenamedDataEntity);

		AdjustLayerProperty( );
	}
	coreCatchExceptionsAddTraceAndThrowMacro(
		DataEntityListBrowser::AddDataEntityToList)
}


/**
Adds an item (row) from the list of the browser that was describing the 
data entity
*/
void DataEntityListBrowser::RemoveDataEntityFromList(
	Core::DataEntity::Pointer dataEntity)
{
	coreAssertMacro(dataEntity.IsNotNull() 
		&& "You may not have a NULL data entity in the data entity list");
	wxTreeItemId  item;

	try
	{
		item = m_TreeControl->GetItem( dataEntity );
	}
	coreCatchExceptionsAddTraceAndThrowMacro(
		DataEntityListBrowser::RemoveDataEntityFromList)
	
	bool OK = item.IsOk();
	if( OK == false)
	{
		Core::Exceptions::CannotRemoveDataEntityInfoException e(
			"DataEntityListBrowser::RemoveDataEntityFromList");
		throw e;
	}
	try
	{
		// If it was selected, deselect it
		if ( dataEntity == m_SelectedDataEntityHolder->GetSubject())
			m_SelectedDataEntityHolder->SetSubject(NULL);
		//! delete the item and its children
		m_TreeControl->Delete(item);

		// Remove node from map
		if ( GetRenderingTree( ).IsNotNull() )
		{
			// Remove node from rendering tree
			GetRenderingTree( )->Remove(dataEntity);
		}


	}
	coreCatchExceptionsAddTraceAndThrowMacro(
		DataEntityListBrowser::RemoveDataEntityFromList)
}

/**
Renews the list and updates its contents with the contents of the 
Core::DataEntityList
*/
void DataEntityListBrowser::UpdateList(void)
{
	try
	{
		m_TreeControl->Update();
	}
	coreCatchExceptionsAddTraceAndThrowMacro(DataEntityListBrowser::UpdateList)
}

void Core::Widgets::DataEntityListBrowser::OnButtonRemoveDataEntity( 
	wxCommandEvent &event )
{
	try
	{
		m_DataEntityList->RemoveSelectedDataEntity();
	}
	coreCatchExceptionsReportAndNoThrowMacro(
		DataEntityListBrowser::OnButtonRemoveDataEntity)
}

void Core::Widgets::DataEntityListBrowser::OnButtonRenderDataEntity( 
	wxCommandEvent &event )
{
	if ( GetRenderingTree( ).IsNull() )
	{
		return;
	}

	try
	{
		Core::DataEntity::Pointer dataEntity = m_SelectedDataEntityHolder->GetSubject();
		GetRenderingTree( )->Add( dataEntity );
	}
	coreCatchExceptionsReportAndNoThrowMacro(
		DataEntityListBrowser::OnButtonRenderDataEntity)
}


void Core::Widgets::DataEntityListBrowser::HighlightCurrentSelected()
{
	if ( m_SelectedDataEntityHolder->GetSubject().IsNotNull())
	{
		wxTreeItemId  selectedItem;
		selectedItem = m_TreeControl->GetItem( m_SelectedDataEntityHolder->GetSubject() );
		
		if ( selectedItem.IsOk() && 
			 selectedItem != m_TreeControl->GetSelection() )
		{
			m_disableSelection = true;
			m_TreeControl->UnselectAll();
			m_TreeControl->SelectItem(selectedItem, true);
			m_disableSelection = false;
		}
	}
}

/**
*/
void Core::Widgets::DataEntityListBrowser::SetSelectedInput( 
	int dataEntityId, 
	bool bTrue )
{
	for (unsigned i = 0; i< m_TreeControl->GetCount(); i++)
	{

		wxTreeItemId dataEntityTreeId = m_TreeControl->GetItem(
			m_DataEntityList->GetDataEntity( dataEntityId ) );

		if (dataEntityTreeId.IsOk())
		{
			wxColor color;
			if ( bTrue )
			{
				color = selectedInputColor;
			}
			else
			{
				color  = unselectedInputColor;
			}
			m_TreeControl->SetItemBackgroundColour( dataEntityTreeId, color );
		}
	}
}

Core::DataEntityList::Pointer 
Core::Widgets::DataEntityListBrowser::GetDataEntityList() const
{
	return m_DataEntityList;
}

Core::DataEntityHolder::Pointer 
Core::Widgets::DataEntityListBrowser::GetSelectedDataEntityHolder() const
{
	return m_SelectedDataEntityHolder;
}


void Core::Widgets::DataEntityListBrowser::EndLabelEdit( 
	wxTreeEvent & event )
{

	// Change the internal name of the data entity
	if ( m_SelectedDataEntityHolder->GetSubject().IsNotNull( ) && !event.GetLabel().IsEmpty() )
	{
		m_SelectedDataEntityHolder->GetSubject()->GetMetadata( )->SetName( 
			std::string((const char *)event.GetLabel().ToAscii()) );
	}
}

void Core::Widgets::DataEntityListBrowser::OnRenamedDataEntity(
	blTagMap* tagMap,
	unsigned long tagId )
{
	//check if the modaliy or the name has been changed, otherwise return
	if ( tagId == Core::DE_TAG_NAME || tagId == Core::DE_TAG_MODALITY )
	{

		Core::DataEntityMetadata* metaData = dynamic_cast<Core::DataEntityMetadata*> (tagMap);
		if ( metaData == NULL )
		{
			return;
		}

		// get the wxTreeItemId of the data entity
		wxTreeItemId item = m_TreeControl->GetItem(
			m_DataEntityList->GetDataEntity( metaData->GetDataEntityId( ) ) );
		m_TreeControl->SetItemText(item, metaData->GetName());
	}
	else
		return;
}


void Core::Widgets::DataEntityListBrowser::OnRightMouseButtonClick(
	wxTreeEvent& event)
{
	wxTreeItemId itemID = event.GetItem( );
	Core::DataEntity::Pointer dataEntity = GetDataEntity( itemID );
	if ( dataEntity.IsNull() )
	{
		return;
	}

	m_RightClickDataEntityID = dataEntity->GetId();

	// Create specific menu
	wxMenu* menu = new wxMenu;
	menu->Append(wxID_RemoveDataEntity, wxT("&Unload"), 
		wxT("Unload data"), wxITEM_NORMAL);

	// If it contains something -> Show it
	if ( menu->GetMenuItemCount( ) )
	{
		PopupMenu(menu);
	}

	// Destry it
	delete menu;
}

void Core::Widgets::DataEntityListBrowser::OnModifiedRenderingTree()
{
	try
	{
		UpdateTree();
	}
	coreCatchExceptionsReportAndNoThrowMacro(
		DataEntityListBrowser::OnModifiedRenderingTree)
}

void Core::Widgets::DataEntityListBrowser::UpdateRenderingTreeState(
	Core::DataEntity::Pointer dataEntity )
{
	if(dataEntity.IsNull() )
	{
		return;
	}

	bool showedDataEntity = false;
	if ( GetRenderingTree( ).IsNotNull() )
	{
		showedDataEntity = GetRenderingTree( )->IsDataEntityShown( dataEntity );
	}

	wxTreeItemId  item = m_TreeControl->GetItem( dataEntity );

	// Item is in the list but is not in the wxTreeCtrl -> Add it
	if ( !item.IsOk() )
	{
		return;
	}
	m_TreeControl->SetItemImage( item, showedDataEntity + 1 );
	
}

void Core::Widgets::DataEntityListBrowser::OnItemStateClick( wxTreeEvent& event )
{
	if ( GetRenderingTree( ).IsNull() )
	{
		return;
	}

	wxTreeItemId itemId = event.GetItem();
	Core::DataEntity::Pointer dataEntity = GetDataEntity( itemId );
	int state = m_TreeControl->GetItemImage( itemId );

	if ( !GetRenderingTree( )->IsDataEntityRendered( dataEntity ) )
	{
		GetRenderingTree( )->Add( dataEntity );

		// If the rendering node is added, we need to notify the appearance suitcase
		if(m_SelectedDataEntityHolder.IsNotNull() && m_SelectedDataEntityHolder->GetSubject().IsNotNull())
		{
			if ( dataEntity->GetId() == m_SelectedDataEntityHolder->GetSubject()->GetId( ) )
			{
				m_SelectedDataEntityHolder->NotifyObservers();
			}
		}
	}

	GetRenderingTree( )->Show( dataEntity, state - 1 );
	GetMultiRenderWindow()->RequestUpdateAll();
}

void Core::Widgets::DataEntityListBrowser::SetRenderingTree( Core::RenderingTree::Pointer val )
{
	m_RenderingTreeObserver.disconnect( );
	m_RenderingTreeObserverAnyNode.disconnect( );

	BaseWindow::SetRenderingTree( val );

	if ( GetRenderingTree( ).IsNotNull() )
	{
		m_RenderingTreeObserver = GetRenderingTree( )->AddObserverOnModified( 
			this, 
			&DataEntityListBrowser::OnModifiedRenderingTree );

		m_RenderingTreeObserverAnyNode = GetRenderingTree( )->AddObserverOnModifiedAnyNode( 
			this, 
			&DataEntityListBrowser::OnModifiedRenderingTree );

		OnModifiedRenderingTree( );
	}
}

Core::BaseProcessor::Pointer Core::Widgets::DataEntityListBrowser::GetProcessor()
{
	return NULL;
}

void Core::Widgets::DataEntityListBrowser::OnItemEndDrag( wxTreeEvent& event )
{
	try
	{
		wxTreeItemId itemSrc = m_TreeControl->GetDraggedItem();
		wxTreeItemId itemDst = event.GetItem( );
		Core::DataEntity::Pointer dataEntitySrc = GetDataEntity( itemSrc );
		Core::DataEntity::Pointer dataEntityDst = GetDataEntity( itemDst );
		if ( dataEntitySrc.IsNull() )
		{
			return;
		}

		if ( dataEntityDst.IsNotNull() )
		{
			wxTreeItemId found;
			found = m_TreeControl->GetItemRecursively( itemSrc, dataEntityDst->GetId() );
			if ( found.IsOk() )
			{
				throw Core::Exceptions::Exception( "OnItemEndDrag", 
					"Id not possible to drop a parent item to a child item" );
			}
		}

		// Remove children
		Core::DataEntity::ChildrenListType childrenList;
		childrenList = dataEntitySrc->GetChildrenList();
		Core::DataEntity::ChildrenListType::iterator it;
		for ( it = childrenList.begin(); it != childrenList.end() ;it++ )
		{
			(*it)->SetFather( NULL );
		}


		bool wasRendered = GetRenderingTree( ).IsNotNull( ) && 
			GetRenderingTree( )->IsDataEntityRendered( dataEntitySrc );

		// Remove from list, change father and add again to list
		m_DataEntityList->Remove( dataEntitySrc );
		dataEntitySrc->SetFather( dataEntityDst );

		// Add children again
		for ( it = childrenList.begin(); it != childrenList.end() ;it++ )
		{
			(*it)->SetFather( dataEntitySrc );
		}

		Core::DataEntityHelper::AddDataEntityToList( dataEntitySrc );

		//Render the data entity if it was previously rendered
		if (wasRendered)
			GetRenderingTree( )->Add( dataEntitySrc );

		// After changing the parent of the childs, we need to refresh the tree
		UpdateTree( );
	}
	coreCatchExceptionsReportAndNoThrowMacro( 
		Core::Widgets::DataEntityListBrowser::OnItemEndDrag )
}

void Core::Widgets::DataEntityListBrowser::AdjustLayerProperty()
{
	int layer = 0;
	AdjustLayerProperty( m_TreeControl->GetRootItem(), layer );
}

void Core::Widgets::DataEntityListBrowser::AdjustLayerProperty(
	wxTreeItemId currentItemId,
	int &layer )
{
	if ( !currentItemId.IsOk( ) || GetRenderingTree().IsNull( ) )
	{
		return;
	}

	// Adjust current item layer and increase
	Core::DataEntity::Pointer dataEntity = m_TreeControl->GetItemDataEntity( currentItemId );
	if ( dataEntity.IsNotNull() )
	{
		GetRenderingTree()->SetProperty( dataEntity, blTag::New( "layer", layer++ ) );
	}

	// Adjust child layers
	wxTreeItemIdValue cookie;
	wxTreeItemId itemId = m_TreeControl->GetFirstChild( currentItemId, cookie );
	while( itemId.IsOk() )
	{
		AdjustLayerProperty( itemId, layer );
		itemId = m_TreeControl->GetNextChild( currentItemId, cookie);
	}

	if (GetMultiRenderWindow())
	{
		GetMultiRenderWindow()->OnDataSelected( dataEntity );
	}
}

void Core::Widgets::DataEntityListBrowser::UpdateTree()
{
	m_TreeControl->Update();

	Core::DataEntityList::iterator it;
	for( it = m_DataEntityList->Begin(); 
		it != m_DataEntityList->End(); 
		m_DataEntityList->Next(it))
	{
		UpdateRenderingTreeState( m_DataEntityList->Get(it) );
	}

	AdjustLayerProperty();
}



void Core::Widgets::DataEntityListBrowser::OnKeyPressEvent(wxKeyEvent& event)
{
	if ( event.GetKeyCode() == WXK_DELETE )
	{
		m_DataEntityList->RemoveSelectedDataEntity();
	}
}

void Core::Widgets::DataEntityListBrowser::OnRemoveDataEntity( wxCommandEvent& event )
{
	m_DataEntityList->Remove( m_RightClickDataEntityID );
}
