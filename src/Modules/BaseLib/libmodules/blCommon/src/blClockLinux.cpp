/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "blClockLinux.h"

/**
\brief   
\ingroup 
\author  
\date    
*/
blClockLinux :: blClockLinux(){
	m_ClockStart = 0;
	m_ClockStop = 0;
}

blClockLinux :: ~blClockLinux(){}

void blClockLinux::Start()	{
	m_ClockStart = clock( );
}

double blClockLinux::Finish()	{
	m_ClockStop = clock( );
	return  Temps( );
}

double blClockLinux::Temps(){
	m_Temps = (double) (m_ClockStop - m_ClockStart ) / CLOCKS_PER_SEC;
	return m_Temps;
}
