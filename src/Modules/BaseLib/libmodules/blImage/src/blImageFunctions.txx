/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

//basic functions for images

#ifndef _blImageFunctions_txx
#define _blImageFunctions_txx

#include <memory.h>

/**
 *  Fill image with zeroes
 */
//--------------------------------------------------
template <class TInputImage>
void ZeroImage( typename TInputImage::Pointer image )
//--------------------------------------------------
{ 
	memset(image->GetBufferPointer(), 0, 
			image->GetBufferedRegion().GetNumberOfPixels() * sizeof(typename TInputImage::PixelType) );
}




/** \brief Plot a line */
template <class TInputImage>
void Line(	typename TInputImage::Pointer image, 
			const typename TInputImage::IndexType& pt1, 
			const typename TInputImage::IndexType& pt2, 
			const typename TInputImage::PixelType color)
{
	if( TInputImage::ImageDimension != 2 ) return;
	typename TInputImage::IndexType pt = pt1;
	
    // Algorithm ripped wholeheartedly from B. Stroustrup's _The C++
    // Programming Language_, Second Edition.  Page 195.
    //
    register int dx  = 1;
    int a = pt2[0] - pt[0];
    if (a < 0) dx = -1, a = -a;

    register int dy  = 1;
    int b = pt2[1] - pt[1];
    if (b < 0) dy = -1, b = -b;

    int two_a = 2 * a, two_b = 2 * b;
    int xcrit = -b + two_a;

    register int eps = 0;

    for ( ; ; ) {
      image->SetPixel(pt, color);
      if (pt[0] == pt2[0] && pt[1] == pt2[1]) break;
      if (eps <= xcrit) pt[0] += dx, eps += two_b;
      if (eps >= a || a <= b) pt[1] += dy, eps -= two_a;
    }
 }



/**
 */
template <class TInputImage>
void Line(	typename TInputImage::Pointer image, 
			const typename TInputImage::IndexType& pt1, 
			const typename TInputImage::IndexType& pt2, 
			const typename TInputImage::PixelType color,
			const int iSize )
{
	if( TInputImage::ImageDimension != 2 ) return;
	typename TInputImage::IndexType pt = pt1;

    // Algorithm ripped wholeheartedly from B. Stroustrup's _The C++
    // Programming Language_, Second Edition.  Page 195.
    //
    register int dx  = 1;
    int a = pt2[0] - pt[0];
    if (a < 0) dx = -1, a = -a;

    register int dy  = 1;
    int b = pt2[1] - pt[1];
    if (b < 0) dy = -1, b = -b;

    int two_a = 2 * a, two_b = 2 * b;
    int xcrit = -b + two_a;

    register int eps = 0;

	for ( ; ; ) {

		typename TInputImage::IndexType ptAux;
		for ( int x = -iSize ; x < iSize ; x++ )
		{
			for ( int y = -iSize ; y < iSize ; y++ )
			{
				ptAux[ 0 ] = pt[ 0 ] + x;
				ptAux[ 1 ] = pt[ 1 ] + y;
				image->SetPixel(ptAux, color);
			}
		}

		if (pt[0] == pt2[0] && pt[1] == pt2[1]) break;
		if (eps <= xcrit) pt[0] += dx, eps += two_b;
		if (eps >= a || a <= b) pt[1] += dy, eps -= two_a;
	}
	
 }


#endif


