/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreScanFolder_H
#define coreScanFolder_H

#include "gmIOWin32Header.h"
#include "coreObject.h"

#include <string>
#include <map>
#include <list>

namespace Core 
{
namespace IO 
{

/** 
\brief Group of filenames with the same name except the number before the extension

\ingroup gmIO
\author Xavi Planes
\date 28 July 2010
*/
class GMIO_EXPORT FilenameGroup 
{
public:

	//!
	void AddFilename( const std::string &filename ){ m_Filenames.push_back( filename ); }

	//!
	std::vector< std::string > &GetFilenames() { return m_Filenames; }

private:

	//!
	std::vector< std::string > m_Filenames;
};

/** 
\brief Scan a folder and returns a list of grouped filenames. Each group
will be used to read a single DataEntity.

For example, demo-CT-pa15_OneChamber00.vtk, demo-CT-pa15_OneChamber01.vtk
and demo-CT-pa15_OneChamber02.vtk will be grouped into demo-CT-pa15_OneChamber.vtk


\ingroup gmIO
\author Xavi Planes
\date 28 July 2010
*/
class GMIO_EXPORT ScanFolder : public Core::SmartPointerObject
{
public:
	typedef std::map< std::string, FilenameGroup > GroupMapType;

public:

	coreDeclareSmartPointerClassMacro(ScanFolder, Core::SmartPointerObject);	

	//!
	void SetFileName( const std::string &filename );

	//!
	void SetFileNames( const std::vector< std::string > &filenames );

	//! 
	void Update();

	//!
	GroupMapType& GetGroups( );

protected:
	//! 
	ScanFolder(void);

	//! 
	virtual ~ScanFolder(void);

	//!
	void ScanDirectory( const std::string &filename );

	//!
	void AddFilename( const std::string &filename );

private:
	coreDeclareNoCopyConstructors(ScanFolder);

private:
	//!
	std::vector< std::string > m_Filenames;
	//!
	GroupMapType m_FilenameGroups;
};

} // namespace IO
} // namespace Core

#endif
