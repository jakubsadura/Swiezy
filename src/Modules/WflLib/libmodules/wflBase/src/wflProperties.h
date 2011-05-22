/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _wflProperties_H
#define _wflProperties_H

#include "WflLibWin32Header.h"
#include "blLightObject.h"
#include <map>

/**
Properties map

\author Xavi Planes
\date 22 Dec 2010
\ingroup WflLib
*/
class WFLLIB_EXPORT wflProperties
{
public:
	//!
	std::string GetProperty( const std::string &name ) const;
	void SetProperty( const std::string &name, const std::string &val);

	//!
	std::map<std::string,std::string> &GetProperties();

protected:
	//!
	wflProperties( );

	//!
	~wflProperties( );

protected:
	//!
	std::map<std::string,std::string> m_Props;
};

#endif //_wflProperties_H
