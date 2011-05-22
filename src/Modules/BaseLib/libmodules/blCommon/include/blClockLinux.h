/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _CLOCK_LINUX
#define _CLOCK_LINUX

#include <ctime>
#include "blClockImpl.h"

/** 
* \brief High precision clock
* \ingroup blUtilities
*/
class blClockLinux : public blClockImpl{
public:
	blClockLinux();
	~blClockLinux();
	/// Store the current time into m_tics1
	void Start();
	/// Store the current time into m_tics2 and compute the time 
	/// between Start( ) and Finish() = m_Temps (msec)
	double Finish();
	/// Return m_Temps
	double Temps();
protected:
	clock_t		m_ClockStart;
	clock_t		m_ClockStop;
	/// Store the last computed time interval
	double			m_Temps;
};

#endif
