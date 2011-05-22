/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreBaseIO_H
#define _coreBaseIO_H

#include "gmIOWin32Header.h"
#include "coreObject.h"
#include "coreDataEntity.h"
#include "coreBaseFilter.h"

namespace Core
{
namespace IO
{


/** 
\brief The BaseIO class is the base class for BaseDataEntityReader and 
BaseDataEntityWriter

\ingroup gmIO
\author Xavi Planes
\date 13 May 2010
*/
class GMIO_EXPORT BaseIO : public Core::BaseFilter
{
public:
	typedef std::list<std::string> ValidExtensionsListType;
	typedef std::list<DataEntityType> ValidTypesListType;

public:
	coreDeclareSmartPointerTypesMacro(BaseIO, Core::BaseFilter);
	coreClassNameMacro(BaseIO);

	//! Get last digits before extension
	static long GetLastDigits( const std::string &filename );

	//! Compare two file paths using last digits
	static bool CompareStringNumbers( const std::string &str1,const std::string &str2 );

	//! GetFilenameWithoutExtension and remove last digits
	static std::string RemoveLastDigits( const std::string &filename );

	//!
	void SetFileNames( const std::vector< std::string > &filenames );

	//! Return the extensions that the reader can read, like ".vtk"
	virtual ValidExtensionsListType GetValidExtensions( );

	//! Return DataEntity types that the reader can read
	virtual ValidTypesListType GetValidDataEntityTypes( );

	//!
	virtual bool Check( Core::DataEntityType type );

	//!
	virtual bool Check( std::string ext );

	//!
	blTagMap::Pointer GetTagMap() const;
	void SetTagMap(blTagMap::Pointer val);

protected:
	BaseIO(void);;
	virtual ~BaseIO(void);

	coreDeclareNoCopyConstructors(BaseIO);

protected:
	//!
	std::vector< std::string > m_Filenames;
	//!
	ValidExtensionsListType m_ValidExtensionsList;
	//!
	ValidTypesListType m_ValidTypesList;
	//!
	blTagMap::Pointer m_TagMap;
};


}
}

#endif
