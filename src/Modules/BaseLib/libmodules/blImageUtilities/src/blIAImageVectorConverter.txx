/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

//Class for extractin image pixels to a vector and back based upon a mask implementation
#ifndef __blIAImageVectorConverter_txx
#define __blIAImageVectorConverter_txx

//#include "blIAImageVectorConverter.h"

#include <math.h>

#include <stdexcept>

template <class TImage, class TMask>
blIAImageVectorConverter<TImage, TMask>::blIAImageVectorConverter():
	_roundUp(rtRound), 
	_performRoundUp(false),
	_vectorLength(0)
{
};

template <class TImage, class TMask>
blIAImageVectorConverter<TImage, TMask>::~blIAImageVectorConverter() 
{
};

template <class TImage, class TMask>
void blIAImageVectorConverter<TImage, TMask>::EstimateNumberOfPixels()  
{
	MaskRegionType region;	
	
	if ( _mask->GetMaskRegion() != NULL ) region = *_mask->GetMaskRegion();
	else region = _mask->GetLargestPossibleRegion();

	MaskIteratorType maskIt( _mask, region );

	int count = 0;
	for ( maskIt.GoToBegin(); !maskIt.IsAtEnd(); ++maskIt)
	{
		if ( maskIt.Get() != 0 ) count++;
	}

	_vectorLength = count * TImage::NumberOfBands;
};



#endif
