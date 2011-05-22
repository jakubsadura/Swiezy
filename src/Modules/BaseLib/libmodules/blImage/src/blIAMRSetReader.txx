/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


//Constructs set of images and allows reading at different resoutions

#ifndef _blIAMRSetReader_txx
#define _blIAMRSetReader_txx

#include "blIAMRSetReader.h"


/** 
 *	This method loads the path to the images
 *
 */
//--------------------------------------------------
template <class TImage>
blIAMRSetReader<TImage>::blIAMRSetReader():
	_resolution(1)
//--------------------------------------------------
{

}



/** 
 *	The Destructor.
 */
//--------------------------------------------------
template <class TImage>
blIAMRSetReader<TImage>::~blIAMRSetReader()
//--------------------------------------------------
{

}



#endif
