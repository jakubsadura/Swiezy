/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreWorkingAreaStorage_H
#define _coreWorkingAreaStorage_H

#include "gmWidgetsWin32Header.h"
#include "coreBaseWorkingAreaStorage.h"

namespace Core
{
namespace Widgets
{

/**
\brief Singleton class that manages XML configuration of stored working
areas. Register new factory for each XML.

\ingroup gmWidgets
\author Xavi Planes
\date 30 Aug 2010
*/
class GMWIDGETS_EXPORT WorkingAreaStorage : public Core::BaseWorkingAreaStorage
{
public:

	//!
	coreDeclareSmartPointerClassMacro(
		WorkingAreaStorage, 
		Core::BaseWorkingAreaStorage);
	//!
	std::string GetWorkingAreaFolder( );

	//!
	void ScanDirectory( const std::string &projectPath );

	//!
	void Save( BaseWindow* workingArea );

	//!
	void SaveAll( );

	//!
	void Delete( BaseWindow* workingArea );

	//!
	wxWindow* Open( const std::string &path );

	//!
	std::string GetDefaultWorkingAreaFactoryName( );

protected:	

	//!
	WorkingAreaStorage(  );

	//!
	~WorkingAreaStorage( );

	//!
	void CreateFolder( const std::string &path );

	//!
	std::string GetConfigFileName( const std::string &name );

	//!
	std::string GetBitmapFileName( const std::string &name );

	//!
	std::string GetBitmapFileNameFromConfigFile( const std::string &configFileName );

private:

	//!
	std::string m_WorkingAreaFolder;

	//!
	std::string m_DefaultWorkingArea;
};

} // namespace Widgets
} // namespace Core

#endif // _coreWorkingAreaStorage_H
