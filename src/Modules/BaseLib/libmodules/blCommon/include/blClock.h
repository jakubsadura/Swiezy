/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _CLOCK
#define _CLOCK

class blClockImpl;

/** 
* \brief High precision clock
* \ingroup blUtilities
*/
class blClock{
public:
	blClock();
	~blClock();
	/// Store the current time 
	void Start();
	/// Store the current time and call Temps()
	double Finish();
	/// Compute the time and return m_Temps
	double Temps();
protected:
	blClockImpl		*m_blClock;
};

#endif
