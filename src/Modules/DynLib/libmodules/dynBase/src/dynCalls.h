/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _dynCalls_H
#define _dynCalls_H

#include "DynLibWin32Header.h"


#ifdef WIN32
#define CALL_MEMBER_FN(object,name,funcType) \
	(object->*( force_cast<funcType>( FindSymbol( name ) ) ) ) ()
#define CALL_MEMBER_FN1(object,name,funcType,val) \
	(object->*( force_cast<funcType>( FindSymbol( name ) ) ) ) (val)
#define CALL_MEMBER_FN2(object,name,funcType,val1, val2) \
	(object->*( force_cast<funcType>( FindSymbol( name ) ) ) ) (val1, val2)
#define CALL_MEMBER_FN3(object,name,funcType,val1,val2,val3) \
	(object->*( force_cast<funcType>( FindSymbol( name ) ) ) ) (val1, val2,val3)
#else
#define CALL_MEMBER_FN(object,name,funcType) \
	( force_cast<funcType>( FindSymbol( name ) ) )( object )
#define CALL_MEMBER_FN1(object,name,funcType,val) \
	( force_cast<funcType>( FindSymbol( name ) ) ) (object,val)
#define CALL_MEMBER_FN2(object,name,funcType,val1,val2) \
	( force_cast<funcType>( FindSymbol( name ) ) ) (object,val1,val2)
#define CALL_MEMBER_FN3(object,name,funcType,val1,val2,val3) \
	( force_cast<funcType>( FindSymbol( name ) ) ) (object,val1,val2,val3)
#endif

#define CALL_FN(name,funcType) \
	( force_cast<funcType>( FindSymbol( name ) ) ) ()

template<class Dest, class Src>
Dest force_cast(Src src)
{
	union
	{
		Dest d;
		Src s;
	} convertor;

	convertor.s = src;
	return convertor.d;
}

#endif // _dynCalls_H

