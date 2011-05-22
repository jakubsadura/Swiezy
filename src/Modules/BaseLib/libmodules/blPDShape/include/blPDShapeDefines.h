/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#ifndef __blPDShapeDefines_h
#define __blPDShapeDefines_h

//#include <vnl/vnl_vector.h>
//#include <vnl/vnl_matrix.h>
//#include <vnl/vnl_matlab_filewrite.h>

//typedef vnl_matrix<double> vnlMatrixDouble;
//typedef vnl_vector<double> vnlVectorDouble;
//typedef vnl_matlab_filewrite vnlMatlabWriter;

//#define PRECISION 1e-6	// current precision 




/**
 * \brief This structure defines bounding box for 2D data. 
 * \ingroup blPDShape
 *
 *	
 */
typedef struct tagBoundingBox2D {
	double m_fXMin;
    double m_fXMax;
    double m_fYMin;
    double m_fYMax;
} BoundingBox2D;

/**
 * \brief This structure defines 2D point for easy access to data. 
 * \ingroup blPDShape
 *
 *  Don't add new data members!
 *  This class is used by Delaunay algortithm do access components of 
 *  double array. It means that the only data member this class must have is 
 *  array of TPixelType! at least while it is used by Delaunay triangulation
 *	
 */

template <class TPixelType, unsigned int PointDimension>
class blPoint {
		public:
			/**\brief default constructor, sets coordinates to 0*/
			blPoint() 
			{
				memset(data,0, sizeof(TPixelType) * PointDimension);
			};

			/**\brief constructor that sets coordinates to specified numbers*/
			blPoint(const TPixelType& x, const TPixelType& y) {X()=x; Y()=y;};
			blPoint(const TPixelType& x, const TPixelType& y, const TPixelType& z) {X()=x; Y()=y; Z()=z;}


			/**\brief functions to access coordinates*/
			TPixelType& X() {return data[0];};
			TPixelType& Y() {return data[1];};
			TPixelType& Z() {return data[2];};
			const TPixelType& X() const {return data[0];};
			const TPixelType& Y() const {return data[1];};
			const TPixelType& Z() const {return data[2];};

			/**\brief overloaded operator to access coordinates*/
			TPixelType& operator[] (int i) {return data[i];};
			const TPixelType& operator[] (int i) const {return data[i];};

			/**\brief some perations on points*/
			bool operator== (const blPoint& a) const 
			{ 
				bool result=true;
			
				for(size_t i=0; i<PointDimension; i++)
				{
					result = result && (a.data[i]==data[i]);
				}
		
				return result; 
			};

			blPoint operator- (const blPoint& a) const 
			{ 
				blPoint result;

				for(size_t i=0; i<PointDimension; i++)
				{
					result[i] = data[i] - a.data[i];
				}

				return result; 
			};


			blPoint& operator+= (const blPoint& a)  
			{ 
				for(int i=0; i<PointDimension; i++)
				{
					data[i] += a.data[i];
				}

				return *this; 
			};

			blPoint& operator/= (const double a)  
			{ 
				for(int i=0; i<PointDimension; i++)
				{
					data[i] /= a;
				}

				return *this; 
			};

		private: 
			TPixelType data[PointDimension];
			//!< coordinates. data[0] - x, data[1] - y
};


/**
 * \brief Vector of 2D points. 
 * \ingroup blPDShape
 *
 * type to refernce 2D points in the vector (the vector can be thus represented as 
 * an array of points by direct assignment: 
 * VectorOf2DPoints akPoint=(VectorOf2DPoints)&Vector[0];
 * where vector is of vnl_vector<double> type
 *	
 */

typedef class blPoint<double, 2> ContinuousPoint2D;
typedef class blPoint<double, 3> ContinuousPoint3D;

typedef ContinuousPoint2D* VectorOf2DContinuousPoints;
typedef ContinuousPoint3D* VectorOf3DContinuousPoints;


//typedef class blPoint<int, 2> IntegerPoint2D;
//typedef class blPoint<int, 3> IntegerPoint3D;
//
//typedef IntegerPoint2D* VectorOf2DIntegerPoints;
//typedef IntegerPoint3D* VectorOf3DIntegerPoints;

//:~


#endif
