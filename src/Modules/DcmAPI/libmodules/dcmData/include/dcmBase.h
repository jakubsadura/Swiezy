/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _dcmBase_H
#define _dcmBase_H

#include "dcmAPIDataWin32Header.h"
//dcmAPI
#include "dcmTypes.h"

//Common libraries
#include <string>
#include <vector>

namespace dcmAPI
{

/** 
\brief Base class for all DICOM structures that contains a list of tags

\ingroup data
\author Pedro Omedas
\date 15 May 2008
*/

class  DCMAPIDATA_EXPORT Base
{
public:
	//! 
	cistibBoostPointerMacro(Base);
	//!
	Base();
	//!
	virtual ~Base();

	//! Convert the value associated with \a _id to a string. If _id is not found, a TagNotFound exception is thrown.
	std::string GetTagAsText(const TagId& _id);
	//! Convert the value associated with \a _id to a double. If _id is not found, a TagNotFound exception is thrown.
	double GetTagAsNumber(const TagId& _id);
	//!
	void AddTag(const TagId& _id, std::string _value);
	//!
	void AddTag(const Tag& tag );
	//! 
	virtual void GetAllTags(TagsMap& tagsMap);

	TagsMap * GetTagsMap() const;
	void SetTagsMap(TagsMap * val);
private:
	//! Not implemented
	Base( class Base const & );
	//! Not implemented
	void operator=(const Base&);

protected:
	TagVector *m_tags;
	TagsMap *m_tagsMap;
};

} // namespace dcmAPI

#endif //_dcmBase_H
