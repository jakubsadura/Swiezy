#ifndef GBLQCOMBOBOXVALUEPROXY_H
#define GBLQCOMBOBOXVALUEPROXY_H

#include "gblValueProxyQt.h"
#include "gblValueProxy.h"
#include <qobject.h>
#include <qcombobox.h>
#include <vector>
#include <string>
#include "CILabNamespaceMacros.h"

namespace gbl {

class QComboBoxChangeObserver
	: public QWidgetChangeObserverBase
{
	Q_OBJECT

public:
	//!
	QComboBoxChangeObserver(boost::shared_ptr<bool> _blockSignalFlag, QComboBox* _comboBox, QWidgetChangeObserverBase::SlotType _slot);

private slots:
	//!
	void comboBoxChanged(int);
};

//! Gets the selected item in the combo box.
template <> double GetNumber(QComboBox* _comboBox);

//! Sets the selected item in the combo box.
template <> void SetNumber(QComboBox* _comboBox, double _number, const int _maxNrOfDecimals);

//! Gets the selected item in the combo box
template <> std::string GetText(QComboBox* _comboBox);

//! Sets the selected item in the combo box based on the text of the item..
template <> void SetText(QComboBox* _comboBox, const std::string& _text);

} // namespace gbl

#endif //GBLQCOMBOBOXVALUEPROXY_H
