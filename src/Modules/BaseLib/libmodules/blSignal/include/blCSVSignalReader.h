/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef blCSVSignalReader_H
#define blCSVSignalReader_H

#include "BaseLibSignalWin32Header.h"
#include "blLightObject.h"
#include "blSignalCollective.h"
#include "CILabExceptionMacros.h"

#include <vector>
#include <list>

/**
blSignal reader for CSV files

\author Xavi Planes
\date 22 July 2009
\ingroup blSignal
*/
class BASELIBSIGNAL_EXPORT blCSVSignalReader : public blLightObject 
{
public:
	typedef blCSVSignalReader Self;
	typedef blSmartPointer<Self> Pointer;
	blNewMacro(Self);
	cilabDeclareExceptionMacro(Exception, std::exception)
	
public:

	//!
	void SetFilename( const char* );

	//!
	void Update( );

	//!
	blSignalCollective::Pointer GetOutput( );


protected:
	blCSVSignalReader( );

	~blCSVSignalReader( );

	void ParseLine( 
		std::string &line,
		std::vector<blSignal::SampleType> &data );

private:

	//!
	blSignalCollective::Pointer m_Output;

	//!
	std::string m_Filename;
};

#endif //blCSVSignalReader_H
