/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __blPDShapeWriter_H
#define __blPDShapeWriter_H


#include <itkSmartPointer.h>

//#include "blDebug.h"
#include "blPDShapeInterface.h"
#include "blXMLWriter.h"
#include "blLightObject.h"
#include "blSmartPointer.h"
#include "blMacro.h"


/**
 * \brief This class is a writer for blPDShape objects.
 * \ingroup blPDShape
 *
 *	This class allows to save in a VTK file the data that defines an blPDShape.
 *
 *	The file format will be XML VTK and the file extension should be .vtp.
 */
class blPDShapeWriter : public blXMLWriter<blPDShapeInterface>
{

    public:	

		// ----------------------------------------------
		// Typedefs
		//-----------------------------------------------

		typedef blPDShapeWriter Self;
		typedef blSmartPointer<Self> Pointer;


		// ----------------------------------------------
		// Methods
		//-----------------------------------------------

		/** \brief static constructor */
		blNewMacro(Self);
				

		/** \brief set the blPDShape object to be saved to file */
		void SetPDShape(blPDShapeInterface::Pointer pdShape)
			{ this->SetObject(pdShape); };
	

		// Methods inherited from the superclass
		//-----------------------------------------------------

		 /** Return non-zero if the filename given is writeable. */
		virtual int CanWriteFile(const char* name);

		/** \brief write the blPDShape */
		virtual int WriteFile();


	protected:

		/** \brief Constructor */
		blPDShapeWriter();

		/** \brief Destructor */		
		virtual ~blPDShapeWriter();

		/** \brief write data related to this writer */
		void WriteData(std::ofstream &output);


    private:        				
		blPDShapeWriter(const Self&); //purposely not implemented
		void operator=(const Self&); //purposely not implemented		

		/**\brief write an array of int*/
		void WriteDataArray(std::vector<unsigned int> dataArray,
												std::ofstream & output);

		/**\brief returns two vectors (connectivity and offsets) 
		from a given vector of cells */
		void GetConnectivityAndOffset(
				const blPDShapeInterface::ShapeCellsContainer & cellContainer,
				std::vector<unsigned int> & connectivity, 
				std::vector<unsigned int> & offsets);


};

#endif
