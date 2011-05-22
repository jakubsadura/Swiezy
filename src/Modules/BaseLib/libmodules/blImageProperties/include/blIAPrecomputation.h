/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#ifndef __blIAPrecomputation_h
#define __blIAPrecomputation_h

#include "blPDShape.h"

///#include "blIADefines.h"

#include "blLightObject.h"
#include "blSmartPointer.h"
#include "blMacro.h"



/** 
 * \brief This class implements computation of all features of IA required 
 *	on a given Hierarchy.
 * \ingroup blImageProperties
 *
 */
class blIAPrecomputation: public blLightObject
{
public:

	typedef blIAPrecomputation Self;
	typedef blSmartPointer<Self> Pointer;

	blNewMacro(Self);


	/** \brief enum type for the properties that can be calculated
	 */
	enum propertiesType {GRAD, Lx, Ly, Lz};

	/** \brief Set Resolution Schedule
	 *
	 *  At a first sight depends on 2D or 3D instantiation.
	 */
	void SetResolutionSchedule(int **resolutionSchedule, 
								int numberOfResolutions, 
								int numberOfDimensions);
	
	/** \brief Set Scale Schedule
	 *
	 *  scaleSchedule applies to each dimension so it's not be affected 
	 *  by 2D or 3D
	 */
	void SetScaleSchedule(int *scaleSchedule, int numberOfResolutions);
	
	/** \brief Set properties requested
	 *
	 *  an enum Type have all properties available defined.
	 *	
	 */
	void SetPropertiesRequested(propertiesType *requested, int numberOfPropertiesRequested);
	
	/** \brief Computes the HYPERMATRIX with ALL DATA.
	 *
	 *	What Update does is:
	 *		resize this->hyperMatrix
	 *		for i = everyImageInTheSet
	 *			for r = everyResolution
	 *				for s = everyScale
	 *					for p = everyProperty
	 *						this->hyperMatrix[i, r, s, p] = 
	 *							accum(this->hyperMatrix[i,r,s,p], 
	 *									computeProperty(p, image[i, r, s] ) );
	 */
	void Update();



protected:
	blIAPrecomputation();
	virtual ~blIAPrecomputation();

private:
	blIAPrecomputation(const Self&); //purposely not implemented
	void operator=(const Self&); //purposely not implemented

	blPDShapeInterface::Pointer shape;

//	vnl_matrix<blImage> *hyperMatrix; // here goes all accum info.
	
};

#endif


