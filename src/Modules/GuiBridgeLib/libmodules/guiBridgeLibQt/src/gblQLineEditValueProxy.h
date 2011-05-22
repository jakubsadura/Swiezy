#ifndef GBLQLINEEDITVALUEPROXY_H
#define GBLQLINEEDITVALUEPROXY_H

#include "gblValueProxyQt.h"
#include <qobject.h>
#include <limits>

class QLineEdit;

namespace gbl
{

class QLineEditChangeObserver
	: public QWidgetChangeObserverBase
{
	Q_OBJECT

public:
	//!
	QLineEditChangeObserver(boost::shared_ptr<bool> _blockSignalFlag, QLineEdit* _lineEdit, QWidgetChangeObserverBase::SlotType _slot);

private slots:
	//!
	void applyChange();
};

//! Set the text field in a QLineEdit
template <> void SetText(QLineEdit* _lineEdit, const std::string& _text);

//! Set the text field in a QLineEdit
template <> std::string GetText(QLineEdit* _lineEdit);

} // namespace gbl

#endif //GBLQLINEEDITVALUEPROXY_H
