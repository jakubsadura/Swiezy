/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _blPDSetReader_H
#define _blPDSetReader_H

#include "blPDSet.h"
#include "blLightObject.h"
#include "blSmartPointer.h"
#include "blMacro.h"

#include "BaseLibWin32Header.h"

// max length of a line in the list file
#ifndef MAX_LINE_LENGTH
	#define MAX_LINE_LENGTH 200
#endif


/**
 * \brief This class reads a new blPDSet from a list of files
 * \ingroup blPDShape
 *
 *	This class reads from a text file (.lst) that contains a number 
 *	of names of shape files, in VTK format, that define PDShapes.
 * 
 *	The file extension should be .lst, with one file name in each line.
 */
class BASELIB_EXPORT blPDSetReader: public blLightObject
{

    public:
		typedef blPDSetReader Self;
		typedef blSmartPointer<Self> Pointer;

		blNewMacro(Self);

		/** \brief Set Filename */
        void SetFilename(const char * fileName);

		
		/** \brief reads from list file and create the PDSet object */
		void Update();

		/** \brief returns the output PDSet */
		blPDSet::Pointer GetOutput() const
			{ return this->pdSet;}	;	
						
	protected:
		/** \brief Constructor */
		blPDSetReader();

		/** \brief Destructor */
		virtual ~blPDSetReader();
    private:        
		blPDSetReader(const Self&); //purposely not implemented
		void operator=(const Self&); //purposely not implemented		

        char * fileName;		//!< file to be read
		blPDSet::Pointer pdSet;		//!< blPDSet to be built			

};

#endif
