/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _blTag_H
#define _blTag_H

#include "blLightObject.h"

#include "CILabExceptionMacros.h"

#include <boost/any.hpp>

#include <string>

/** 
\brief Id, name and value

\note size_t cannot be used because it's different depending on the platform (32 or 64 bits)

\author Xavi Planes
\date 28 July 2009
\ingroup blUtilities
*/
class blTag : public blLightObject
{
public:
	typedef blTag Self;
	typedef blSmartPointer<Self> Pointer;
	blNewMacro2param(Self, const std::string, boost::any );
	blNewMacro3param(Self, unsigned long, const std::string, boost::any );
	cilabDeclareExceptionMacro(Exception, std::exception)

public:

	//! Get the name
	std::string GetName() const;

	//! 
	void SetName(std::string val);

	//! 
	void SetValue(boost::any val);

	//!
	boost::any GetValue();

	//! Templated GET method
	template <typename T>
	bool GetValue(T& value ) const
	{
		try
		{
			value = boost::any_cast<T>(m_Value);
		}
		catch(boost::bad_any_cast&)
		{
			return false;
		}
		return true; 
	}

	//! Templated GET method
	template <typename T>
	T GetValueCasted( ) const
	{
		return boost::any_cast<T>( m_Value );
	}

	//! automatically cast double, float, char, int,long and std::string types 
	std::string GetValueAsString();

	//! automatically cast double, float, char, int,long and std::string types 
	void SetValueAsString( const std::string &typeName, std::string value );

	//!
	unsigned long GetId() const;
	void SetId(unsigned long val);

	//! Compare the value of the two tags
	bool Compare( blTag::Pointer );

	/** Return the name of a type
	Using typeid( std::string ).name( ) is not cross platform
	*/
	static std::string GetTypeName( const std::type_info& info );

	//! 
	std::string GetTypeName( );

protected:
	//!
	blTag( 
		unsigned long id = 0, 
		const std::string &name = "", 
		boost::any value = NULL );

	//!
	blTag( 
		const std::string &name, 
		boost::any value = NULL );

	//!
	virtual ~blTag();

private:

	//!
	unsigned long m_Id;
	//!
	std::string m_Name;
	//!
	boost::any m_Value;
};


#endif // _coreDataEntityTag_H

