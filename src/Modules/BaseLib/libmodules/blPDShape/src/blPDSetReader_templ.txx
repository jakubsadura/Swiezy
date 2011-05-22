/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
//This class allow to create a new PDSet from a file that 

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "blPDSetReader_templ.h"
#include "blPDShapeReader.h"

//#define DEBUG_MESSAGES_SETREADER
#ifndef MAX_LINE_LENGTH
	#define MAX_LINE_LENGTH 200
#endif


/** 
 *	This method creates a new PDSet object.
 *	
 */
//--------------------------------------------------
template< class ArgMatrixType >
blPDSetReader_templ<ArgMatrixType>::blPDSetReader_templ() : fileName(NULL)
//--------------------------------------------------
{
	//this->pdSet = 0; // null pointer
	//this->fileName = 0;
	m_verbose = false;
	m_allShapesInfoKeep = false;
}



/** 
 *	The Destructor.
 */
//--------------------------------------------------
template< class ArgMatrixType >
blPDSetReader_templ<ArgMatrixType>::~blPDSetReader_templ()
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
template< class ArgMatrixType >
void blPDSetReader_templ<ArgMatrixType>::SetFilename(const char * fileName)
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
template< class ArgMatrixType >
void blPDSetReader_templ<ArgMatrixType>::Update()
//--------------------------------------------------
{

	if (m_verbose)
	{
		std::cout << "Reading shape set list...";
	}

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

	if (m_verbose)
	{
		std::cout << numberFiles << " elements found" << std::endl;
	}


// Create the blPDShapes for this files
/*=======================================================================*/
	
	// create a new blPDSet
	this->pdSet = blPDSet_templ<MatrixType>::New();
	

	// array to store the new blPDShapes	
	blPDShapeInterface::Pointer* shapeArray = 0;
	
	if (m_allShapesInfoKeep)
	{
		shapeArray = new blPDShapeInterface::Pointer [numberFiles];
	}
	

	blPDShapeInterface::Pointer ptShape;					// pointer to blPDShape
	blPDShapeReader::Pointer ptShapeReader;		// pointer to blPDShapeReader
	
	if (m_verbose)
	{
		std::cout << "\tReading shapes =>      ";
	}

	// for all the file names
	int i;
	int numberShapes = 0;	// number of blPDShapes created
	unsigned int numberOfCoordinates = 0;
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

			if (m_allShapesInfoKeep)
			{
				shapeArray[numberShapes] = ptShape;
			}			
			else
			{	
				if (numberShapes == 0)
				{
					numberOfCoordinates = ptShape->GetNumberOfLandmarks() * ptShape->GetDimension();
					pdSet->SetFirstShape (ptShape);
					this->thePoints = MatrixNamespace::New();
					bllao::Resize( *this->thePoints, numberFiles, numberOfCoordinates);
				}

				vnl_vector<double> new_points (numberOfCoordinates);
				ptShape->GetPoints( new_points );
				bllao::CopyVectorToRow( *(this->thePoints), numberShapes, new_points );
						
			}
			numberShapes++;
		}
		else
		{
			std::cerr << "Error in file name : " << 
							(char *) fileNames[i].c_str() << 
								". PDShape not created" << std::endl;
		}

		if (m_verbose)
		{
			char aux_char[10];
            sprintf (aux_char, "%5d", i + 1);
			std::cout << "\b\b\b\b\b" << aux_char;			
		}
	}
    	
	
	if (numberShapes)
	{
		if (m_allShapesInfoKeep)
		{
			this->pdSet->SetParameters(numberShapes, shapeArray);
		}
		else
		{			
			this->pdSet->SetParameters ( thePoints );
		}	
	}
/*	else
	{
		this->pdSet = blPDSet::New();
		this->pdSet->SetParameters(numberShapes, 0); //NULL pointer
	}*/

	if (m_allShapesInfoKeep)
	{
		delete[] shapeArray;
	}

	if (m_verbose)
	{
		std::cout << std::endl;
	}
}


		

