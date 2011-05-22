#include "gblQCheckBoxValueProxy.h"
#include "gblValueConverter.h"
#include <qcheckbox.h>

gbl::QCheckBoxChangeObserver::QCheckBoxChangeObserver( boost::shared_ptr<bool> _blockSignalFlag, QCheckBox* _checkBox, QWidgetChangeObserverBase::SlotType _slot )
{
	this->blockSignalFlag = _blockSignalFlag;
	this->slotToCallOnChange = _slot;
	connect( _checkBox, SIGNAL(toggled(bool)), this, SLOT(checkBoxChanged(bool)));
}

void gbl::QCheckBoxChangeObserver::checkBoxChanged(bool _value)
{
	this->QWidgetChangeObserverBase::CallSlot();
}

template <>
void gbl::SetText( QCheckBox* _checkBox, const std::string& _text )
{
	bool newValue = gbl::ValueConverter::TextToFlag(_text);
	if( newValue != _checkBox->isOn() )
		_checkBox->setChecked( newValue );
}

template <>
std::string gbl::GetText( QCheckBox* _checkBox )
{
	return gbl::ValueConverter::FlagToText( _checkBox->isOn() );
}
