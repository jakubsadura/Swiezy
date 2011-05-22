#ifndef GBLQCHECKBOXVALUEPROXY_H
#define GBLQCHECKBOXVALUEPROXY_H

#include "gblValueProxyQt.h"
#include <qobject.h>

class QCheckBox;

namespace gbl
{

class QCheckBoxChangeObserver
	: public QWidgetChangeObserverBase
{
	Q_OBJECT

public:
	//!
	QCheckBoxChangeObserver(boost::shared_ptr<bool> _blockSignalFlag, QCheckBox* _checkBox, QWidgetChangeObserverBase::SlotType _slot);

private slots:
	//!
	void checkBoxChanged(bool);
};

//! Overload to set checkbox contents as text. See gblValueProxy.h
template <> void SetText(QCheckBox* _checkBox, const std::string& _text);

//! Overload to get checkbox contents as text. See gblValueProxy.h
template <> std::string GetText(QCheckBox* _checkBox);

} // namespace gbl

#endif //GBLQCHECKBOXVALUEPROXY_H
