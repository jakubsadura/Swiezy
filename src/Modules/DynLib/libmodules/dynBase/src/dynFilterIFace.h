/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _dynFilterIFace_H
#define _dynFilterIFace_H


/**
Internal interface for Generic Filter

This is the generic interface for any filter.

\author Xavi Planes
\date 09 July 2010
\ingroup DynLib
*/
class dynFilterIFace
{
public:
	static dynFilter* New( );
	void SetBool(bool val);
	void SetInt(int val);
	void SetFloat(float val);
	void SetDouble(double val);
	void SetString(const char* val);

	void SetPtr( int num, void  *input);
	void *GetPtr( int num );
};



#ifdef WIN32
typedef void (dynFilterIFace::*exSetDouble)(double lower);
typedef void (dynFilterIFace::*exSetFloat)(float lower);
typedef void (dynFilterIFace::*exSetInt)(int val);
typedef void (dynFilterIFace::*exSetUInt)(unsigned int val);
typedef void (dynFilterIFace::*exSetBool)(bool val);
typedef void (dynFilterIFace::*exSetString)(const char* val);
typedef void (dynFilterIFace::*exEmpty)();
typedef void (dynFilterIFace::*exSetNthInput)(unsigned int num, void *input);
typedef void (dynFilterIFace::*exSetPtrNum)(int num, void  *input);
typedef void  *(dynFilterIFace::*exGetPtr)(int num);
typedef void (dynFilterIFace::*exSet3Double)(double, double, double);
#else
typedef void (*exSetDouble)(dynFilterIFace*, double lower);
typedef void (*exSetFloat)(dynFilterIFace*, float lower);
typedef void (*exSetInt)(dynFilterIFace*, int val);
typedef void (*exSetUInt)(dynFilterIFace*, unsigned int val);
typedef void (*exSetBool)(dynFilterIFace*, bool val);
typedef void (*exSetString)(dynFilterIFace*, const char* val);
typedef void (*exEmpty)(dynFilterIFace*);
typedef void (*exSetNthInput)(dynFilterIFace*,unsigned int num, void *input);
typedef void (*exSetPtrNum)(dynFilterIFace*,int num, void  *input);
typedef void (*exSetPtr)(dynFilterIFace*, int num, void  *input);
typedef void* (*exGetPtr)(dynFilterIFace*, int num);
typedef void (*exSet3Double)(dynFilterIFace*, double, double, double);
#endif// _WIN32

#endif // _dynFilterIFace_H

