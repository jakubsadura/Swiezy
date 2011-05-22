/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef blXMLTagMapWriter_H
#define blXMLTagMapWriter_H

#include "blBaseTagMapIO.h"
#include "blMacro.h"
#include "blTagMap.h"

class TiXmlNode;

/**
XML Writer for blTagMap

Please see blXMLTagMapReader for the format of the XML file.

\author Xavi Planes
\date Oct 2010
\ingroup blNumericData
*/
class blXMLTagMapWriter : public blBaseTagMapIO 
{
public:
	typedef blXMLTagMapWriter Self;
	typedef blSmartPointer<Self> Pointer;
	blNewMacro(Self);
	cilabDeclareExceptionMacro(Exception, std::exception)
	
public:

	//!
	static void SaveData( 
		TiXmlNode* node, 
		blTagMap::Pointer tagMapInstance, 
		const std::string &nodeName );

protected:
	blXMLTagMapWriter( );

	~blXMLTagMapWriter( );

	//!
	void InternalUpdate( );
private:

};

#endif //blSignalWriter_H
