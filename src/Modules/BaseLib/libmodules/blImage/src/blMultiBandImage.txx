/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

//Multi-Band image base class

#ifndef _blMultiBandImage_txx
#define _blMultiBandImage_txx

#include <blMultiBandImage.h>

/**
 * Constructor
 */
//--------------------------------------------------
template <class TImage, unsigned int NBands>
blMultiBandImage<TImage, NBands>::blMultiBandImage():
	_resolution(1)
{ 
	_bands.resize( NBands );
	_sampling_bands.resize( NBands );
	_auto_bands.resize( NBands );

	for( int i=0; i<NBands; i++ )
	{
		_bands[i] = ImageType::New();
		_sampling_bands[i] = true;
		_auto_bands[i] = false;
	}
};



/**
 *	Destructor
 */
//--------------------------------------------------
template <class TImage, unsigned int NBands>
blMultiBandImage<TImage, NBands>::~blMultiBandImage() 
//--------------------------------------------------
{ 
};





template <class TImage, unsigned int NBands>
void blMultiBandImage<TImage, NBands>::SetRegions( RegionType region )
{
	for( int i=0; i<NBands; i++ )
	{
		_bands[i]->SetRegions( region );
		_bands[i]->Allocate();
	}
}





#endif


