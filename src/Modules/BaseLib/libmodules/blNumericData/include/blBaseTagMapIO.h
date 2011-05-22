/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _blBaseTagMapIO_H
#define _blBaseTagMapIO_H

#include "blLightObject.h"
#include "blTagMap.h"
#include "CILabExceptionMacros.h"

/**
Base reader/writer for all blTagMap reader/writers. 

\author Xavi Planes
\date Oct 2010
\ingroup blNumericData
*/
class blBaseTagMapIO : public blLightObject 
{
public:
	typedef blBaseTagMapIO Self;
	typedef blSmartPointer<Self> Pointer;
	cilabDeclareExceptionMacro(Exception, std::exception)
	
public:

	//! Set the complete filename of the numeric csv data entity to be read
	void SetFilename( const char* );

	//!
	void SetInput( blTagMap::Pointer tagMap );

	//! execute the reading
	void Update( );

	//! Returns the output of the reader, containing all the tags present in the file
	blTagMap::Pointer GetOutput( );


protected:
	blBaseTagMapIO( );

	~blBaseTagMapIO( );

	//!
	virtual void InternalUpdate( ) = 0;

protected:

	//!
	blTagMap::Pointer m_Data;

	//!
	std::string m_Filename;

	//!
	unsigned int m_Version;
};

#endif //_blBaseTagMapIO_H
