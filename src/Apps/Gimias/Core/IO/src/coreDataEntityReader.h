/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreDataEntityReader_H
#define coreDataEntityReader_H

#include "coreObject.h"
#include "coreDataEntity.h"
#include "coreBaseDataEntityReader.h"
#include "coreBaseFilter.h"

#include <string>
#include <map>
#include <list>

namespace Core 
{
namespace IO 
{

/** 
\brief Reader of data entities of any type.

The DataEntityReader will register, at runtime, any new format 
provided by a plugin developer.
This high level manager will be singleton, referenced from the Kernel instance.
Registering a format will require storing a FormatDescriptor object into 
the DataEntity.

When the user selects a DataEntity and selects File->Load, the Core will 
search over a list of registered readers, to find a suitable one that 
can read the requested file. The first reader that is able to do this is 
called to read the data and output a DataEntity.

Each plug-in is able to register readers and writers with the Core. 
When using a specialized reader, register it to the DataEntityReader
class as the Reader available for that format, so the application and other 
plugins can notice and benefit of this format reader.

DataEntityReader provides methods to check that the tuple 
<format type, data type> is uniquely identified in the list of registered 
formats.

\ingroup gmIO
\sa Core::Runtime::Kernel
\author Juan Antonio Moya
\date 11 Feb 2007
*/

class GMIO_EXPORT DataEntityReader : public Core::BaseFilter
{
public:
	typedef std::map<std::string,BaseDataEntityReader::Pointer> RegisteredReadersType;

public:

	coreDeclareSmartPointerClassMacro(DataEntityReader, Core::BaseFilter);	

	//!
	void SetFileName( const std::string &filename );

	//!
	void SetFileNames( const std::vector< std::string > &filenames );

	//!
	bool ReadHeader( );

	//! 
	void Update();

	//! Register a reader
	static void RegisterFormatReader(BaseDataEntityReader::Pointer);

	//! UnRegister a reader
	static void UnRegisterFormatReader(BaseDataEntityReader::Pointer);

	//! Get a registered reader. Use the name of the class. For example "DICOMFileReader"
	static BaseDataEntityReader::Pointer GetRegisteredReader( const std::string &name);

	//!
	static void UnRegisterAllFormats( );

	/**
	\brief Returns a filter wild card for the open file dialog, that 
	filters all the available reader data formats registered.
	*/
	static std::string GetFileFilterTypesForRead(void);

	//!
	blTagMap::Pointer GetTagMap() const;
	void SetTagMap(blTagMap::Pointer val);

protected:
	//! 
	DataEntityReader(void);

	//! 
	virtual ~DataEntityReader(void);

	//! 
	static BaseIO::ValidExtensionsListType 
		GetRegisteredReaderFormats( Core::DataEntityType type);

	//!
	void ReadGroup( 
		const std::string name,
		const std::vector<std::string> &filenames );

private:
	coreDeclareNoCopyConstructors(DataEntityReader);

private:

	//!
	static RegisteredReadersType m_RegisteredReaders;

	//!
	std::vector< std::string > m_Filenames;

	//! Header information
	blTagMap::Pointer m_TagMap;
};

} // namespace IO
} // namespace Core

#endif
