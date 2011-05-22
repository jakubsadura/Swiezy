/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _blXMLBoostTagMapReader_H
#define _blXMLBoostTagMapReader_H

#include "blBaseTagMapIO.h"

/**
XML Boost reader using serialize. This reader is deprecated because
the format of the XML file is difficult to read for humans.
Please use blXMLTagMapReader.

\code
<?xml version="1.0" encoding="UTF-8" standalone="yes" ?>
<!DOCTYPE boost_serialization>
<boost_serialization signature="serialization::archive" version="5">
<tagMap class_id="0" tracking_level="0" version="0">
	<PropertiesSize>10</PropertiesSize>
	<PropertyId>1</PropertyId>
	<PropertyName>Modality</PropertyName>
	<PropertyValueType>int</PropertyValueType>
	<PropertyValue>3</PropertyValue>
	<PropertyId>2</PropertyId>
	<PropertyName>Name</PropertyName>
	<PropertyValueType>class std::basic_string&lt;char,struct std::char_traits&lt;char&gt;,class std::allocator&lt;char&gt; &gt;</PropertyValueType>
	<PropertyValue>SmallImageOut</PropertyValue>
	<PropertyId>3</PropertyId>
	<PropertyName>Patient name</PropertyName>
	<PropertyValueType>class std::basic_string&lt;char,struct std::char_traits&lt;char&gt;,class std::allocator&lt;char&gt; &gt;</PropertyValueType>
	<PropertyValue></PropertyValue>
	<PropertyId>4</PropertyId>
	<PropertyName>Patient sex</PropertyName>
	<PropertyValueType>class std::basic_string&lt;char,struct std::char_traits&lt;char&gt;,class std::allocator&lt;char&gt; &gt;</PropertyValueType>
	<PropertyValue></PropertyValue>
...
\endcode

\author Xavi Planes
\date Oct 2010
\ingroup blNumericData
*/
class blXMLBoostTagMapReader : public blBaseTagMapIO 
{
public:
	typedef blXMLBoostTagMapReader Self;
	typedef blSmartPointer<Self> Pointer;
	blNewMacro(Self);
	cilabDeclareExceptionMacro(Exception, std::exception)

protected:
	blXMLBoostTagMapReader( );

	~blXMLBoostTagMapReader( );

	//!
	void InternalUpdate();

private:

};

#endif //_blXMLBoostTagMapReader_H
