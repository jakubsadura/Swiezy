/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __blAAMVisualizer_txx
#define __blAAMVisualizer_txx

//#include "blAAMVisualizer.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkImage.h" //needed for neighborhood iterator
#include "blIAImageVectorConverter.h"
#include "blMath.h"
#include "blImageFunctions.h"

#include "blImageWriter.h"

/**
 *	Constructor
 */
template <class TInImage, class TOutImage, class TShape>
blAAMVisualizer<TInImage, TOutImage, TShape>::blAAMVisualizer():
	_renderWireframe(false),
	_renderPatch(true),
	_meanShapeChanged(true),
	_textureRenderType( LinearStretch ),
	_wireframeColor( (typename OutputImageType::PixelType) -1 ),
	_shapeMeanSize(0),
	_mirroredEdge(1)
{
};

/**
 *	Destructor
 */
template <class TInImage, class TOutImage, class TShape>
blAAMVisualizer<TInImage, TOutImage, TShape>::~blAAMVisualizer() 
{
};
	




template <class TInImage, class TOutImage, class TShape>
void blAAMVisualizer<TInImage, TOutImage, TShape>::Update()
{
		const int numberOfLandmarks = _meanShape->GetNumberOfLandmarks();
		const int shapeDimensions = _meanShape->GetDimension();
		const int shapeLength = numberOfLandmarks * shapeDimensions;

		GenerateReferenceFrame();

		//typedef itk::ImageRegionIterator<InputImageType::ImageType> ImageIterator;
		//typedef itk::RescaleIntensityImageFilter<InputImageType, OutputImageType> RescaleIntensityType;
		
		typedef blIAImageVectorConverter<InputImageType, MaskType> ConverterType;
	

		typename InputImageType::Pointer image = InputImageType::New();

	
		//--------------------------------------------
		//
		// copy image 
		//
		//--------------------------------------------

		//caluclate the bounding box of the output shape
		const BoundingBoxType* BoundingBox = _outputShape->GetMeshBoundingBox();
		itk::FixedArray<double,4> Bounds = BoundingBox->GetBounds(); // 4 = Dimensions * 2
	
		typename InputImageType::IndexType top_left, bottom_right;
		for( int i=0; i<2; i++ ) // 2 = Dimensions
		{
			top_left[i] = (long)Bounds[i*2]; 
			bottom_right[i] = (long)Bounds[i*2+1]; //set to maximums
		}


		// Allocate the image
		if( !_bkImage.IsNull() )
		{
			for( int i=0; i<_bkImage->GetNumberOfBands(); i++ )
			{
				typename InputImageType::ImageType::Pointer im = InputImageType::ImageType::New();
				im->SetRegions( _bkImage->GetBand(i)->GetLargestPossibleRegion() );
				im->Allocate();

				// Do the copy
				memcpy( im->GetBufferPointer(), _bkImage->GetBand(i)->GetBufferPointer(),
									_bkImage->GetBand(0)->GetLargestPossibleRegion().GetNumberOfPixels()*sizeof(typename InputImageType::PixelType));

				image->SetBand( i, im, _bkImage->CanSample(i), _bkImage->IsAutoBand(i) );
			}
	
		}
		else
		{
			typename InputImageType::SizeType size;
			for( int i=0; i<2; i++ ) // 2 = Dimensions
			{
				size[i] = (long) ceil(Bounds[i*2+1]); //set to maximums
			}
		
			typename InputImageType::IndexType index;
			typename InputImageType::RegionType region;
			
			index[0] = 0;
			index[1] = 0;
			region.SetIndex(index);
			region.SetSize(size);
		
			image -> SetRegions( region );

			for( int i=0; i<image->GetNumberOfBands(); i++ )
				image -> GetBand(i)->FillBuffer(0);
		}
	

		//-------------------------------------------------------------------------
		//
		//   check that image size is sufficient to render model
		//
		//-------------------------------------------------------------------------
		
		if( ! ( image->GetBand(0)->GetLargestPossibleRegion().IsInside(top_left) && image->GetBand(0)->GetLargestPossibleRegion().IsInside(bottom_right) ) )
		{
			std::cout<<"RenderOutputToImage: Unable to render texture, shape is beyond the image"<<std::endl;
			return;
		}
	
	
		//-------------------------------------------------------------------------
		//
		//   continue
		//
		//-------------------------------------------------------------------------
		typename ConverterType::Pointer i2vConverter = ConverterType::New();

		//set the dynamic range of the output texture
		double text_min = (typename OutputImageType::PixelType)0;
		double text_max = (typename OutputImageType::PixelType)-1;
		
		if ( _renderPatch )
		{

			//make a copy before modifying
			VectorPointer texture = VectorNamespace::New( _texture->size() );
			texture -> copy_in( _texture->begin() );

			//copy triangulation information
			//shape->SetTesselationCells( _referenceShape->GetTesselationCells() );
		

			switch( _textureRenderType )
			{
				case SampledStatistics:	
					{
						//sample the texture from the image to get variance and mean
						VectorPointer sampledTexture = VectorNamespace::New( texture->size() );
						_referenceFrame -> SampleShape( _outputShape, _bkImage, sampledTexture.get() );
						
						blMath::AlignVectors( texture->begin(), sampledTexture->begin(), texture->size() );
				
						for( int iii=0; iii<texture->size(); iii++) 
							(*texture)[iii] = __limit( (*texture)[iii], text_min, text_max ); 
					};

					break;

				case LinearStretch:

					//update texture dynamic range from the sampled texture
					VectorNamespace::LinearStretchMinMax( *texture, text_min, text_max );
					
					break;
					
				default:
					throw "blAAMVisualizer<>::Update(): incorrect RenderType. Aborting.";
			};
				
		
			//synthesize image from the texture
			_referenceFrame->Synthesize( _outputShape, image, texture );
		
		}
		if (_renderWireframe) //draw wireframe from shape_pts
		{
			typename InputImageType::IndexType ptc, pt1, pt2;
	
			typename ShapeType::ShapeCellsContainer lines;
			typename ShapeType::ShapeCellsContainer polys;
			typename ShapeType::PointIdsContainer ptIDs;
		
			_meanShape->GetPolylines(lines);
			_meanShape->GetPolygons(polys);
	
			VectorPointer shape_pts = VectorNamespace::New( shapeLength );
			_outputShape->GetPoints( shape_pts->begin() );

			VectorOf2DContinuousPoints akPoint=(VectorOf2DContinuousPoints)shape_pts->begin();
	
			for( int visual_band = 0; visual_band<image->GetNumberOfBands(); visual_band++ )
			{
				//draw
				if( lines.size()>0 || polys.size()>0 ) //if there is connectivity information - use it
				{ 
					for( int i=0; i<lines.size(); i++)
					{
						ptIDs = lines[i];
						pt1[0] = (long) akPoint[ptIDs[0]].X();
						pt1[1] = (long) akPoint[ptIDs[0]].Y();
						
						for( int j = 1; j<ptIDs.size(); j++ )
						{
							pt2[0] = (long) akPoint[ptIDs[j]].X();
							pt2[1] = (long) akPoint[ptIDs[j]].Y();
							
							Line< typename InputImageType::ImageType >(image->GetBand(visual_band), pt1, pt2, _wireframeColor);
				
							pt1 = pt2;
						}				
					}
		
					for( int i=0; i<polys.size(); i++)
					{
						ptIDs = polys[i];
						pt1[0] = (long) akPoint[ptIDs[0]].X();
						pt1[1] = (long) akPoint[ptIDs[0]].Y();
						
						for( int j = 1; j<ptIDs.size(); j++ )
						{
							pt2[0] = (long) akPoint[ptIDs[j]].X();
							pt2[1] = (long) akPoint[ptIDs[j]].Y();
							
							Line< typename InputImageType::ImageType >( image->GetBand(visual_band), pt1, pt2, _wireframeColor);
				
							pt1 = pt2;
						}				
		
						//close the poly
						pt2[0] = (long) akPoint[ptIDs[0]].X();
						pt2[1] = (long) akPoint[ptIDs[0]].Y();
						Line< typename InputImageType::ImageType >(image->GetBand(visual_band), pt1, pt2, _wireframeColor);
					}
				}
				else //if there is no connectivity information, just plot the crosses at points
				{
					for( int i=0; i<shape_pts->size();)
					{
						ptc[0] = (long)(*shape_pts)[i++];
						ptc[1] = (long)(*shape_pts)[i++];
		
						pt1[0] = ptc[0] - 1;
						pt1[1] = ptc[1];
		
						pt2[0] = ptc[0] + 1;
						pt2[1] = ptc[1];
		
						Line< typename InputImageType::ImageType >(image->GetBand(visual_band), pt1, pt2, _wireframeColor);
		
						pt1[0] = ptc[0];
						pt1[1] = ptc[1] - 1;
		
						pt2[0] = ptc[0];
						pt2[1] = ptc[1] + 1;
		
						Line< typename InputImageType::ImageType >(image->GetBand(visual_band), pt1, pt2, _wireframeColor);
					}
				}
			}
		}	
		
		//change pixeltype
		_outputImage = OutputImageType::New();

		for( int i=0; i< image->GetNumberOfBands(); i++ )
		{
			typename OutputImageType::ImageType::Pointer im = OutputImageType::ImageType::New();

			im->SetRegions( image->GetBand(0)->GetLargestPossibleRegion() );
			im->Allocate();

			itk::ImageRegionIterator<typename InputImageType::ImageType> itIn( image->GetBand(i), image->GetBand(0)->GetLargestPossibleRegion() );
			itk::ImageRegionIterator<typename OutputImageType::ImageType> itOut( im, image->GetBand(0)->GetLargestPossibleRegion() );
			for( itIn.GoToBegin(), itOut.GoToBegin(); !itIn.IsAtEnd(); ++itIn, ++itOut )
				itOut.Set( __limit( int( itIn.Get()+0.5 ), 0, (typename OutputImageType::PixelType)-1 ) );

			_outputImage->SetBand( i, im, image->CanSample(i), image->IsAutoBand(i) );
		}
};











template <class TInImage, class TOutImage, class TShape>
void blAAMVisualizer<TInImage, TOutImage, TShape>::GenerateReferenceFrame()
{
	if (_meanShapeChanged)
	{
		_referenceFrame = ReferenceFrameType::New();
		_referenceFrame->Setup( _meanShape, _shapeMeanSize, _mirroredEdge );

		_meanShapeChanged = false;
	}	
}



template <class TInImage, class TOutImage, class TShape>
void blAAMVisualizer<TInImage, TOutImage, TShape>::UpdateAndSave( const char* filename )
{
	Update();
	
	//write output image to a file
	//typename blImageWriter<OutputImageType>::Pointer writer = blImageWriter<OutputImageType>::New();
	//writer->SetInput ( _outputImage );
	//writer->SetFileName( filename );
	//writer->Update();
	if ( _outputImage.IsNotNull( ) ){
		_outputImage->SaveImage( filename );
	}
}






#endif
