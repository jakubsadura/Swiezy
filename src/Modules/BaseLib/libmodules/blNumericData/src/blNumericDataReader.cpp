/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#include "blNumericDataReader.h"
#include "blTextUtils.h"


#include <fstream>
#include <stdlib.h>


blNumericDataReader::blNumericDataReader() 
{
}

blNumericDataReader::~blNumericDataReader()
{
}

void blNumericDataReader::InternalUpdate()
{
	// save the file as CSV
	std::ifstream numericDataFile;
	numericDataFile.open ( m_Filename.c_str() );

	if ( numericDataFile.good())
	{
		m_Data = blTagMap::New( );

		bool fileisOk = true;
		std::string line;
		while ( fileisOk )
		{
			fileisOk = std::getline(numericDataFile,line) && fileisOk;
			// check that the type is a signal (the first line), and not a numeric data entity
			if ( line != "Numeric Data" && fileisOk)
			{
				m_Data = NULL;
				return;
			}

			fileisOk = std::getline(numericDataFile,line) && fileisOk;
			ParseLineAndAddTag( line );
			
			
		numericDataFile.close();
	}
}

}

void blNumericDataReader::ParseLineAndAddTag( std::string &line )
{
	// now bust the line up into individual words
	while(line != "")
	{
		std::string name;
		std::string value;
		int namePos = line.find(';');
		if(namePos > 0)
		{
			name = line.substr(0,namePos);
			line = line.substr(namePos+1);
		}
		int valuePos = line.find(";");
		if (valuePos>0)
		{
			value =line.substr(0,valuePos);
			line = line.substr(valuePos+1);

		}

		if ( name != "" || value != "")
			m_Data->AddTag(name,value);
	}
}