/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _blGDFUtils_H
#define _blGDFUtils_H

#include <map>
#include <string>

#include "blSignal.h"
#include "blGDFSampleType.h"
#include "blGDFEvent.h"
#include "blSignalCollective.h"

/**
GDF class for Channel

\author Chiara
\date 29 July 2009
\ingroup blSignal
*/
struct blGDFChannel{
	unsigned long SPR;
	unsigned short GDFTYP;
	unsigned char label[16];
	unsigned char phydim[6];
	unsigned short phydimnew;
	double PhysMin;
	double PhysMax;
	double digMin;
	double digMax;
	unsigned short bit;
	//! Added by Xavi to use uV units
	std::string units;
};


/**
blSignal reader for GDF files

\author Chiara Riccobene
\date 22 July 2009
\ingroup blSignal
*/
struct blGDFHeader1{
	//! GDF version
	float VERSION;
	//! GDF version
	unsigned char version[8];
	//! Patient identification (Pid)
	char PatientName[ 80 ];
	//! Header length (number of 256 byte blocks)
	uint32_t HeadLen;
	//! NS: number of signals (channels)
	uint16_t NS;
	//! Number of data records (-1 if unknown)
	int64_t numRec;
	/** Duration of a data record, as a rational number in seconds (first 
		the numerator, secondly the denominator.
	*/
	uint32_t Dur[ 2 ];
	//! The file byte order is little endian
	bool LittleEndian;

	//!
	bool ROW_BASED_CHANNELS;

	//! 
	double 		SampleRate;

	//! Startdate and time of recording
	int64_t		Startdate;

	blGDFHeader1 ( )
	{
		VERSION = 0.0;
		version[ 0 ] = 0;
		PatientName[ 0 ] = 0;
		HeadLen = 0;
		NS = 0;
		numRec = 0;
		Dur[ 0 ] = 0;
		Dur[ 1 ] = 0;
		LittleEndian = true;
		ROW_BASED_CHANNELS = 0;
		SampleRate = 0;
	}
};

/**
GDF utils for reader and writer

\author Xavi Planes
\date 9 July 2010
\ingroup blSignal
*/
class blGDFTag
{
public:
	unsigned char m_Id;
	uint32_t m_Length;
	//! depends	on tag
	char m_Value[ 256 ];
};

/**
GDF utils for reader and writer

\author Xavi Planes
\date 29 July 2009
\ingroup blSignal
*/
class blGDFUtils
{
public:
	typedef int64_t 		gdf_time; /* gdf time is represented in 64 bits */

	//!
	static void InitializeGDFTYPMap( );

	//! Convert uchar buffer to float
	static blSignal::SampleType ConvertUCHAR2Float( 
		blGDFChannel &channel, 
		unsigned char* buffer,
		bool littleEndian);

	/** Read channel data
	\param [in] subSamplingFactor: Read 1 of each N values
	*/
	static blSignalCollective::Pointer ReadDataRecord( 
		FILE *file,
		size_t &currentFilePosition,
		struct blGDFHeader1 fileHeader,
		std::vector<blGDFChannel> &chList,
		int subSamplingFactor = 1 );

	//!
	static std::map<unsigned, blGDFTYP> m_GDFTYPMap;
};

#endif //_blGDFUtils_H
