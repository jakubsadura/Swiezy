/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

//This class allow to create a new PDSet from a file that 
//containts a list of shape files.

#include <fstream>
#include <iostream>
#include <cstring>
#include "blIASetReader.h"

/** 
 *	This method loads the path to the images
 *
 */
//--------------------------------------------------
blIASetReader::blIASetReader()
//--------------------------------------------------
{
	// initialization 
	this->fileName = 0;
}



/** 
 *	The Destructor.
 */
//--------------------------------------------------
blIASetReader::~blIASetReader()
//--------------------------------------------------
{

	delete [] this->fileName;
}



/** 
 *	This method loads the path to the images
 *
 */
//--------------------------------------------------
void blIASetReader::SetFilename(const char * fileName)
//--------------------------------------------------
{
	if (this->fileName != NULL) 
		delete[] this->fileName;

	this->fileName = new char [strlen(fileName) + 1]; 
					//+1 because of terminating '\x0'
	strcpy(this->fileName, fileName);
}




/** 
 *	This method reads from the file and creates the blPDSet object
 *	with all the blPDShapes.
 */
//--------------------------------------------------
void blIASetReader::Update()
//--------------------------------------------------
{

	// number of files read
	this->numberOfFiles = 0;	


// Get the file names from file .lst
/*=======================================================================*/

	// buffer for each line
	const unsigned int SETREADER_MAX_LINE_LENGTH = 200;
	char buffer[SETREADER_MAX_LINE_LENGTH];		


	// open the file for reading
	std::ifstream file(this->fileName);
	
	if (!file)	// error
	{
		std::cout << "Error: File " << this->fileName << " not found" << std::endl;
	}
	else
	{
		// read a new line while it is possible
		while (file.getline(buffer,SETREADER_MAX_LINE_LENGTH))
		{		
			if (buffer[0] != '#')	// not a comment line
			{									
				int start = 0;				// starting position		
				int end = 0;				// ending position
				bool wordStarted = false;	// we are into a word ??

				unsigned position = 0;				// current position
				char posChar = buffer[position];	// current char

				// -------------------------------------------------
				// find start and end position in this line
				// -------------------------------------------------

				while (position < strlen(buffer) && end == 0) 
				{
					// different from non-blank and # chars
					if (posChar != ' ' && posChar != '\t' 
						&& posChar != '#')	
					{
						if (!wordStarted)
						{						
							start = position;	// first non-blank char
							wordStarted = true;
						}
					}

					// blank char
					else									
					{
						if (wordStarted)
						{						
							end = position - 1;	// last non-blank char							
						}
					}

					position++;
					posChar = buffer[position];
				}


				// -------------------------------------------------
				// store the file name
				// -------------------------------------------------

				// if non-blank line
				if (wordStarted)	
				{
					if (end == 0) 
					{
						end = strlen(buffer) - 1;
					}
					
					// length of the file name
					int nameLength = end - start + 1;	
					
					// string to store this name
					std::string name;
					name.reserve(nameLength + 1);

					// copy the chars into name[]
					int i;
					for (i=0; i < nameLength; i++)
					{
						name += buffer[i + start];
					}
					name +='\0'; // end of string
					

					// store name in the array fileNames
					this->fileNames.push_back(name);

					// update this->numberOfFiles
					this->numberOfFiles++;
				}
			} // if not a comment line			
		} // while getline
		file.close();
	}
}


