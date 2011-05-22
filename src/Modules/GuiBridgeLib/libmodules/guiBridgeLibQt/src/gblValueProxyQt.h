#ifndef GBLVALUEPROXYQT_H
#define GBLVALUEPROXYQT_H

#include "gblBridge.h"
#include <qobject.h>
#include "boost/signal_patched.hpp"

class QWidget;

namespace gbl
{

/**
Base class for objects observing a change in a Qt control and calling a slot function.

\author Maarten Nieber
\date 11 Jan 2008
*/

class QWidgetChangeObserverBase
	: public QObject
{
	Q_OBJECT
public:
	typedef boost::function<void ()> SlotType;

protected:
	//! This function calls slotToCallOnChange unless *(this->blockSignalFlag).
	void CallSlot()
	{
		if( *(this->blockSignalFlag) == false )
			this->slotToCallOnChange();
	}

protected:
	//! Call this slot when the qt control changes
	SlotType slotToCallOnChange;
	/**
	Pointer to the boolean that switches on/off calling slotToCallOnChange when the qt control changes.
	If *blockSignalFlag == true, then the signal is NOT sent.
	*/
	boost::shared_ptr<bool> blockSignalFlag;
};

} // namespace gbl

#endif //GBLVALUEPROXYQT_H
