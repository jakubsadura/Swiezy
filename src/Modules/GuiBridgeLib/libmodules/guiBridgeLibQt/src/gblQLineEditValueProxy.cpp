// Copyright 2006 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "gblQLineEditValueProxy.h"
#include <qlineedit.h>

gbl::QLineEditChangeObserver::QLineEditChangeObserver( boost::shared_ptr<bool> _blockSignalFlag, QLineEdit* _lineEdit, SlotType _slot )
{
	this->blockSignalFlag = _blockSignalFlag;
	this->slotToCallOnChange = _slot;
	connect( _lineEdit, SIGNAL(lostFocus()), this, SLOT(applyChange()));
	connect( _lineEdit, SIGNAL(returnPressed()), this, SLOT(applyChange()));
}

void gbl::QLineEditChangeObserver::applyChange()
{
	this->QWidgetChangeObserverBase::CallSlot();
}

template <> 
void gbl::SetText( QLineEdit* _lineEdit, const std::string& _text )
{
	if( _lineEdit->text().ascii() != _text )
		_lineEdit->setText( _text );
}

template <> 
std::string gbl::GetText( QLineEdit* _lineEdit )
{
	return _lineEdit->text().ascii();
}
