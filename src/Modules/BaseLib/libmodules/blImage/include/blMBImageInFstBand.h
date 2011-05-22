/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _blMBImageInFstBand_h
#define _blMBImageInFstBand_h


//#include <blImage.h>
#include <blMultiBandImage.h>

#include "blLightObject.h"
#include "blSmartPointer.h"
#include "blMacro.h"

#include <vector>


/**
 * \brief Multi-Band image, original image is loaded into the first band
 * \ingroup blImage 
 */
template <class TImage, unsigned int NBands>
class blMBImageInFstBand: public blMultiBandImage<TImage, NBands>
{
	public:
		
		typedef blMBImageInFstBand Self;
		typedef blSmartPointer<Self>  Pointer;
		typedef blSmartPointer<const Self>  ConstPointer;

		/** \brief Static constructor */
		blNewMacro(Self);

		virtual void LoadImageFromFilename( const char* filename );

		virtual void SaveImage( const char* filename );

	protected:
		blMBImageInFstBand();
		virtual ~blMBImageInFstBand();

	private:
		blMBImageInFstBand(const Self&); //purposedly not implemented (see itk::Image)
		void operator= (const Self&); //purposedly not implemented (see itk::Image)


};


#include "blMBImageInFstBand.txx"

#endif
