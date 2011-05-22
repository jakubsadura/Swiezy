/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#ifndef __blProjectiveAlignmentFilter_h
#define __blProjectiveAlignmentFilter_h

#include <blLightObject.h>
#include <blSmartPointer.h>
#include <blMacro.h>

#include <blMath.h>


/**
 * \brief 
 * \ingroup blPDAlignment
 */
class blProjectiveAlignmentFilter: public blLightObject
{

public:
	typedef blProjectiveAlignmentFilter Self;
	typedef blSmartPointer<Self> Pointer;

	blNewMacro(Self);


	/**\brief returns a const reference to the output data */
	const vnlMatrixDouble & GetOutputRef() const
	{ 
		return _data;	// returning aligned shapes in rows
	};

	

	/**\brief sets input data, only 2D is supported*/
	void SetInput(const vnlMatrixDouble & theMatrix, 
					unsigned int numberOfPoints, 
					unsigned int numberOfDimensions);



	int Update(); 

	void SetEpsilon( double eps )
	{
		_eps = eps;
	}

	double GetEpsilon() const
	{
		return _eps;
	}

	void SetMaximumNumberOfIterations( unsigned int iters )
	{
		_maxIterations = iters;
	}

	unsigned int GetMaximumNumberOfIterations() const
	{
		return _maxIterations;
	}

protected:
	
	// Description:
	// Standard constructor
	blProjectiveAlignmentFilter();

	// Description:
	// Standard destructor
	virtual ~blProjectiveAlignmentFilter();

private:
	blProjectiveAlignmentFilter(const Self&); //purposely not implemented
	void operator=(const Self&); //purposely not implemented


	vnlVectorDouble _mean;
	vnlMatrixDouble _data;
	int _numberOfPoints;
	int _numberOfDimensions;

	double _eps; //convergence threshold
	unsigned int _maxIterations;

};


/**
 * \brief 
 * \ingroup blPDAlignment
 */
class blAllign2FacesWithHomology 
{
public:
	typedef std::vector<unsigned int> IndexArray;

	blAllign2FacesWithHomology();

	~blAllign2FacesWithHomology() {};

	void AlignFaces(const vnlVectorDouble& shape, const vnlVectorDouble& refShape, int NumberOfLandmarks);

	vnlVectorDouble GetAlignedFace()
	{
		return _alignedShape;
	};

	vnlVectorDouble TransformFace( const vnlVectorDouble& face, const vnlVectorDouble& transform, int NumberOfLandmarks ) const;

protected:

	vnlVectorDouble ExtractSubshape( const vnlVectorDouble& shape, const IndexArray& indices ) const;
	vnlMatrixDouble ComputeLine( const vnlVectorDouble& shape, const IndexArray& line_indices ) const;

	//pt_ref - reference point
	//pt_tr - transformed by frontal homography
	//A - corresponding matrix
	double MinimizationSumElement( const double* pt_ref, const double* pt_tr, const vnlMatrixDouble& A ) const;
	
	// compute |Ax|^2
	double Ax( const double* pt_tr, const vnlMatrixDouble& A ) const;

	double ComputeCR(  const vnlVectorDouble& shape_ref, const vnlVectorDouble& shape_tr, const vnlMatrixDouble& A, const IndexArray& indices ) const;

	void TransformSelectedPoints( vnlVectorDouble& shape, const vnlMatrixDouble& T, const IndexArray& indices ) const;

private:
	vnlVectorDouble _alignedShape;

	struct _tag_imm_transform_dependencies
	{
		IndexArray nose_tip; //= 58
		IndexArray front_plane; //= [13:57]
		IndexArray left_plane; //= [0:6]
		IndexArray right_plane; //= [7:12]
		IndexArray left_line; //= 25, 52
		IndexArray right_line; //= 17, 52
		IndexArray nose_line; //= 17, 25
	} _imm_transform_dependencies;


};

#endif
