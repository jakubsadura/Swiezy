/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _CLOCK_IMPL
#define _CLOCK_IMPL

/** 
* \brief High precision clock
* \ingroup blUtilities
*/
class blClockImpl{
public:
	blClockImpl();
	~blClockImpl();
	/// Store the current time 
	virtual void Start() = 0;
	/// Store the current time and call Temps()
	virtual double Finish() = 0;
	/// Compute the time and return m_Temps
	virtual double Temps() = 0;
};

#endif

