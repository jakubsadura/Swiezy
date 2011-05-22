/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef blSliceImage_txx
#define blSliceImage_txx

#include "blSliceImage.h"
#include "itkCastImageFilter.h"


template< class TPixel >
Slice< TPixel > blSliceImage::GetSliceImage( int i) const
{
	Slice<TPixel> slice;
	slice.orientation = m_slices.at(i).orientation;
	slice.position = m_slices.at(i).position;

	if ( typeid( TPixel ) != typeid( double ) )
	{
		typedef itk::Image< TPixel, 2 > ImageType;
		typedef itk::Image< double, 2 > InternalImageType;
		typedef itk::CastImageFilter< InternalImageType, ImageType > CastFilter;
		typename CastFilter::Pointer filter  = CastFilter::New();
		filter->SetInput( m_slices.at(i).image );
		filter->Update();
		slice.image = filter->GetOutput( );
	}
	else
	{
		slice.image = m_slices.at(i).image;
	}

	return slice;
}

template< class TPixel >
std::vector< Slice< TPixel > > blSliceImage::GetData() const
{
	std::vector< Slice < TPixel > > sliceVect;
	sliceVect.resize(m_slices.size());
	for (size_t i=0; i < m_slices.size(); i++)
		sliceVect.at(i) = GetSliceImage< TPixel >(i);
	return sliceVect;
}

template< class TPixel>
void blSliceImage::SetInternalImage( typename itk::Image<TPixel,2>::Pointer slice, int i)
{
	typedef itk::Image< TPixel, 2 > ImageType;
	typedef itk::Image< double, 2 > InternalImageType;
	typedef itk::CastImageFilter< ImageType, InternalImageType > CastFilter;
	typename CastFilter::Pointer filter  = CastFilter::New();
	filter->SetInput( slice );
	filter->Update();
	m_slices.at(i).image = filter->GetOutput();
	m_slices.at(i).pixelTypeAsString = "double";
}


#endif //blSliceImage_txx
