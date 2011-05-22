/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "dynFilterFunction.h"

dynFilterFunction::dynFilterFunction( const std::string name, const std::string hierarchy )
{
	m_Name = name;
	m_Hierarcy = hierarchy;
	m_Parameters= "()";
}

dynFilterFunction::dynFilterFunction( const char *name )
{
	m_Name = name;
	m_Parameters= "()";
}

dynFilterFunction::~dynFilterFunction()
{
}

std::string dynFilterFunction::GetHierarcy()
{
	return m_Hierarcy;
}

void dynFilterFunction::AddClass( std::string classname )
{
	if ( !m_Hierarcy.empty() )
		m_Hierarcy += ",";
	m_Hierarcy += classname;
}

std::string &dynFilterFunction::GetParameters()
{
	return m_Parameters;
}

void dynFilterFunction::AddParameter( std::string val )
{
	// Add a comma
	if ( m_Parameters != "()" )
	{
		m_Parameters[ m_Parameters.size( ) - 1 ] = ',';
		m_Parameters += " ";
	}

	// Add parameter 
	m_Parameters = m_Parameters.substr( 0, m_Parameters.size( ) - 1 ) + val + ")";
}

std::string dynFilterFunction::GetName() const
{
	return m_Name;
}

void dynFilterFunction::SetName( std::string val )
{
	m_Name = val;
}

std::string dynFilterFunction::GetReturnType()
{
	return m_ReturnType;
}

void dynFilterFunction::SetReturnType( std::string val )
{
	m_ReturnType = val;
}

std::string dynFilterFunction::GetClass( int i )
{
	size_t initialPos = 0;
	size_t finalPos = m_Hierarcy.find( ',' );
	if ( finalPos == std::string::npos )
	{
		finalPos = m_Hierarcy.size();
	}
	int count = 0;
	while ( i != count && initialPos != m_Hierarcy.size() )
	{
		initialPos = std::min( finalPos + 1, m_Hierarcy.size() );
		finalPos = m_Hierarcy.find( ',', initialPos + 1 );
		if ( finalPos == std::string::npos )
		{
			finalPos = m_Hierarcy.size();
		}
		count++;
	}

	size_t size = std::max( long( 0 ), long( finalPos ) - long( initialPos ) );
	return m_Hierarcy.substr( initialPos, size );
}

void dynFilterFunction::OpenParenthesis()
{
	size_t pos = m_Parameters.find( ")" );
	if ( pos != std::string::npos )
	{
		m_Parameters = m_Parameters.substr( 0, pos );
	}
}
