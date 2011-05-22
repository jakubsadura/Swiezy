/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreBaseWorkingAreaStorage_H
#define _coreBaseWorkingAreaStorage_H

#include "gmKernelWin32Header.h"

namespace Core
{

/**
\brief Singleton class that manages configuration of stored working
areas. 

\ingroup gmWidgets
\author Xavi Planes
\date 30 Aug 2010
*/
class GMKERNEL_EXPORT BaseWorkingAreaStorage : public Core::SmartPointerObject
{
public:

	//!
	coreDeclareSmartPointerTypesMacro(
		BaseWorkingAreaStorage, 
		Core::SmartPointerObject);
	coreClassNameMacro(BaseWorkingAreaStorage);

	//! Scan projectPath + "WorkingAreas" and open all working areas
	virtual void ScanDirectory( const std::string &projectPath ) = 0;

	//! Save a working area
	virtual void Save( BaseWindow* workingArea ) = 0;

	//! Save all working areas and icons to projectHomePath + "WorkingAreas"
	virtual void SaveAll( ) = 0;

	//! Delete a working area
	virtual void Delete( BaseWindow* workingArea ) = 0;

	//!
	virtual std::string GetDefaultWorkingAreaFactoryName( ) = 0;

protected:	

	//!
	BaseWorkingAreaStorage(  ){};

	//!
	~BaseWorkingAreaStorage( ){};

private:
};

} // namespace Core

#endif // _coreBaseWorkingAreaStorage_H
