#include "gblQRadioButtonValueProxy.h"
#include <qradiobutton.h>

gbl::QRadioButtonChangeObserver::QRadioButtonChangeObserver( boost::shared_ptr<bool> _blockSignalFlag, QRadioButton* _radioButton, QWidgetChangeObserverBase::SlotType _slot )
{
	this->blockSignalFlag = _blockSignalFlag;
	this->slotToCallOnChange = _slot;
	connect( _radioButton, SIGNAL(toggled(bool)), this, SLOT(radioButtonChanged(bool)));
}

void gbl::QRadioButtonChangeObserver::radioButtonChanged(bool _value)
{
	this->QWidgetChangeObserverBase::CallSlot();
}

template <>
void gbl::SetText( QRadioButton* _radioButton, const std::string& _text )
{
	bool newValue = gbl::ValueConverter::TextToFlag(_text);
	if( newValue != _radioButton->isChecked() )
		_radioButton->setChecked( newValue );
}

template <>
std::string gbl::GetText( QRadioButton* _radioButton )
{
	return gbl::ValueConverter::FlagToText( _radioButton->isChecked() );
}
