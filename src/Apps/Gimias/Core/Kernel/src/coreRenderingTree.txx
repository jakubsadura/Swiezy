/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreRenderingTree_TXX
#define coreRenderingTree_TXX

#include "coreRenderingTree.h"
#include "coreReportExceptionMacros.h"
#include "boost/bind.hpp"


/**
Notifies when any node is modified. This is mainly used to update the 
visible property for the data tree

\param observer is the class instance of the observer
\param slotFunction is the Slot member of the observer class, following 
the specified signature
\return the boost connection, but you might not need it
*/
template <class T>
boost::signals::connection Core::RenderingTree::AddObserverOnModifiedAnyNode(
	T* observer, void (T::*slotFunction)(void))
{
	try
	{
		RenderingTree::TreeModifiedSlot _slot = boost::bind(slotFunction, observer);
		return this->m_OnModifiedAnyNodeSignal.connect(_slot);
	}
	coreCatchExceptionsAddTraceAndThrowMacro(RenderingTree::AddObserverOnModified)
}

#endif

