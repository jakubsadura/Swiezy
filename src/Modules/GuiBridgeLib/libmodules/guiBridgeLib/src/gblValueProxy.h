/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef GBLVALUEPROXY_H
#define GBLVALUEPROXY_H

#include "CILabNamespaceMacros.h"
#include "boost/shared_ptr.hpp"
#include <string>
#include <limits>
#include "static_check.h"
#include "gblValueConverter.h"

namespace gbl{

/**
This is the default implementation of getting a flag from a widget.
*/
template< class T >
std::string GetText(T* _widget)
{
	LOKI_STATIC_CHECK(false, GetText_not_implemented_for_this_type);
	return std::string("");
}

//! This is the default implementation of setting a number in a widget.
template< class T >
void SetText(T* _widget, const std::string& _value)
{
	LOKI_STATIC_CHECK(false, SetText_not_implemented_for_this_type);
}

/**
This is the default implementation of getting a number from a widget.
*/
template< class T >
double GetNumber(T* _widget)
{
	return gbl::ValueConverter::TextToNumber( GetText(_widget) );
}

/**
This is the default implementation of setting a number in a widget.
It will set the contents of _widget to _number, unless _widget already contains a number equal to _number.
*/
template< class T >
void SetNumber(T* _widget, double _number, const int _maxNrOfDecimals = std::numeric_limits<double>::digits10 + 2)
{
	double existingNumber( 0 );
	bool hasExistingNumber(true);
	try
	{
		existingNumber = gbl::GetNumber(_widget);
	}
	catch( std::bad_cast& )
	{
		hasExistingNumber = false;
	}
	// Only set the text of the widget if the existing number-contents differ from _number rounded to _maxNrOfDecimals.
	if( (!hasExistingNumber) || existingNumber != gbl::RoundToXDecimals(_number, _maxNrOfDecimals) )
	{
		gbl::SetText(_widget, gbl::ValueConverter::NumberToText(_number, _maxNrOfDecimals));
	}
}

/**
This is the default implementation of getting a flag from a widget.
*/
template< class T >
bool GetFlag(T* _widget)
{
	return gbl::ValueConverter::TextToFlag( GetText(_widget) );
}

//! This is the default implementation of setting a number in a widget.
template< class T >
void SetFlag(T* _widget, bool _value)
{
	gbl::SetText(_widget, gbl::ValueConverter::FlagToText(_value));
}


/**
Base class for ControlUpdater

\author Xavi Planes
\date 15 July 2010
\ingroup DynLib
*/
class ControlUpdaterBase
{
public:
	ControlUpdaterBase(const std::string &name) : m_Name( name ){}
	virtual ~ControlUpdaterBase(){}

	//!
	virtual void SetValue( const std::string &val ) = 0;
	virtual std::string GetValue( )	= 0 ;

	//!
	std::string GetName() const { return m_Name; }
	void SetName(std::string val) { m_Name = val; }

	//!
	virtual void* GetControl( ) = 0;

private:
	//!
	std::string m_Name;
};

/**
Sets/Gets a wxWidgets control value

\author Xavi Planes
\date 15 July 2010
\ingroup DynLib
*/
template<class T>
class ControlUpdater : public ControlUpdaterBase
{
public:
	ControlUpdater(const std::string &name, T* control) : ControlUpdaterBase( name ), m_Control(control){}
	virtual ~ControlUpdater(){}

	//!
	void SetValue( const std::string &val ){SetText( m_Control, val );}
	std::string GetValue( )	{return GetText( m_Control );}

	//!
	virtual void* GetControl( ){return m_Control; }

private:
	//!
	T* m_Control;
};


} // namespace gbl{

#endif //GBLVALUEPROXY_H
