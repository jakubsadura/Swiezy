/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifdef WIN32
#ifndef _CLOCK_WIN32
#define _CLOCK_WIN32

#include <conio.h>
#include <windows.h>
#include "blClockImpl.h"

/** 
* \brief High precision clock
* \ingroup blUtilities
*/
class blClockWin32 : public blClockImpl{

public:
	blClockWin32();
	~blClockWin32();
	/// Store the current time into m_tics1
	void Start();
	/// Store the current time into m_tics2 and compute the time 
	/// between Start( ) and Finish() = m_Temps (msec)
	double Finish();
	/// Return m_Temps
	double Temps();

protected:
	/// Freq. of the CPU
	LARGE_INTEGER	m_FreqCPU;
	/// Start ticks stored
	LARGE_INTEGER	m_tics1;
	/// End ticks stored
	LARGE_INTEGER	m_tics2;
	/// Store the last computed time interval
	double			m_Temps;
};

#endif

#endif // _WIN32
