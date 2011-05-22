#ifndef GBLQRADIOBUTTONVALUEPROXY_H
#define GBLQRADIOBUTTONVALUEPROXY_H

#include "gblValueProxyQt.h"
#include "gblBridge.h"
#include <qobject.h>

class QRadioButton;

namespace gbl
{

class QRadioButtonChangeObserver
	: public QWidgetChangeObserverBase
{
	Q_OBJECT

public:
	//!
	QRadioButtonChangeObserver(boost::shared_ptr<bool> _blockSignalFlag, QRadioButton* _radioButton, QWidgetChangeObserverBase::SlotType _slot);

private slots:
	//!
	void radioButtonChanged(bool);
};

//! Overload to set checkbox contents as text. See gblValueProxy.h
template <> void SetText(QRadioButton* _radioButton, const std::string& _text);

//! Overload to get checkbox contents as text. See gblValueProxy.h
template <> std::string GetText(QRadioButton* _radioButton);

} // namespace gbl

#endif //GBLQRADIOBUTTONVALUEPROXY_H
