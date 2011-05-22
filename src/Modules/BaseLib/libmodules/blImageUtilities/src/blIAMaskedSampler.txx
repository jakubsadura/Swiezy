/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

//Implementation file for the masked sampler.

#ifndef __blIAMaskedSampler_txx
#define __blIAMaskedSampler_txx

//#include "blIAMaskedSampler.h"


template <class TImage, class TPixel, unsigned int VImageDimension>
blIAMaskedSampler<TImage, TPixel, VImageDimension>::blIAMaskedSampler():
	_nonMaskedPixelValue(1)
{
};

template <class TImage, class TPixel, unsigned int VImageDimension>
blIAMaskedSampler<TImage, TPixel, VImageDimension>::~blIAMaskedSampler() 
{
};




#endif
