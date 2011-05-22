/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef wxMitkToolBoxControl_H
#define wxMitkToolBoxControl_H

#include "TpExtLibWxWidgetsWin32Header.h"
#include <wx/panel.h>
#include <list>

class wxBoxSizer;


class wxToolBoxItem;
class wxToolBoxItemSelectedEvent;

/*
The wxToolBoxControl is a widget container that provides a column 
of tabbed widget items.

When exclusive mode is set to true, the user will be able to open just one 
item at a time. The others
will collapse automatically.

The widget of the selected item will be enabled or disabled when this is
the active one of all the plugins.

A wxToolBoxItemSelectedEvent is emitted when the current item is changed. 
You may catch it with the EVT_TBOX_ITEM_SELECTED macro
\sa wxToolBoxItem

\ingroup TpExtLibWxWidgets
\author Juan Antonio Moya
\date 14 Dec 2007
*/
class TPEXTLIBWXWIDGETS_EXPORT wxToolBoxControl : public wxPanel
{
public:
	wxToolBoxControl(
		wxWindow* parent, 
		wxWindowID id = wxID_ANY, 
		bool exclusive = false, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxBORDER_NONE, 
		const wxString& name = wxT("wxToolBoxControl"));
	virtual ~wxToolBoxControl(void);

	void Add(wxToolBoxItem* item);
	void Remove(wxToolBoxItem* item);
	wxToolBoxItem* GetItemByCaption(const wxString& caption);
	wxToolBoxItem* GetItemByWidget(wxWindow* widget);
	void SetExclusive(bool exclusiveMode);
	void Clear(void);
	
	// Changes the current selected item
	void SetSelectedItem( wxWindow* widget );

	void CollapseAll(void);

	//! Function called when the selected item has changed
	virtual void OnSelectedItem( int iWidgetID );

	//! Call Enable( bVal ) to the expanded item
	void SetEnable( bool bVal );

protected:
	typedef std::list<wxToolBoxItem*> ItemList;
	int GetNumberOfItems(void);

private:
	/**
	\brief Collapse all items and expand the selected one.
	This also calls Enable on the expanded item
	*/
	void OnToggleItem(wxToolBoxItemSelectedEvent& event);

private:
	ItemList m_registeredItems;
	wxBoxSizer* m_internalLayout;
	wxBoxSizer* m_internalLayoutTop;
	wxBoxSizer* m_internalLayoutBottom;
	bool m_exclusiveMode;

	DECLARE_EVENT_TABLE();
};
 


#endif // wxMitkToolBoxControl_H
