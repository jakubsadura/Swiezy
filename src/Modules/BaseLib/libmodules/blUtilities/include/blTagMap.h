/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _blTagMap_H
#define _blTagMap_H

#include "blLightObject.h"
#include "blTag.h"

#include "CILabExceptionMacros.h"

#include <boost/signals.hpp>
#include <boost/bind.hpp>

#include <string>
#include <map>

/** 
\brief Map of tags

\author Xavi Planes
\date 28 July 2009
\ingroup blUtilities
*/
class blTagMap : public blLightObject
{
public:
	typedef blTagMap Self;
	typedef blSmartPointer<Self> Pointer;
	blNewMacro(Self );
	cilabDeclareExceptionMacro(Exception, std::exception)
	typedef std::map<unsigned long, blTag::Pointer> TagMapType;

public:
	typedef boost::signal2<void, blTagMap*, unsigned long> TagChangedSignal;
	typedef std::map<unsigned long, blTag::Pointer>::iterator Iterator;

public:
	//! Add the tag. If it already exists, overwrite it
	void AddTag( blTag::Pointer tag );

	/** Add the tag. If it already exists, overwrite it
	It creates a new tag type ID
	\return tag type ID
	*/
	unsigned long AddTag( 
		const std::string &name, 
		boost::any value );

	//! Return a tag
	blTag::Pointer GetTag( unsigned long type );
	blTag::Pointer GetTag( Iterator it );

	//! Return the first tag searching it by name
	blTag::Pointer FindTagByName( const std::string &name );

	//! Returns the number of rows of the map
	size_t GetLength();

	//! Add all tags
	void AddTags( blTagMap::Pointer tagMap );

	//! returns the start iterator of the map
	Iterator GetIteratorBegin();

	//! returns the end iterator of the map
	Iterator GetIteratorEnd();

	/**
	Connects an observer to be notified when a tag changes the value
	using AddTag( ). 

	\param observer is the class instance of the observer
	\param slotFunction is the Slot member of the observer class, following 
		the specified signature 
	\return the boost connection, but you might not need it
	*/
	template <class T> 
	boost::signals::connection AddObserverOnChangedTag(
		T* observer, 
		void (T::*slotFunction)(blTagMap*,unsigned long))
	{
		return this->m_OnChangedTagSignal.connect(boost::bind<void>(slotFunction, observer, _1, _2));
	};

	//!  Send a signal to notify that the tag has changed
	void NotifyObserversOnChangedTag(blTagMap* tagMap, unsigned long tagId);

	//!
	void RemoveAll( );

	//!
	TagMapType GetTagMap() const;
	void SetTagMap(const TagMapType &val);


	bool Compare( blTagMap::Pointer source );

	//! If tag is not found, return NULL
	template<class T>
	T GetTagValue( const std::string &name )
	{
		blTag::Pointer tag = FindTagByName( name );
		if ( tag.IsNull() )
		{
			return NULL;
		}

		T val = NULL;
		tag->GetValue( val );
		return val;
	}

protected:
	//!
	blTagMap( );

	//!
	virtual ~blTagMap();

private:

	//!
	std::map<unsigned long, blTag::Pointer> m_TagMap;
	// 
	TagChangedSignal m_OnChangedTagSignal;
};


#endif // _blTagMap_H

