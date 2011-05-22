/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _blIAMRSetReader_H
#define _blIAMRSetReader_H
	
#include <string>
#include <vector>


#include "blLightObject.h"
#include "blSmartPointer.h"
#include "blMacro.h"

#include "blImageReader.h"
#include "blIASetReader.h"
//#include "blImage.h"

#include "blIAMultiResolutionFilter.h"



/**
 * \brief Loads list of images. Allows loading image for specified 
 * resolution.	
 * \ingroup blImage 
 *
 * it is advised to use MultiBandImage floating point image type as TImage.
 *  Otherwise TImage must have implemented LoadImage with possibility to 
 * change resolution and SetResolution
 */
template <class TImage>
class blIAMRSetReader: public blIASetReader
{

    public:
		typedef blIAMRSetReader Self;
		typedef blSmartPointer<Self> Pointer;
	
		typedef TImage ImageType;

		typedef blImageReader<ImageType> ImageReaderType;


		blNewMacro(Self);

		//SetResolution() will clear blag, no multiresolution
		void SetResolution( unsigned resolution )
		{
			_resolution = resolution;
		};

		unsigned GetResolution() const
		{
			return _resolution;
		};
		
		/** \brief load the image 
		 *  should be removed from this class. This class must not depend on 
		 *  data access methods
		 *
		 *  This is the reason for bloating point type image. I'm lazy to extend this function or 
		 *  class to support rescaling. :)
		 *  Who wants, he already has a piece of code that would do rescaling, he just need to 
		 *  do some changes to the class.
		 *  Actually it is possibly to implement a templated function LoadRescaledImage() for example.
		 */
		virtual typename ImageType::Pointer LoadImageFromFilename(int i)
		{
			typename ImageType::Pointer image = ImageType::New();
			image->SetResolution( _resolution );
			image->LoadImageFromFilename( GetFile(i).c_str() );

			return image;
		};

	protected:

		/** \brief Constructor */
        blIAMRSetReader();

		/** \brief Destructor */
		virtual ~blIAMRSetReader();


    private:        

		blIAMRSetReader(const Self&); //purposely not implemented
		void operator=(const Self&); //purposely not implemented

		unsigned _resolution;

};

#include "blIAMRSetReader.txx"

#endif
