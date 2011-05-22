/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef blGDFFileWriter_h
#define blGDFFileWriter_h


// basic includes
#include <string>
using std::string;

#include "BaseLibSignalWin32Header.h"
#include "blLightObject.h"
#include "blSignal.h"
#include "blSignalCollective.h"
#include "CILabExceptionMacros.h"

/**
blSignal writer for GDF files

\author Chiara Riccobene
\date 22 July 2009
\ingroup blSignal
*/

class BASELIBSIGNAL_EXPORT blGDFFileWriter : public blLightObject
{

public:
	typedef blGDFFileWriter Self;
	typedef blSmartPointer<Self> Pointer;
	blNewMacro(Self);
	cilabDeclareExceptionMacro(Exception, std::exception)

public:

	//! Sets the filename of output gdf file to be written
	/*!
	\param  [in] the filename 
	*/
	void SetFilename( const char* filename);

	//! Set the blSignal to be written as a gdf file
	/*!
	\param signal [in] 
	*/
	void SetInput( blSignalCollective::Pointer signal );
	
	//! Writes the gdf file. Call it after set input
	/*!
	*/
	void Update( );
		
protected:
		//!
		blGDFFileWriter();
		//!
		~blGDFFileWriter();

private:

	//! Target
	blSignalCollective::Pointer m_SignalInput;

	std::string m_Filename;
};

#endif // blGDFFileWriter_h
