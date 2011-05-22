/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreDataArray_H
#define _coreDataArray_H

#include "blLightObject.h"
#include "gmDataHandlingWin32Header.h"

namespace Core{

/**
Stores a pointer to a buffer of basic types like char, int, float, ...

Allows to copy and cast between two buffers of different types

Use the name to match registered types

\author Xavi Planes
\date 22 sept 2010
\ingroup gmDataHandling
*/
class GMDATAHANDLING_EXPORT AnyBuffer : public blLightObject
{
public:
	static std::map<std::string, int> m_MapRegisteredTypes;

public:
	typedef AnyBuffer Self;
	typedef blSmartPointer<Self> Pointer;

	//! Create an array using the buffer pointer and number of elements
	static Pointer NewBuffer( const std::string &name, size_t n = 0, void* p = 0 );

	//! Get name of the array
	virtual std::string GetName( ) = 0;

	//! Get name of the element types
	virtual std::string GetDataTypeName( ) = 0;

	//! Get buffer pointer
	virtual void* GetBuffer( ) = 0;

	//! Get number of elements
	virtual size_t GetSize( ) = 0;

private:
	//!
	static void Initialize( );

protected:
	AnyBuffer( ){}

	virtual ~AnyBuffer( ){}
};

/**
Copy and cast between two buffers of different types. If the types are the
same, use memcpy, else iterate over the buffer and cast all elements

\author Xavi Planes
\date 22 sept 2010
\ingroup gmDataHandling
*/
bool GMDATAHANDLING_EXPORT CopyBuffer( 
	AnyBuffer::Pointer src, AnyBuffer::Pointer dst );


} // Core

#endif // _coreDataArray_H
