/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef wxMitkToolBoxItem_H
#define wxMitkToolBoxItem_H

#include "TpExtLibWxWidgetsWin32Header.h"
#include <wx/panel.h>
#include <list>

class wxBoxSizer;
class wxToggleButton;
class wxColour;


// ------------------------------------------------------------------------
/*
The wxToolBoxItem is a widget container tabbed by a button on top.

The container itself can display or collapse, showing and hiding a widget 
just below the button. The user can display or collapse it by clicking 
the button, where it will show a caption as a title for the tool item. 
When this happens, a wxToolBoxItemSelectedEvent is cast, that can be 
caught by means of EVT_TBOX_ITEM_SELECTED.

A set of wxToolBoxItem are managed by the wxMitkToolBoxControl class.
\sa wxMitkToolBoxControl

\ingroup TpExtLibWxWidgets
\author Juan Antonio Moya
\date 14 Dec 2007
*/
class TPEXTLIBWXWIDGETS_EXPORT wxToolBoxItem : public wxPanel
{
public:
	wxToolBoxItem(
		wxWindow* parent, 
		wxWindowID id, 
		wxWindow* widget, 
		const wxString& caption = wxT("<item>"), 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxBORDER_NONE | wxVSCROLL, 
		const wxString& name = wxT("wxToolBoxItem"));
	virtual ~wxToolBoxItem(void);

	wxWindow* GetWidget(void) const;
	wxString GetCaption(void) const;
	void SetWidget(wxWindow* widget);
	void SetCaption(wxString& caption);

	void Toggle(void);
	void Collapse(void);
	void Expand(void);
	bool IsCollapsed(void) const;
	bool IsExpanded(void) const;

private:
	void OnButtonToggled(wxCommandEvent& event);
	void UpdateObserversOfToolBoxItem(void);
	
	wxWindow* m_widget;
	wxToggleButton* m_button;
	wxBoxSizer* m_internalLayout;
	
	DECLARE_EVENT_TABLE();
};



#endif // wxMitkToolBoxItem_H
