/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#ifndef coreBaseInputControl_H
#define coreBaseInputControl_H

#include "coreObject.h"
#include "coreDataHolder.h"
#include "coreDataEntityList.h"
#include "coreBaseWindow.h"
#include <wx/panel.h>
#include <wx/arrstr.h>
#include <wx/string.h>

#include <string>
#include <vector>
#include <map>



// Forward declarations
class wxTextCtrl;
class wxButton;
class wxCommandEvent;
class wxStaticText;
class wxStaticBox;
class wxComboBox;
class wxArrayString;

// Define the id's for the widgets and menu items
#define wxID_BaseInputControl_ListBox	wxID( "wxID_BaseInputControl_ListBox" )


namespace Core
{
namespace Widgets
{ 

class wxListViewComboPopup;
class SelectionComboBox;
class DataEntityListBrowser;


/** 
\brief Select input data from DataEntityList

It shows a widget that allows to hold a current selected item data into 
a different holder, usually the one that would hold the data for being the 
input of your algorithms. Therefore, it allows the user to set the current 
selected item as an input for something, in a quick and easy way both for 
programmers and final users.

You may give a DataHolder to this class using SetAcquiredInputDataHolder(), 
so this class will call SetSubject() to that holder with the subject of 
the current selected holder that has been also given to it. This is called 
at any time this widget acquires a new input, that is when the user clicks 
the "Set as input" button it displays. It also will SetSubject(NULL) - 
so unselect the input - when the user clicks the "x" button it displays.

You may also give to this class the holder that keeps the "Current Selected" 
item of the application or plugin using CurrentSelectedDataHolder(). 
This holder is not the one keeping your input data. An example of this 
holder could be the Application's CurrentSelectedDataEntityHolder or your 
plugin's CurrentSelectedRenderingNodeHolder.

\ingroup gmWidgets
\author Juan Antonio Moya
\date 06 Feb 2008
*/
class GMWIDGETS_EXPORT BaseInputControl : 
	public wxPanel, public BaseWindow
{
public:

	//!
	coreClassNameMacro(BaseInputControl);

	//!
	BaseInputControl(wxWindow* parent, wxWindowID id , 
		const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, 
		long style = wxBORDER_NONE, const wxString& name = "BaseInputControl",
		bool bAutomaticSelection = false,
		bool bSetDefaultDataEntity = true);

	//!
	virtual ~BaseInputControl(void);

	/**
	\brief This is a shorter version of the above
	*/
	void SetDataEntityListBrowser( 
		Core::Widgets::DataEntityListBrowser* dataEntityListBrowser );

	//! Enable the buttons to set and remove selection
	void SetAutomaticSelection( bool bVal );

	//! Call SetSelectData( )
	virtual bool Enable(bool enable = true);

	//!
	void SetHeaderText(std::string val);

	//!
	void SetLabel(std::string label);

	//!
	bool GetDefaultDataEntityFlag() const;
	void SetDefaultDataEntityFlag(bool val);

	//! The subject of the holder should be a children of the father
	void SetFatherHolder( DataEntityHolder::Pointer val );

	//!
	void SetRenderingTree( Core::RenderingTree::Pointer renderingTree );

	//!
	void SetMultiRenderWindow(Core::Widgets::RenderWindowBase* val);

	//!
	void SetEnableOpacitySelection( bool enable );
	bool GetEnableOpacitySelection( );

	//!
	Core::BaseProcessor::Pointer GetProcessor( );

protected:

	//!
	void SetDataEntityList(Core::DataEntityList::Pointer val);

	//! If the input is NULL, select some data
	virtual void SetDefaultDataEntity( );

	//!
	DataEntityHolder::Pointer GetAcquiredInputDataHolder(void) const;

	//!
	DataEntityHolder::Pointer GetCurrentSelectedDataHolder() const;

	//!
	virtual void SetCurrentSelectedDataHolder( DataEntityHolder::Pointer val );

	//!
	virtual void SetAcquiredInputDataHolder( DataEntityHolder::Pointer val );

	//! Update list box selection
	virtual void UpdateWidget(void);

	//! Check if subject is ok to be selected or not
	virtual bool CheckSubjectIsOk( DataEntity::Pointer &subject );

	//! 
	bool CheckFatherIsOk( const Core::DataEntity::Pointer &subject );

	//! Things to do when the m_AcquiredInputDataHolder has been modified
	virtual void OnAcquiredInputModified();

	//!
	int FindListBoxItem( Core::DataEntity::Pointer dataEntity );

	//!
	void FillListBox( );

	//! The father is modified, check again if this subject is a childern
	void OnFatherModified( );

	//! 
	virtual bool SetAcquiredInputData( Core::DataEntity::Pointer &subject ) = 0;

	//!
	void OnDataEntityAddedToList( Core::DataEntity::Pointer dataEntity );

	//!
	void OnDataEntityRemovedFromList( Core::DataEntity::Pointer dataEntity );

	//! This function is called on Linux when the function Enable is called for the parent widget
    	virtual void OnParentEnable( bool enable );


private:
	/** Event handler that listens to user selection on the data entity list */
	void OnSelectedModified();
	
	/** Event handler that listens to user clicks on the ListBox */
	virtual void OnAcquireInputListBoxChanged(wxCommandEvent& event);

protected:
	//!
	wxStaticBox* m_StaticBox;

	//!
	wxListViewComboPopup* m_ifaceComboBox;

	//!
	SelectionComboBox* m_wxListBox;

	//! Enable the buttons to set and remove selection
	bool m_AutomaticSelection;

	//! Enable default data entity selection
	bool m_SetDefaultDataEntity;

	//! Current selected data holder from the core data entity list
	DataEntityHolder::Pointer m_CurrentSelectedDataHolder;

	/**
	 \brief When the user press m_BtnAcquire, the pointer inside 
	 m_CurrentSelectedDataHolder will be used to call SetSubject( )
	 to this variable
	 */
	DataEntityHolder::Pointer m_AcquiredInputDataHolder;


	//!
	Core::DataEntityList::Pointer m_DataEntityList;

	//!
	Core::Widgets::DataEntityListBrowser* m_DataEntityListBrowser;

	//! Connection on add data entity to list
	boost::signals::connection m_ConnOnAdd;

	//! Connection on remove data entity from list
	boost::signals::connection m_ConnOnRem;

	/**
	When the father holder changes, m_AcquiredInputDataHolder is checked 
	again to be a children of the father to be able to be selected
	*/
	DataEntityHolder::Pointer m_FatherHolder;

};

}
}

#endif // AcquireRenderingNodeInputControl_H
