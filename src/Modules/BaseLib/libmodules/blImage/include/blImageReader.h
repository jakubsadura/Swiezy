/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _blImageReader_h
#define _blImageReader_h


#include <itkImageFileReader.h>

#include "blLightObject.h"
#include "blSmartPointer.h"
#include "blMacro.h"

#include "BaseLibWin32Header.h"


/** \brief Reader for blImages
 * \ingroup blImage 
 *
 *  It is derived from itk::ImageFileReader<TOutputImage, 
 *	ConvertPixelTraits = DefaultConvertPixelTraits>
 *
 */
template <class TOutputImage>
class blImageReader: public itk::ImageFileReader<TOutputImage>

{
	public:
		
		typedef blImageReader Self;
		typedef blSmartPointer<Self>  Pointer;
		typedef blSmartPointer<const Self>  ConstPointer;

		/** \brief Static constructor */
		blNewMacro(Self);


	protected:
		blImageReader();
		virtual ~blImageReader();

	private:
		blImageReader(const Self&); 
			//purposedly not implemented (see itk::Image)
		void operator= (const Self&); 
			//purposedly not implemented (see itk::Image)
};

#include "blImageReader.txx"

#endif
