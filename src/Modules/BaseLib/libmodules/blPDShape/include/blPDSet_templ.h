/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef BLPDSET_TEMPL_H
#define BLPDSET_TEMPL_H

#include <vnl/vnl_matrix.h>
#include "blPDShapeInterface.h"
#include "blLightObject.h"
#include "blSmartPointer.h"
#include "blMacro.h"
#include "blLinearAlgebraOperations.h"

#include "blMath.h"
#include "blUblasConvertibleArray.h"
#include <blVector.h>
#include "blLinearAlgebraTypes.h"

/**
 * \brief This class defines a Point Distribution Set
 * \ingroup blPDShape
 *
 *	This class defines a Point Distribution Set that contains 
 *	a number of blPDShapes.
 */
template< class ArgMatrixType >
class blPDSet_templ: public blLightObject
{
public:
	typedef blPDSet_templ Self;
	typedef blSmartPointer<Self> Pointer;

	typedef ArgMatrixType MatrixType;
	typedef typename bllao::Namespace<MatrixType> MatrixNamespace;
	typedef typename MatrixNamespace::Pointer MatrixPointer;
	typedef baselib::VnlVectorType VectorType;

	typedef typename blPDSet_templ<ArgMatrixType>::Pointer blPDSet_templPointer;



	blNewMacro(Self);

	/** \brief Constructor with PShapes */
	void SetParameters(unsigned int numberOfShapes, blPDShapeInterface::Pointer shapes[]);  

	void SetParameters(MatrixPointer m_ptr);
	
	/** \brief return the dimension of PDShape points */
	unsigned int GetDimension() const
		{ return this->numberDimensions; };

	/** \brief return the number of landmarks in a PDShape 
		*	This method returns the number of landmarks in a PDShape.
		*	It is supposed that all the PDShapes have the same number of landmarks,
		*	since they represent the same 'shape'
		*/
	unsigned int GetNumberOfLandmarks() const
		{ return this->numberLandmarks; };

	/** \brief return the number of PDShapes in the PDSet */
	unsigned int GetNumberOfShapes() const
		{ return this->numberShapes; };		

	/** \brief return the point coordinates of all the PDShapes */
	void GetPoints(double ** points);		

	/** \brief return the point coordinates of all the PDShapes */
	void GetPoints(vnl_matrix<double> &points);

	/** \brief return the point coordinates of all the PDShapes */
	template< class T, class L, class A >
	void GetPoints(Ublas::matrix<T, L, A>& points)
	{	
		for (unsigned int i = 0; i < numberShapes; i++)
		{			
			vnl_vector<double> row(bllao::NrCols(points));
			this->shapes[i]->GetPoints(row);
			bllao::CopyVectorToRow(points, i, row);
		}	
	}

	/** \brief return the point coordinates of all the PDShapes, 
		for the points that belong to the specified landmark group */
	void GetPoints(blLandmarkGroup * landmarkGroup, double ** points);

	/** \brief return a PDShape of the set 
		* \param shapeIndex the index of the specified PDShape in the PDSet.
		*/
	blPDShapeInterface::Pointer GetShape(unsigned int shapeIndex) const
	{
        return this->shapes[shapeIndex];
	}
			

	blPDSet_templPointer GetCopy()
	{
		Pointer result = New();

		result->numberDimensions = numberDimensions;
		result->numberLandmarks = numberLandmarks;
		result->numberShapes = numberShapes;

		//copy shapes
		result->shapes = new blPDShapeInterface::Pointer [numberShapes];


		for (unsigned int i=0; i<numberShapes; i++)
		{
			result->shapes[i] = shapes[i]->GetCopy();
		}	

		return result;
	}


	void SetFirstShape (blPDShapeInterface *shape_ptr)
	{
		this->firstShape = shape_ptr;
		this->numberDimensions = this->firstShape->GetDimension();
		this->numberLandmarks = this->firstShape->GetNumberOfLandmarks();
	}

	blPDShapeInterface *GetFirstShape ()
	{
		return this->firstShape;
	}

	MatrixPointer GetPointsMatrix ()
	{
		return this->thePoints;
	}


protected:
	/** \brief Empty Constructor */
	blPDSet_templ();   

	/** \brief Destructor */
	virtual ~blPDSet_templ();  

private:        
	blPDSet_templ(const Self&); //purposely not implemented
	void operator=(const Self&); //purposely not implemented		

    unsigned int numberDimensions;			//!< number of dimensions
	unsigned int numberLandmarks;			//!< number of landmarks
	unsigned int numberShapes;				//!< number of PDShapes

	blPDShapeInterface::Pointer * shapes;	//!< array of PDShapes <<< THIS MUST BE REMOVED
	blPDShapeInterface::Pointer firstShape;	//!< THE FIRST PDShape

	MatrixPointer thePoints;		
};

#endif //BLPDSET_TEMPL_H

