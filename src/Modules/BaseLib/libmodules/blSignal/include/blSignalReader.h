/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef blSignalReader_H
#define blSignalReader_H

#include "BaseLibSignalWin32Header.h"
#include "blLightObject.h"
#include "blSignalCollective.h"
#include "CILabExceptionMacros.h"
#include "blTagMap.h"

#include <vector>
#include <list>

/**
blSignal base reader for all file formats

\author Xavi Planes
\date 22 July 2009
\ingroup blSignal
*/
class BASELIBSIGNAL_EXPORT blSignalReader : public blLightObject 
{
public:
	typedef blSignalReader Self;
	typedef blSmartPointer<Self> Pointer;
	blNewMacro(Self);
	cilabDeclareExceptionMacro(Exception, std::exception)

	typedef std::vector<std::string> ExtensionVectorType;
	
public:

	//!
	void SetFilename( const char* );

	//!
	void Update( );

	//!
	blSignalCollective::Pointer GetOutput( );

	//!
	static ExtensionVectorType GetValidExtensions( );

	//!
	blTagMap::Pointer GetTagMapHeader() const;

	//!
	void SetTagMapHeader( blTagMap::Pointer tagMap );

	//!
	bool ReadHeader( );

protected:
	blSignalReader( );

	~blSignalReader( );

private:

	//!
	blSignalCollective::Pointer m_Output;

	//!
	std::string m_Filename;

	//!
	blTagMap::Pointer m_TagMapHeader;
};

#endif //blSignalReader_H
