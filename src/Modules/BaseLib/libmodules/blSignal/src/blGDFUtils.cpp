/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "blGDFUtils.h"

#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>

std::map<unsigned, blGDFTYP> blGDFUtils::m_GDFTYPMap;

#define fix(m)     	(m<0 ? ceil(m) : floor(double(m)))	

#if __BYTE_ORDER == __BIG_ENDIAN
float l_endian_f32(float x) 
#elif __BYTE_ORDER == __LITTLE_ENDIAN
float b_endian_f32(float x) 
#endif
{
	union {
		float f32;
		uint32_t u32;
	} b1; 
	b1.f32 = x; 
	b1.u32 = bswap_32(b1.u32);
	return(b1.f32);
}

#if __BYTE_ORDER == __BIG_ENDIAN
double l_endian_f64(double x) 
#elif __BYTE_ORDER == __LITTLE_ENDIAN
double b_endian_f64(double x) 
#endif
{
	union {
		double f64;
		uint32_t u32[2];
	} b1,b2; 
	b1.f64 = x; 
	b2 = b1; 
	b2.u32[0] = bswap_32(b1.u32[1]);
	b2.u32[1] = bswap_32(b1.u32[0]);
	return(b2.f64);
}


void blGDFUtils::InitializeGDFTYPMap()
{

	if ( !m_GDFTYPMap.empty() )
	{
		return;
	}

	m_GDFTYPMap[ blGDFTYPCode_int8 ] = blGDFTYP( blGDFTYPCode_int8, "int8", 1 );
	m_GDFTYPMap[ blGDFTYPCode_uint8 ] = blGDFTYP( blGDFTYPCode_uint8, "uint8", 1 );
	m_GDFTYPMap[ blGDFTYPCode_int16 ] = blGDFTYP( blGDFTYPCode_int16, "int16", 2 );
	m_GDFTYPMap[ blGDFTYPCode_uint16 ] = blGDFTYP( blGDFTYPCode_uint16, "uint16", 2 );
	m_GDFTYPMap[ blGDFTYPCode_int32 ] = blGDFTYP( blGDFTYPCode_int32, "int32", 4 );
	m_GDFTYPMap[ blGDFTYPCode_uint32 ] = blGDFTYP( blGDFTYPCode_uint32, "uint32", 4 );
	m_GDFTYPMap[ blGDFTYPCode_int64 ] = blGDFTYP( blGDFTYPCode_int64, "int64", 8 );
	m_GDFTYPMap[ blGDFTYPCode_uint64 ] = blGDFTYP( blGDFTYPCode_uint64, "uint64", 8 );
	m_GDFTYPMap[ blGDFTYPCode_float32 ] = blGDFTYP( blGDFTYPCode_float32, "loat32", 4 );
	m_GDFTYPMap[ blGDFTYPCode_float64 ] = blGDFTYP( blGDFTYPCode_float64, "float64", 8 );
	m_GDFTYPMap[ blGDFTYPCode_float128 ] = blGDFTYP( blGDFTYPCode_float128, "float128", 16 );
	m_GDFTYPMap[ blGDFTYPCode_int24 ] = blGDFTYP( blGDFTYPCode_int24, "int24", 24 );
	m_GDFTYPMap[ blGDFTYPCode_uint24 ] = blGDFTYP( blGDFTYPCode_uint24, "uint24", 23 );

}

blSignal::SampleType blGDFUtils::ConvertUCHAR2Float( 
	blGDFChannel &channel, 
	unsigned char* buffer, 
	bool littleEndian )
{
	// byte order-> Do we need to swap?
#if (__BYTE_ORDER == __BIG_ENDIAN)
	char SWAP = littleEndian;  
#elif (__BYTE_ORDER == __LITTLE_ENDIAN)
	char SWAP = !littleEndian;  
#endif

	// Switch for each type of sample
	union {int16_t i16; uint16_t u16; uint32_t i32; float f32; uint64_t i64; double f64;} u;
	uint8_t bitoff = 0;
	blSignal::SampleType sampleValue = 0;
	size_t			k5 = 0,SZ = 0;
	int32_t			int32_value;

	switch ( channel.GDFTYP )
	{
	case blGDFTYPCode_int8: sampleValue = (blSignal::SampleType)(*(int8_t*)buffer);break;
	case blGDFTYPCode_uint8: sampleValue = (blSignal::SampleType)(*(uint8_t*)buffer);break;
	case blGDFTYPCode_int16: 
		if (SWAP) {
			sampleValue = (blSignal::SampleType)(int16_t)bswap_16(*(int16_t*)buffer); 
		}
		else {
			sampleValue = (blSignal::SampleType)(*(int16_t*)buffer); 
		}
		break;
	case blGDFTYPCode_uint16: 
		if (SWAP) {
			sampleValue = (blSignal::SampleType)(uint16_t)bswap_16(*(uint16_t*)buffer); 
		}
		else {
			sampleValue = (blSignal::SampleType)(*(uint16_t*)buffer); 
		}
		break;
	case blGDFTYPCode_int32: 
		if (SWAP) {
			sampleValue = (blSignal::SampleType)(int32_t)bswap_32(*(int32_t*)buffer); 
		}
		else {
			sampleValue = (blSignal::SampleType)(*(int32_t*)buffer); 
		}
		break;
	case blGDFTYPCode_uint32: 
		if (SWAP) {
			sampleValue = (blSignal::SampleType)(uint32_t)bswap_32(*(uint32_t*)buffer); 
		}
		else {
			sampleValue = (blSignal::SampleType)(*(uint32_t*)buffer); 
		}
		break;
	case blGDFTYPCode_int64: 
		if (SWAP) {
			sampleValue = (blSignal::SampleType)(int64_t)bswap_64(*(int64_t*)buffer); 
		}
		else {
			sampleValue = (blSignal::SampleType)(*(int64_t*)buffer); 
		}
		break;
	case blGDFTYPCode_uint64: 
		if (SWAP) {
			sampleValue = (blSignal::SampleType)(uint64_t)bswap_64(*(uint64_t*)buffer); 
		}
		else {
			sampleValue = (blSignal::SampleType)(*(uint64_t*)buffer); 
		}
		break;
	case blGDFTYPCode_float32: 
		if (SWAP) {
			u.i32 = bswap_32(*(uint32_t*)(buffer));
			sampleValue = (blSignal::SampleType)(u.f32);
		}
		else {
			sampleValue = (blSignal::SampleType)(*(float*)(buffer));
		}
		break;
	case blGDFTYPCode_float64: 
		if (SWAP) {
			u.i64 = bswap_64(*(uint64_t*)(buffer));
			sampleValue = (blSignal::SampleType)(u.f64);
		}
		else {
			sampleValue = (blSignal::SampleType)(*(double*)(buffer));
		}
		break;
	case blGDFTYPCode_float128: 
		break;
	case 128:	// Nihon-Kohden little-endian int16 format  
		u.u16 = leu16p(buffer) + 0x8000;
		sampleValue = (blSignal::SampleType) (u.i16); 
		break;
	case 255+12: 
		if ( littleEndian ) {
			bitoff = k5*SZ & 0x07;
#if __BYTE_ORDER == __BIG_ENDIAN
			u.i16 = (leu16p(buffer) >> (4-bitoff)) & 0x0FFF;
#elif __BYTE_ORDER == __LITTLE_ENDIAN
			u.i16 = (leu16p(buffer)>>bitoff) & 0x0FFF;
#endif		
			if (u.i16 & 0x0800) u.i16 -= 0x1000; 
			sampleValue = (blSignal::SampleType)u.i16; 
		}
		else {
			bitoff = k5*SZ & 0x07;
#if __BYTE_ORDER == __BIG_ENDIAN
			u.i16 = (beu16p(buffer) >> (4-bitoff)) & 0x0FFF;
#elif __BYTE_ORDER == __LITTLE_ENDIAN
			u.i16 = (beu16p(buffer) >> (4-bitoff)) & 0x0FFF;
#endif
			if (u.i16 & 0x0800) u.i16 -= 0x1000; 
			sampleValue = (blSignal::SampleType)u.i16; 
		}
		break;	

	case 511+12: 
		bitoff = k5*SZ & 0x07;
		if (littleEndian) {
#if __BYTE_ORDER == __BIG_ENDIAN
			sampleValue = (blSignal::SampleType)((leu16p(buffer) >> (4-bitoff)) & 0x0FFF); 
#elif __BYTE_ORDER == __LITTLE_ENDIAN
			sampleValue = (blSignal::SampleType)((leu16p(buffer) >> bitoff) & 0x0FFF); 
#endif		
		} else {
#if __BYTE_ORDER == __BIG_ENDIAN
			sampleValue = (blSignal::SampleType)((beu16p(buffer) >> (4-bitoff)) & 0x0FFF); 
#elif __BYTE_ORDER == __LITTLE_ENDIAN
			sampleValue = (blSignal::SampleType)((beu16p(buffer) >> (4-bitoff)) & 0x0FFF); 
#endif		
		}
		break;

	// blGDFTYPCode_int24
	case 255+24:
		if (littleEndian) {
			int32_value = (*(uint8_t*)(buffer)) + (*(uint8_t*)(buffer+1)<<8) + (*(int8_t*)(buffer+2)*(1<<16)); 
			sampleValue = (blSignal::SampleType)int32_value; 
		}
		else {
			int32_value = (*(uint8_t*)(buffer+2)) + (*(uint8_t*)(buffer+1)<<8) + (*(int8_t*)(buffer)*(1<<16)); 
			sampleValue = (blSignal::SampleType)int32_value; 
		}
		break;

	// blGDFTYPCode_uint24
	case 511+24:
		if (littleEndian) {
			int32_value = (*(uint8_t*)(buffer)) + (*(uint8_t*)(buffer+1)<<8) + (*(uint8_t*)(buffer+2)<<16); 
			sampleValue = (blSignal::SampleType)int32_value; 
		}
		else {
			int32_value = (*(uint8_t*)(buffer+2)) + (*(uint8_t*)(buffer+1)<<8) + (*(uint8_t*)(buffer)<<16); 
			sampleValue = (blSignal::SampleType)int32_value; 
		}
		break; 	

	}
	
	return sampleValue;
}

blSignalCollective::Pointer blGDFUtils::ReadDataRecord( 
	FILE *file,
	size_t &currentFilePosition,
	struct blGDFHeader1 fileHeader,
	std::vector<blGDFChannel> &chList,
	int subSamplingFactor /*= 1*/
	)
{
	blSignalCollective::Pointer signalCollective;
	signalCollective = blSignalCollective::New( );

	blGDFUtils::InitializeGDFTYPMap();


	if ( chList.size( ) == 0 )
	{
		return NULL;
	}


	// Read raw data
	// Get size of sample in bytes
	const size_t bit = blGDFUtils::m_GDFTYPMap[ chList.at(0).GDFTYP ].m_Size;

	//size_t offset = 65536;	
	unsigned long spr = chList.at(0).SPR; 
	const int64_t nelem = fileHeader.NS*fileHeader.numRec * spr;
	currentFilePosition += nelem;

	currentFilePosition = fileHeader.HeadLen;
	for (size_t k2 = 0; k2<fileHeader.NS ; k2++)
	{
		std::cout<<"Reading data set of channel : " << chList.at(k2).label<<std::endl;

		unsigned int numberOfRecords = (unsigned int)( fileHeader.numRec );
		if ( spr == 1 )
		{
			numberOfRecords /= subSamplingFactor;
		}
		else
		{
			spr /= subSamplingFactor;
		}

		std::string name = reinterpret_cast<const char*>( chList.at(k2).label);
		blSignal::Pointer signal = blSignal::Build( 
			name, 
			numberOfRecords,
			blSignal::Duration( fileHeader.Dur[ 0 ], fileHeader.Dur[ 1 ] ), 
			spr );
		signal->SetInternalStartTime( fileHeader.Startdate );
		signalCollective->AddSignal( signal );
	}


	// Read data values
	unsigned char* rawdata = new unsigned char[ (unsigned int)( bit ) ];

	for (size_t k2 = 0; k2<fileHeader.NS*fileHeader.numRec*spr ; k2++)
	{
		long channel;
		long sampleCount;
		if ( !fileHeader.ROW_BASED_CHANNELS )
		{
			channel = long( k2 / (fileHeader.numRec*spr) );
			sampleCount = long( k2 % (fileHeader.numRec*spr) );
		}
		else
		{
			channel = k2 % fileHeader.NS;
			sampleCount = k2 / fileHeader.NS;
		}

		size_t obj = fread( rawdata, bit, 1, file);

		// Sub sampling
		if ( sampleCount % subSamplingFactor == 0 )
		{
			sampleCount /= subSamplingFactor;
			blSignal::SampleType sampleValue = blGDFUtils::ConvertUCHAR2Float(
				chList.at(channel), 
				rawdata,
				fileHeader.LittleEndian );
			if ( sampleCount < signalCollective->GetSignal( channel )->GetNumberOfValues() )
			{
				signalCollective->GetSignal( channel )->SetValue( sampleCount, sampleValue );
			}
		}

	}


	delete [] rawdata;

	return signalCollective;
}
