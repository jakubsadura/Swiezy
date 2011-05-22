// Copyright 2006 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "gblChangeInQWidgetObserver.h"

gbl::ChangeInQWidgetObserver::ChangeInQWidgetObserver()
: blockSignalFlag(new bool)
{
	*blockSignalFlag = false;
}

void gbl::ChangeInQWidgetObserver::Observe( QLineEdit* _lineEdit )
{
	new QLineEditChangeObserver(this->blockSignalFlag, _lineEdit, this->slotFunction);
	// to do: delete the object automatically when it is no longer needed
}

void gbl::ChangeInQWidgetObserver::Observe( QCheckBox* _checkBox)
{
	new QCheckBoxChangeObserver(this->blockSignalFlag, _checkBox, this->slotFunction);
}

void gbl::ChangeInQWidgetObserver::Observe( QComboBox* _comboBox )
{
	new QComboBoxChangeObserver(this->blockSignalFlag, _comboBox, this->slotFunction);
}

void gbl::ChangeInQWidgetObserver::Observe( QRadioButton* _radioButton )
{
	new QRadioButtonChangeObserver(this->blockSignalFlag, _radioButton, this->slotFunction);
}

void gbl::ChangeInQWidgetObserver::SetEnabled( bool _enabled )
{
	*(this->blockSignalFlag) = !_enabled;
}

bool gbl::ChangeInQWidgetObserver::GetEnabled() const
{
	return !(*(this->blockSignalFlag));
}
