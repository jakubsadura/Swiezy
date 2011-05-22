#include "gblQPushButtonEventProxy.h"
#include "gblException.h"

gbl::QPushButtonEventProxy::QPushButtonEventProxy(Bridge _bridge, const std::string& _eventId, QPushButton* _button)
: QObject(_button)
, EventProxy(_bridge, _eventId)
{
	if( !_button )
		throw gbl::Exception("Cannot create an QPushButtonEventProxy with NULL");
	connect( _button, SIGNAL(clicked()), this, SLOT(slotButtonPushed()));
}

void gbl::QPushButtonEventProxy::slotButtonPushed()
{
	this->CallEvent();
};

void gbl::AddBridgeEvent(Bridge _bridge, const std::string& _id, QPushButton* _button)
{
	new gbl::QPushButtonEventProxy(_bridge, _id, _button);
}
