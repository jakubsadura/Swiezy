/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

//Image + gradient bands

#ifndef _blImageWithGradientBands_txx
#define _blImageWithGradientBands_txx

#include "blImageWithGradientBands.h"
#include <blIAFeaturesCalculator.h>

//#include <blImage.h>
#include <itkImageRegionIterator.h>
#include <blImageWriter.h>
#include <itkNeighborhoodIterator.h>
#include <itkGradientImageFilter.h>


/**
 * Constructor
 */
//--------------------------------------------------
template <class TImage>
blImageWithGradientBands<TImage>::blImageWithGradientBands()
{ 
	InitializeBands();
}



/**
 *	Destructor
 */
//--------------------------------------------------
template <class TImage>
blImageWithGradientBands<TImage>::~blImageWithGradientBands() 
//--------------------------------------------------
{ 
}


template <class TImage>
void blImageWithGradientBands<TImage>::LoadImageFromFilename( const char* filename )
{
	Superclass::LoadImageFromFilename( filename );

	SetBand( 0, this->GetBand(0), false, false );

	RecomputeBands();
}



template <class TImage>
void blImageWithGradientBands<TImage>::SaveImage( const char* filename )
{
	//SetBand(0, GetBand(1), CanSample(0), IsAutoBand(0) );

	//typedef blImage<unsigned char> ImageType;
	//typedef itk::ImageRegionIterator<ImageType> IntIterator;
	//typedef itk::ImageRegionIterator<TImage> FltIterator;

	//ImageType::Pointer im = ImageType::New();
	//im->SetRegions( GetBand(1)->GetLargestPossibleRegion() );
	//im->Allocate();

	//IntIterator int_it( im, im->GetLargestPossibleRegion() );
	//FltIterator flt_it( GetBand(1),  GetBand(1)->GetLargestPossibleRegion() );
	//for( int_it.GoToBegin(), flt_it.GoToBegin(); !int_it.IsAtEnd(); ++int_it, ++flt_it )
	//{
	//	int_it.Set( floor( (float)flt_it.Get() ) );
	//}

	//blImageWriter<ImageType>::Pointer wr = blImageWriter<ImageType>::New();
	//wr->SetFileName("d:\\hhh.png");
	//wr->SetInput( im );
	//wr->Update();

	Superclass::SaveImage( filename );
}



template <class TImage>
void blImageWithGradientBands<TImage>::RecomputeBands()
{
	typedef itk::GradientImageFilter<ImageType> GradientFilterType;
	typedef typename GradientFilterType::OutputImageType GradientImageType;
	typedef typename GradientFilterType::OutputPixelType GradientPixelType;

	typename GradientFilterType::Pointer grad_calc = GradientFilterType::New();
	grad_calc->SetUseImageSpacingOff();
	grad_calc->SetInput( this->GetBand(0) );
	grad_calc->Update();

	typename GradientImageType::Pointer grad = grad_calc->GetOutput();

	typename ImageType::Pointer fx = ImageType::New();
	typename ImageType::Pointer fy = ImageType::New();
	fx -> SetRegions( grad->GetLargestPossibleRegion() );
	fy -> SetRegions( grad->GetLargestPossibleRegion() );
	fx->Allocate();
	fy->Allocate();

	itk::ImageRegionIterator<GradientImageType> it_grad( grad, grad->GetLargestPossibleRegion() );
	itk::ImageRegionIterator<ImageType> it_fx( fx, grad->GetLargestPossibleRegion() );
	itk::ImageRegionIterator<ImageType> it_fy( fy, grad->GetLargestPossibleRegion() );

	//compute the mean gradient magnitude
	double mean_mag = 0;
	unsigned long int num_o_pixels = 0;
	for( it_grad.GoToBegin(); !it_grad.IsAtEnd(); ++it_grad )
	{
		mean_mag += it_grad.Get().Get_vnl_vector().magnitude();
		num_o_pixels++;
	}
	mean_mag /= num_o_pixels;
	
	//normalize
	for( it_grad.GoToBegin(), it_fx.GoToBegin(), it_fy.GoToBegin(); !it_grad.IsAtEnd(); ++it_grad, ++it_fx, ++it_fy )
	{
		const GradientPixelType grad_pix = it_grad.Get();
		const double norm = grad_pix.Get_vnl_vector().magnitude() + mean_mag;
		
		it_fx.Set( grad_pix[0] / norm );
		it_fy.Set( grad_pix[1] / norm );
	}



	SetBand(1, fx, true, true );
	SetBand(2, fy, true, true );


}




template <class TImage>
void blImageWithGradientBands<TImage>::InitializeBands()
{
	SetBand( 0, this->GetBand(0), false, false ); //image (will not be sampled)
	
	for( int i=1; i<this->GetNumberOfBands(); i++ )
		SetBand( i, this->GetBand(i), true, false ); //gradient band
}






#endif


