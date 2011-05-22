/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __blHomogeneous3DTransform_h
#define __blHomogeneous3DTransform_h

#include <vnl/vnl_matrix.h>
#include <vnl/vnl_vector.h>

#include "blLightObject.h"
#include "blSmartPointer.h"
#include "blMacro.h"


#define DEBUG_MESSAGES_blHOMOGENEOUS3DTRANSFORM

/**
 * \brief This class defines a homogeneous transformation for 3D points
 * \ingroup blPDAlignment
 *
 *	The transformation is defined by a 4x4 matrix.
 *
 */
class blHomogeneous3DTransform: public blLightObject
{
public:
	typedef blHomogeneous3DTransform Self;
	typedef blSmartPointer<Self> Pointer;

	blNewMacro(Self);

	/**\brief sets matrix that defines a new transform */
		vnl_matrix<double> & GetMatrix();

	/**\brief sets matrix that defines a new transform */
		void SetMatrix(const vnl_matrix<double> & matrix4x4);

	/**\brief applies the transform on the points */
	void TransformPoints(const vnl_vector<double> & points,
							vnl_vector<double> & outputPoints,
							unsigned int numberOfPoints) const;

	/**\brief applies the transform on the points */
	void TransformPoints(const double points[],
							double outputPoints[],
							unsigned int numberOfPoints) const;

	/**\brief applies the transform in the given points */
	void TransformPoints( vnl_vector<double> & points, 
							unsigned int numberOfPoints) const;

	/**\brief creates the inverse transformation */
	blHomogeneous3DTransform::Pointer Inverse();

protected:
	/**\brief Constructor */
	blHomogeneous3DTransform();

	/**\brief Destructor */
	virtual ~blHomogeneous3DTransform();

private:
	blHomogeneous3DTransform(const Self&); //purposely not implemented
	void operator=(const Self&); //purposely not implemented
		
		vnl_matrix<double> matrix;	//!< transformation matrix for 3D

};

#endif
