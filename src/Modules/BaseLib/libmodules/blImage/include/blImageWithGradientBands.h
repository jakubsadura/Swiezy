/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _blImageWithGradientBands_h
#define _blImageWithGradientBands_h


//#include <blImage.h>
#include <blMBImageInFstBand.h>
#include <blIAMultiResolutionFilter.h>

#include "blLightObject.h"
#include "blSmartPointer.h"
#include "blMacro.h"

#define NUMBER_OF_BANDS 3

/**
 * \brief 
 * \ingroup blImage 
 *
 * band 0 stores the image
 */
template <class TImage>
class blImageWithGradientBands: public blMBImageInFstBand<TImage, NUMBER_OF_BANDS> 
{
	public:
		
		typedef blImageWithGradientBands Self;
		typedef blSmartPointer<Self>  Pointer;
		typedef blSmartPointer<const Self>  ConstPointer;
		typedef blMBImageInFstBand<TImage, NUMBER_OF_BANDS> Superclass;

		
		typedef TImage ImageType;

		/** \brief Static constructor */
		blNewMacro(Self);

		void LoadImageFromFilename( const char* filename );

		void SaveImage( const char* filename ) ;


		void RecomputeBands();


	protected:
	
		blImageWithGradientBands();
		virtual ~blImageWithGradientBands();

		void InitializeBands();

	private:
		blImageWithGradientBands(const Self&); //purposedly not implemented (see itk::Image)
		void operator= (const Self&); //purposedly not implemented (see itk::Image)

};


#include "blImageWithGradientBands.txx"

#endif
