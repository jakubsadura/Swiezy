/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "blTag.h"
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include "blTagMap.h"

blTag::blTag( unsigned long id, const std::string &name, boost::any value )
{
	m_Id = id;
	m_Name = name;
	m_Value = value;
}

blTag::blTag( const std::string &name, boost::any value )
{
	m_Id = 0;
	m_Name = name;
	m_Value = value;
}

blTag::~blTag()
{
}

std::string blTag::GetName() const
{
	return m_Name;
}

void blTag::SetName( std::string val )
{
	m_Name = val;
}

boost::any blTag::GetValue()
{
	return m_Value;
}

std::string blTag::GetValueAsString()
{
	std::string value;
	//currently default 4 digits accuracy for double and float types
	try
	{
		if( m_Value.type() == typeid( bool ) )
			value = boost::lexical_cast<std::string>(  boost::any_cast<bool>(m_Value) );
		else if( m_Value.type() == typeid( std::string ) )
			value = boost::any_cast<std::string>(m_Value);
		else if( m_Value.type() == typeid( float ) )
			value = boost::str( boost::format("%.4f") % boost::any_cast<float>(m_Value));
		else if( m_Value.type() == typeid( double ) )
			value = boost::str( boost::format("%.4f") % boost::any_cast<double>(m_Value));
		else if( m_Value.type() == typeid( char ) )
			value = boost::str( boost::format("%d") % (int)boost::any_cast<char>(m_Value));
		else if( m_Value.type() == typeid( unsigned char ) )
			value = boost::str( boost::format("%u") % (unsigned int)boost::any_cast<unsigned char>(m_Value));
		else if( m_Value.type() == typeid( short ) )
			value = boost::str( boost::format("%d") % boost::any_cast<short>(m_Value));
		else if( m_Value.type() == typeid(unsigned short ) )
			value = boost::str( boost::format("%u") % boost::any_cast<unsigned short>(m_Value));
		else if( m_Value.type() == typeid( int ) )
			value = boost::str( boost::format("%d") % boost::any_cast<int>(m_Value));
		else if( m_Value.type() == typeid(unsigned int ) )
			value = boost::str( boost::format("%u") % boost::any_cast<unsigned int>(m_Value));
		else if( m_Value.type() == typeid( long ) )
			value = boost::str( boost::format("%d") % boost::any_cast<long>(m_Value));
		else if( m_Value.type() == typeid(unsigned long ) )
			value = boost::str( boost::format("%u") % boost::any_cast<unsigned long>(m_Value));
		else
			value = "Unsupported value type";
	}
	catch(boost::bad_any_cast&)
	{
		value = "Unsupported value type";
	}

	return value; 
}

void blTag::SetValueAsString( const std::string &typeName, std::string value )
{
	try
	{
		using boost::lexical_cast;
		using boost::bad_lexical_cast;

		if( typeName == GetTypeName( typeid( bool ) ) )
			m_Value = lexical_cast<bool>( value );
		else if( typeName == GetTypeName( typeid( std::string ) ) )
			m_Value = value;
		else if( typeName == GetTypeName( typeid( float ) ) )
			m_Value = lexical_cast<float>( value );
		else if( typeName == GetTypeName( typeid( double ) ) )
			m_Value = lexical_cast<double>( value );
		else if( typeName == GetTypeName( typeid( char ) ) )
			m_Value = lexical_cast<char>( value );
		else if( typeName == GetTypeName( typeid( unsigned char ) ) )
			m_Value = lexical_cast<unsigned char>( value );
		else if( typeName == GetTypeName( typeid( short ) ) )
			m_Value = lexical_cast<short>( value );
		else if( typeName == GetTypeName( typeid(unsigned short ) ) )
			m_Value = lexical_cast<unsigned short>( value );
		else if( typeName == GetTypeName( typeid(int ) ) )
			m_Value = lexical_cast<int>( value );
		else if( typeName == GetTypeName( typeid(unsigned int ) ) )
			m_Value = lexical_cast<unsigned int>( value );
		else if( typeName == GetTypeName( typeid( long ) ) )
			m_Value = lexical_cast<long>( value );
		else if( typeName == GetTypeName( typeid(unsigned long ) ) )
			m_Value = lexical_cast<unsigned long>( value );
		else
			value = "Unsupported value type";
	}
	catch(boost::bad_any_cast&)
	{
		m_Value = NULL;
	}
	catch(boost::bad_lexical_cast&)
	{
		m_Value = NULL;
	}
}

void blTag::SetValue( boost::any val )
{
	m_Value = val;
}

unsigned long blTag::GetId() const
{
	return m_Id;
}

void blTag::SetId( unsigned long val )
{
	m_Id = val;
}

bool blTag::Compare( blTag::Pointer tag )
{

	bool compare = false;

	if ( m_Value.type() != tag->m_Value.type( ) )
	{
		return false;
	}

	if( m_Value.type() == typeid( bool ) )
		return boost::any_cast<bool>(m_Value) == boost::any_cast<bool>(tag->m_Value);
	else if( m_Value.type() == typeid( std::string ) )
		return boost::any_cast<std::string>(m_Value) == boost::any_cast<std::string>(tag->m_Value);
	else if( m_Value.type() == typeid( float ) )
		return boost::any_cast<float>(m_Value) == boost::any_cast<float>(tag->m_Value);
	else if( m_Value.type() == typeid( double ) )
		return boost::any_cast<double>(m_Value) == boost::any_cast<double>(tag->m_Value);
	else if( m_Value.type() == typeid( char ) )
		return boost::any_cast<char>(m_Value) == boost::any_cast<char>(tag->m_Value);
	else if( m_Value.type() == typeid( unsigned char ) )
		return boost::any_cast<unsigned char>(m_Value) == boost::any_cast<unsigned char>(tag->m_Value);
	else if( m_Value.type() == typeid( short ) )
		return boost::any_cast<short>(m_Value) == boost::any_cast<short>(tag->m_Value);
	else if( m_Value.type() == typeid(unsigned short ) )
		return boost::any_cast<unsigned short>(m_Value) == boost::any_cast<unsigned short>(tag->m_Value);
	else if( m_Value.type() == typeid( int ) )
		return boost::any_cast<int>(m_Value) == boost::any_cast<int>(tag->m_Value);
	else if( m_Value.type() == typeid(unsigned int ) )
		return boost::any_cast<unsigned int>(m_Value) == boost::any_cast<unsigned int>(m_Value);
	else if( m_Value.type() == typeid( long ) )
		return boost::any_cast<long>(m_Value) == boost::any_cast<long>(tag->m_Value);
	else if( m_Value.type() == typeid(unsigned long ) )
		return boost::any_cast<unsigned long>(m_Value) == boost::any_cast<unsigned long>(tag->m_Value);

	return compare;
}

std::string blTag::GetTypeName( const std::type_info& info )
{
	if( info == typeid( bool ) )
		return "bool";
	else if( info == typeid( std::string ) )
		return "std::string";
	else if( info == typeid( float ) )
		return "float";
	else if( info == typeid( double ) )
		return "double";
	else if( info == typeid( char ) )
		return "char";
	else if( info == typeid( unsigned char ) )
		return "unsigned char";
	else if( info == typeid( short ) )
		return "short";
	else if( info == typeid(unsigned short ) )
		return "unsigned short";
	else if( info == typeid( int ) )
		return "int";
	else if( info == typeid(unsigned int ) )
		return "unsigned int";
	else if( info == typeid( long ) )
		return "long";
	else if( info == typeid(unsigned long ) )
		return "unsigned long";
	else if ( info == typeid( blTagMap::Pointer ) )
		return "class blSmartPointer<class blTagMap>";

	return "";
}

std::string blTag::GetTypeName()
{
	return blTag::GetTypeName( GetValue().type() );
}

