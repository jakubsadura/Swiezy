#include "coreDataEntityTreeCtrl.h"

// Copyright 2006 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.


// For compilers that don't support precompilation, include "wx/wx.h"
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif


BEGIN_EVENT_TABLE(Core::Widgets::DataEntityTreeCtrl, blwxTreeCtrl)
	EVT_KEY_DOWN			(Core::Widgets::DataEntityTreeCtrl::OnKeyPressEvent)
END_EVENT_TABLE()



// Data Entity Tree Item Data implementation
Core::Widgets::DataEntityTreeItemData::DataEntityTreeItemData(int dataEntityID) 
{
	m_DataEntityID = dataEntityID;
} 

const int Core::Widgets::DataEntityTreeItemData::GetId() const
{
	return m_DataEntityID; 
}



/**
*/
Core::Widgets::DataEntityTreeCtrl::DataEntityTreeCtrl( 
	wxWindow *parent, 
	const wxWindowID id, 
	const wxPoint& pos, 
	const wxSize& size, 
	long style ) : blwxTreeCtrl(parent, id, pos, size, style)
{
	
}

wxTreeItemId Core::Widgets::DataEntityTreeCtrl::GetItemRecursively(
	const wxTreeItemId& root, 
	int dataEntityId )
{

	// Check root item
	if ( GetItemDataEntityID( root ) == dataEntityId )
	{
		return root;
	}

	//Iterate the whole tree looking for the dataEntityId
	wxTreeItemIdValue cookie;
	wxTreeItemId itemId = this->GetFirstChild( root, cookie );

	while( itemId.IsOk() )
	{
		
		if ( GetItemDataEntityID( itemId ) == dataEntityId )
		{
			return itemId;
		}

		//look in the children
		if ( this->ItemHasChildren(itemId))
		{
			wxTreeItemId auxID = this->GetItemRecursively(itemId, dataEntityId );
			if (auxID.IsOk())
			{
				return auxID;
			}
		}

		itemId = this->GetNextChild( root, cookie);
	}

	/* Not found */
	wxTreeItemId dummy;
	return dummy;
}

void Core::Widgets::DataEntityTreeCtrl::OnKeyPressEvent(wxKeyEvent& event)
{
	if ( event.GetKeyCode() == WXK_F2 && ( GetWindowStyle() & wxTR_MULTIPLE ) == 0 )
	{
		wxTreeItemId selected = GetSelection( );
		if ( selected.IsOk() )
		{
			EditLabel( selected );
		}
	}
	else 
	{
		// Propagate to the parent window
		event.ResumePropagation( 1 );
		event.Skip();
	}
}

wxTreeItemId Core::Widgets::DataEntityTreeCtrl::AppendItem( 
	Core::DataEntity::Pointer dataEntity )
{
	if(dataEntity.IsNull() )
	{
		return 0;
	}

	wxTreeItemId k = GetItem( dataEntity );
	if ( k.IsOk() )
	{
		return k;
	}

	wxTreeItemId father;
	// if it doesn't have a father, add it to the root
	if ( dataEntity->GetFather().IsNull() )
	{
		father = GetRootItem();
	}
	else
	{
		// get the item, and add it to the wxTreeItemId
		father = GetItemRecursively(
			GetRootItem( ), 
			dataEntity->GetFather()->GetId());
	}

	// Append item
	DataEntityTreeItemData* itemData = new DataEntityTreeItemData(dataEntity->GetId());
	k = blwxTreeCtrl::AppendItem(
		father,
		_U( dataEntity->GetMetadata( )->GetName() ),
		-1,
		-1,
		itemData );

	// Add children items
	Core::DataEntity::ChildrenListType childrenList = dataEntity->GetChildrenList();
	Core::DataEntity::ChildrenListType::iterator it;
	for ( it = childrenList.begin() ; it != childrenList.end( ) ; it++ )
	{
		AppendItem( (*it) );
	}

	ExpandAll( );

	return k;
}

void Core::Widgets::DataEntityTreeCtrl::Update( )
{
	if ( !CheckRelations( ) )
	{
		Build();
	}
}

void Core::Widgets::DataEntityTreeCtrl::Build( )
{
	Hide();

	DeleteAllItems();
	AddRoot(wxT("Root"), -1, -1);

	// Append only root items, the rest will be appended recursivelly
	Core::DataEntityList::iterator it;
	for( it = m_DataEntityList->Begin(); 
		it != m_DataEntityList->End(); 
		m_DataEntityList->Next(it))
	{
		Core::DataEntity::Pointer dataEntity = m_DataEntityList->Get(it);
		if ( dataEntity.IsNotNull() && dataEntity->GetFather().IsNull() )
		{
			AppendItem( m_DataEntityList->Get(it) );
		}
	}

	ExpandAll();

	Show( );
}

bool Core::Widgets::DataEntityTreeCtrl::CheckRelations( )
{
	if ( m_DataEntityList->GetCount() != GetCount( ) - 1 )
	{
		return false;
	}

	Core::DataEntityList::iterator it;
	for( it = m_DataEntityList->Begin(); 
		it != m_DataEntityList->End(); 
		m_DataEntityList->Next(it))
	{
		Core::DataEntity::Pointer dataEntity = m_DataEntityList->Get(it);
		if ( dataEntity.IsNull() )
		{
			continue;
		}

		wxTreeItemId itemId = GetItem( dataEntity );
		if ( !itemId.IsOk() )
		{
			return false;
		}

		if ( GetItemParent( itemId ) != GetItemFather( dataEntity ) )
		{
			return false;
		}
	}

	return true;
}

Core::DataEntity::Pointer Core::Widgets::DataEntityTreeCtrl::GetItemDataEntity( 
	const wxTreeItemId &itemId )
{
	int dataEntityID = GetItemDataEntityID( itemId );
	if ( dataEntityID == -1 )
	{
		return NULL;
	}

	Core::DataEntity::Pointer dataEntity;
	// If data entity is not found -> return NULL
	try
	{
		dataEntity = m_DataEntityList->GetDataEntity( dataEntityID );
	}
	catch (...)
	{
	}

	return dataEntity;
}

void Core::Widgets::DataEntityTreeCtrl::SetDataEntityList( 
	Core::DataEntityList::Pointer val )
{
	m_DataEntityList = val;

	// Add root
	Build();
}

wxTreeItemId Core::Widgets::DataEntityTreeCtrl::GetItem( 
	Core::DataEntity::Pointer dataEntity )
{
	if ( dataEntity.IsNull() )
	{
		return 0;
	}

	return GetItemRecursively( GetRootItem(), dataEntity->GetId() );
}

wxTreeItemId Core::Widgets::DataEntityTreeCtrl::GetItemFather( 
	Core::DataEntity::Pointer dataEntity )
{
	if ( dataEntity.IsNull() )
	{
		return 0;
	}

	Core::DataEntity::Pointer father = dataEntity->GetFather();
	wxTreeItemId itemId = GetItem( dataEntity );
	wxTreeItemId fatherItemId = GetRootItem();

	if ( father.IsNotNull() )
	{
		fatherItemId = GetItem( father );
	}

	return fatherItemId;
}

int Core::Widgets::DataEntityTreeCtrl::GetItemDataEntityID( 
	const wxTreeItemId &itemId )
{
	if( !itemId.IsOk() )
	{
		return -1;
	}

	DataEntityTreeItemData* itemData;
	itemData = static_cast<DataEntityTreeItemData*>(this->GetItemData(itemId));
	if ( itemData == NULL )
	{
		return -1;
	}

	return itemData->GetId();
}

