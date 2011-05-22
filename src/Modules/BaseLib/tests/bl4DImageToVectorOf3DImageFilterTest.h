/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#ifndef BASELIBITKTESTRUNNER_H
#define BASELIBITKTESTRUNNER_H

#include <cxxtest/TestSuite.h>
#include "bl4DImageToVectorOf3DImageFilter.h"
#include "itkImage.h"
#include "itkImageRegionIterator.h"


/**
\brief The unit test contains a single test for blImage4DToVectorOfImage3DFilter.
\ingroup BaseLibITKTestRunner
 */
class bl4DImageToVectorOf3DImageFilterTest : public CxxTest::TestSuite 
{
public:
	void testFilter()
	{
		typedef itk::Image<unsigned short,4> ImageType4D;
		typedef itk::Image<unsigned short,3> ImageType3D;
		ImageType4D::Pointer image = ImageType4D::New();
		const int nrTimePoints = 3;
		const int planeSize1 = 16;
		const int planeSize2 = 32;
		const int nrSlices = 8;
		ImageType4D::SizeType size4D = {planeSize1, planeSize2, nrSlices, nrTimePoints};
		ImageType4D::IndexType start = {0, 0, 0, 0};
		ImageType4D::RegionType region4D;
		region4D.SetSize(size4D);
		region4D.SetIndex(start);
		image->SetRegions(region4D);
		image->Allocate();

		typedef itk::ImageRegionIterator<ImageType4D> IteratorType;
		IteratorType it( image, image->GetLargestPossibleRegion() );

		int iPixel = 0;
		for( it.Begin(); !it.IsAtEnd(); ++it, ++iPixel)
		{
			unsigned short value = 50 + 100 * ((iPixel) / (planeSize1 * planeSize2 * nrSlices));
			it.Set(value);
		}

		typedef blImage4DToVectorOfImage3DFilter< ImageType4D, ImageType3D > ImageToImageFilter;
		ImageToImageFilter::Pointer image4DToImage3D = ImageToImageFilter::New();
		image4DToImage3D->SetInput(image);
		image4DToImage3D->Update();

		ImageToImageFilter::ImageCollectionType* result = image4DToImage3D->GetOutput();
		TS_ASSERT( result->images.size() == nrTimePoints );
		for( int i = 0; i < result->images.size(); ++i )
		{
			ImageType3D* resultImage = result->images[i];
			TS_ASSERT( resultImage->GetLargestPossibleRegion().GetSize()[0] == planeSize1 );
			TS_ASSERT( resultImage->GetLargestPossibleRegion().GetSize()[1] == planeSize2 );
			TS_ASSERT( resultImage->GetLargestPossibleRegion().GetSize()[2] == nrSlices );
			unsigned short value = 50 + 100 * i;
			typedef itk::ImageRegionIterator<ImageType3D> Iterator3DType;
			Iterator3DType it( resultImage, resultImage->GetLargestPossibleRegion() );
			for( it.Begin(); !it.IsAtEnd(); ++it )
			{
				TS_ASSERT( it.Get() == value );
			}
		}
	}
};

#endif //BASELIBITKTESTRUNNER_H
