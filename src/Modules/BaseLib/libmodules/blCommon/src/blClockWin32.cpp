/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "blClockWin32.h"

/**
\brief   
\ingroup 
\author  
\date    
*/

#ifdef WIN32

blClockWin32 :: blClockWin32(){
	if ( QueryPerformanceFrequency(&m_FreqCPU) == 0 )
		MessageBox(NULL, "H.P.T. No detectat \n", "", MB_OK);
	//else
	//	MessageBox(NULL, "H.P.T. Detectat a la maquina \n", "", MB_OK);

}

blClockWin32 :: ~blClockWin32(){}

void blClockWin32::Start()	{
	QueryPerformanceCounter(&m_tics1);
}

double blClockWin32::Finish()	{
	QueryPerformanceCounter(&m_tics2);
	return  Temps( );
}

double blClockWin32::Temps(){
	m_Temps = (double) (m_tics2.QuadPart - m_tics1.QuadPart ) / m_FreqCPU.QuadPart;
	return m_Temps;
}
#endif // _WIN32
