/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreAnyBuffer.h"

#include "vtkType.h"

using namespace Core;

/**
Stores a pointer to a buffer of basic types like char, int, float, ...

\author Xavi Planes
\date 22 sept 2010
\ingroup gmDataHandling
*/
template <class T>
class AnyBufferTemplate: public AnyBuffer
{
public:
	typedef AnyBufferTemplate Self;
	typedef blSmartPointer<Self> Pointer;
	blNewMacro( AnyBufferTemplate<T> )
	static typename AnyBuffer::Pointer NewBase(const std::string &name, size_t n, T* buffer = 0 )
	{
		AnyBuffer::Pointer smartPtr;
		AnyBuffer* rawPtr = new AnyBufferTemplate<T>(name, n, buffer );
		smartPtr = rawPtr;
		rawPtr->UnRegister();
		return smartPtr;
	}

	std::string GetName( ){return m_Name;}
	std::string GetDataTypeName( ){return typeid( T ).name( );}
	virtual void* GetBuffer( ){return m_Array;}
	virtual size_t GetSize( ){return m_ArraySize;}

private:

	AnyBufferTemplate( const std::string &name, size_t n, T* buffer = 0 )
	{
		m_Name = name;
		m_Array = buffer;
		m_ArraySize = n;
		m_AllocatedMemory = false;
		if ( buffer == 0 && m_ArraySize > 0 )
		{
			buffer = new T[ n ];
			m_AllocatedMemory = true;
		}
	}

	AnyBufferTemplate( const AnyBufferTemplate& a );

	~AnyBufferTemplate( )
	{
		if ( m_AllocatedMemory )
		{
			delete [] m_Array;
			m_Array = NULL;
		}
	}

	void operator=(const AnyBufferTemplate&);

private:
	//! Pointer to data
	T* m_Array;
	//!
	size_t m_ArraySize;
	//!
	std::string m_Name;
	//!
	bool m_AllocatedMemory;
};


/**
Enumeration used for optimize the access to a subclass, using a switch instead
of comparing the string of the name of the type_info
*/
enum
{
	TYPE_CHAR,
	TYPE_UCHAR,
	TYPE_INT,
	TYPE_UINT,
	TYPE_SHORT,
	TYPE_USHORT,
	TYPE_FLOAT,
	TYPE_DOUBLE,
	TYPE_LONG,
	TYPE_ULONG,
	TYPE_VTK_ID_TYPE
} INTERNAL_TYPE;

std::map<std::string, int> Core::AnyBuffer::m_MapRegisteredTypes;

void Core::AnyBuffer::Initialize()
{
	if ( m_MapRegisteredTypes.empty() )
	{
		m_MapRegisteredTypes[ "char" ] = TYPE_CHAR;
		m_MapRegisteredTypes[ "unsigned char" ] = TYPE_UCHAR;
		m_MapRegisteredTypes[ "int" ] = TYPE_INT;
		m_MapRegisteredTypes[ "unsigned int" ] = TYPE_UINT;
		m_MapRegisteredTypes[ "short" ] = TYPE_SHORT;
		m_MapRegisteredTypes[ "unsigned short" ] = TYPE_USHORT;
		m_MapRegisteredTypes[ "float" ] = TYPE_FLOAT;
		m_MapRegisteredTypes[ "double" ] = TYPE_DOUBLE;
		m_MapRegisteredTypes[ "long" ] = TYPE_LONG;
		m_MapRegisteredTypes[ "unsigned long" ] = TYPE_ULONG;
		m_MapRegisteredTypes[ "vtkIdType" ] = TYPE_VTK_ID_TYPE;
	}
}

/**
Copy two buffers of same size
*/
template <class T1, class T2>
void CopyInternalBuffer( T1* src, T2* dst, size_t n )
{
	if ( typeid(T1) == typeid(T2) )
	{
		memcpy(dst, src, n * sizeof( T1 ) );
	}
	else
	{
		for ( int i = 0 ; i < n ; i++ )
		{
			dst[ i ] = src[ i ];
		}
	}
}

/**
Call function func defining the typedef T as the type of the elements of arrayT
*/
#define CHECK_TYPE( T, typeN, type1, func ) \
	case typeN: \
	{ \
		typedef type1 T; \
		func; \
	} \
	break;

/**
Call function func defining the typedef T as the type of the elements of arrayT
*/
#define CHECK_ALL_TYPES( T, typeN, func ) \
	switch( typeN ) \
	{ \
	CHECK_TYPE( T, TYPE_CHAR, char, func ) \
	CHECK_TYPE( T, TYPE_UCHAR, unsigned char, func ) \
	CHECK_TYPE( T, TYPE_INT, int, func ) \
	CHECK_TYPE( T, TYPE_UINT, unsigned int, func ) \
	CHECK_TYPE( T, TYPE_SHORT, short, func ) \
	CHECK_TYPE( T, TYPE_USHORT, unsigned short, func ) \
	CHECK_TYPE( T, TYPE_FLOAT, float, func ) \
	CHECK_TYPE( T, TYPE_DOUBLE, double, func ) \
	CHECK_TYPE( T, TYPE_LONG, long, func ) \
	CHECK_TYPE( T, TYPE_ULONG, unsigned long, func ) \
	CHECK_TYPE( T, TYPE_VTK_ID_TYPE, vtkIdType, func ) \
	}


Core::AnyBuffer::Pointer Core::AnyBuffer::NewBuffer( const std::string &name, size_t n, void* buffer )
{
	Initialize();

	int typeN = m_MapRegisteredTypes[ name ];

	CHECK_ALL_TYPES( T1, typeN, return AnyBufferTemplate<T1>::NewBase( name, n, (T1*) buffer ); )

	return NULL;
}


bool Core::CopyBuffer( AnyBuffer::Pointer src, AnyBuffer::Pointer dst )
{
	if ( src->GetSize() != dst->GetSize() )
	{
		return false;
	}

	int type1 = Core::AnyBuffer::m_MapRegisteredTypes[ src->GetName( ) ];
	int type2 = Core::AnyBuffer::m_MapRegisteredTypes[ dst->GetName( ) ];

	CHECK_ALL_TYPES( T1, type1, 
		CHECK_ALL_TYPES( T2, type2, 
			CopyInternalBuffer( (T1*) src->GetBuffer( ), (T2*) dst->GetBuffer( ), src->GetSize( ) ) ) )

	return true;
}
