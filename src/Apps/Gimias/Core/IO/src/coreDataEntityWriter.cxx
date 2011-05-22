/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreDataEntityWriter.h"
#include "coreAssert.h"
#include "coreStringHelper.h"
#include "coreDataEntityInfoHelper.h"
#include "coreExceptionMacros.h"
#include "coreException.h"

#include <itksys/SystemTools.hxx>

Core::IO::DataEntityWriter::RegisteredWritersListType Core::IO::DataEntityWriter::m_RegisteredWriters;

//!
Core::IO::DataEntityWriter::DataEntityWriter()
{
	SetNumberOfInputs( 1 );
}

//!
Core::IO::DataEntityWriter::~DataEntityWriter(void)
{
}

//!
Core::IO::BaseIO::ValidExtensionsListType
Core::IO::DataEntityWriter::GetRegisteredWriterFormats(Core::DataEntityType type)
{
	BaseIO::ValidExtensionsListType formatList;
	try
	{
		RegisteredWritersListType::iterator it;
		for( it = m_RegisteredWriters.begin(); it != m_RegisteredWriters.end();++it)
		{
			if ( (*it)->CanWrite( type ) )
			{
				BaseIO::ValidExtensionsListType extensions;
				extensions = (*it)->GetValidExtensions();
				formatList.insert(formatList.end(), extensions.begin(), extensions.end());
			}
		}

	}
	coreCatchExceptionsAddTraceAndThrowMacro(GetRegisteredWriterFormats);
	
	return formatList;
}

void Core::IO::DataEntityWriter::SetFileName( const std::string &filename )
{
	m_Filenames.clear();
	m_Filenames.push_back( filename );
}

void Core::IO::DataEntityWriter::SetFileNames( const std::vector< std::string > &filenames )
{
	m_Filenames = filenames;
}

void Core::IO::DataEntityWriter::Update()
{
	bool correct = false;
	std::string errMessage;
	try
	{
		if ( GetInputDataEntity( 0 ).IsNull())
			return ;

		DataEntityType dataType;
		dataType = GetInputDataEntity( 0 )->GetType();

		// Infer the extension from the filename
		std::string ext = itksys::SystemTools::GetFilenameLastExtension(m_Filenames[ 0 ]);
		std::string extension = itksys::SystemTools::LowerCase(ext);

		// For each reader, check if it can read
		RegisteredWritersListType::iterator it = m_RegisteredWriters.begin( );
		while ( it != m_RegisteredWriters.end() && !correct )
		{
			if ( (*it)->CanWrite( extension ) &&
				 (*it)->CanWrite( dataType ) )
			{
				// Found a writer, instantiate it and try write
				BaseDataEntityWriter::Pointer writer;
				writer = static_cast<BaseDataEntityWriter*>((*it)->CreateAnother().GetPointer());
				if(writer.IsNotNull())
				{
					try
					{
						writer->SetFileNames( m_Filenames );
						writer->SetInputDataEntity( 0, GetInputDataEntity( 0 ) );
						writer->Update();
						correct = true;
					}
					catch( std::exception &e )
					{
						errMessage = e.what();
						correct = false;
					}
					catch(...)
					{
						correct = false;
					}
				}
			} // endif

			++it;
		} // endfor
	}
	coreCatchExceptionsAddTraceAndThrowMacro(Update);

	if ( !correct )
	{
		std::ostringstream strError;
		strError 
			<< "The selected data could not be saved or either the application \n"
			<< "was not able to find a suitable writer for that format.\n"
			<< errMessage;
		throw Core::Exceptions::Exception(
			"Core::IO::DataEntityWriter::Update",
			strError.str().c_str() );
	}
}

std::string Core::IO::DataEntityWriter::GetFileFilterTypesForWrite(
	Core::DataEntityType type)
{
	Core::IO::DataEntityWriter::Pointer dataEntityWriter;
	dataEntityWriter = Core::IO::DataEntityWriter::New( );
	std::ostringstream filter;
	bool addedToFilter = false;

	// For each entity type
	BaseIO::ValidExtensionsListType formatList;
	BaseIO::ValidExtensionsListType::iterator formatListIt;

	formatList = dataEntityWriter->GetRegisteredWriterFormats(type);
	formatListIt = formatList.begin();
	addedToFilter = (formatListIt != formatList.end());

	if ( addedToFilter )
	{
		// Append the Entity type string
		filter << Core::DataEntityInfoHelper::GetEntityTypeAsString(type) << "(";

		// Append the types to the string
		while(formatListIt != formatList.end())
		{
			filter << "*" << (*formatListIt);
			++formatListIt;
			if(formatListIt != formatList.end())
				filter << ", ";
		}
		filter << ")|";

		// Append now the wildcards
		formatListIt = formatList.begin();
		while( formatListIt != formatList.end() )
		{
			filter << "*" << (*formatListIt);
			++formatListIt;
			if(formatListIt != formatList.end())
				filter << ";";
		}

		filter << "|";
	}

	filter << "All files(*.*)|*.*";

	// return the cumulated filter
	return filter.str();
}

std::string Core::IO::DataEntityWriter::GetDefaultFileTypeForWrite(Core::DataEntityType type)
{
	std::string fileType;

	Core::IO::DataEntityWriter::Pointer dataEntityWriter;
	dataEntityWriter = Core::IO::DataEntityWriter::New( );
	std::ostringstream filter;
	bool addedToFilter = false;

	// For each entity type
	BaseIO::ValidExtensionsListType formatList;
	BaseIO::ValidExtensionsListType::iterator formatListIt;

	formatList = dataEntityWriter->GetRegisteredWriterFormats(type);
	formatListIt = formatList.begin();
	if(formatListIt != formatList.end())
	{
		fileType = *formatListIt;
	}
	return fileType;
}

void Core::IO::DataEntityWriter::UnRegisterAllFormats()
{
	m_RegisteredWriters.clear( );
}

void Core::IO::DataEntityWriter::RegisterFormatWriter( 
	Core::IO::BaseDataEntityWriter::Pointer writer )
{
	m_RegisteredWriters.push_back( writer );
}
