/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _blXMLTagMapReader_H
#define _blXMLTagMapReader_H

#include "blBaseTagMapIO.h"
#include "blTagMap.h"
#include "CILabExceptionMacros.h"

class TiXmlNode;

/**
XML Reader for blTagMap.

\sec Example

\code
<?xml version="0" ?>
<tagmap size="10">
    <tag name="Modality" type="int">0</tag>
    <tag name="Name" type="std::string">SmallImageProps</tag>
    <tag name="Patient name" type="std::string"></tag>
    <tag name="Patient sex" type="std::string"></tag>
    <tag name="Rendering" type="class blSmartPointer&lt;class blTagMap&gt;">Unsupported value type
        <tagmap size="2">
            <tag name="binary" type="bool">0</tag>
            <tag name="volumerendering" type="bool">0</tag>
        </tagmap>
    </tag>
</tagmap>
\endcode


\author Xavi Planes
\date Oct 2010
\ingroup blNumericData
*/
class blXMLTagMapReader : public blBaseTagMapIO 
{
public:
	typedef blXMLTagMapReader Self;
	typedef blSmartPointer<Self> Pointer;
	blNewMacro(Self);
	cilabDeclareExceptionMacro(Exception, std::exception)

	//!
	static void LoadData( 
		TiXmlNode* node, 
		blTagMap::Pointer tagMapInstance,
		const std::string &nodeName );

protected:
	blXMLTagMapReader( );

	~blXMLTagMapReader( );

	//!
	void InternalUpdate( );

private:
};

#endif //_blXMLTagMapReader_H
