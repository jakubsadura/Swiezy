/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef blSignalWriter_H
#define blSignalWriter_H

#include "BaseLibSignalWin32Header.h"
#include "blLightObject.h"
#include "blMacro.h"
#include "blSignalCollective.h"

#include <vector>
#include "CILabExceptionMacros.h"

/**
blSignal writer for all file formats

\author Xavi Planes
\date 23 Sept 2009
\ingroup blSignal
*/
class BASELIBSIGNAL_EXPORT blSignalWriter : public blLightObject 
{
public:
	typedef blSignalWriter Self;
	typedef blSmartPointer<Self> Pointer;
	blNewMacro(Self);
	cilabDeclareExceptionMacro(Exception, std::exception)
	
	typedef std::vector<std::string> ExtensionVectorType;

public:

	//!
	void SetInput( blSignalCollective::Pointer signal );

	//!
	void SetFilename( const char* );

	//!
	void Update( );

	//!
	static ExtensionVectorType GetValidExtensions( );

protected:
	blSignalWriter( );

	~blSignalWriter( );

private:

	//!
	blSignalCollective::Pointer m_Input;

	//!
	std::string m_Filename;
};

#endif //blSignalWriter_H
