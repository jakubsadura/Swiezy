/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreDataEntityWriter_H
#define coreDataEntityWriter_H

#include "coreObject.h"
#include "coreDataEntity.h"
#include "coreBaseDataEntityWriter.h"
#include "coreBaseFilter.h"

#include <string>
#include <map>
#include <list>

namespace Core 
{
namespace IO 
{

/** 
\brief Writer of data entities of any type.

The DataEntityWriter will register, at runtime, any new format 
provided by a plugin developer.

When the user selects a DataEntity and selects File->Save, the Core will 
search over a list of registered writers, to find a suitable one that can 
write the requested format. The first writer that is able to do this is called 
to write the data and output the processing data of the DataEntity to a file.

DataEntityWriter provides methods to check that the tuple 
<format type, data type> is uniquely identified in the list of registered 
formats.

\section Examples Examples
\code
// Signal data entity
Core::IO::BaseDataEntityReader::ExtensionVectorType validExtensions;

validExtensions = Core::IO::SignalWriter::GetValidExtensions();
for ( int i = 0 ; i < validExtensions.size() ; i++ )
{
	Core::IO::DataEntityWriter::RegisterFormatWriter<Core::IO::SignalWriter>(
		validExtensions[ i ], 
		Core::SignalTypeId);
}
\endcode

\ingroup gmIO
\sa Core::Runtime::Kernel
\author Juan Antonio Moya
\date 11 Feb 2007
*/

class GMIO_EXPORT DataEntityWriter : public Core::BaseFilter
{
public:
	typedef std::list<Core::IO::BaseDataEntityWriter::Pointer> RegisteredWritersListType;

public:

	coreDeclareSmartPointerClassMacro(DataEntityWriter, Core::BaseFilter);	

	//!
	void SetFileName( const std::string &filename );

	//!
	void SetFileNames( const std::vector< std::string > &filenames );

	//! 
	virtual void Update( );

	//! Register a writer
	static void RegisterFormatWriter(
		Core::IO::BaseDataEntityWriter::Pointer writer );

	//!
	static void UnRegisterAllFormats( );

	/**
	\brief Returns a filter wild card for the save file dialog, that filters all 
	the available writer data formats registered.
	*/
	static std::string GetFileFilterTypesForWrite(Core::DataEntityType type);

	/**
	\brief Returns a default file type for specific data entity type
	*/
	static std::string GetDefaultFileTypeForWrite(Core::DataEntityType type);

protected:

	//! 
	DataEntityWriter(void);

	//! 
	virtual ~DataEntityWriter(void);

	//! 
	static BaseIO::ValidExtensionsListType 
		GetRegisteredWriterFormats(Core::DataEntityType type);


private:
	coreDeclareNoCopyConstructors(DataEntityWriter);

private:

	//!
	static RegisteredWritersListType m_RegisteredWriters;

	//!
	std::vector< std::string > m_Filenames;
};

} // namespace IO
} // namespace Core

#endif
