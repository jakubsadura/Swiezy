/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#ifndef _blIASampler_txx
#define _blIASampler_txx

//#include "blIASampler.h"

/** \brief Default Constructor
 *
 */
template <class TImage>
blIASampler<TImage>::blIASampler()
{
}

/** \brief Default Constructor
 *
 */
template <class TImage>
blIASampler<TImage>::~blIASampler()
{

}




/** \brief Set Normal direction
 *  
 */
//----------------------------------------------------
template <class TImage>
void blIASampler<TImage>::SetNormal(PointType point)
//----------------------------------------------------
{	
	this->xNormal = point[0];
	this->yNormal = point[1];
}

	

/** \brief Set Point to sample around
 *  
 */
//----------------------------------------------------
template <class TImage>
void blIASampler<TImage>::SetPoint(PointType point)
//----------------------------------------------------
{
	this->xPoint = point[0];
	this->yPoint = point[1];
}

#endif
