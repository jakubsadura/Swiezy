/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _blGDFSampleType_H
#define _blGDFSampleType_H

#include <map>
#include <string>


#if	WIN32
#define __BIG_ENDIAN  	 4321
#define __LITTLE_ENDIAN  1234
#define __BYTE_ORDER 	__LITTLE_ENDIAN
#endif 

#ifdef WIN32
  #ifdef _MSC_VER
    // not after VC++ 2010
    #if _MSC_VER < 1600
    typedef unsigned __int64 uint64_t;
    typedef __int64 int64_t;
    typedef unsigned long uint32_t;
    typedef long int32_t;
    typedef unsigned short uint16_t;
    typedef short int16_t;
    typedef unsigned char uint8_t;
    typedef char int8_t;
    #else
    #include <stdint.h>
    #endif
  #endif /* _MSC_VER */
#else
#include <inttypes.h>
#endif /* WIN32 */

#ifndef _BYTESWAP_H
/* define our own version - needed for Max OS X*/
#define bswap_16(x)   \
	((((x) & 0xff00) >> 8) | (((x) & 0x00ff) << 8))

#define bswap_32(x)   \
	((((x) & 0xff000000) >> 24) \
	| (((x) & 0x00ff0000) >> 8)  \
	| (((x) & 0x0000ff00) << 8)  \
	| (((x) & 0x000000ff) << 24))

#define bswap_64(x) \
	((((x) & 0xff00000000000000ull) >> 56)	\
	| (((x) & 0x00ff000000000000ull) >> 40)	\
	| (((x) & 0x0000ff0000000000ull) >> 24)	\
	| (((x) & 0x000000ff00000000ull) >> 8)	\
	| (((x) & 0x00000000ff000000ull) << 8)	\
	| (((x) & 0x0000000000ff0000ull) << 24)	\
	| (((x) & 0x000000000000ff00ull) << 40)	\
	| (((x) & 0x00000000000000ffull) << 56))
#endif  /* _BYTESWAP_H */


#if __BYTE_ORDER == __BIG_ENDIAN
#define l_endian_u16(x) ((uint16_t)bswap_16((uint16_t)(x)))
#define l_endian_u32(x) ((uint32_t)bswap_32((uint32_t)(x)))
#define l_endian_u64(x) ((uint64_t)bswap_64((uint64_t)(x)))
#define l_endian_i16(x) ((int16_t)bswap_16((int16_t)(x)))
#define l_endian_i32(x) ((int32_t)bswap_32((int32_t)(x)))
#define l_endian_i64(x) ((int64_t)bswap_64((int64_t)(x)))
float   l_endian_f32(float x); 
double  l_endian_f64(double x); 

#define b_endian_u16(x) ((uint16_t)(x))
#define b_endian_u32(x) ((uint32_t)(x))
#define b_endian_u64(x) ((uint64_t)(x))
#define b_endian_i16(x) ((int16_t)(x))
#define b_endian_i32(x) ((int32_t)(x))
#define b_endian_i64(x) ((int64_t)(x))
#define b_endian_f32(x) ((float)(x))
#define b_endian_f64(x) ((double)(x))

#elif __BYTE_ORDER==__LITTLE_ENDIAN
#define l_endian_u16(x) ((uint16_t)(x))
#define l_endian_u32(x) ((uint32_t)(x))
#define l_endian_u64(x) ((uint64_t)(x))
#define l_endian_i16(x) ((int16_t)(x))
#define l_endian_i32(x) ((int32_t)(x))
#define l_endian_i64(x) ((int64_t)(x))
#define l_endian_f32(x) ((float)(x))
#define l_endian_f64(x) ((double)(x))

#define b_endian_u16(x) ((uint16_t)bswap_16((uint16_t)(x)))
#define b_endian_u32(x) ((uint32_t)bswap_32((uint32_t)(x)))
#define b_endian_u64(x) ((uint64_t)bswap_64((uint64_t)(x)))
#define b_endian_i16(x) ((int16_t)bswap_16((int16_t)(x)))
#define b_endian_i32(x) ((int32_t)bswap_32((int32_t)(x)))
#define b_endian_i64(x) ((int64_t)bswap_64((int64_t)(x)))
float   b_endian_f32(float x); 
double  b_endian_f64(double x); 

#endif /* __BYTE_ORDER */

// if misaligned data words can be handled 
#define leu16p(i) l_endian_u16(*(uint16_t*)(i))
#define lei16p(i) l_endian_i16(*( int16_t*)(i))
#define leu32p(i) l_endian_u32(*(uint32_t*)(i))
#define lei32p(i) l_endian_i32(*( int32_t*)(i))
#define leu64p(i) l_endian_u64(*(uint64_t*)(i))
#define lei64p(i) l_endian_i64(*( int64_t*)(i))
#define lef32p(i) l_endian_f32(*(float*)(i))
#define lef64p(i) l_endian_f64(*(double*)(i))

#define beu16p(i) b_endian_u16(*(uint16_t*)(i))
#define bei16p(i) b_endian_i16(*( int16_t*)(i))
#define beu32p(i) b_endian_u32(*(uint32_t*)(i))
#define bei32p(i) b_endian_i32(*( int32_t*)(i))
#define beu64p(i) b_endian_u64(*(uint64_t*)(i))
#define bei64p(i) b_endian_i64(*( int64_t*)(i))
#define bef32p(i) b_endian_f32(*(float*)(i))
#define bef64p(i) b_endian_f64(*(double*)(i))

#define leu16a(i,r) (*(uint16_t*)r = l_endian_u16(i))
#define lei16a(i,r) (*( int16_t*)r = l_endian_i16(i))
#define leu32a(i,r) (*(uint32_t*)r = l_endian_u32(i))
#define lei32a(i,r) (*( int32_t*)r = l_endian_i32(i))
#define leu64a(i,r) (*(uint64_t*)r = l_endian_u64(i))
#define lei64a(i,r) (*( int64_t*)r = l_endian_i64(i))
#define lef32a(i,r) (*(uint32_t*)r = l_endian_f32(i))
#define lef64a(i,r) (*(uint64_t*)r = l_endian_f64(i))

#define beu16a(i,r) (*(uint16_t*)r = b_endian_u16(i))
#define bei16a(i,r) (*( int16_t*)r = b_endian_i16(i))
#define beu32a(i,r) (*(uint32_t*)r = b_endian_u32(i))
#define bei32a(i,r) (*( int32_t*)r = b_endian_i32(i))
#define beu64a(i,r) (*(uint64_t*)r = b_endian_u64(i))
#define bei64a(i,r) (*( int64_t*)r = b_endian_i64(i))
#define bef32a(i,r) (*(uint32_t*)r = b_endian_f32(i))
#define bef64a(i,r) (*(uint64_t*)r = b_endian_f64(i))


/**
GDF sample type

\author Xavi Planes
\date 29 July 2009
\ingroup blSignal
*/
enum blGDFTYPCode
{
	blGDFTYPCode_int8 = 1,
	blGDFTYPCode_uint8 = 2,
	blGDFTYPCode_int16 = 3,
	blGDFTYPCode_uint16 = 4,
	blGDFTYPCode_int32 = 5, 
	blGDFTYPCode_uint32 = 6,
	blGDFTYPCode_int64 = 7,
	blGDFTYPCode_uint64 = 8,
	blGDFTYPCode_float32 = 16,
	blGDFTYPCode_float64 = 17,
	blGDFTYPCode_float128 = 18,
	blGDFTYPCode_int24 = 279,
	blGDFTYPCode_uint24 = 535
};


/**
GDF class for GDFTYP

\author Xavi Planes
\date 29 July 2009
\ingroup blSignal
*/
class blGDFTYP
{
public:

	blGDFTYP( unsigned code = 0, std::string name = "", unsigned size = 0 )
	{
		m_Name = name;
		m_Size = size;
		m_Code = code;
	}

public:
	//! Code
	unsigned m_Code;
	//! Name
	std::string m_Name;
	//! Size per sample in bytes
	unsigned m_Size;
};

#endif //_blGDFSampleType_H
