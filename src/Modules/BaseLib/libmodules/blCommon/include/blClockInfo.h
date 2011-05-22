/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __blClockInfo_h
#define __blClockInfo_h

#include <ctime>

/**
\brief   Clock info for debug
\ingroup blCommon
\author  xplanes
\date    
*/
class blClockInfo {
public:
	blClockInfo ( );
	void StartTime( );
	void StopTime( );
	void RemoveDiskIOTime( blClockInfo &clockInfo );

	/**
	* \brief Get the time from the parameter to the moment invoked in seconds
	*/
	double GetTime( );

private:
	clock_t		m_ClockStart;
	clock_t		m_ClockStop;
	bool		m_bIsRunning;
};

#endif
