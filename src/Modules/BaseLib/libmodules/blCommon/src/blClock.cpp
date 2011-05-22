/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "blClock.h"
#include "blClockWin32.h"
#include "blClockLinux.h"

/**
\brief   
\ingroup 
\author  
\date    
*/
blClock :: blClock(){

#ifdef WIN32
	m_blClock = new blClockWin32;
#else 
	m_blClock = new blClockLinux;
#endif
}

blClock :: ~blClock(){
	delete m_blClock;
	m_blClock = NULL;
}

void blClock::Start()	{
	m_blClock->Start();
}

double blClock::Finish()	{
	return  m_blClock->Finish();
}

double blClock::Temps(){
	return m_blClock->Temps();
}
