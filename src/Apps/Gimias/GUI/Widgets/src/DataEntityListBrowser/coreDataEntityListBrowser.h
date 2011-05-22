/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreDataEntityListBrowser_H
#define coreDataEntityListBrowser_H


#include "gmWidgetsWin32Header.h"
#include "coreObject.h"
#include "coreDataEntityList.h"
#include "coreDataHolder.h"
#include <boost/signals.hpp>

//#include "coreMeshCreationProcessor.h"
#include "coreRenderingTree.h"
#include "coreBaseWindow.h"

#include "wx/treebase.h"
#include "wxID.h"

// Forward declarations
class wxTreeCtrl;
class wxButton;
class wxListEvent;


namespace Core
{
namespace Widgets
{

class DataEntityTreeCtrl;


#define wxID_DataEntityListBrowser wxID("wxID_DataEntityListBrowser")

/** 
\brief This browser maintains a coherent view to the Core::DataEntityList 
of the application model. It displays the list of data entities available 
to be selected and handled for processing (so is the DataEntityList).

This widget is designed for internal use in the application, and 
plugin designers do not 

\ingroup gmWidgets
\author Juan Antonio Moya
\date 24 Jan 2008
*/
class GMWIDGETS_EXPORT DataEntityListBrowser : public wxPanel, public BaseWindow
{
public:
	//!
	coreDefineBaseWindowFactory( DataEntityListBrowser );
	coreClassNameMacro(DataEntityListBrowser)
		
	DataEntityListBrowser(
		wxWindow* parent, 
		wxWindowID id = wxID_DataEntityListBrowser, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxTAB_TRAVERSAL, 
		const wxString& name = wxT("DataEntityListBrowser"));
	virtual ~DataEntityListBrowser(void);

	void InitConnections( );

	void UpdateList(void);

	//! Change the color of the dataEntityId to selected or unselected
	void SetSelectedInput( int dataEntityId, bool bTrue );

	Core::DataEntityList::Pointer GetDataEntityList() const;

	Core::DataEntityHolder::Pointer GetSelectedDataEntityHolder() const;

	//!
	void SetRenderingTree( Core::RenderingTree::Pointer val );

	//!
	Core::BaseProcessor::Pointer GetProcessor( );

protected:
	void AddDataEntityToList(Core::DataEntity::Pointer dataEntity);
	void RemoveDataEntityFromList(Core::DataEntity::Pointer dataEntity);

private:
	void OnItemSelectedFromList(wxTreeEvent & event);
	void OnItemUnSelectedFromList(wxListEvent& event);
	
	void OnDataEntityAddedToList(Core::DataEntity::Pointer dataEntity);
	void OnDataEntityRemovedFromList(Core::DataEntity::Pointer dataEntity);

	//!
	void EndLabelEdit(wxTreeEvent & event);

	//!
	void OnRenamedDataEntity(
		blTagMap* renamedDataEntity,
		unsigned long);

	//!
	void OnRightMouseButtonClick(wxTreeEvent& event);

	//! Remove right click data entity
	void OnRemoveDataEntity(wxCommandEvent& event );

private:

	//! Respond to button for rendering the selected data entity
	void OnButtonRenderDataEntity(wxCommandEvent &event);

	//! Respond to button for removing the selected data entity
	void OnButtonRemoveDataEntity(wxCommandEvent &event);

	//! End dragging a data entity
	void OnItemEndDrag( wxTreeEvent& event );

	//! Highlight the current selected data entity
	void HighlightCurrentSelected();

	//!
	Core::DataEntity::Pointer GetDataEntity( wxTreeItemId &item );

	//!
	void OnModifiedRenderingTree();

	//!
	void UpdateRenderingTreeState( Core::DataEntity::Pointer dataEntity );

	//!
	void OnItemStateClick(wxTreeEvent& event);

	//!
	void AdjustLayerProperty( );

	//!
	void AdjustLayerProperty( wxTreeItemId itemId, int &layer );

	//! Update tree control relations, checkbox and layer property
	void UpdateTree( );

	//! 
	void OnKeyPressEvent(wxKeyEvent& event);

	DECLARE_EVENT_TABLE();


private:
	//!
	Core::Widgets::DataEntityTreeCtrl* m_TreeControl;
	//!
	wxButton* m_UnloadButton;
	//!
	Core::DataEntityHolder::Pointer m_SelectedDataEntityHolder;
	//!
	boost::signals::connection m_ConnOnAdd;
	//!
	boost::signals::connection m_ConnOnRemove;

	//! Data entity list
	Core::DataEntityList::Pointer m_DataEntityList;

	//!
	unsigned int m_RightClickDataEntityID;

	//! 
	boost::signals::connection m_RenderingTreeObserver;

	//! 
	boost::signals::connection m_RenderingTreeObserverAnyNode;


	/** Disable selection handler when this class is changing the
	selection*/
	bool m_disableSelection;
};

} // namespace Widgets
} // namespace Core

#endif
