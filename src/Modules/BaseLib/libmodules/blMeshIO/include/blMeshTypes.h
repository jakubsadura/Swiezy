// Copyright 2008 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.


#ifndef BLMeshTypes_H
#define BLMeshTypes_H


// BaseLib-includes
// ...none at the moment...

// includes from other Toolkit-Libs
// ...none at the moment...

// Third Party Library includes
// ...none at the moment...

// C/C++ includes
#include <istream>
#include <functional>


namespace base {
namespace meshio {


//! 3 component vector
template<typename T>
class vector3D {

public:
	//! Default constructor NOT initializing the components
	vector3D();

	//! Constructor initializing the components with the given values
	vector3D(T x, T y, T z);

	//!
	T m_x;
	//!
	T m_y;
	//!
	T m_z;
};


//! Vector containing 3 double components
typedef vector3D<double> dVector3D;


class tetrahedron {
public:
	//! Constructor leaving all members uninitialized
	tetrahedron();

	//! Constructor storing the indices of the tetrahedron corners
	tetrahedron(int cornerA, int cornerB, int cornerC, int cornerD);

	//!
	int m_cornerA;

	//!
	int m_cornerB;

	//!
	int m_cornerC;

	//!
	int m_cornerD;
};


//!
class triangle {
public:
	//! Constructor leaving all members uninitialized
	triangle();

	//! Constructor storing the indices of the triangle corners
	triangle(int cornerA, int cornerB, int cornerC);

	//!
	int m_cornerA;

	//!
	int m_cornerB;

	//!
	int m_cornerC;
};


//! Line between 2 points
class line {
public:
	//! Constructor leaving all members uninitialized
	line();

	//! Constructor storing index of start and end point of the line
	line(int start, int end);

	//! index of start point
	int m_start;

	//! index of end point
	int m_end;
};


struct lessDVector3D : public std::binary_function<const dVector3D&, const dVector3D&, bool> {
	bool operator() (const dVector3D& x, const dVector3D& y) const;
};


} // meshio
} // base


//! Reads a vector3D<T> from an input stream by simply using the >> operator for T 3 times
template<typename T>
std::istream &operator>>(std::istream &in, base::meshio::vector3D<T> &point);

//! Writes a vector3D<T> to an output stream by simply using the << operator for T 3 times
template<typename T>
std::ostream &operator<<(std::ostream &out, base::meshio::vector3D<T> &point);

//!
std::istream &operator>>(std::istream &in, base::meshio::tetrahedron &point);

//!
std::ostream &operator<<(std::ostream &out, base::meshio::tetrahedron &point);

//!
std::istream &operator>>(std::istream &in, base::meshio::triangle &point);

//!
std::ostream &operator<<(std::ostream &out, base::meshio::triangle &point);

//!
std::ostream &operator<<(std::ostream &out, base::meshio::line &point);


#endif // BLMeshTypes_H

