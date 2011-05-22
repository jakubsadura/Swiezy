/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "blPDSetReader.h"
#include "blPDShapeReader.h"

//#define DEBUG_MESSAGES_SETREADER


/** 
 *	This method creates a new PDSet object.
 *	
 */
//--------------------------------------------------
blPDSetReader::blPDSetReader() : fileName(NULL)
//--------------------------------------------------
{
	//this->pdSet = 0; // null pointer
	//this->fileName = 0;
}



/** 
 *	The Destructor.
 */
//--------------------------------------------------
blPDSetReader::~blPDSetReader()
//--------------------------------------------------
{
	if ( this->fileName!=NULL ) 
	{
		delete[] this->fileName;
	}

}


/** 
 *	This method creates a new PDSet object.
 *
 *	\param fileName name of the file that contains a 
 *	list of names of shape files
 */
//--------------------------------------------------
void blPDSetReader::SetFilename(const char * fileName)
//--------------------------------------------------
{
	if ( this->fileName!=NULL )
	{
		delete[] this->fileName;
	}

	this->fileName = new char [strlen(fileName) + 1];
	strcpy(this->fileName, fileName);
}



/** 
 *	This method reads from the file and creates the blPDSet object
 *	with all the blPDShapes.
 */
//--------------------------------------------------
void blPDSetReader::Update()
//--------------------------------------------------
{

	// number of files read
	int numberFiles = 0;	

	// vector to store names of shape files
	std::vector<std::string> fileNames;	


// Get the file names from file .lst
/*=======================================================================*/

	// buffer for each line
	char buffer[MAX_LINE_LENGTH];		


	// open the file for reading
	std::ifstream file(this->fileName);
	
	if (!file)	// error
	{
		std::cerr << "Error: File " << this->fileName << 
									" not found" << std::endl;
	}
	else
	{
		// read a new line while it is possible
		while (file.getline(buffer,MAX_LINE_LENGTH))
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
					fileNames.push_back(name);

					// update numberFiles
					numberFiles++;
				}
			} // if not a comment line			
		} // while getline

		file.close();
	}



// Create the blPDShapes for this files
/*=======================================================================*/
	

	int numberShapes = 0;	// number of blPDShapes created

	// array to store the new blPDShapes	
	blPDShapeInterface::Pointer* shapeArray= 
			new blPDShapeInterface::Pointer [numberFiles];
	
	blPDShapeInterface::Pointer ptShape;					// pointer to blPDShape
	blPDShapeReader::Pointer ptShapeReader;		// pointer to blPDShapeReader
	

	// for all the file names
	int i;
	for (i = 0; i < numberFiles; i++)
	{
		// create the blPDShape

		std::string fileName = fileNames[i];

		ptShapeReader = blPDShapeReader::New();
		ptShapeReader->SetFilename((char *) fileName.c_str());

		ptShapeReader->Update();
		ptShape = ptShapeReader->GetOutput();

		if (ptShape.IsNotNull())
		{
			#ifdef DEBUG_MESSAGES_SETREADER
				std::cout << "New Shape: " << fileName.c_str() << std::endl;
			#endif

			shapeArray[numberShapes] = ptShape;
			numberShapes++;
		}
		else
		{
			std::cerr << "Error in file name : " << 
							(char *) fileNames[i].c_str() << 
								". PDShape not created" << std::endl;
		}
	}
	

	// create a new blPDSet
	this->pdSet = blPDSet::New();
	
	if (numberShapes)
	{
		this->pdSet->SetParameters(numberShapes, shapeArray);
	}
/*	else
	{
		this->pdSet = blPDSet::New();
		this->pdSet->SetParameters(numberShapes, 0); //NULL pointer
	}*/

	delete[] shapeArray;
}


		

