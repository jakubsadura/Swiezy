#ifndef GBLQPUSHBUTTONEVENTPROXY_H
#define GBLQPUSHBUTTONEVENTPROXY_H

#include "gblBridge.h"
#include "gblEventProxy.h"
#include "qpushbutton.h"
#include "qobject.h"

namespace gbl
{

/**
This is an event proxy for QPushButton.
\sa EventProxy.

\author Maarten Nieber
\date 25 Sep /2007
*/

class /*GUIBRIDGELIB_EXPORT*/ QPushButtonEventProxy 
: public QObject
, public EventProxy
{
Q_OBJECT

public:
	//!
	QPushButtonEventProxy(Bridge _bridge, const std::string& _eventId, QPushButton* _button);

public slots:
	//!
	void slotButtonPushed();
};

//! Use this function to add a QPushButtonEventProxy to the bridge.
void AddBridgeEvent(Bridge _bridge, const std::string& _id, QPushButton* _button);

} // namespace gbl

#endif //GBLQPUSHBUTTONEVENTPROXY_H
