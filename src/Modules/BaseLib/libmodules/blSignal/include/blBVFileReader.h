/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#ifndef blBVFileReader_h
#define blBVFileReader_h


// basic includes
#include <string>

#include "BaseLibSignalWin32Header.h"
#include "blLightObject.h"
#include "blSignal.h"
#include "blSignalCollective.h"
#include "blGDFUtils.h"
#include "blTagMap.h"

// Exception
#include "CILabExceptionMacros.h"

/**
blSignal reader for BrainVision files

\author Chiara Riccobene
\date 22 July 2009
\ingroup blSignal
*/
struct blBVHeader1 : public blGDFHeader1{

	//! Filename
	const char* 	FileName;
	//! Extension of filename
	const char* 	ext;
	//! Header ASCII buffer
	uint8_t*	Header;
	//! 
	//double 		SampleRate;
	//! Extension of data file
	char 	DataFileName[256];
	//! data file is ASCII or Binary
	bool FLAG_ASCII;
	//! 
	enum o_t{VEC,MUL} orientation;

	//!
	double physmax,physmin,digmax,digmin,cal,off;
	//!
	char OVERFLOWDETECTION;
	//! total bytes per block 
	uint32_t bpb; 
	//! samples per record (block)
	uint32_t 	SPR;
	//! Number of points
	uint32_t 	dataPoints;

	blBVHeader1 ( )
	{
		FileName = 0;
		ext = 0;
		Header = 0;
	//	SampleRate = 0;
		DataFileName[0] = 0;
		FLAG_ASCII = 0;
		orientation = blBVHeader1::MUL;
		digmax = 0 ;
		digmin = 0;
		VERSION = 0.0;
		version[ 0 ] = 0;
		PatientName[ 0 ] = 0;
		HeadLen = 0;
		NS = 0;
		numRec = 0;
		Dur[ 0 ] = 0;
		Dur[ 1 ] = 0;
		LittleEndian = true;
		physmax=1e6;
		physmin=-1e6;
		digmax=1e6;
		digmin=-1e6;
		cal=1.0;
		off=0.0;
		OVERFLOWDETECTION = 0;
		bpb = 0;
	}
};

/**
blSignal reader for GDF files

\author Chiara Riccobene
\date 22 July 2009
\ingroup blSignal
*/
class BASELIB_EXPORT blBVFileReader : public blLightObject 
{
public:  
	typedef blBVFileReader Self;
	typedef blSmartPointer<Self> Pointer;
	blNewMacro(Self);
	cilabDeclareExceptionMacro(Exception, std::exception)

public:

	//! Get the read gdf file as a collection of signals 
	/*!
	\return The GDF file as a collection of signals
	*/
	blSignalCollective::Pointer GetOutput();	
	
	//! Reads the GDF file. Call it before calling GetOutput 
	/*!
	\sa GetOutput
	*/
	void Update();
	
	//! Sets the filename of the gdf file to be read
	/*!
	\param  filename the name of the file
	\return 
	*/
	void  SetFilename( const char* filename);

	//! Read header info
	void ReadHeader( );

	//!
	void GetHeaderTags( blTagMap::Pointer tagMap );

	//!
	void SetHeaderTags( blTagMap::Pointer tagMap );

protected:
	//!
	blBVFileReader( );

	//!
	~blBVFileReader( );

	//!
	void ReadFileHeader();

	//! Reads the header 2 (256*NS) of a GDF file
	void DecodeHeader();

	//! Reads the data record header (where the signal data is stored) of a GDF file
	void ReadDataRecord( );

	//! Reads the event table
	void ReadEventTable( );

	//! Read [Common Infos] sequence
	void ReadSeq1( char *t );

	//! Read [Binary Infos]
	void ReadSeq2( char *t );

	//! Read [Channel Infos]
	void ReadSeq3( char *t );

	//! Fill channels info
	void InitChannels();

private:

	//! output
	blSignalCollective::Pointer m_signalCollective;
	//! filename
	std::string m_Filename;
	// header struct
	struct blBVHeader1 m_FileHeader;
	// list of channels
	std::vector<blGDFChannel> m_chList;
	//!
	FILE* m_file;
	//!
	size_t m_lSize;
	//!
	size_t m_CurrentFilePosition;
	//! type of data
	unsigned short m_gdftyp;
	//!
	int m_SubSamplingFactor;
};

#endif // blGDFFileReader_h
