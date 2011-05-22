#ifndef GBLCHANGEINQWIDGETOBSERVER_H
#define GBLCHANGEINQWIDGETOBSERVER_H

// Copyright 2007 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "boost/bind.hpp"

#include "CILabNamespaceMacros.h"
#include "gblQLineEditValueProxy.h"
#include "gblQCheckBoxValueProxy.h"
#include "gblQComboBoxValueProxy.h"
#include "gblQRadioButtonValueProxy.h"

CILAB_BEGIN_NAMESPACE(gbl)

class ChangeInQWidgetObserver
{
public:
	ChangeInQWidgetObserver();

	template< class T >
	void SetSlotFunction(T* _recipient, void (T::*_recipientMemberFunction)( void ))
	{
		this->slotFunction = boost::bind(_recipientMemberFunction, _recipient);
	}

	/**
	\param _enabled - If true, then changes in observed qwidget controls cause calling this->slotFunction.
	\sa blockSignalFlag.
	*/
	void SetEnabled(bool _enabled);

	/**
	If this function returns true, then changes in observed qwidget controls cause calling this->slotFunction.
	\sa blockSignalFlag.
	*/
	bool GetEnabled() const;

	//! Call this->slotFunction when _lineEdit changes.
	void Observe(QLineEdit* _lineEdit);

	//! Call this->slotFunction when _checkBox changes.
	void Observe(QCheckBox* _checkBox);

	//! Call this->slotFunction when _checkBox changes.
	void Observe(QComboBox* _comboBox);

	//! Call this->slotFunction when _checkBox changes.
	void Observe(QRadioButton* _radioButton);

private:
	//! If this flag is true, then changes in observed qwidget controls do not cause calling this->slotFunction.
	boost::shared_ptr<bool> blockSignalFlag;
	//! If one of the observed qwidget controls changes, this->slotFunction is called (unless this->blockSignalFlag is true).
	boost::function<void ()> slotFunction;
};

CILAB_END_NAMESPACE(gbl)

#endif //GBLCHANGEINQWIDGETOBSERVER_H
