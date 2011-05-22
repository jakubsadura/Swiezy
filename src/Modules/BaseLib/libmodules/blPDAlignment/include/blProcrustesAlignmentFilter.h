/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __blProcrustesAlignmentFilter_h
#define __blProcrustesAlignmentFilter_h

#include "blMath.h"
#include "blUblasConvertibleArray.h"
#include <blVector.h>
#include "blLightObject.h"
#include "blSmartPointer.h"
#include "blMacro.h"
#include "blLinearAlgebraOperations.h"
#include "blLinearAlgebraTypes.h"

// #define DEBUG_MESSAGES_ALIGNMENT

/**
 * \brief This class implements the Procrustes Alignment algorithm
 * \ingroup blPDAlignment
 *
 *  Defines Procrustes (Execute) and some other utility functions for the class.
 */
template< class ArgMatrixType >
class blProcrustesAlignmentFilter: public blLightObject
{
public:
	typedef blProcrustesAlignmentFilter Self;
	typedef blSmartPointer<Self> Pointer;
	typedef ArgMatrixType MatrixType;
	typedef typename bllao::Namespace<MatrixType> MatrixNamespace;
	typedef typename MatrixNamespace::Pointer MatrixPointer;
	typedef baselib::VnlVectorType VectorType;

	//! \param output - If not NULL, the filter will use \a output as the matrix where the aligned shapes are stored.
	static Pointer New(MatrixPointer output = MatrixPointer())
	{
		Pointer smartPtr = new Self(output);
		smartPtr->UnRegister();
		return smartPtr;
	}

	/**\brief returns the value for epsilon to determine 
		when the error is small enough*/
	double GetEpsilon()
	{
		return this->epsilon;
	};

	// Description:
	// Retrieve the input point set with index idx (usuall only for pipeline tracing).
	VectorType GetInput(int idx) const
	{
		VectorType result;
		bllao::CopyRowToVector(this->GetMatrix(), idx, result);
		return result;
	};

	/**\brief returns the maximum number of iterations to align the shapes */
	unsigned int GetMaxNumberOfIterations()
	{
		return this->maxNumberOfIterations;
	};

	// Description:
	// Returns the dimension of the data.
	int GetNumberOfDimensions() const
	{ 
		return this->numberOfDimensions; 
	};
	
	// Description:
	// Returns how many inputs have been set.
	int GetNumberOfInputs() const
	{ 
		return this->numberOfInputs; 
	};

	// Description:
	// Returns how many points have been set.
	int GetNumberOfPoints() const
	{ 
		return this->numberOfPoints; 
	};

	// Description:
 	//	Output is a vector representing the aligned shape idx
	// Retrieve the output point set with index idx.
	VectorType GetOutput(int idx) const
	{ 
		if (idx<0 || idx>=this->GetNumberOfInputs())
		{
  		    VectorType nullVector(0);
			std::cerr <<"Procrustes: Index out of bounds in GetOutput!" << std::endl;
			return nullVector;
		}

		VectorType result;
		bllao::CopyRowToVector(this->GetMatrix(), idx, result);
		return result;
	};

	/**\brief returns a const reference to the output data */
	const MatrixType& GetOutputRef() const
	{ 
		#ifdef DEBUG_MESSAGES_ALIGNMENT
			std::cout << "blProcrustes: output dimension = (" 
				<< bllao::NrRows(this->GetMatrix()) << "," << bllao::NrCols(this->GetMatrix()) << ")" 
				<< std::endl;
		#endif

		return this->GetMatrix();	// returning aligned shapes in rows
	};

	/**\brief sets a value for epsilon to determine 
		when the error is small enough*/
	void SetEpsilon(double epsilon)
	{
		this->epsilon = epsilon;
	};

	/**\brief sets input data as a const reference, 
		so data are copied and not modified */
	void SetInput(const MatrixType& theMatrix, unsigned int numberOfPoints, unsigned int numberOfDimensions);

	/**\brief sets the maximum number of iterations to align the shapes*/
	void SetMaxNumberOfIterations(unsigned int numberOfIterations)
	{
		this->maxNumberOfIterations = numberOfIterations;
	};

	// Description:
	// Usual data generation method.
	int Update(); 

	//set to rotate the mean shape to the reverse of the half of the mean angle 
	//(e.g. if the face was tilted to the left and to the right, 
	//then in the mean shape it will be straight)	
	//
	// returns previous setting
	bool CorrectMeanShapeAngle( bool correct )
	{
		bool result = _correctMeanShapeAngle;
		_correctMeanShapeAngle = correct;
		
		return result;
	}

	//sets custom weigths to copute mean instead of the usual 1/N weigth
	//optional function
	//weights must add up to 1
	void SetWeights( const VectorType& weights )
	{
		_weights = weights;
	}

	const VectorType& GetConvergedMean()
	{
		return _mean;
	}

	void DoNotRemoveScale (bool b)
	{
		this->doNotRemoveScale = b;
		if (b)
		{
			std::cout << "\nWARNING: Procrustes alignment up to rigid transformation only\n";
		}
	}


protected:

	//! \param output - See function New().
	blProcrustesAlignmentFilter(MatrixPointer output = MatrixPointer());

	// Description:
	// Standard destructor
	virtual ~blProcrustesAlignmentFilter();

private:
	blProcrustesAlignmentFilter(const Self&); //purposely not implemented
	void operator=(const Self&); //purposely not implemented

	//! Getter (helper function).
	MatrixType& GetMatrix()
	{
		return *(this->m_Data);
	}
	const MatrixType& GetMatrix() const
	{
		return *(this->m_Data);
	}

	/**\brief Aligns a 2D shape to a mean shape*/
	double AlignShape2Mean( const VectorType& data, 
							const VectorType& mean,
							VectorType& output);


	/**\brief Aligns a 3D shape to a mean shape*/
	void AlignShape3Mean(const VectorType& data, 
								const VectorType& mean,
								VectorType& output);

	MatrixPointer m_Data;

	int numberOfInputs;		// numberOfShapes
	int numberOfPoints;		// numberOfLandmarks
	int numberOfDimensions;	// 2

	bool _correctMeanShapeAngle; //set to rotate the mean shape to the reverse of the half of the mean angle 
								  //(e.g. if the face was tilted to the left and to the right, 
								  //then in the mean shape it will be straight)

	double epsilon;						//!< value of epsilon 
	unsigned int maxNumberOfIterations;	//!< max number of iterations

	VectorType _weights;
	VectorType _mean;

	bool doNotRemoveScale;	//!< If true, the alignment will not modify the size of the shapes
};

#endif
