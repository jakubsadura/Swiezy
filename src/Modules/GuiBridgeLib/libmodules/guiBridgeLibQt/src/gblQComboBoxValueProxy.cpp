#include "gblQComboBoxValueProxy.h"
#include "boost/format.hpp"
#include "boost/algorithm/string/split.hpp"
#include "boost/foreach.hpp"
#include "boost/algorithm/string.hpp"
#include <qcombobox.h>
#include <assert.h>

const char* separator = ";";
const char separatorChar = ';';

template <> std::string gbl::GetText(QComboBox* _comboBox)
{
	std::string result = _comboBox->currentText().ascii();
	return result;
}

template <> void gbl::SetText(QComboBox* _comboBox, const std::string& _x)
{
	bool found(false);
	for( int i = 0; i < _comboBox->count(); ++i )
	{
		if( _comboBox->text(i) == _x )
		{
			found = true;
			_comboBox->setCurrentItem(i);
			break;
		}
	}
	if( !found )
	{
		std::string message = (boost::format("Cannot find combo box item %1%") % _x).str();
		throw gbl::Exception(message.c_str());
	}
}

template <>
double gbl::GetNumber( QComboBox* _comboBox )
{
	return _comboBox->currentItem();
}

template <>
void gbl::SetNumber( QComboBox* _comboBox, double _number, const int _maxNrOfDecimals )
{
	_comboBox->setCurrentItem(_number);
}

gbl::QComboBoxChangeObserver::QComboBoxChangeObserver( boost::shared_ptr<bool> _blockSignalFlag, QComboBox* _comboBox, QWidgetChangeObserverBase::SlotType _slot )
{
	this->blockSignalFlag = _blockSignalFlag;
	this->slotToCallOnChange = _slot;
	connect( _comboBox, SIGNAL(activated(int)), this, SLOT(comboBoxChanged(int)));
}

void gbl::QComboBoxChangeObserver::comboBoxChanged(int _selectedItem)
{
	this->QWidgetChangeObserverBase::CallSlot();
}
