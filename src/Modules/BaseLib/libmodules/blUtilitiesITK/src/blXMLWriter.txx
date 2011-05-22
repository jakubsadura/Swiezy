/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
//This class defines tokenizer for strings.

#ifndef __blXMLWriter__TXX
#define __blXMLWriter__TXX

#include "blXMLWriter.h"

/**
 *	Constructor.
 */
//--------------------------------------------------
template <typename T>
blXMLWriter<T>::blXMLWriter()
//--------------------------------------------------
{
	this->indentLevel = 0;
	this->indentSize = 2;

	this->doublePrecision = 6;	// by default
}



/**
 *	Destructor
 */
//--------------------------------------------------
template <typename T>
blXMLWriter<T>::~blXMLWriter()
//--------------------------------------------------
{
}



/**
 *	This method writes an start element with the corresponding indentation.
 *
 *	\param tag the name of the element
 *	\param file the output stream to the file
 */
//--------------------------------------------------
template <typename T>
void blXMLWriter<T>::WriteStartElementIndent(const char * const tag, 
											 std::ofstream & output)
//--------------------------------------------------
{
	this->WriteIndentation(output);
	Superclass::WriteStartElement(tag, output);
	this->indentLevel++;
}




/**
 *	This method writes an start element with the corresponding indentation.
 *
 *	\param tag the name of the element
 *	\param file the output stream to the file
 */
//--------------------------------------------------
template <typename T>
void blXMLWriter<T>::WriteStartElementIndent(const std::string & tag, 
											 std::ofstream & output)
//--------------------------------------------------
{
	this->WriteStartElementIndent(tag.c_str(), output);
}



/**
 *	This method writes an end element with the corresponding indentation.
 *
 *	\param tag the name of the element
 *	\param file the output stream to the file
 */
//--------------------------------------------------
template <typename T>
void blXMLWriter<T>::WriteEndElementIndent(const char * const tag, 
										   std::ofstream & output)
//--------------------------------------------------
{
	this->indentLevel--;
	this->WriteIndentation(output);
	Superclass::WriteEndElement(tag, output);
}




/**
 *	This method writes an end element with the corresponding indentation.
 *
 *	\param tag the name of the element
 *	\param file the output stream to the file
 */
//--------------------------------------------------
template <typename T>
void blXMLWriter<T>::WriteEndElementIndent(const std::string & tag, 
										   std::ofstream & output)
//--------------------------------------------------
{
	this->WriteEndElementIndent(tag.c_str(), output);
}



/**
 *	Write a indentation whose size is indentSize * indentLevel blank spaces 
 */
//--------------------------------------------------
template <typename T>
void blXMLWriter<T>::WriteIndentation(std::ofstream & outStream)
//--------------------------------------------------
{
	size_t i;
	for (i = 0; i < this->indentSize * this->indentLevel; i++)
	{
		outStream << " ";
	}
}



/**
 *	Write a vnl_vector<double> in a single line 
 *	(including end of line, endl), with scientific format
 *
 */
//--------------------------------------------------
template <typename T>
void blXMLWriter<T>::WriteVector(const vnl_vector<double> & vector, 
											std::ofstream & outStream,
											bool indentation)
//--------------------------------------------------
{
	this->WriteVector( vector.begin(), vector.size() ,outStream, indentation);
}



/**
 *	Write a std::vector<double> in a single line
 *	(including end of line, endl), with scientific format
 */
//--------------------------------------------------
template <typename T>
void blXMLWriter<T>::WriteVector(const std::vector<double> & vector, 
											std::ofstream & outStream,
											bool indentation)
//--------------------------------------------------
{
  this->WriteVector(&*vector.begin(), vector.size(), outStream, indentation); //Ir Fix this properly
}



/**
 *	Write a double array in several lines (NUMBER_OF_COLS_PER_LINE)
 *	with scientific format and with precision specified 
 *	by this->doublePrecision
 */
//--------------------------------------------------
template <typename T>
void blXMLWriter<T>::WriteVector(const double array[], unsigned int arraySize, 
											std::ofstream & outStream,
											bool indentation)
//--------------------------------------------------
{
	// save previous precision
	unsigned int precision = outStream.precision();
	outStream.precision(this->doublePrecision);

	// scientific format "e-10"
	outStream.setf(std::ios::scientific);

	
	int colsCounter = 0;

	const double * doublePt;

	unsigned i;
	for (i = 0, doublePt = array; i < arraySize; i++, doublePt++)
	{
		// indentation
		if (colsCounter == 0)
		{
			if (indentation)
			{
				this->WriteIndentation(outStream);
			}
		}

		outStream << (double) *doublePt << " " ;
		colsCounter++;

		// new line
		if (colsCounter == NUMBER_OF_COLS_PER_LINE)
		{
			outStream << std::endl;
			colsCounter = 0;
		}
	}

	if (colsCounter != 0)
	{
		outStream << std::endl;
	}	

	
	// restore precision
	outStream.precision(precision);	
}

#endif






