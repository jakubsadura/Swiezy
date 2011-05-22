/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef blSignalCompare_H
#define blSignalCompare_H

#include "BaseLibSignalWin32Header.h"
#include "blLightObject.h"
#include "blSignalCollective.h"
#include "CILabExceptionMacros.h"

#include <vector>
#include <list>

/**
blSignal reader for CSV files

\ingroup 
\author Xavi Planes
\date 22 July 2009
*/
class BASELIBSIGNAL_EXPORT blSignalCompare : public blLightObject 
{
public:
	typedef blSignalCompare Self;
	typedef blSmartPointer<Self> Pointer;
	blNewMacro(Self);
	cilabDeclareExceptionMacro(Exception, std::exception)
	
public:

	//!
	void SetFilename1( const char* );

	//!
	void SetFilename2( const char* );

	//!
	void Update( );

	//!
	bool GetOutput( );

protected:
	blSignalCompare( );

	~blSignalCompare( );

private:

	//!
	std::string m_Filename1;

	//!
	std::string m_Filename2;

	//!
	bool m_bResult;
};

#endif //blSignalCompare_H
