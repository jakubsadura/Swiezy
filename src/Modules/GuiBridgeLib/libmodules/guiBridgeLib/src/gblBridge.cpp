/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "gblBridge.h"
#include "boost/signal.hpp"
#include "boost/algorithm/string.hpp"

struct gbl::Bridge::Impl
{
	typedef boost::signal<void ()>::slot_function_type InternalSlotType;

	void SetEventSlot(const std::string& _id, SlotType slot)
	{
		Impl::EventSlotMap::iterator it = this->eventSlots.find(_id);
		if( it != this->eventSlots.end() )
		{
			std::string message = "Id " + _id + " already in use";
			throw gbl::BridgeException(message.c_str());
		}
		this->eventSlots.insert( std::make_pair(_id, slot) ); 
	}

	void CallEvent(const std::string& _id)
	{
		Impl::EventSlotMap::iterator it = this->eventSlots.find(_id);
		if( it == this->eventSlots.end() )
		{
			std::string message = "gbl::Bridge::CallEvent Cannot find _id " + _id;
			throw gbl::BridgeException(message.c_str());
		}
		this->eventSlots[_id]();
	}

	//! Stores the mapping from event id to slot.
	typedef std::map< std::string, InternalSlotType> EventSlotMap;
	//! See EventSlotMap.
	EventSlotMap eventSlots;
};

gbl::Bridge::Bridge() : impl(new gbl::Bridge::Impl)
{
}

gbl::Bridge::~Bridge()
{
}

void gbl::Bridge::SetEventSlot(const std::string& id, SlotType slot)
{
	this->impl->SetEventSlot(this->m_autoPrefix + id, slot);
}

void gbl::Bridge::CallEvent(const std::string& id)
{
	this->impl->CallEvent(this->m_autoPrefix + id);
}

gbl::Bridge gbl::Bridge::operator[](const std::string& _prefix) const
{
	Bridge result(*this);
	result.impl = this->impl;
	result.m_autoPrefix = this->m_autoPrefix + _prefix + ".";
	return result;
}

const std::string& gbl::Bridge::GetAutoPrefix() const
{
	return this->m_autoPrefix;
}

void gbl::Bridge::SetAutoPrefix(const std::string& _prefix)
{
	this->m_autoPrefix = _prefix;
}

void gbl::Bridge::ShallowCopyFrom(gbl::Bridge _rhs)
{
	this->impl = _rhs.impl;
	this->m_autoPrefix = _rhs.m_autoPrefix;
}

gbl::Bridge::Bridge(const Bridge& _rhs)
{
	this->impl = _rhs.impl;
	this->m_autoPrefix = _rhs.m_autoPrefix;
}

const gbl::Bridge& gbl::Bridge::operator=(const gbl::Bridge& _rhs)
{
	this->impl = _rhs.impl;
	this->m_autoPrefix = _rhs.m_autoPrefix;
	return *this;
}

bool gbl::Bridge::HasEvent(const std::string& _id) const
{
	return this->impl->eventSlots.find(this->m_autoPrefix + _id) != impl->eventSlots.end();
}

bool gbl::InitBridge(Bridge& _leftHandSide, Bridge _rightHandSide)
{
	//if( _leftHandSide.impl->values.size() > 0 || _leftHandSide.impl->eventSlots.size() > 0 )
	if( _leftHandSide.impl->eventSlots.size() > 0 )
	{
		std::string message = "Logical error: InitBridge should be called with an empty Bridge as left hand side argument.";
		throw gbl::BridgeException(message.c_str());
	}
	if( _leftHandSide.impl == _rightHandSide.impl )
	{
		return false;
	}

	_leftHandSide.ShallowCopyFrom(_rightHandSide);
	return true;
}

struct TestSetBridgeEventSlot
{
public:
	void Run()
	{
		gbl::Bridge bridge;
		gbl::SetBridgeEventSlot(bridge, "test", this, &TestSetBridgeEventSlot::Run);
	}
};
