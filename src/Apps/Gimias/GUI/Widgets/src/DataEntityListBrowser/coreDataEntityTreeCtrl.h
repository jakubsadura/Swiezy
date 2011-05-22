/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreDataEntityTreeCtrl_H
#define coreDataEntityTreeCtrl_H

#include "coreCommonMacros.h"
#include "gmWidgetsWin32Header.h"
#include <blwxTreeCtrl.h>

namespace Core
{
namespace Widgets
{


/**
Auxiliar class for storing treeDataEntity information
\ingroup gmWidgets
\author Xavi Planes
\date 06 May 2010
*/
class DataEntityTreeItemData : public wxTreeItemData
{
public:
	DataEntityTreeItemData(int dataEntityID);

	const int GetId() const;

private:
	int m_DataEntityID;
};

/** 
\brief This class extend the wxTreeCtrl functionalities.  

This class iterates over the dataEntityTree, looking for the dataentity 
that has a certaing ID. This class is designed for internal use in the 
application. 

\ingroup gmWidgets
\author Martin Bianculli
\date 22 Apr 2009
*/
class GMWIDGETS_EXPORT DataEntityTreeCtrl : public blwxTreeCtrl
{
public:
	coreClassNameMacro(DataEntityTreeCtrl);

	//!
	DataEntityTreeCtrl(
		wxWindow *parent, const wxWindowID id,
		const wxPoint& pos, const wxSize& size,
		long style);

	/** Returns the wxTreeItemId of an element of the tree that has the 
	corresponding dataEntityId
	Recursevely iterates throughout the data entity tree, looking in all 
	the children for the specified dataEntityId
	*/
	wxTreeItemId GetItemRecursively(const wxTreeItemId& root, int dataEntityId );

	//! Append a data entity item using the father item of the dataEntity
	wxTreeItemId AppendItem( Core::DataEntity::Pointer dataEntity );

	//! Check if father of each DataEntity is correct, if not, rebuild tree
	void Update( );

	//! Get DataEntity from an item
	Core::DataEntity::Pointer GetItemDataEntity( const wxTreeItemId &itemId );

	//! Get DataEntity from an item
	int GetItemDataEntityID( const wxTreeItemId &itemId );

	//!
	void SetDataEntityList(Core::DataEntityList::Pointer val);

	//!
	wxTreeItemId GetItem( Core::DataEntity::Pointer dataEntity );

	//!
	wxTreeItemId GetItemFather( Core::DataEntity::Pointer dataEntity );

private:
	DECLARE_EVENT_TABLE();

	//! Check parent relations of data entities in DataEntityList
	bool CheckRelations( );

	//! Build tree from DataEntityList
	void Build( );

	//!
	void OnKeyPressEvent(wxKeyEvent& event);

protected:
	//! Data entity list
	Core::DataEntityList::Pointer m_DataEntityList;
};
}//namespace Widgets

}//namespace Core

#endif //coreDataEntityTreeCtrl_H
