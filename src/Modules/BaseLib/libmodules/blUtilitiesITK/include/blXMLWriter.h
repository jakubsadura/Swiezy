/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __blXMLWriter_H
#define __blXMLWriter_H

#include <vnl/vnl_vector.h>
#include <vnl/vnl_matrix.h>
#include <itkXMLFile.h>

#include "blLightObject.h"
#include "blSmartPointer.h"
#include "blMacro.h"
#include "blLinearAlgebraOperations.h"

/**
 * \brief This class defines a generic XML writer for SMoLib with functionality
 *	for write tags with indentation.
 * \ingroup blUtilities
 *
 *	This template class has as parameter the type of the object 
 *	that will be written.
 *	This is because a pointer to this object exists in the Superclass. 
 *	See itk::XMLWriter for more	details
 */
template <typename T>
class blXMLWriter: public itk::XMLWriterBase<T>
{

	public:
		typedef blXMLWriter Self;
		typedef blSmartPointer<Self> Pointer;

		blNewMacro(Self);

		// Typedefs
		//-----------------------------------------------------
		typedef itk::XMLWriterBase<T> Superclass;


		static const int NUMBER_OF_COLS_PER_LINE;

		// Methods
		//-----------------------------------------------------


		void SetDoublePrecision(unsigned char precision)
		{
			this->doublePrecision = precision;
		};

		/** sets the number of spaces for an indentation level */
		void SetIndentationSize(unsigned int numberOfSpaces)
			{ this->indentSize = numberOfSpaces; };

		/**\brief writes a start element with the corresponding indentation */
		void WriteStartElementIndent(const char * const tag, 
										std::ofstream &file);

		/**\brief writes a start element with the corresponding indentation */
		void WriteStartElementIndent(const std::string & tag, 
										std::ofstream &file);

		/**\brief writes an end element with the corresponding indentation */
		void WriteEndElementIndent(const char * const tag, 
										std::ofstream &file);

		/**\brief writes an end element with the corresponding indentation */
		void WriteEndElementIndent(const std::string & tag, 
										std::ofstream &file);

	protected:
		
		unsigned int indentSize;	//!< size in spaces for an indentation 
		unsigned int indentLevel;	//!< current indentation level

		unsigned int doublePrecision;	//!< precision for double float numbers


		// Methods
		//-----------------------------------------------------

		/**\brief Constructor */
		blXMLWriter();

		/**\brief Desctructor */
		virtual ~blXMLWriter();


		/**\brief write the corresponding indentation in the file */
		void WriteIndentation(std::ofstream & outStream);

		/**
		*	Write a matrix, with scientific format. 
		*	Each row in a different line.
		*/
		template< class MatrixType >
		void WriteMatrix(const MatrixType& matrix, std::ofstream & outStream, bool indentation)
		{
			// save previous precision
			unsigned int precision = outStream.precision();
			outStream.precision(this->doublePrecision);

			// scientific format i.e.: "e-10"
			outStream.setf(std::ios::scientific);

			for (int row = 0; row < bllao::NrRows(matrix); row++)
			{
				if (indentation)
				{
					this->WriteIndentation(outStream);
				}

				for (int col = 0; col < bllao::NrCols(matrix); col++)
				{
					outStream << (double) matrix(row, col) << " " ;
				}

				outStream << std::endl;	// end of line
			}

			outStream.precision(precision);	// restore precision
		}

		void WriteVector(const std::vector<double> & vector, 
											std::ofstream & outStream,
											bool indentation = false);

		void WriteVector(const vnl_vector<double> & vector, 
											std::ofstream & outStream,
											bool indentation = false);

	
		void WriteVector(const double array[], unsigned int arraySize, 
											std::ofstream & outStream,
											bool indentation);

	private:
		blXMLWriter(const Self&); //purposely not implemented
		void operator=(const Self&); //purposely not implemented		

};

/**\brief definition of static constant for number of columns per line */
template<typename T>
const int blXMLWriter<T>::NUMBER_OF_COLS_PER_LINE = 6;

#include "blXMLWriter.txx"
#endif
