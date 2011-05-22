/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

//Implementation file for the Independent Component Analysis.

#ifndef _blIA3DSampler_txx
#define _blIA3DSampler_txx

//#include "blIA3DSampler.h"

/** \brief Default Constructor
 *
 */
template <class TImage>
blIA3DSampler<TImage>::blIA3DSampler()
{
}

/** \brief Default Constructor
 *
 */
template <class TImage>
blIA3DSampler<TImage>::~blIA3DSampler()
{

}

///** \brief Set Normal direction
// *  
// */
////----------------------------------------------------
//template <class TImage>
//void blIA3DSampler<TImage>::SetNormal(PointType normal)
////----------------------------------------------------
//{	
//	this->mNormal = normal;
//}

///** \brief Set Point to sample around
// *  
// */
////----------------------------------------------------
//template <class TImage>
//void blIA3DSampler<TImage>::SetOrigin(PointType origin)
////----------------------------------------------------
//{
//	this->mOrigin = origin;
//}

/** \brief Set Sampler Geometry Type
 *  
 */
//----------------------------------------------------
template <class TImage>
void blIA3DSampler<TImage>::SetGeometryType (GeometryType geometryType)
//----------------------------------------------------
{
	this->mGeometryType = geometryType;
}

/** \brief Set Sampler Normalization Type
 *  
 */
//----------------------------------------------------
//template <class TImage>
//void blIA3DSampler<TImage>::SetNormalizationType (NormalizationType normalizationType)
//----------------------------------------------------
//{
//	this->mNormalizationType = normalizationType;
//}


#endif
