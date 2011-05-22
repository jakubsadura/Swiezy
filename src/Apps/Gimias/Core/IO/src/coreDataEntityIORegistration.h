/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreDataEntityIORegistration_H
#define coreDataEntityIORegistration_H

#include "gmIOWin32Header.h"
#include "coreObject.h"

namespace Core{

namespace IO{

/** 
DataEntityIORegistration registers available Readers and writers


\ingroup gmIO
\author Xavi Planes
\date 08 Jun 2009
*/
class GMIO_EXPORT DataEntityIORegistration : public Core::Object
{
public:
	//!
	static void RegisterDefaultFormats( );

protected:
	//! 
	DataEntityIORegistration();
	//! 
	~DataEntityIORegistration();

};

} // namespace IO{
} // namespace Core{

#endif
