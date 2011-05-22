/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "blAlignment.h"
#include "blMath.h"

#include <iostream>
#include <limits>

#include <vnl/algo/vnl_symmetric_eigensystem.h>
#include <vnl/algo/vnl_svd_economy.h>
#include <vnl/vnl_matrix_ref.h>
#include <vnl/vnl_vector_ref.h>

//% Returns: 
//%          homography - the 3x3 homography such that refShape = H*shape 
void blAlignment::AlignShape2D_Projective(const vnl_vector<double> & shape, 
								const vnl_vector<double> & refShape,
								vnl_vector<double> & outputShape,
								vnl_matrix<double> & homography)
{
	const int dim = 2; //number of dimensions
	const int num_o_pts = shape.size()/dim;

	//homogenize data
	vnl_matrix<double> homog_shape_pts(3, num_o_pts);
	for( int i=0; i<num_o_pts; i++ )
	{
		homog_shape_pts[0][i] = shape[dim*i];
		homog_shape_pts[1][i] = shape[dim*i+1];
		homog_shape_pts[2][i] = 1;
	}


	vnl_matrix<double> homog_refshape_pts(3, num_o_pts);
	for( int i=0; i<num_o_pts; i++ )
	{
		homog_refshape_pts[0][i] = refShape[dim*i];
		homog_refshape_pts[1][i] = refShape[dim*i+1];
		homog_refshape_pts[2][i] = 1;
	}


	vnl_matrix<double> homog_shape_pts_norm, T, T_inv;
	vnl_matrix<double> homog_refshape_pts_norm, T_ref, T_ref_inv;

	Normalize2Dpts( homog_shape_pts, homog_shape_pts_norm, T, T_inv);
	Normalize2Dpts( homog_refshape_pts, homog_refshape_pts_norm, T_ref, T_ref_inv);

    vnl_matrix<double> A(3*num_o_pts,9);
	A.fill(0);


  //  for n = 1:Npts
		//X = x1(:,n)';
		//x = x2(1,n); y = x2(2,n); w = x2(3,n);
		//A(3*n-2,:) = [  O  -w*X  y*X];
		//A(3*n-1,:) = [ w*X   O  -x*X];
		//A(3*n  ,:) = [-y*X  x*X   O ];
  //  end

	for( int i = 0; i<num_o_pts; i++ )
	{	
		vnl_vector<double> X = homog_shape_pts_norm.get_column(i);
		const double x = homog_refshape_pts_norm[0][i];
		const double y = homog_refshape_pts_norm[1][i];
		const double w = homog_refshape_pts_norm[2][i];
		
		vnl_vector_ref<double> row1( A.cols(), A[3*i]  );
		vnl_vector_ref<double> row2( A.cols(), A[3*i+1] );
		vnl_vector_ref<double> row3( A.cols(), A[3*i+2] );

		for( int j=0; j<3; j++ )
		{
			//insert w*X
			row1[j+3] = -w*X[j];
			row2[j]   =  w*X[j];
	
			//insert y*X
			row1[j+6] =  y*X[j];
			row3[j]   = -y*X[j];

			//insert x*X
			row2[j+6] = -x*X[j];
			row3[j+3]   =  x*X[j];
		}
	}

	//compute SVD
	vnl_vector<double> v = vnl_svd_economy<double>( A ).V().get_column(8); //get last column

	//% Extract homography
    //H = reshape(V(:,9),3,3)';
	homography.set_size(3,3);
	homography.copy_in( v.begin() );

    //% Denormalise
    //H = T2\H*T1;
	homography = T_ref_inv * homography * T;


	vnl_matrix<double> homog_out_pts = homography * homog_shape_pts;

	//normalize and copy out
	outputShape.set_size( shape.size() );
	int c=0;
	for( size_t i=0; i<homog_out_pts.cols(); i++ )
	{
		const double w = homog_out_pts[2][i];
		for( int j=0; j<2; j++ )
		{
			outputShape[c++] = homog_out_pts[j][i] / w;
		}
	}

}



// params = [ scale_x, scale_y, angle, translation_x, translation_y, v1, v2 ]
//				0		1		2		3				4			5	 6
vnl_matrix<double> blAlignment::SynthesizeNonSkewHomography( const vnl_vector<double> params )
{
	vnl_matrix<double> res(3,3);
	
	const double sx = params[0];
	const double sy = params[1];
	const double ang = params[2];
	const double tx = params[3];
	const double ty = params[4];
	const double v1 = params[5];
	const double v2 = params[6];

	const double s = sin( ang );
	const double c = cos( ang );


	res[0][0] = sx*c + tx*v1;
	res[0][1] = -sy*s + tx*v2;
	res[0][2] = tx;

	res[1][0] = sx*s + ty*v1;
	res[1][1] = sy*c + ty*v2;
	res[1][2] = ty;

	res[2][0] = v1;
	res[2][1] = v2;
	res[2][2] = 1;

	return res;
}


void blAlignment::TransformPointsHomog( vnl_vector<double>& pts, const vnl_matrix<double> T )
{
	const int NumberOfDimensions = T.cols()-1;

	for( size_t i=0; i<pts.size(); i+=NumberOfDimensions )
	{
		vnl_vector<double> point(NumberOfDimensions+1);
		for( int j=0; j<NumberOfDimensions; j++ ) point[j] = pts[i+j];
		point[NumberOfDimensions]=1;

		point.pre_multiply( T );
		point /= point[NumberOfDimensions]; 
		for( int j=0; j<NumberOfDimensions; j++ ) pts[i+j] = point[j];
	}
}



void blAlignment::Normalize2Dpts(const vnl_matrix<double>& pts, vnl_matrix<double>& normalized_pts, vnl_matrix<double>& T, vnl_matrix<double>& T_inv)
{

    //% Find the indices of the points that are not at infinity
    //finiteind = find(abs(pts(3,:)) > eps);
    //
    //if length(finiteind) ~= size(pts,2)
    //    warning('Some points are at infinity');
    //end
    //
    //% For the finite points ensure homogeneous coords have scale of 1
    //pts(1,finiteind) = pts(1,finiteind)./pts(3,finiteind);
    //pts(2,finiteind) = pts(2,finiteind)./pts(3,finiteind);
    //pts(3,finiteind) = 1;
    
    //c = mean(pts(1:2,finiteind)')';            % Centroid of finite points
	vnl_vector<double> mean;
	blMath::ComputeMeanColumn( pts, mean );

    //newp(1,finiteind) = pts(1,finiteind)-c(1); % Shift origin to centroid.
    //newp(2,finiteind) = pts(2,finiteind)-c(2);
	//subtract mean
	normalized_pts.set_size( pts.rows(), pts.cols() );
	normalized_pts.fill(1);
	for( size_t i=0; i<pts.rows()-1; i++ )
		for( size_t j=0; j<pts.cols(); j++ )
			normalized_pts[i][j] = pts[i][j] - mean[i];


	//meandist = mean( sqrt(newp(1,:).^2 + newp(2,:).^2) );
	double meandist = 0;
	for( size_t i=0; i<normalized_pts.cols(); i++ )
	{
		meandist += sqrt( normalized_pts[0][i]*normalized_pts[0][i] + normalized_pts[1][i]*normalized_pts[1][i]  );
	}
    meandist /= normalized_pts.cols();

    double scale = sqrt(2.0)/meandist;
    
    //T = [scale   0   -scale*c(1)
    //     0     scale -scale*c(2)
    //     0       0      1      ];
    //
    //T_inv = [1/scale   0   c(1)
    //     0     1/scale c(2)
    //     0       0      1      ];

	T.set_size(3,3);
	T.fill(0);
	T[0][0] = scale;
	T[1][1] = scale;
	T[2][2] = 1;
	T[0][2] = - scale * mean[0];
	T[1][2] = - scale * mean[1];

	T_inv.set_size(3,3);
	T_inv.fill(0);
	T_inv[0][0] = 1/scale;
	T_inv[1][1] = 1/scale;
	T_inv[2][2] = 1;
	T_inv[0][2] = mean[0];
	T_inv[1][2] = mean[1];

	normalized_pts = T*pts;
    
}



/**
 *	This method aligns a shape with a reference shape and returns
 *	the new coordinates for the aligned shape and parameters that defines
 *	the transform.
 *
 *	\param shape point coordinates of shape to be aligned
 *	\param refShape	 points coordinates of reference shape
 *	\param outputShape new points coordinates aligned with reference shape
 *	\param a element of the transform matrix T
 *	\param b element of the transform matrix T
 *	\param x_t translation on x-axis
 *	\param y_t translation on y-axis
 */
//--------------------------------------------------
void blAlignment::AlignShape2D(const vnl_vector<double> & shape, 
							const vnl_vector<double> & refShape,
							vnl_vector<double> & outputShape,
							unsigned int numberOfPoints,
							double & a, double & b, 
							double & x_t, double & y_t)
//--------------------------------------------------
{

	vnl_vector<double> centredShape(shape.size());
	vnl_vector<double> centredRefShape(refShape.size());

	double shape_x_t, shape_y_t, refShape_x_t, refShape_y_t;


	// first, center both shapes on the origin (0,0)
	blAlignment::CenterShape2DTo(0, 0, shape, centredShape, numberOfPoints,
									shape_x_t, shape_y_t);


	blAlignment::CenterShape2DTo(0, 0, refShape, centredRefShape, 
									numberOfPoints, 
									refShape_x_t, refShape_y_t);

	// then, align centred shape with centred reference shape
	blAlignment::AlignShape2D(centredShape, centredRefShape, outputShape, 
								numberOfPoints, a, b);




	x_t = -b * shape_y_t + a * shape_x_t - refShape_x_t;
	y_t = b * shape_x_t + a * shape_y_t - refShape_y_t;

#ifdef DEBUG_MESSAGES_ALIGNMENT
	std::cout << "Alignment shape_x_t, shape_y_t :" << 
						shape_x_t << " " << shape_y_t << std::endl;

	std::cout << "Alignment refShape_x_t, refShape_y_t :" << 
						refShape_x_t << " " << refShape_y_t << std::endl;

	std::cout << "Alignment x_t, y_t :" << x_t << " " << y_t << std::endl;
	std::cout << std::endl;
#endif


	// translate outputShape to refShape centroid
	unsigned int xIndex = 0;

	for (size_t i = 0; i < numberOfPoints; i++)
	{
		outputShape[xIndex] += 	- refShape_x_t;
		outputShape[xIndex + 1] += - refShape_y_t;

		xIndex += 2;
	}

}





/**
 *	This method aligns a shape with a reference shape and returns
 *	the matrix that defines	the transformation.
 *
 *	\param shape point coordinates of shape to be aligned
 *	\param refShape	 points coordinates of reference shape
 *	\param outputShape new points coordinates aligned with reference shape
 *	\param transformMatrix homogeneous transformation matrix [3,3]
 */
//--------------------------------------------------
void blAlignment::AlignShape2D(const vnl_vector<double> & shape, 
									const vnl_vector<double> & refShape,
									vnl_vector<double> & outputShape,
									unsigned int numberOfPoints,
									vnl_matrix<double> & transformMatrix)
//--------------------------------------------------
{
	double a, b, tx, ty;

	blAlignment::AlignShape2D(shape, refShape, outputShape, 
								numberOfPoints, a, b, tx, ty);

	// homogeneous matrix for 2D
	transformMatrix.set_size(3,3);
	transformMatrix[0][0] = a;
	transformMatrix[0][1] = -b;
	transformMatrix[1][0] = b;
	transformMatrix[1][1] = a;

	transformMatrix[2][0] = 0;
	transformMatrix[2][1] = 0;

	transformMatrix[0][2] = tx;
	transformMatrix[1][2] = ty;
	transformMatrix[2][2] = 1;
	
}




/** 
 *	This method aligns a given shape with a reference shape.
 *	It is supposed that both shapes are centered on the origin.
 *	
 *	This returns a and b parameters, which define the transform matrix
 *
 *	[(a -b) (b a)]
 *
 *	where s (scaling) = sqrt (a^2 + b^2) and 
 *	RHO (rotation) = arctg(b/a)
 *
 *	\param shape point coordinates of shape to be aligned
 *	\param refShape	 points coordinates of reference shape
 *	\param outputShape new points coordinates aligned with reference shape
 *	\param a element of the transform matrix T
 *	\param b element of the transform matrix T
 *	
 */
//--------------------------------------------------
void blAlignment::AlignShape2D( const vnl_vector<double> & shape, 
								const vnl_vector<double> & refShape,
								vnl_vector<double> & outputShape,
								unsigned int numberOfPoints,
								double & a, double & b)
//--------------------------------------------------
{
	
	a = 0.0;	// init a and b to 0.0
	b = 0.0;

	// define number of dimensions
	unsigned int numberOfDimensions = 2;

	
	// calculate a and b in the transform matrix
	// -------------------------------------------

	// |x|^2
	double shapeNorm2 = blMath::Dot(shape, shape);

	// a = (x * x') / |x|^2
	a = blMath::Dot(shape, refShape) / shapeNorm2;

	unsigned int xIndex = 0;
	unsigned int yIndex = 1;

	size_t j;
	for (j = 0; j < numberOfPoints; j++)
	{

		b += shape[xIndex] * refShape[yIndex] -
				shape[yIndex] * refShape[xIndex];

		xIndex += numberOfDimensions;
		yIndex = xIndex + 1;
	}	

	b = b / shapeNorm2;	// b = SUM (xy'- yx') / |x|^2


	// get new coordinates for the aligned shape
	// -------------------------------------------

	outputShape.set_size(shape.size());	// set_size output vector

	xIndex = 0;
	yIndex = 1;

	for (j = 0; j < numberOfPoints; j++)
	{
		outputShape[xIndex] = a * shape[xIndex] - b * shape[yIndex];
		outputShape[yIndex] = b * shape[xIndex] + a * shape[yIndex];

		xIndex += numberOfDimensions;
		yIndex = xIndex + 1;
	}	
}






/** 
 *	Aligns a 3D shape to a reference shape and returns the aligned shape and 
 *	the matrix that represents the similarity transformation.
 *
 *	\param shape shape to be aligned (xyzxyz...)
 *	\param refShape reference shape (xyzxyz...)
 *	\param outputShape aligned shape (xyzxyz...)
 *	\param numberOfPoints number of points in the shapes
 *	\param transformMatrix similarity 3D transform matrix 
 */
//--------------------------------------------------
void blAlignment::AlignShape3D(const double shape[],
								const double refShape[],
								double outputShape[],
								unsigned int numberOfPoints,
								double transformMatrix[4][4],
								bool doOnlyRigidAlign)
//--------------------------------------------------
{

	// rename shapes
	const double * source = shape;
	const double * target = refShape;


	int i;
	int j;
 

	// --- compute the necessary transform to match the two sets of landmarks ---

	/*
		The solution is based on
		Berthold K. P. Horn (1987),
		"Closed-form solution of absolute orientation using unit quaternions,"
		Journal of the Optical Society of America A, 4:629-642
	*/

	// Original python implementation by David G. Gobbi
	int N_PTS = numberOfPoints;
	
////
/*
	if (source.size() != target.size() )
	{
	    std::cerr << "blAlignment::AlignShape3D(): " 
					<< "Source and Target Landmarks contain a " 
					<< "different number of points" << std::endl;
		return;
    }
*/
////

	// -- if no points, stop here
	if (N_PTS == 0)
	{
		std::cerr << "blAlignment::AlignShape3D(): No points!!" << std::endl;
		return;
	}

	// -- find the centroid of each set --		--> Modify this !!! -> call Centroid() method
/*
	vnlVectorDouble source_centroid;
	source_centroid.set_size(3);
	source_centroid.fill(0.0);
	vnlVectorDouble target_centroid;
	target_centroid.set_size(3);
	target_centroid.fill(0.0);
	
	for (i = 0; i < N_PTS; i++)
    {
///		int auxIndex = i * numberOfDimensions;
		int auxIndex = i * 3;

		source_centroid(0) += source(auxIndex);
		source_centroid(1) += source(auxIndex + 1);
		source_centroid(2) += source(auxIndex + 2);
		
		target_centroid(0) += target(auxIndex);
		target_centroid(1) += target(auxIndex + 1);
		target_centroid(2) += target(auxIndex + 2);
    }
	source_centroid(0) /= N_PTS;
	source_centroid(1) /= N_PTS;
	source_centroid(2) /= N_PTS;
	target_centroid(0) /= N_PTS;
	target_centroid(1) /= N_PTS;
	target_centroid(2) /= N_PTS;

*/

////	vnl_vector<double> source_centroid(3, 0.0);	// size 3, init with 0.0
////	vnl_vector<double> target_centroid(3, 0.0);

	double source_centroid[3];
	double target_centroid[3];


	blAlignment::Centroid(source, 3, N_PTS, source_centroid);
	blAlignment::Centroid(target, 3, N_PTS, target_centroid);



	// -- if only one point, stop right here
	if (N_PTS == 1)
    {
		// set to identity
		transformMatrix[0][0] = 1;
		transformMatrix[0][1] = 0;
		transformMatrix[0][2] = 0;
		transformMatrix[0][3] = 0;
		
		transformMatrix[1][0] = 0;
		transformMatrix[1][1] = 1;
		transformMatrix[1][2] = 0;
		transformMatrix[1][3] = 0;

		transformMatrix[2][0] = 0;
		transformMatrix[2][1] = 0;
		transformMatrix[2][2] = 1;
		transformMatrix[2][3] = 0;

		transformMatrix[3][0] = 0;
		transformMatrix[3][1] = 0;
		transformMatrix[3][2] = 0;
		transformMatrix[3][3] = 1;


		// translations 
		transformMatrix[0][3] = target_centroid[0] - source_centroid[0];
		transformMatrix[1][3] = target_centroid[1] - source_centroid[1];
		transformMatrix[2][3] = target_centroid[2] - source_centroid[2];

		
		outputShape[0] = refShape[0];	// 1 point
		outputShape[1] = refShape[1];
		outputShape[2] = refShape[2];
    }

	else //if (N_PTS > 1)
	{
		// -- build the 3x3 matrix M --
///		vnl_matrix<double> M(3,3, 0.0);		// filled with 0.0
///		vnl_matrix<double> AAT(3,3, 0.0);

		double M[3][3], AAT[3][3];
		
		for (i = 0; i < 3; i++) 
	    {
			AAT[i][0] = M[i][0] = 0.0; // fill M and AAT with zeros
			AAT[i][1] = M[i][1] = 0.0; 
			AAT[i][2] = M[i][2] = 0.0; 
		}

		
		int pt;
///		vnl_vector<double> a(3);	// a and b with size 3
///		vnl_vector<double> b(3);
		double a[3], b[3];
		
		double sa = 0.0, sb = 0.0;

		// for each point 
		// -----------------------------------
		int pointIndex = 0;	// index of x coordinate of each point

		for (pt = 0; pt < N_PTS; pt++, pointIndex += 3)
	    {
			
			// get the origin-centred point (a) in the source set
			// source is sorted as XyzXyz
			a[0] = source[pointIndex];
			a[1] = source[pointIndex + 1];
			a[2] = source[pointIndex + 2];
			a[0] -= source_centroid[0];
			a[1] -= source_centroid[1];
			a[2] -= source_centroid[2];
    
			// get the origin-centred point (b) in the target set
			b[0] = target[pointIndex];
			b[1] = target[pointIndex + 1];
			b[2] = target[pointIndex + 2];
			b[0] -= target_centroid[0];
			b[1] -= target_centroid[1];
			b[2] -= target_centroid[2];
	    
			// accumulate the products a*T(b) into the matrix M
			for(i = 0; i < 3; i++) 
			{
				M[i][0] += a[i] * b[0];
				M[i][1] += a[i] * b[1];
				M[i][2] += a[i] * b[2];
/*
				// for the affine transform, compute ((a.a^t)^-1 . a.b^t)^t.
				// a.b^t is already in M.  here we put a.a^t in AAT.
				if (this->Mode == VTK_LANDMARK_AFFINE)
				{
					AAT[i][0] += a[i]*a[0];
					AAT[i][1] += a[i]*a[1];
					AAT[i][2] += a[i]*a[2];
				}
*/
			}
			// accumulate scale factors (if desired)
			sa += a[0] * a[0] + a[1] * a[1] + a[2] * a[2];
			sb += b[0] * b[0] + b[1] * b[1] + b[2] * b[2];
		}

/*
	if(this->Mode == VTK_LANDMARK_AFFINE)
    {
		// AAT = (a.a^t)^-1
	    vtkMath::Invert3x3(AAT,AAT);

	    // M = (a.a^t)^-1 . a.b^t
	    vtkMath::Multiply3x3(AAT,M,M);

		// this->Matrix = M^t
		for(i=0;i<3;++i) 
		{
			for(j=0;j<3;++j)
			{
				this->Matrix->Element[i][j] = M[j][i];
			}
		}
    }
  else
	    {
*/
			// compute required scaling factor (if desired)
			double scale = sqrt(sb/sa);
	    
			// -- build the 4x4 matrix N --
///			vnl_matrix<double> Ndata(4,4, 0.0);	// filled with 0.0s
///			vnl_matrix<double> N(4,4, 0.0);	// filled with 0.0s

			double N[4][4];
			
	
			for(i = 0; i < 4; i++)
			{
				// N[i] = Ndata[i];	// this line has no utility ??? // it has something to do with pointer aritmetics
				N[i][0] = 0.0; // fill N with zeros
				N[i][1] = 0.0;
				N[i][2] = 0.0;
				N[i][3] = 0.0;
			}

			
			// on-diagonal elements
			N[0][0] = M[0][0] + M[1][1] + M[2][2];
			N[1][1] = M[0][0] - M[1][1] - M[2][2];
			N[2][2] = - M[0][0] + M[1][1] - M[2][2];
			N[3][3] = - M[0][0] - M[1][1] + M[2][2];

			// off - diagonal elements
			N[0][1] = N[1][0] = M[1][2] - M[2][1];
			N[0][2] = N[2][0] = M[2][0] - M[0][2];
			N[0][3] = N[3][0] = M[0][1] - M[1][0];
	
			N[1][2] = N[2][1] = M[0][1] + M[1][0];
			N[1][3] = N[3][1] = M[2][0] + M[0][2];
			N[2][3] = N[3][2] = M[1][2] + M[2][1];
	
			// -- eigen-decompose N (is symmetric) --
		
			//-----------------------------------------------------------------
			// Compute eigenvalues and eigenvectors.
			//-----------------------------------------------------------------

#ifdef DEBUG_MESSAGES_ALIGNMENT			
			std::cout << "blAlignment::AlignShape3D(): " << 
							"solving symmetric_eigensystem" << std::endl;	
#endif

			// vnl_matrix reference to N
			vnl_matrix_ref<double> Nref(4, 4, (double *) N);	

			// returns eigenvalues in increasing order
			vnl_symmetric_eigensystem<double> mySystem(Nref); // ????
			
///			eigenvectorData.set_size(mySystem.V.rows(), mySystem.V.columns());
///			eigenvectorData = mySystem.V;	// we store eigenvectors in eigenvectorData
			
			// I'm afraid eigenvectors has no use for us.
	
///			eigenvalues.set_size(mySystem.D.rows() );
///			for (i = 0; i < mySystem.D.rows(); i++)
///				eigenvalues(i) = mySystem.D(i,i);
			

		
			int eigenValuesSize = mySystem.D.rows();

			vnl_matrix<double> eigenvectors(mySystem.V.rows(), mySystem.V.cols());
			vnl_vector<double> eigenvalues(eigenValuesSize);
			
			// copy eigenvalues and blip elements (increasing -> decreasing order)
			for (i = 0; i < mySystem.D.rows(); i++)
			{
				eigenvalues[eigenValuesSize - i - 1] = mySystem.D[i];

				size_t j;
				for (j = 0; j < mySystem.V.rows(); j++)
				{
					eigenvectors[j][eigenValuesSize - i - 1] = mySystem.V[j][i];
				}
			}



			// the eigenvector with the largest eigenvalue is the quaternion we want
			// (they are sorted in decreasing order for us by JacobiN)
			double w,x,y,z;
	
			// first: if points are collinear, choose the quaternion that 
			// results in the smallest rotation.
			if (eigenvalues[0] == eigenvalues[1] || N_PTS == 2)
			{
				double s0[3], t0[3], s1[3], t1[3];
			
				// get points 0 and 1 from source and target
				s0[0] = source[0];
				s0[1] = source[1];
				s0[2] = source[2];

				s1[0] = source[3];
				s1[1] = source[4];
				s1[2] = source[5];

				t0[0] = target[0];
				t0[1] = target[1];
				t0[2] = target[2];

				t1[0] = target[3];
				t1[1] = target[4];
				t1[2] = target[5];

	
				double ds[3], dt[3];			
				double rs = 0, rt = 0;

				for (i = 0; i < 3; i++)
				{
					ds[i] = s1[i] - s0[i];      // vector between points
					rs += ds[i] * ds[i];
					dt[i] = t1[i] - t0[i];
					rt += dt[i] * dt[i];
				}
	
				// normalize the two vectors
				rs = sqrt(rs);
				ds[0] /= rs; ds[1] /= rs; ds[2] /= rs; 
				rt = sqrt(rt);
				dt[0] /= rt; dt[1] /= rt; dt[2] /= rt; 
	
				// take dot & cross product
				w = ds[0] * dt[0] + ds[1] * dt[1] + ds[2] * dt[2];
				x = ds[1] * dt[2] - ds[2] * dt[1];
				y = ds[2] * dt[0] - ds[0] * dt[2];
				z = ds[0] * dt[1] - ds[1] * dt[0];
	    
				double r = sqrt(x*x + y*y + z*z);
				double theta = atan2(r,w);
	
				// construct quaternion
				w =	cos(theta/2.0);
				if (r != 0)
				{
					r = sin(theta/2.0)/r;
					x = x*r;
					y = y*r;
					z = z*r;
				}
				else // rotation by 180 degrees: special case
				{
					std::cout << "blAlignment::Align3DShape(): special case. " 
								<< "Implemented a rotation by 180 degrees" 
								<< std::endl;

					// rotate around a vector perpendicular to ds					
					blAlignment::Perpendiculars(ds, dt, 0, 0);
					
					r = sin(theta/2.0);
					x = dt[0] * r;
					y = dt[1] * r;
					z = dt[2] * r;
				}
			}
			else // points are not collinear
			{
				w = eigenvectors[0][0];
				x = eigenvectors[1][0];
				y = eigenvectors[2][0];
				z = eigenvectors[3][0];
			}
	
			// convert quaternion to a rotation matrix
	
			double ww = w*w;
			double wx = w*x;
			double wy = w*y;
			double wz = w*z;

		    double xx = x*x;
			double yy = y*y;
			double zz = z*z;
	
			double xy = x*y;
			double xz = x*z;
			double yz = y*z;
	
			transformMatrix[0][0] = ww + xx - yy - zz; 
			transformMatrix[1][0] = 2.0 * (wz + xy);
			transformMatrix[2][0] = 2.0 * (-wy + xz);
	
			transformMatrix[0][1] = 2.0 * (-wz + xy);  
			transformMatrix[1][1] = ww - xx + yy - zz;
			transformMatrix[2][1] = 2.0 * (wx + yz);
	
			transformMatrix[0][2] = 2.0 * (wy + xz);
			transformMatrix[1][2] = 2.0 * (-wx + yz);
			transformMatrix[2][2] = ww - xx - yy + zz;


			if ( !doOnlyRigidAlign )
			{

				// THIS IS NECESSARY FOR SIMILARITY CASE
	
	///			if (this->Mode != VTK_LANDMARK_RIGIDBODY)
	///			{ // add in the scale factor (if desired)
				for(i=0; i<3; i++) 
				{
					transformMatrix[i][0] *= scale;
					transformMatrix[i][1] *= scale;
					transformMatrix[i][2] *= scale;
				}
			}

//		}

		// the translation is given by the difference in the transformed source
		// centroid and the target centroid
		double sx, sy, sz;
	
		sx = transformMatrix[0][0] * source_centroid[0] +
				transformMatrix[0][1] * source_centroid[1] +
				transformMatrix[0][2] * source_centroid[2];

		sy = transformMatrix[1][0] * source_centroid[0] +
				transformMatrix[1][1] * source_centroid[1] +
				transformMatrix[1][2] * source_centroid[2];

		sz = transformMatrix[2][0] * source_centroid[0] +
				transformMatrix[2][1] * source_centroid[1] +
				transformMatrix[2][2] * source_centroid[2];
	
		transformMatrix[0][3] = target_centroid[0] - sx;
		transformMatrix[1][3] = target_centroid[1] - sy;
		transformMatrix[2][3] = target_centroid[2] - sz;
	
		// fill the bottom row of the 4x4 matrix
		transformMatrix[3][0] = 0.0;
		transformMatrix[3][1] = 0.0;
		transformMatrix[3][2] = 0.0;
		transformMatrix[3][3] = 1.0;

	} // else, if N_PTS > 1
	
	

	
	for (j = 0; j < N_PTS; j++)
	{
		int index = j * 3;

		// this is an vtkLinearTransformPoint(...);
		outputShape[index] =	transformMatrix[0][0] * source[index] +
								transformMatrix[0][1] * source[index + 1] +
								transformMatrix[0][2] * source[index + 2] +
								transformMatrix[0][3];

		outputShape[index + 1] =	transformMatrix[1][0] * source[index] +
									transformMatrix[1][1] * source[index + 1] +
									transformMatrix[1][2] * source[index + 2] +
									transformMatrix[1][3];

		outputShape[index + 2] =	transformMatrix[2][0] * source[index] +
									transformMatrix[2][1] * source[index + 1] + 
									transformMatrix[2][2] * source[index + 2] +
									transformMatrix[2][3];
	}

}



//--------------------------------------------------
void blAlignment::AlignShape3D(const vnl_vector<double> & shape,
								const vnl_vector<double> & refShape,
								vnl_vector<double> & outputShape,
								unsigned int numberOfPoints,
								vnl_matrix<double> & transformMatrix,
								bool doOnlyRigidAlign)
//--------------------------------------------------
{
	if (shape.size() != refShape.size() )
	{
	    std::cerr << "blAlignment::AlignShape3D(): " 
					<< "Source and Target shapes contain a " 
					<< "different number of points" << std::endl;
		return;
    }

	// set_size output data
	outputShape.set_size(shape.size());
	transformMatrix.set_size(4,4);

	double auxTransformMatrix[4][4];

	blAlignment::AlignShape3D(shape.data_block(), 
								refShape.data_block(), 
								outputShape.data_block(), 
								numberOfPoints, 
								auxTransformMatrix,
								doOnlyRigidAlign);

	// copy transform matrix
	transformMatrix.copy_in((const double *) auxTransformMatrix);
}






/**
 *	This method calculates new coordinates to center a given shape on a
 *	specified 2D point.
 *
 *	\param x coordinate x of point the shape will be centred on
 *	\param y coordinate y of point the shape will be centred on
 *	\param shape points of the shape
 *	\param outputShape new point coordinates
 *	\param dX translation on x-axis
 *	\param dY translation on y-axis
 */
//--------------------------------------------------
void blAlignment::CenterShape2DTo(double x, double y, 
											const vnl_vector<double> & shape,											
											vnl_vector<double> & outputShape,
											unsigned int numberOfPoints,
											double & dX, double & dY)
//--------------------------------------------------
{

	vnl_vector<double> centroid(2);
	blAlignment::Centroid(shape, 2, numberOfPoints, centroid);

	dX = x - centroid[0];	// compute difference between centroids
	dY = y - centroid[1];

	TranslateShape2DTo( dX, dY, shape, outputShape, numberOfPoints );
}




/**
 *	This method calculates new coordinates to center a given shape on a
 *	specified 2D point.
 *
 *	\param x coordinate x of point the shape will be centred on
 *	\param y coordinate y of point the shape will be centred on
 *	\param shape points of the shape
 *	\param outputShape new point coordinates
 *	\param dX translation on x-axis
 *	\param dY translation on y-axis
 */
//--------------------------------------------------
void blAlignment::CenterShapeTo(const double center[], 
									const double shape[],	
									double outputShape[],
									unsigned int numberOfPoints,
									unsigned int numberOfDimensions,
									double translation[])
//--------------------------------------------------
{
	vnl_vector<double> centroid(numberOfDimensions);
	blAlignment::Centroid(shape, numberOfDimensions, numberOfPoints, centroid.data_block());
	vnl_vector<double> diff(center, numberOfDimensions);

	//diff = center;		// diff = center - centroid
	diff -= centroid;
	unsigned int pointIndex = 0;

	size_t i;
	for (i = 0; i < numberOfPoints; i++, pointIndex += numberOfDimensions)
	{
		for (size_t d = 0; d < numberOfDimensions; d++)
		{
			outputShape[pointIndex + d] = shape[pointIndex + d] + diff[d];
		}
	}	
}



/**
 *	This method calculates new coordinates to center a given shape on a
 *	specified 2D point.
 *
 *	\param x coordinate x of point the shape will be centred on
 *	\param y coordinate y of point the shape will be centred on
 *	\param shape points of the shape
 *	\param outputShape new point coordinates
 *	\param dX translation on x-axis
 *	\param dY translation on y-axis
 */
//--------------------------------------------------
void blAlignment::CenterShape3DTo(double x, double y, double z,
								const vnl_vector<double> & shape,
								vnl_vector<double> & outputShape,
								unsigned int numberOfPoints,
								double & dX, double & dY, double & dZ)
//--------------------------------------------------
{
	double center[3];
	double translation[3];

	center[0] = x;
	center[1] = y;
	center[2] = z;

	blAlignment::CenterShapeTo(center, shape.data_block(), outputShape.data_block(), 
									numberOfPoints, 3, translation);

	dX = translation[0];
	dY = translation[1];
	dZ = translation[2];
}
	




/**
 *	This method calculates the centroid of a set of points
 *
 *	\param pointCoordinates coordinates of the points : xy[z]xy[z]...
 *	\param numberOfDimensions number of dimensions of points
 *	\param numberOfPoints number of points
 *	\param centroid coordinates of the centroid
 */
//--------------------------------------------------
void blAlignment::Centroid(const vnl_vector<double> & pointCoordinates,
									unsigned int numberOfDimensions,
									unsigned int numberOfPoints,
									vnl_vector<double> & centroid)
//--------------------------------------------------
{
	centroid.set_size(numberOfDimensions);
	centroid = 0;
/*
	int i;
	int d;
	for (i = 0; i < numberOfPoints; i++)
	{
		for (d = 0; d < numberOfDimensions; d++)
		{
			centroid[d] += pointCoordinates[i * numberOfDimensions + d];
		}
	}
	
	centroid /= (double) numberOfPoints;
*/

	// points iterators
	vnl_vector<double>::const_iterator pointCoordIter, pointCoordIterEnd;
	pointCoordIterEnd = pointCoordinates.end();


	// centroid iterators
	vnl_vector<double>::iterator centroidIter, centroidIterBegin, centroidIterEnd; 
	centroidIterBegin = centroid.begin();
	centroidIterEnd = centroid.end();
	centroidIter = centroidIterBegin;


	size_t pointCounter = 0;
	for (pointCoordIter = pointCoordinates.begin();  
			pointCoordIter < pointCoordIterEnd && 
				pointCounter < numberOfPoints; pointCoordIter++)
	{
		*centroidIter += *pointCoordIter;

		centroidIter++;
		if (centroidIter == centroidIterEnd)
		{
			centroidIter = centroidIterBegin;
			pointCounter++;
		}
	}

	centroid /= (double) numberOfPoints;
}




/**
 *	This method calculates the centroid of a set of points
 *
 *	\param pointCoordinates coordinates of the points : xy[z]xy[z]...
 *	\param numberOfDimensions number of dimensions of points
 *	\param numberOfPoints number of points
 *	\param centroid coordinates of the centroid
 */
//--------------------------------------------------
void blAlignment::Centroid(const double pointCoordinates[],
									unsigned int numberOfDimensions,
									unsigned int numberOfPoints,
									double centroid[])
//--------------------------------------------------
{
	// init centroid to 0.0
	size_t d;
	for (d = 0; d < numberOfDimensions; d++)
	{
		centroid[d] = 0.0;
	}
	

	// points iterators
	const double * pointCoordIter = pointCoordinates;
	double * centroidIter;

	size_t pointCounter = 0;

	// accumulate into centroid array

	for (pointCounter = 0; pointCounter < numberOfPoints; pointCounter++)
	{
		centroidIter = centroid; // beginning of centroid array

		for (d = 0; d < numberOfDimensions; d++)
		{
			*centroidIter += *pointCoordIter;
			centroidIter++;
			pointCoordIter++;
		}		
	}


	// divide by numberOfPoints

	centroidIter = centroid; // beginning of centroid array

	for (d = 0; d < numberOfDimensions; d++)
	{
		*centroidIter /= ((double) numberOfPoints);
		centroidIter++;		
	}

}





/**
 */
//----------------------------------------------------------------------------
void blAlignment::Perpendiculars(const double x[3], 
									double y[3], 
												double z[3], double theta)
//----------------------------------------------------------------------------
{
	int dx,dy,dz;
	double x2 = x[0]*x[0];
	double y2 = x[1]*x[1];
	double z2 = x[2]*x[2];
	double r = sqrt(x2 + y2 + z2);

	  // transpose the vector to avoid divide-by-zero error
	if (x2 > y2 && x2 > z2)
	{
	    dx = 0; dy = 1; dz = 2;
	}
	else if (y2 > z2) 
	{
	    dx = 1; dy = 2; dz = 0;
	}
	else 
	{
	    dx = 2; dy = 0; dz = 1;
	}

	double a = x[dx] / r;
	double b = x[dy]/r;
	double c = x[dz]/r;

	double tmp = sqrt(a*a+c*c);

	if (theta != 0)
    {
	    double sintheta = sin(theta);
	    double costheta = cos(theta);

// for our problem y will never be NULL
//	    if (y)
	    {
			y[dx] = (c*costheta - a*b*sintheta)/tmp;
			y[dy] = sintheta*tmp;
			y[dz] = (-a*costheta - b*c*sintheta)/tmp;
		}

		if (z)
		{
			z[dx] = (-c*sintheta - a*b*costheta)/tmp;
			z[dy] = costheta*tmp;
			z[dz] = (a*sintheta - b*c*costheta)/tmp;
		}
    }
	else
    {

// for our problem y will never be NULL		
//		if (y)
		{
			y[dx] = c/tmp;
			y[dy] = 0;
			y[dz] = -a/tmp;
		}

		if (z)
		{
			z[dx] = -a*b/tmp;
			z[dy] = tmp;
			z[dz] = -b*c/tmp;
		}
	}      
}





/**
 *
 *	\param pointCoordinates coordinates of the points : xy[z]xy[z]...
 *	\param numberOfDimensions number of dimensions of points
 *	\param numberOfPoints number of points
 *	\param boundingBox coordinates of the bounding Box
 */
//--------------------------------------------------
void blAlignment::ComputeBoundingBox(const vnl_vector<double> & pointCoordinates,
									unsigned int numberOfDimensions,
									unsigned int numberOfPoints,
									vnl_vector<double> & boundingBox )
//--------------------------------------------------
{
	// Min and max for each dimension
	boundingBox.set_size(numberOfDimensions * 2);
	boundingBox = 0;

	for ( size_t i = 0 ; i < numberOfDimensions ; i++ )
	{
		boundingBox[ i * 2 ] = std::numeric_limits<double>::max();
		boundingBox[ i * 2 + 1 ] = std::numeric_limits<double>::min();
	}


	// points iterators
	vnl_vector<double>::const_iterator pointCoordIter, pointCoordIterEnd;
	pointCoordIterEnd = pointCoordinates.end();


	// centroid iterators
	vnl_vector<double>::iterator boundingBoxIter, boundingBoxIterBegin, boundingBoxIterEnd; 
	boundingBoxIterBegin = boundingBox.begin();
	boundingBoxIterEnd = boundingBox.end();
	boundingBoxIter = boundingBoxIterBegin;


	size_t pointCounter = 0;
	for (	pointCoordIter = pointCoordinates.begin();  
			pointCoordIter < pointCoordIterEnd && 
				pointCounter < numberOfPoints; 
			pointCoordIter++)
	{

		// min
		if ( *pointCoordIter < *boundingBoxIter )
		{
			*boundingBoxIter = *pointCoordIter;
		}
		boundingBoxIter++;


		// max
		if ( *pointCoordIter > *boundingBoxIter )
		{
			*boundingBoxIter = *pointCoordIter;
		}
		boundingBoxIter++;

		if (boundingBoxIter == boundingBoxIterEnd)
		{
			boundingBoxIter = boundingBoxIterBegin;
			pointCounter++;
		}
	}

}





/**
 *
 *	\param x coordinate x of point the shape will be centered on
 *	\param y coordinate y of point the shape will be centered on
 *	\param shape points of the shape
 *	\param outputShape new point coordinates
 *	\param dX translation on x-axis
 *	\param dY translation on y-axis
 */
//--------------------------------------------------
void blAlignment::MoveShape2DTo(double x, double y, 
						const vnl_vector<double> & shape,
						vnl_vector<double> & outputShape,
						unsigned int numberOfPoints,
						double & dX, double & dY)
//--------------------------------------------------
{

	vnl_vector<double> boundingBox;
	blAlignment::ComputeBoundingBox(shape, 2, numberOfPoints, boundingBox);

	// upper left corner
	dX = x - boundingBox[ 0 ];
	dY = y - boundingBox[ 2 ];

	TranslateShape2DTo( dX, dY, shape, outputShape, numberOfPoints );

}






/**
 */
//--------------------------------------------------
void blAlignment::TranslateShape2DTo(double dX, double dY, 
						const vnl_vector<double> & shape,
						vnl_vector<double> & outputShape,
						unsigned int numberOfPoints)
//--------------------------------------------------
{

	outputShape.set_size(shape.size());
	outputShape = 0;

	unsigned int xIndex = 0;

	size_t i;
	for (i = 0; i < numberOfPoints; i++)
	{
		outputShape[xIndex] = shape[xIndex] + dX;
		outputShape[xIndex + 1] = shape[xIndex + 1] + dY;

		xIndex +=2;
	}	

}

