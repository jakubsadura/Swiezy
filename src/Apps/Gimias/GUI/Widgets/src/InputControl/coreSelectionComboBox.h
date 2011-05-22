/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreSelectionComboBox_H
#define _coreSelectionComboBox_H

#include "gmWidgetsWin32Header.h"
#include "coreBaseInputControl.h"
#include "coreDataEntity.h"
#include "coreMultiRenderWindow.h"
#include <wx/combo.h>

namespace Core
{
namespace Widgets 
{ 
/** 
Special combo box that changes the opacity of currently selected item

\ingroup gmWidgets
\author Xavi Planes
\date 31 Aug 2010
*/
class GMWIDGETS_EXPORT SelectionComboBox : public wxComboCtrl
{
public:

	//!
	SelectionComboBox(
		wxWindow* parent, 
		wxWindowID id, 
		const wxString& value = wxEmptyString,
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		int n = 0, const wxString choices[] = NULL,
		long style = 0,
		const wxValidator& validator = wxDefaultValidator,
		const wxString& name = wxComboBoxNameStr);

	//!
	virtual ~SelectionComboBox(void);

protected:

protected:
};

/** 
Combo box interface

\ingroup gmWidgets
\author Xavi Planes
\date 31 Aug 2010
*/
class wxListViewComboPopup : public wxListView,
                             public wxComboPopup
{
public:

    // Initialize member variables
    virtual void Init();

    // Create popup control
    virtual bool Create(wxWindow* parent);

    // Return pointer to the created control
    virtual wxWindow *GetControl();

    // Translate string into a list selection
    virtual void SetStringValue(const wxString& s);

    // Get list selection as a string
    virtual wxString GetStringValue() const;

	//
	// Popup event handlers
	//

    // Do mouse hot-tracking (which is typical in list popups)
    void OnMouseMove(wxMouseEvent& event);

    // On mouse left up, set the value and close the popup
    void OnMouseClick(wxMouseEvent& WXUNUSED(event));

	void OnSize(wxSizeEvent& event);

	void OnPopup();

	void OnDismiss();

	//
	// Utilies for item manipulation
	//

	void Append( const wxString& selstr, unsigned int data );

	void SetSelection( long item );

	//!
	void SetRenderingTree( Core::RenderingTree::Pointer renderingTree );

	//!
	void SetMultiRenderWindow(Core::Widgets::RenderWindowBase* val);

	//!
	void SetEnableOpacitySelection( bool enable );
	bool GetEnableOpacitySelection( );

protected:
	//!
	void SelectDataEntity( int item );
	//!
	void RestoreOpacity( );
	//!
	void BackupOpacity( );
protected:

	//! current item index
	int             m_value; 
	//! hot item in popup
	int             m_itemHere;
	//! Change the opacity to show only the item over the mouse pointer
	Core::RenderingTree::Pointer m_RenderingTree;
	//! Update views
	Core::Widgets::RenderWindowBase* m_MultiRenderWindow;
	//! Backup opacity map when this popup is shown
	std::map<long,float> m_BackupOpacityMap;
	//!
	bool m_EnableOpacitySelection;
	//!
	wxImageList *m_ImageList;
	//!
	int m_SelectedDataEntity;
private:
    DECLARE_EVENT_TABLE()
};

}
}


#endif // _coreSelectionComboBox_H
