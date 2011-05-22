/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef GBLWXCONNECTOROFWIDGETCHANGESTOSLOTFUNCTION_H
#define GBLWXCONNECTOROFWIDGETCHANGESTOSLOTFUNCTION_H

#include "boost/bind.hpp"
#include "CILabNamespaceMacros.h"
#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/tglbtn.h>
//choicebk not included in wx/wx.h
#include <wx/choicebk.h>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <map>

class wxBitmapComboBox;

CILAB_BEGIN_NAMESPACES(gbl, wx)

/**
For a list of widgets, this class detects changes in these widgets (checking a checkbox, typing enter in a
text box, etc). When a change is detected, it calls a slot function which is set by the user.
The monitoring of the widgets continues for as long as the widgets are alive.

\note ConnectorOfWidgetChangesToSlotFunction creates helper objects such as TextCtrlChangeObserver. These helper
objects are deleted by the WxWidgets controls they are attached to (e.g. the destructor of wxTextCtrl will 
delete the TextCtrlChangeObserver).

\author Maarten Nieber
\date 19 mar 2008
*/

class ConnectorOfWidgetChangesToSlotFunction
{
public:
	typedef ConnectorOfWidgetChangesToSlotFunction Self;

	//!
	ConnectorOfWidgetChangesToSlotFunction();
	//! Removes all observed controls
	~ConnectorOfWidgetChangesToSlotFunction();

	template< class T >
	void SetSlotFunction(T* _recipient, void (T::*_recipientMemberFunction)( void ))
	{
		this->m_SlotFunction = boost::bind(_recipientMemberFunction, _recipient);
	}

	/**
	\param _enabled - If true, then changes in observed wxwidget controls cause calling this->m_SlotFunction.
	\sa m_BlockSignalFlag.
	*/  
	void SetEnabled(bool _enabled);

	/**
	If this function returns true, then changes in observed wxwidget controls cause calling this->m_SlotFunction.
	\sa m_BlockSignalFlag.
	*/
	bool GetEnabled() const;

	//! Call this->m_SlotFunction when _lineEdit changes.
	void Observe(wxTextCtrl* _lineEdit);

	//! Call this->m_SlotFunction when _checkBox changes.
	void Observe(wxRadioButton* _radioButton);

	//! Call this->m_SlotFunction when _checkBox changes.
	void Observe(wxToggleButton* _toggleButton);

	//! Call this->m_SlotFunction when _checkBox changes.
	void Observe(wxCheckBox* _checkBox);

	//! Call this->m_SlotFunction when _checkBox changes.
	void Observe(wxListBox* listBox);

	//! Call this->m_SlotFunction when _checkBox changes.
	void Observe(wxComboBox* listBox);

	//! Call this->m_SlotFunction when bitmapComboBox changes.
	void Observe(wxBitmapComboBox* bitmapComboBox);

	//! Call this->m_SlotFunction when _checkBox changes.
	void Observe(wxChoice* choice);

	//! Call this->m_SlotFunction when _checkBox changes.
	void Observe(wxChoicebook* choice);

	//! Removes \a control from the list of observed controls.
	void Remove(wxWindow *control);

public:
	void ReceiveWxEvent(wxEvent &event);

private:
	class Handler;
	Handler* CreateHandler();

	//! Adds \a control to the list of observed controls.
	void Add(wxWindow *control, Handler* handler);

	//! If this flag is true, then changes in observed wxwidget controls do not cause calling this->m_SlotFunction.
	boost::shared_ptr<bool> m_BlockSignalFlag;
	//! If one of the observed wxwidget controls changes, this->m_SlotFunction is called (unless this->m_BlockSignalFlag is true).
	boost::function<void ()> m_SlotFunction;
	//! List of observed controls
	typedef std::map<wxWindow*, Handler* > Control2Handler;
	Control2Handler m_Controls;
};

CILAB_END_NAMESPACES(gbl, wx)

#endif //GBLWXCONNECTOROFWIDGETCHANGESTOSLOTFUNCTION_H
