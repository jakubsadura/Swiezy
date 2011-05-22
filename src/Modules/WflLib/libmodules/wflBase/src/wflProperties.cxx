/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#include "wflProperties.h"


wflProperties::wflProperties()
{

}

wflProperties::~wflProperties()
{

}


std::string wflProperties::GetProperty( const std::string &name ) const
{
	std::map<std::string,std::string>::const_iterator it = m_Props.find( name );
	if ( it == m_Props.end() )
	{
		return "";
	}

	return it->second;
}

void wflProperties::SetProperty( const std::string &name, const std::string &val )
{
	m_Props[ name ] = val;
}

std::map<std::string,std::string> & wflProperties::GetProperties()
{
	return m_Props;
}