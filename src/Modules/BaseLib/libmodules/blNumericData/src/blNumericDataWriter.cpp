/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#include "blNumericDataWriter.h"
#include "blTextUtils.h"

#include <fstream>

blNumericDataWriter::blNumericDataWriter()
{
}

blNumericDataWriter::~blNumericDataWriter()
{
}

void blNumericDataWriter::InternalUpdate()
{
	std::string ext = blTextUtils::GetFilenameLastExtension( m_Filename );

	// save the file as CSV
	std::ofstream numericDataFile;
	numericDataFile.open ( m_Filename.c_str() );

	if (numericDataFile.good())
	{
		blTagMap::Iterator it = m_Data->GetIteratorBegin();

		//First write a header to know that it's a numeric csv data instead of a signal data (both .csv files)
		numericDataFile<< "Numeric Data" << std::endl;
		// For each line write a the collumns

		while ( it != m_Data->GetIteratorEnd() )
		{
			blTag::Pointer currentTag = m_Data->GetTag(it);
			numericDataFile<< currentTag->GetName() << ";";
			numericDataFile<< currentTag->GetValueAsString()<< ";";
			it++;
		} // end while ( it != m_Data->GetIteratorEn()
		// Two end of lines
		numericDataFile << std::endl << std::endl;
		numericDataFile.close();
	}// end if (signalFile.good())
}


