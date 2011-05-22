/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef blCSVSignalWriter_H
#define blCSVSignalWriter_H

#include "BaseLibSignalWin32Header.h"
#include "blLightObject.h"
#include "blMacro.h"
#include "blSignalCollective.h"

#include <vector>
#include "CILabExceptionMacros.h"

/**
blSignal writer for CSV files

\author Xavi Planes
\date 22 July 2009
\ingroup blSignal
*/
class BASELIBSIGNAL_EXPORT blCSVSignalWriter : public blLightObject 
{
public:
	typedef blCSVSignalWriter Self;
	typedef blSmartPointer<Self> Pointer;
	blNewMacro(Self);
	cilabDeclareExceptionMacro(Exception, std::exception)
	
public:

	//!
	void SetInput( blSignalCollective::Pointer signal );

	//!
	void SetFilename( const char* );

	//!
	void Update( );

protected:
	blCSVSignalWriter( );

	~blCSVSignalWriter( );

private:

	//!
	blSignalCollective::Pointer m_Input;

	//!
	std::string m_Filename;
};

#endif //blCSVSignalWriter_H
