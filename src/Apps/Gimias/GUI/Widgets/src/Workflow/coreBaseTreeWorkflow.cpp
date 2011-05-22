/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#include "coreBaseTreeWorkflow.h"
#include "coreWorkflowTreeItemData.h"


Core::Widgets::BaseTreeWorkflow::BaseTreeWorkflow( 
	wxWindow* parent, wxWindowID id, const wxPoint& pos /*= wxDefaultPosition*/, const wxSize& size /*= wxDefaultSize*/, long style /*= wxTR_HAS_BUTTONS*/, const wxValidator& validator /*= wxDefaultValidator*/, const wxString& name /*= "treeCtrl"*/ )
	: wxTreeCtrl( parent, id, pos, size, style, validator, name )
{

}

std::string Core::Widgets::BaseTreeWorkflow::GetItemFactoryName( const wxTreeItemId &itemId )
{
	WindowItemData* data = dynamic_cast <WindowItemData*> ( GetItemData( itemId ) );
	if ( data == NULL )
	{
		return "";
	}
	return data->m_FactoryName;
}

wxTreeItemId Core::Widgets::BaseTreeWorkflow::AppendItem( 
	const wxTreeItemId& parent, const wxString& text, int image /*= -1*/ )
{
	return wxTreeCtrl::AppendItem( parent, text, image, image, new WindowItemData( text.c_str() ) );
}

wxTreeItemId Core::Widgets::BaseTreeWorkflow::AppendItem( 
	const wxTreeItemId& parent, const wxString& text, int image /*= -1*/, const wxString& factoryName )
{
	return wxTreeCtrl::AppendItem( parent, text, image, image, new WindowItemData( factoryName.c_str() ) );
}

wxTreeItemId Core::Widgets::BaseTreeWorkflow::InsertItem( 
	const wxTreeItemId& parent, size_t index, const wxString& text, int image /*= -1*/, const wxString& classname )
{
	return wxTreeCtrl::InsertItem( parent, index, text, image, image, new WindowItemData( classname.c_str() ) );
}
