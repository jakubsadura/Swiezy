/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef blNumericDataWriter_H
#define blNumericDataWriter_H

#include "blBaseTagMapIO.h"
#include "blMacro.h"
#include "blTagMap.h"

#include <vector>
#include "CILabExceptionMacros.h"

/**
blNumericData writer for all file formats

\author Martin Bianculli
\date Oct 2009
\ingroup blSignal
*/
class blNumericDataWriter : public blBaseTagMapIO 
{
public:
	typedef blNumericDataWriter Self;
	typedef blSmartPointer<Self> Pointer;
	blNewMacro(Self);
	cilabDeclareExceptionMacro(Exception, std::exception)
	
protected:
	blNumericDataWriter( );

	~blNumericDataWriter( );

	//!
	void InternalUpdate( );

private:
};

#endif //blSignalWriter_H
