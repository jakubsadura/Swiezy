/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _blIASetReader_H
#define _blIASetReader_H
	
#include <string>
#include <vector>


#include "blLightObject.h"
#include "blSmartPointer.h"
#include "blMacro.h"

#include "BaseLibWin32Header.h"


/** \brief This class reads the i-th image from an image .lst set
 * \ingroup blImage 
 *
 *	This class reads the i-th image from an image .lst set
 *	It won't ever read no set of image for avoiding lacks of memory
 */
class BASELIB_EXPORT blIASetReader: public blLightObject
{

    public:
		typedef blIASetReader Self;
		typedef blSmartPointer<Self> Pointer;

		typedef std::vector<std::string> FilenamesVectorType;

		blNewMacro(Self);
		
		/** \brief returns the loaded image */
		FilenamesVectorType GetOutput() const
			{ return this->fileNames; };	
		
		/** \brief retrieves number of filenames read by update */
		int GetNumberOfFilesInTheSet() const
			{ return this->numberOfFiles; }; 
		
		/** \brief Set filename */
        void SetFilename(const char * fileName);

		/** \brief reads from list file and loads the i-th image */
		void Update();

		/** \brief return filename from the set */
		std::string GetFile ( unsigned index ) const 
		{
			return fileNames[index];
		}

	protected:

		/** \brief Constructor */
        blIASetReader();

		/** \brief Destructor */
		virtual ~blIASetReader();


    private:        
        char * fileName;				// image .lst to be read
		FilenamesVectorType fileNames;	// vector to store names of shape files

		int numberOfFiles;				// number of filenames read

		blIASetReader(const Self&); //purposely not implemented
		void operator=(const Self&); //purposely not implemented
};

#endif
