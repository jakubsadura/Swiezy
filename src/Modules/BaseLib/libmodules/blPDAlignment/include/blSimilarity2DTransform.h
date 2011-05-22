/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __blSimilarity2DTransform_h
#define __blSimilarity2DTransform_h


#include "vnl/vnl_vector.h"

#include "blLightObject.h"
#include "blSmartPointer.h"
#include "blMacro.h"


/**
 * \brief This class defines a similarity transform for 2D points
 * \ingroup blPDAlignment
 *
 *	The similarity transform is defined by params: 
 *
 *	- a, b (elements of the rotation-scaling matrix), 
 *	- tx, ty (translation on x and y axis).
 *
 *	Thus the transform is given by:
 *
 *		x' = [(a -b)(b a)] * x + (tx ty)
 *
 *	a = s * cos(RHO), b = s * sin(RHO)
 *	
 *	The center of rotation is always the origin (0,0).
 */
class blSimilarity2DTransform: public blLightObject
{

	public:

		typedef blSimilarity2DTransform Self;
		typedef blSmartPointer<Self> Pointer;

		blNewMacro(Self);

		
		/**\brief sets new params that defines a new transform */
		void SetParameters(double a, double b, double tx, double ty);

		/**\brief applies the transform on the points */
		void TransformPoints(const vnl_vector<double> & points,
								vnl_vector<double> & outputPoints,
								unsigned int numberOfPoints);

		void TransformPoints( vnl_vector<double> & points );

		/**\brief applies the inverse transform on the points */
		void InverseTransformPoints(const vnl_vector<double> & points,
									vnl_vector<double> & outputPoints,
									unsigned int numberOfPoints);

		void InverseTransformPoints( vnl_vector<double> & points );

	protected:

		/**\brief Constructor */
		blSimilarity2DTransform();

		/**\brief Destructor */
		virtual ~blSimilarity2DTransform();

	private:

		blSimilarity2DTransform(const Self&); //purposely not implemented
		void operator=(const Self&); //purposely not implemented
		
		double a;	//!< element a of the rotation-scaling matrix
		double b;	//!< element b of the rotation-scaling matrix
		double tx;	//!< translation on x-axis
		double ty;	//!< translation on y-axis
};

#endif
