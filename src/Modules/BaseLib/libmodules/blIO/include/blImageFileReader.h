/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _blImageFileReader_h
#define _blImageFileReader_h


// basic includes
#include <string>
using std::string;

#include <blLightObject.h>
#include <blSmartPointer.h>
#include <blMacro.h>

#include "BaseLibVTKWin32Header.h"


/// \brief Abstract definition of a class that reads an arbitrary image from a file.

class BASELIBVTK_EXPORT blImageFileReader: public blLightObject
{
	public:

		//-----------------------------------------------------
		// Typedefs
		//-----------------------------------------------------

		typedef blImageFileReader Self;
		typedef blSmartPointer< Self > Pointer;


		//-----------------------------------------------------
		// Methods
		//-----------------------------------------------------

		/// \brief Set the file name.
		void SetFileName( string fileName ) 
		{
			this->fileName = fileName;
		}

		/// \brief Read the image to the file specified using the method SetFileName.
		virtual void Update() = 0;


	protected:
		
		/// \brief Default Constructor.
		blImageFileReader();

		/// \brief Destructor.
		virtual ~blImageFileReader();


		//-----------------------------------------------------
		// Variables
		//-----------------------------------------------------

		string	fileName;

		/// \brief Get the file name.
		string GetFileName() 
		{
			return fileName;
		}

	private:

		blImageFileReader( const blImageFileReader& );	//purposely not implemented
		void operator = ( const blImageFileReader& );	//purposely not implemented
};


#endif // _blImageFileReader_h
