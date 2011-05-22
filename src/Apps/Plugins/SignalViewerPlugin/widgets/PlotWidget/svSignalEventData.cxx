// Copyright 2006 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "svSignalEventData.h"
#include "blMitkUnicode.h"


svSignalEventData::svSignalEventData( blSignalEvent& event )
{
	m_SignalEvent = event;
}

blSignalEvent & svSignalEventData::GetSignalEvent()
{
	return m_SignalEvent;
}
