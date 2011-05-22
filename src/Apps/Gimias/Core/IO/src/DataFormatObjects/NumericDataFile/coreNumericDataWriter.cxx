// Copyright 2008 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "coreNumericDataWriter.h"
#include "coreDataEntity.h"
#include "coreException.h"

#include "blNumericDataWriter.h"

using namespace Core::IO;

//!
NumericDataWriter::NumericDataWriter(void) 
{
	m_ValidExtensionsList.push_back( ".csv" );
	m_ValidTypesList.push_back( NumericDataTypeId );
}

//!
NumericDataWriter::~NumericDataWriter(void)
{
}

//!
void NumericDataWriter::WriteData( )
{
	WriteAllTimeSteps( );
}

void Core::IO::NumericDataWriter::WriteSingleTimeStep( 
	const std::string& fileName, 
	Core::DataEntity::Pointer dataEntity, 
	int iTimeStep )
{
	blTagMap::Pointer tagMap = NULL;
	bool worked = dataEntity->GetProcessingData( tagMap, iTimeStep );
	if( !worked || tagMap == NULL )
	{
		throw Core::Exceptions::Exception(
			"NumericDataWriterWriter::WriteSingleTimeStep",
			"Input data is not of the correct type" );
	}

	blNumericDataWriter::Pointer writer = blNumericDataWriter::New( );
	writer->SetFilename( fileName.c_str() );
	writer->SetInput( tagMap );
	writer->Update();
}


