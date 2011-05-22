/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __blWarp2DReferenceFrame_h
#define __blWarp2DReferenceFrame_h

#include "blMacro.h"
#include "blLightObject.h"
#include "blIAWarp2DFilter.h"
#include "blIAImageVectorConverter.h"
#include "blVector.h"
#include "blImageMask.h"

//-----------------------------------------------
/** 
 * \brief This class implements reference frame for AAM warping
 * \ingroup blImageUtilities
 *
 * \note USE MULTIBAND IMAGE AS TImage
 *
 * The use is quite simple. Call Setup where you can specify shape and 
 * it's scale that are to be used to create reference shape and image. 
 * The shape will be rescaled by the specified scale factor and
 * displaced so that it has only positive landmark coordinates. 
 * This shape can be retrieved by the GetReferenceShape()
 *
 * To sample pixels of some image from under the specified shape use 
 * SampleShape()
 * 
 * If you want to Synthesize texture patch with the given shape unto the 
 * image, use Synthesize(). 
 * But don't forget to set mirroredEdgeWidth parameter during the call to 
 * Setup() to a value different from 0. Otherwise you risk seeing the 
 * black border around the rendered patch due to the 
 * interpolation. The common value for this would be 1.
 *
 * Value of mirroredEdgeWidth does not affect anything else aside from 
 * Synthesize(). But it maybe better to use 0 if there is no need to 
 * call Synthesize() because the image of the reference frame has size of 
 * the reference shape plus 2 times the mirroredEdgeWidth, therefore you 
 * can save some space and time.
 */
template <class TImage, class TShape>
class blWarp2DReferenceFrame: public blLightObject 
{
	public:
		typedef blWarp2DReferenceFrame Self;
		typedef blSmartPointer<Self> Pointer;

		typedef TImage ImageType;
		typedef TShape ShapeType;
		typedef blIAWarp2DFilter<ImageType> WarpType;
		typedef blIAImageVectorConverter<ImageType, typename WarpType::MaskType> I2VConverterType;
		typedef blVector<double> VectorNamespace;
		typedef typename VectorNamespace::VectorType VectorType;
		typedef typename VectorNamespace::Pointer VectorPointer;
		
		typedef typename ImageType::SizeType ImageSizeType;
		
		blNewMacro(Self);

		// mirroredEdgeWidth - affects only Synthesize()
		// if you don't intend to call it, leave this parameters 0
		void Setup( ShapeType* shape, double scaleFactor = 1, unsigned mirroredEdgeWidth=0 );
		
		//bMirrorEdge - edge will be mirrored in the input image!
		void SampleShape( ShapeType* shape, ImageType* image, VectorType* pixels = NULL, bool bMirrorEdge = false );

		void Synthesize( ShapeType* shape, ImageType* image, VectorPointer texture );

		typename ShapeType::Pointer GetReferenceShape()
		{
			return _referenceShape;
		}


		typename ImageType::Pointer GetReferenceImage()
		{
			return _referenceImage;
		}


		unsigned GetNumberOfPixels()
		{
			return _textureLength;
		}

		
		void FillReferenceImage( VectorType* pixels )
		{
			_i2vConverter -> FillUnderMask( pixels );
		}
		
		void GetSampledPixels( VectorPointer pixels )
		{
			pixels->set_size( _lastSampledPixels->size() );
			pixels->copy_in( _lastSampledPixels->begin() );
		}

		VectorPointer GetSampledPixels( )
		{
			return _lastSampledPixels;
		}

	protected:
		/** \brief Constructor and destructor
		 *	
		 */
		blWarp2DReferenceFrame();
		virtual ~blWarp2DReferenceFrame();
		
		void MirrorEdge();
		
		void Initialize();
		
		void Sample();

		void PrepareShape4ImageGeneration(ShapeType* shape, ImageSizeType* size );
		
	private:
		blWarp2DReferenceFrame(const Self&); //purposely not implemented
		void operator=(const Self&); //purposely not implemented

		typename ShapeType::Pointer _referenceShape;
		typename ImageType::Pointer _referenceImage;
		

  	typename WarpType::Pointer _referenceWarper;
		
		int _textureLength;
		int _mirroredEdge;
		double _shapeMeanSize;
		
		typename I2VConverterType::Pointer _i2vConverter;
		
		typename ShapeType::Pointer _lastSampledShape;
		typename ImageType::Pointer _lastSampledImage;
		VectorPointer _lastSampledPixels;
	
};


#include "blWarp2DReferenceFrame.txx"

#endif


