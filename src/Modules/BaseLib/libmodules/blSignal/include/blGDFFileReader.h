/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#ifndef blGDFFileReader_h
#define blGDFFileReader_h


// basic includes
#include <string>
using std::string;

#include "BaseLibSignalWin32Header.h"
#include "blLightObject.h"
#include "blSignal.h"
#include "blSignalCollective.h"
#include "blGDFUtils.h"

// Exception
#include "CILabExceptionMacros.h"


/**
blSignal reader for GDF files

\author Chiara Riccobene
\date 22 July 2009
\ingroup blSignal
*/
class BASELIBSIGNAL_EXPORT blGDFFileReader : public blLightObject 
{
public:
	typedef blGDFFileReader Self;
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
		

protected:
	//!
	blGDFFileReader( );

	//!
	~blGDFFileReader( );

	//!
	void ReadHeader1();

	//! Reads the header 2 (256*NS) of a GDF file
	void ReadHeader2();

	//! Reads the event table
	void ReadEventTable( );

	//! Read free event names
	void ReadFreeEventNames( );

private:

	//! output
	blSignalCollective::Pointer m_signalCollective;
	//! filename
	std::string m_Filename;
	// header struct
	struct blGDFHeader1 m_fileHeader;
	// list of channels
	std::vector<blGDFChannel> m_chList;
	//!
	FILE* m_file;
	//!
	size_t m_lSize;
	//!
	size_t m_CurrentFilePosition;
};

#endif // blGDFFileReader_h
