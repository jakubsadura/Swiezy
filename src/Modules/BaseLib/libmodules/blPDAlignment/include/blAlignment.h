/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#ifndef __blAlignment_h
#define __blAlignment_h

#include <vnl/vnl_matrix.h>
#include <vnl/vnl_vector.h>

//#define DEBUG_MESSAGES_ALIGNMENT

/**
 * \brief class with static methods for aligment
 * \ingroup blPDAlignment
 *
 *	This class defines static methods to be used in alignment of shapes.
 */
class blAlignment
{
public:
	/**\brief aligns a shape with a reference shape */
	static void AlignShape2D(const vnl_vector<double> & shape, 
									const vnl_vector<double> & refShape,
									vnl_vector<double> & outputShape,
									unsigned int numberOfPoints,
									double & a, double & b, 
									double & x_t, double & y_t);

	/**\brief aligns a shape with a reference shape */
	static void AlignShape2D(const vnl_vector<double> & shape, 
									const vnl_vector<double> & refShape,
									vnl_vector<double> & outputShape,
									unsigned int numberOfPoints,
									vnl_matrix<double> & transformMatrix);

	/**\brief aligns a shape with a reference shape, 
	both centred on the origin*/
	static void AlignShape2D_Projective(const vnl_vector<double> & shape, 
								const vnl_vector<double> & refShape,
								vnl_vector<double> & outputShape,
								vnl_matrix<double> & homography);

	/**\brief Synthesizes homography from parameters (without the angle specific to affine transform
	 * which allows skewing shapes):
	 * params = [ scale_x, scale_y, angle, translation_x, translation_y, v1, v2 ]
	 */
	static vnl_matrix<double> SynthesizeNonSkewHomography( const vnl_vector<double> params );

	//transforms by any transformation T that needs homogeneous representation
	static void TransformPointsHomog( vnl_vector<double>& pts, const vnl_matrix<double> T);

	static void AlignShape2D(const vnl_vector<double> & shape, 
								const vnl_vector<double> & refShape,
								vnl_vector<double> & outputShape,
								unsigned int numberOfPoints,
								double & a, double & b);
	


	static void AlignShape3D(const vnl_vector<double> & shape,
								const vnl_vector<double> & refShape,
								vnl_vector<double> & outputShape,
								unsigned int numberOfPoints,
								vnl_matrix<double> & matrix,
								bool doOnlyRigidAlign = false);

	static void AlignShape3D(const double shape[],
								const double refShape[],
								double outputShape[],
								unsigned int numberOfPoints,
								double transformMatrix[4][4],
								bool doOnlyRigidAlign = false);


	/** 
	 * \brief centers a shape on a given point, with origin in the 
	 * centroid
	 */
	static void CenterShape2DTo(double x, double y, 
								const vnl_vector<double> & shape,
								vnl_vector<double> & outputShape,
								unsigned int numberOfPoints,
								double & dX, double & dY);

	static void CenterShape3DTo(double x, double y, double z,
								const vnl_vector<double> & shape,
								vnl_vector<double> & outputShape,
								unsigned int numberOfPoints,
								double & dX, double & dY, double & dZ);
	

	static void CenterShapeTo(const double center[], 
									const double shape[],	
									double outputShape[],
									unsigned int numberOfPoints,
									unsigned int numberOfDimensions,
									double translation[]);


	static void Centroid(const double pointCoordinates[],
									unsigned int numberOfDimensions,
									unsigned int numberOfPoints,
									double centroid[]);


	/**\brief calculates the centroid of given points */
	static void Centroid(const vnl_vector<double> & pointCoordinates,
							unsigned int numberOfDimensions,
							unsigned int numberOfPoints,
							vnl_vector<double> & centroid);

	static void Perpendiculars(const double x[3], double y[3], 
												double z[3], double theta);



	/** 
	 * \brief This method calculates the bounding box of a set of points
	 * minX, maxX, minY, maxY, ....
	 */
	static void ComputeBoundingBox(
						const vnl_vector<double> & pointCoordinates,
						unsigned int numberOfDimensions,
						unsigned int numberOfPoints,
						vnl_vector<double> & boundingBox );

	/**
	 * \brief This method calculates new coordinates to move a given 
	 *	shape on a specified 2D point, with origin in the upper-left corner
	 * of the bounding-box
	 */
	static void MoveShape2DTo(double x, double y, 
						const vnl_vector<double> & shape,
						vnl_vector<double> & outputShape,
						unsigned int numberOfPoints,
						double & dX, double & dY);

	/// Add (dx, DY) to all the shape points 
	static void TranslateShape2DTo(double dX, double dY, 
						const vnl_vector<double> & shape,
						vnl_vector<double> & outputShape,
						unsigned int numberOfPoints);

protected:

	//% Argument:
	//%   pts -  3xN array of 2D homogeneous coordinates (3rd coordinate must be 1)
	//%
	//% Returns:
	//%   normalized_pts -  3xN array of transformed 2D homogeneous coordinates.  The
	//%             scaling parameter is normalised to 1 unless the point is at
	//%             infinity. 
	//%   T      -  The 3x3 transformation matrix, newpts = T*pts
	static void Normalize2Dpts(const vnl_matrix<double>& pts, vnl_matrix<double>& normalized_pts, vnl_matrix<double>& T, vnl_matrix<double>& T_inv);


};

#endif
