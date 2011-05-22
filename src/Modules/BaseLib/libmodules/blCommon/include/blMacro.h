/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __blMacro_h
#define __blMacro_h

/*************************************************************
*	MSVC++ 6.0 WARNING DISABLE
*	warning C4275: non dll-interface 
*	class 'std::_Complex_base<bloat>' used as base for dll-interface class 
*	'std::complex<bloat>'
*************************************************************/
	
/************************************************************/

/**
\brief   Definitions of common macros
\ingroup blCommon
\author  Constantine Butakoff
\date    2007-10-16
*/

/**
 * \brief Macro for new operators
 * \ingroup 
 */
#define blNewMacro(x) \
static Pointer New(void) \
{ \
  Pointer smartPtr; \
  x* rawPtr = new x; \
  smartPtr = rawPtr; \
  rawPtr->UnRegister(); \
  return smartPtr; \
}

#define blNewMacro1param(x,param1) \
	static Pointer New(param1 p1) \
{ \
	Pointer smartPtr; \
	x* rawPtr = new x(p1); \
	smartPtr = rawPtr; \
	rawPtr->UnRegister(); \
	return smartPtr; \
}

#define blNewMacro2param(x,param1,param2) \
	static Pointer New(param1 p1, param2 p2) \
{ \
	Pointer smartPtr; \
	x* rawPtr = new x(p1,p2); \
	smartPtr = rawPtr; \
	rawPtr->UnRegister(); \
	return smartPtr; \
}

#define blNewMacro3param(x,param1,param2,param3) \
	static Pointer New(param1 p1, param2 p2, param3 p3) \
{ \
	Pointer smartPtr; \
	x* rawPtr = new x(p1,p2,p3); \
	smartPtr = rawPtr; \
	rawPtr->UnRegister(); \
	return smartPtr; \
}

#endif

/*************************************************************
*	COMPILATION FLAGS TO INCLUDE / EXCLUDE SPECIFIC
*			blSMOLUB PARTS
*************************************************************/

///#define _INCLUDE_blSMOLIB_AAM
///#define _INCLUDE_MAHALANOBIS_DATACLASSIFIER

