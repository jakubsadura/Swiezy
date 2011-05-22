/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#ifndef __blNeighborhoodImageProduct_txx
#define __blNeighborhoodImageProduct_txx

#include <iostream>
#include <itkConstNeighborhoodIterator.h>
#include <itkConstantBoundaryCondition.h>

//#include "blNeighborhoodImageProduct.h"




//-----------------------------------------------------------
template <class TImage, class TNeighborhoodMask>
blNeighborhoodImageProduct<TImage, TNeighborhoodMask>::
blNeighborhoodImageProduct()
//-----------------------------------------------------------
{
}




/**
 *	Destructor
 */
//-----------------------------------------------------------
template <class TImage, class TNeighborhoodMask>
blNeighborhoodImageProduct<TImage, TNeighborhoodMask>::
~blNeighborhoodImageProduct() 
//-----------------------------------------------------------
{

}


/**
 *
 */
//-----------------------------------------------------------
template <class TImage, class TNeighborhoodMask>
void blNeighborhoodImageProduct<TImage, TNeighborhoodMask>::
SetNeighborhoodMask(TNeighborhoodMask * neighborhoodMask)
//-----------------------------------------------------------
{
	this->neighborhoodMask = neighborhoodMask;

	// update size of output vector
	// ---------------------------------
	typename NeighborhoodMaskType::SizeType size = this->neighborhoodMask->GetSize();

	unsigned int outputSize = 1;

	int d = 0;
	for (d = 0; d < ImageType::ImageDimension; d++)
	{
		outputSize *= size[d];
	}

#ifdef DEBUG_MESSAGES_bl_NEIGHBORHOOD_IMAGE_PRODUCT
	std::cout <<"blNeigborhoodImageProduct::SetNeighborhoodMask(): output vector size:" 
				<< outputSize << std::endl;
#endif
	
	if (this->outputVector.size() != outputSize)
	{
		this->outputVector.set_size(outputSize);
	}

};



/**
 *	Calculate product of each pixel from the image and each pixel from the mask.
 */
//-----------------------------------------------------------
template <class TImage, class TNeighborhoodMask>
void blNeighborhoodImageProduct<TImage, TNeighborhoodMask>::Update()
//-----------------------------------------------------------
{

#ifdef DEBUG_MESSAGES_bl_NEIGHBORHOOD_IMAGE_PRODUCT
	std::cout << "blNeigborhoodImageProduct::Update(): index " 
				<< this->index[0] << "," << this->index[1] << std::endl;

	std::cout <<"blNeigborhoodImageProduct::Update(): mask radius: ";
//	std::cout << radius[0] << "," << radius[1] << std::endl;
#endif

	
	this->neighborhoodIterator.Initialize(this->neighborhoodMask->GetRadius(),
											this->inputImage.GetPointer(), 
											this->inputImage->GetLargestPossibleRegion());

	this->neighborhoodIterator.ResetBoundaryCondition(); // important to do this before initialize

	
	// locate the neighborhood iterator at given index
	this->neighborhoodIterator.SetLocation(this->index);



	// iterate over the mask (neighborhood)
	typename NeighborhoodMaskType::Iterator maskIter = this->neighborhoodMask->Begin();

	vnl_vector<double>::iterator outputIter = this->outputVector.begin();


#ifdef DEBUG_MESSAGES_bl_NEIGHBORHOOD_IMAGE_PRODUCT		
	int index = 0;
#endif

	int i = 0;

	while (	maskIter != this->neighborhoodMask->End() )
	{
		*outputIter = *maskIter * 
						((double) this->neighborhoodIterator.GetPixel(i));

#ifdef DEBUG_MESSAGES_bl_NEIGHBORHOOD_IMAGE_PRODUCT		
		std::cout << std::endl;
		std::cout <<"blNeigborhoodImageProduct::Update(): index: " << 
					index << std::endl;
		std::cout << "blNeigborhoodImageProduct::Update(): imageValue: " 
			<< (double) this->neighborhoodIterator.GetPixel(i) << std::endl;

		std::cout << "blNeigborhoodImageProduct::Update(): maskValue: " 
			<< ((double) *maskIter )  << std::endl;

		std::cout << "blNeigborhoodImageProduct::Update(): outputValue: " 
			<< ((double) *outputIter )  << std::endl;
#endif
		
		i++;

		maskIter++;
		outputIter++;

#ifdef DEBUG_MESSAGES_bl_NEIGHBORHOOD_IMAGE_PRODUCT		
		index++;
#endif

	}

}



#endif
