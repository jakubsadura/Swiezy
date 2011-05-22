/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __blWarp2DReferenceFrame_txx
#define __blWarp2DReferenceFrame_txx

//#include "blWarp2DReferenceFrame.h"
#include "itkImageRegionIterator.h"
//#include "blImageWriter.h"

#include "itkImageRegionIteratorWithIndex.h"
#include "itkConstNeighborhoodIterator.h"
#include <blSimilarity2DTransform.h>
#include <blAlignment.h>

template <class TImage, class TShape>
blWarp2DReferenceFrame<TImage, TShape>::blWarp2DReferenceFrame():
	_textureLength(0),
	_mirroredEdge(0)
{
	_referenceShape = ShapeType::New();
	_referenceImage = ImageType::New();
	_referenceWarper = 	WarpType::New();
	_i2vConverter = I2VConverterType::New();
		
	_lastSampledShape = ShapeType::New();
	_lastSampledImage = ImageType::New();
	_lastSampledPixels = VectorNamespace::New();

};

template <class TImage, class TShape>
blWarp2DReferenceFrame<TImage, TShape>::~blWarp2DReferenceFrame() 
{
};


template <class TImage, class TShape>
void blWarp2DReferenceFrame<TImage, TShape>::Setup( ShapeType* shape, double scaleFactor, unsigned mirroredEdgeWidth )
{
	_referenceShape = (ShapeType*)shape->GetCopy().GetPointer();
	
	_shapeMeanSize = scaleFactor;
	
	_mirroredEdge = mirroredEdgeWidth;

	Initialize();
}



template <class TImage, class TShape>
void blWarp2DReferenceFrame<TImage, TShape>::SampleShape( ShapeType* shape, ImageType* image, VectorType* pixels, bool bMirrorEdge )
{
	_lastSampledShape = shape;
	_lastSampledImage = image;
		
	if ( bMirrorEdge ) MirrorEdge();

	Sample();
	
	if( pixels != NULL )
	{
		pixels->set_size( _lastSampledPixels->size() );
		pixels->copy_in( _lastSampledPixels->begin() );
	}
}


template <class TImage, class TShape>
void blWarp2DReferenceFrame<TImage, TShape>::Initialize()
{
	typename ImageType::SizeType referenceShapeSize;
	PrepareShape4ImageGeneration( _referenceShape.GetPointer(), &referenceShapeSize ); //shift and rescale
	
	//image
	typename ImageType::IndexType index;
	index[0] = 0;
	index[1] = 0;
	typename ImageType::RegionType region;
	region.SetIndex( index );
	region.SetSize( referenceShapeSize );
	_referenceImage = ImageType::New();
	_referenceImage->SetRegions( region );
	
	for( int i=0; i<_referenceImage->GetNumberOfBands(); i++ )
		_referenceImage->GetBand(i)->FillBuffer(0);
	
	//warper
	_referenceWarper = WarpType::New();
	_referenceWarper -> SetOutputParameters( _referenceImage, _referenceShape );
	_referenceWarper -> Precalculate();

	//sampler
	_i2vConverter = I2VConverterType::New();
	_i2vConverter -> SetImage( _referenceImage ); 
	_i2vConverter -> SetImageMask( _referenceWarper->GetMask() );

	//get texture length and make sure it is the same as mean texture
	//to avoid problems
	_textureLength = _i2vConverter->GetNumberOfSampledPixels();
	
}



template <class TImage, class TShape>
void blWarp2DReferenceFrame<TImage, TShape>::PrepareShape4ImageGeneration(ShapeType* shape, ImageSizeType* size )
{

	int numberOfLandmarks = shape->GetNumberOfLandmarks();
	int shapeLength = numberOfLandmarks * shape->GetDimension();

	int i;

	vnl_vector<double> shapePoints(shapeLength);
	shape->GetPoints( shapePoints );



	if (_shapeMeanSize != 1)
	{
		//move o origin and change the scale
		vnl_vector<double> centroid(2);
		typename blSimilarity2DTransform::Pointer similarity = blSimilarity2DTransform::New();

		blAlignment::Centroid( shapePoints.begin(), 2, numberOfLandmarks, centroid.begin() );

		similarity->SetParameters( 1, 0, -centroid[0], -centroid[1] );
		similarity->TransformPoints( shapePoints );

		shapePoints.normalize(); //originally it was not (may not be) at center but have unit length, 
							//after shifting everything's changed ->must renormalize

		shapePoints *= _shapeMeanSize;

		//update shape, to be able to get bounding box
		shape->SetPoints( shapePoints );
	}



	//---------------------------------
	//
	// calculate bounding box 
	//
	//---------------------------------
	const typename ShapeType::BoundingBoxType* BoundingBox = shape->GetMeshBoundingBox();
	itk::FixedArray<double,4> Bounds = BoundingBox->GetBounds(); // 4 = Dimensions * 2

	//ImageType::SizeType boundingBoxSize;
	double boundingBoxIndex[2]; // 2 = Dimensions

	for( i=0; i<2; i++ ) // 2 = Dimensions
	{
		boundingBoxIndex[i] = Bounds[i*2];
		(*size)[i] = (unsigned long)ceil(Bounds[i*2+1] - boundingBoxIndex[i]) 
				+1 +2*_mirroredEdge; //+1 because last valid image pixel is at size[i]-1
									 //and also reserve some space for mirroring
									 //in principle this size can be larger. precision is not critical
	}


	//---------------------------------
	//
	//shift mean shape to have only positive coordinates (USING BOUNDING BOX INFORMATION)
	// and generally to have minimums at 0 (this will allow to make smaller image for 
	// appearance model
	//
	//---------------------------------
	VectorOf2DContinuousPoints akPoint=(VectorOf2DContinuousPoints)&shapePoints[0];

	for (i=0; i<numberOfLandmarks; i++)
	{
		akPoint[i].X() = (akPoint[i].X() - boundingBoxIndex[0])  + _mirroredEdge;
		akPoint[i].Y() = (akPoint[i].Y() - boundingBoxIndex[1])  + _mirroredEdge;
	}


	shape->SetPoints( shapePoints );


}




template <class TImage, class TShape>
void blWarp2DReferenceFrame<TImage, TShape>::Sample()
{
//	for( int i=0; i<_referenceImage->GetNumberOfBands(); i++ )
//		_refrenceImage->SetBand( i, _referenceImage->GetBand(i), _lastSampledImage->CanSample(i), _lastSampledImage

	_referenceWarper -> SetInputParameters( _lastSampledImage, _lastSampledShape );
	_referenceWarper -> Update(); //normalizedImage now points to normalized texture

	_lastSampledPixels->set_size( _textureLength );
	_i2vConverter -> SampleUnderMask( _lastSampledPixels );
}






template <class TImage, class TShape>
void blWarp2DReferenceFrame<TImage, TShape>::MirrorEdge( )
{
	//don't do anything if there's nothing to do
	if( _mirroredEdge==0 ) return;



	typedef typename WarpType::MaskType MaskType;
	typedef itk::ConstNeighborhoodIterator< typename MaskType::Superclass > NHoodIteratorType; 

	typename NHoodIteratorType::RadiusType radius;
	radius.Fill( _mirroredEdge );

	typename MaskType::Pointer mask = _referenceWarper->GetMask();

	const double largeConstant =  _mirroredEdge * _mirroredEdge * 100; //just to make it big

	//for every band
	for( int i=0; i< _referenceImage->GetNumberOfBands(); i++ )
	{
		if( _referenceImage->CanSample(i) )
		{
			NHoodIteratorType maskIter( radius, mask, mask->GetLargestPossibleRegion() ); //due to possible reusability issue
			itk::ImageRegionIteratorWithIndex<typename ImageType::ImageType> imageIter( _referenceImage->GetBand(i), mask->GetLargestPossibleRegion() );

			typename NHoodIteratorType::OffsetType offset;
			double distance; 
			typename ImageType::PixelType pixel;
			bool pixelFound;

			typename ImageType::IndexType idx;

			for( maskIter.GoToBegin(), imageIter.GoToBegin(); !maskIter.IsAtEnd(); ++maskIter, ++imageIter )
			{
				if( maskIter.GetCenterPixel() == 0 ) //for each masked pixel check its vicinity for notmasked pixels
				{
					pixelFound = false; //reset the blag
					distance = largeConstant; //smth big

					for( offset[0]=-_mirroredEdge; offset[0]<=_mirroredEdge; offset[0]++ )
					{
						for( offset[1]=-_mirroredEdge; offset[1]<=_mirroredEdge; offset[1]++ )
						{
							if( !(offset[0] == 0 && offset[1] == 0) ) //except the central pixel
							{
								if( maskIter.GetPixel(offset) > 0 )
								{
									//calculate distance to get the closest pixel in the window
									const double temp_distance = offset[0]*offset[0] + offset[1]*offset[1];
									if( temp_distance < distance ) 
									{
										idx = imageIter.GetIndex();
										idx += offset;
										pixel = _referenceImage->GetBand(i)->GetPixel( idx );

										distance = temp_distance;

										pixelFound = true;
									}
								}
							}
						}
					} //for ( offset[0]

					if( pixelFound ) imageIter.Set( pixel );
				}
			}
		}
	}
}





template <class TImage, class TShape>
void blWarp2DReferenceFrame<TImage, TShape>::Synthesize( ShapeType* shape, ImageType* image, VectorPointer texture )
{
	_i2vConverter -> FillUnderMask( texture );

	//do inverse warping
	typename WarpType::Pointer warper = WarpType::New();
	warper -> SetOutputParameters( image, shape );
	warper -> Precalculate(); 

	MirrorEdge();

	warper -> SetInputParameters( _referenceImage, _referenceShape );
	warper -> Update();
}



#endif
