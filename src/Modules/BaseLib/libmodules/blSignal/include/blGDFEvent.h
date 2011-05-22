/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _blGDFEvent_H
#define _blGDFEvent_H

#include <map>
#include <string>
#include "blGDFSampleType.h"

/**
GDF class for Event

\author Chiara
\date 29 July 2009
\ingroup blSignal
*/
struct blGDFEvent{
	uint32_t position;
	uint32_t duration;
	uint16_t type;
	uint16_t channel;
};

/**
GDF class for Event Table

\author Xavi Planes
\date 29 July 2009
\ingroup blSignal
*/
struct blGDFEventTable{
	uint8_t mode;
	uint32_t numOfEvt;
	float  	sampleRate;
};

#endif //_blGDFEvent_H
