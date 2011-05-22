/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef blNumericDataReader_H
#define blNumericDataReader_H

#include "blBaseTagMapIO.h"
#include "blTagMap.h"
#include "CILabExceptionMacros.h"

#include <vector>
#include <list>

/**
blNumericDataReader base reader for all file formats. Note: for the moment its is assumed that
both name and value fields are of type std::string

\author Martin Bianculli
\date Oct 2009
\ingroup blNumericData
*/
class blNumericDataReader : public blBaseTagMapIO 
{
public:
	typedef blNumericDataReader Self;
	typedef blSmartPointer<Self> Pointer;
	blNewMacro(Self);
	cilabDeclareExceptionMacro(Exception, std::exception)
	
protected:
	blNumericDataReader( );

	~blNumericDataReader( );

	//! execute the reading
	void InternalUpdate( );

	//! analyze the line in search of the ";" char, and adds a tag if succeded
	void ParseLineAndAddTag( std::string &line);
private:
};

#endif //blNumericDataReader_H
