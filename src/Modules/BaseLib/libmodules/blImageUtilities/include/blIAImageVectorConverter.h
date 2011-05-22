/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __blIAImageVectorConverter_h
#define __blIAImageVectorConverter_h

//#include "blImage.h"
//#include "blDefines.h"
#include "blLightObject.h"
#include "blSmartPointer.h"
#include "blMacro.h"
#include "blVector.h"
#include <itkImageRegionIterator.h>

/**-----------------------------------------------
 * \note USE MULTIBAND IMAGE
 *
 * \ingroup blImageUtilities
 * If RoundUp is used (default) then TImage::PixelType must be 
 * one of the integer types and vector elements must must be of a type that can be  
 * passed as a parameter to ceil, floor or trunc function.
 * 
 * Example:
 * \code
 *		SetImage()
 *		// masked pixels are marked with 0
 *		SetImageMask()
 *
 *	// You can choose one of these functions:
 *		// to get the vector of pixels or
 *		SampleUnderMask() 
 *		// to render vector onto the image under the mask
 *		FillUnderMask() 
 * \endcode
 *
 */
template <class TImage, class TMask>
class blIAImageVectorConverter: public blLightObject
{
public:
	typedef TImage ImageType; 
	typedef typename ImageType::PixelType PixelType;
	typedef blIAImageVectorConverter Self;
	typedef blSmartPointer<Self> Pointer;
	typedef blVector<double> VectorNamespace;
	typedef VectorNamespace::VectorType VectorType;
	typedef VectorNamespace::Pointer VectorPointer;

	typedef typename ImageType::PixelType ImagePixelType;
	
	typedef TMask MaskType;
	typedef typename MaskType::RegionType MaskRegionType;
	typedef itk::ImageRegionIterator< MaskType > MaskIteratorType;
	typedef itk::ImageRegionIterator< typename ImageType::ImageType > IteratorType;

	blNewMacro(Self);

	// Generic type to cpecify type of rounding required 
	// rtRound - default, usual rounding ( = int( x + 0.5 ) )
	// rtFloor - roundup towards the smallest integer
	// rtCeil - roundup towards the largest integer
	// rtTrunc - truncate number ( =  int( x ) ) 
	enum RoundupType { rtRound, rtFloor, rtCeil, rtTrunc };

	void UseRoundUp (bool performRoundUp) 
		{_performRoundUp = performRoundUp; };

	/** \brief Set/get image and mask for processing
		*  For now only grayscale.
		*/
	void SetImage (ImageType* image) { _image = image; };
	typename ImageType::Pointer GetImage() const { return _image; };

	void SetImageMask (MaskType* mask) 
	{ 
		_mask = mask; 
		EstimateNumberOfPixels();
	};
	
	
	typename MaskType::Pointer GetImageMask () const { return _mask; };

	/** \brief Set type of round up
		*  When bloating point pixels are inserted into image
		*  pixels have to be rounded up
		*/
	void SetRoundupType (RoundupType type) { _roundUp = type; };

	


	/** \brief Extract nonmasked pixels into vector, 
		*
		*      vector already must have correct size 
		*      use EstimateNumberOfPixels() to get the required size,
		*      no length check!
		* 
		* (pixels in the vector are arranged rowwise)	
		* VC6++ bug: it is unable to work with templated function in a 
		* templated class if this function's body is outside of the class
		*
		* Another VC6++ bug? : 
		*  typedef blVector<double> VectorType;
		*  VectorPointer vect = VectorNamespace::New(...)
		*  SampleUnderMask( vect ) // bug of VC6: cannot resolve template parameter
		*  SampleUnderMask( vect.GetPointer() ) // OK
		*/
	template <class VectorPointerType>
	void SampleUnderMask( VectorPointerType pixels ) const
	{
		pixels->set_size(_vectorLength);
		MaskRegionType region;	
		
		if ( _mask->GetMaskRegion() != NULL ) region = *_mask->GetMaskRegion();
		else region = _mask->GetLargestPossibleRegion();
		
		int count = 0;
		for( int i=0; i<_image->GetNumberOfBands(); i++ )
		{
			if( _image->CanSample(i) )
			{
				MaskIteratorType maskIt( _mask, region ); //don't put it outside. There is a bug somewhere in ITK
				IteratorType imageIt( _image->GetBand(i), region );

				maskIt.GoToBegin();
				imageIt.GoToBegin();
				while (  !maskIt.IsAtEnd() && !imageIt.IsAtEnd() )
				{
					if ( maskIt.Get() != 0 )
					{
						(*pixels)(count++) = imageIt.Get();
					}

					++maskIt; 
					++imageIt;
				}
			}
		}

		assert( count == _vectorLength ); 
	};


	/** \brief Set nonmasked pixels to vector values	
		* (pixels in the vector are arranged rowwise)	
		*/
	template <class VectorPointerType>
	void FillUnderMask( VectorPointerType pixels )
	{
		assert( pixels->size() == _vectorLength );
		
		typename MaskType::RegionType region;	
		
		if ( _mask->GetMaskRegion() != NULL ) region = *_mask->GetMaskRegion();
		else region = _mask->GetLargestPossibleRegion();
		int count=0;

		const bool performRoundUp = _performRoundUp;

		for( int i=0; i<_image->GetNumberOfBands(); i++ )
		{
			if( _image->CanSample(i) )
			{
				MaskIteratorType maskIt( _mask, region ); //don't put it outside. There is a bug somewhere in ITK
				IteratorType imageIt( _image->GetBand(i), region );

				maskIt.GoToBegin();
				imageIt.GoToBegin();
				while( !maskIt.IsAtEnd()  && !imageIt.IsAtEnd() )
				{
					if ( maskIt.Get() != 0 )
					{
						if (performRoundUp) imageIt.Set( RoundUp( (*pixels)(count++) ) );
						else imageIt.Set( (PixelType)(*pixels)(count++) );
					}

					++maskIt;
					++imageIt;
				}
			

			}
		}

		assert( count == _vectorLength ); 
	};


	unsigned GetNumberOfSampledPixels()
	{
		return _vectorLength;
	}

protected:
	/** \brief Constructor and destructor
		*/
	blIAImageVectorConverter();
	virtual ~blIAImageVectorConverter();
	
	template <class TElement>
	int RoundUp(TElement x) const
	{
		int result=0;

		switch( _roundUp )
		{
			case rtRound:
				result = int(x + 0.5); 
				break;
			case rtFloor:
				result = (int)floor(x); 
				break;
			case rtCeil:
				result = (int)ceil(x); 
				break;
			case rtTrunc:
				result = int(x); 
				break;

			default:
				throw "blIAImageVectorConverter::RoundUp : incorrect _roundUp type";
		};

		return result;
	};

	/** \brief Calculate number of pixels under the mask */
	void EstimateNumberOfPixels();

private:	
	blIAImageVectorConverter(const Self&); //purposely not implemented
	void operator=(const Self&); //purposely not implemented

	typename ImageType::Pointer _image;
	typename MaskType::Pointer _mask;

	RoundupType _roundUp;
	unsigned _vectorLength;
	bool _performRoundUp;
};

#include "blIAImageVectorConverter.txx"

#endif
