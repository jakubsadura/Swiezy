// Copyright 2008 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "coreNumericDataReader.h"

#include "blNumericDataReader.h"


//!
Core::IO::NumericDataReader::NumericDataReader(void) 
{
	m_ValidExtensionsList.push_back( ".csv" );
	m_ValidTypesList.push_back( NumericDataTypeId );
}

//!
Core::IO::NumericDataReader::~NumericDataReader(void)
{
}

//!
void Core::IO::NumericDataReader::ReadData( )
{
	//build the data entity
	return ReadAllTimeSteps( Core::NumericDataTypeId );
}

boost::any Core::IO::NumericDataReader::ReadSingleTimeStep( int iTimeStep, const std::string &filename )
{
	blTagMap::Pointer output;
	blNumericDataReader::Pointer reader = blNumericDataReader::New( );
	reader->SetFilename( filename.c_str() );
	reader->Update();
	output = reader->GetOutput( );

	if ( output.IsNull() )
	{
		return boost::any( );
	}

	return output;
}
