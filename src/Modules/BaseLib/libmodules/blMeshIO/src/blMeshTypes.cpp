// Copyright 2008 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.


// own header-file
#include "blMeshTypes.h"

// BaseLib-includes
// ...none at the moment...

// includes from other Toolkit-Libs
// ...none at the moment...

// Third Party Library includes
// ...none at the moment...

// C/C++ includes
// ...none at the moment...


using namespace base::meshio;


// if the compiler doesn't support the "export" keyword, then it is removed from the file
// and a set of pre-defined instances of the template defined (see also end of this file)
#ifndef USE_EXPORT_KEYWORD
  #define export /*nothing*/
#endif


// public function: documentation in corresponding .h file
export template<typename T>
vector3D<T>::vector3D()
{}

// public function: documentation in corresponding .h file
export template<typename T>
vector3D<T>::vector3D(T x, T y, T z)
: m_x(x), m_y(y), m_z(z)
{}

// public function: documentation in corresponding .h file
export template<typename T>
std::istream &operator>>(std::istream &in, vector3D<T> &point) {
	return in >> point.m_x >> point.m_y >> point.m_z;
}

// public function: documentation in corresponding .h file
template<typename T>
std::ostream &operator<<(std::ostream &out, vector3D<T> &point) {
	return out << point.m_x << " " << point.m_y << " " << point.m_z;
}

// public function: documentation in corresponding .h file
tetrahedron::tetrahedron()
{
}

// public function: documentation in corresponding .h file
tetrahedron::tetrahedron(int cornerA, int cornerB, int cornerC, int cornerD)
: m_cornerA(cornerA), m_cornerB(cornerB), m_cornerC(cornerC), m_cornerD(cornerD)
{}

// public function: documentation in corresponding .h file
std::istream &operator>>(std::istream &in, tetrahedron &tetra) {
	return in >> tetra.m_cornerA >> tetra.m_cornerB >> tetra.m_cornerC >> tetra.m_cornerD;
}

// public function: documentation in corresponding .h file
std::ostream &operator<<(std::ostream &out, tetrahedron &point) {
	return out << point.m_cornerA << " " << point.m_cornerB << " " << point.m_cornerC << " " << point.m_cornerD;
}

// public function: documentation in corresponding .h file
triangle::triangle()
{
}

// public function: documentation in corresponding .h file
triangle::triangle(int cornerA, int cornerB, int cornerC)
: m_cornerA(cornerA), m_cornerB(cornerB), m_cornerC(cornerC)
{}

// public function: documentation in corresponding .h file
std::istream &operator>>(std::istream &in, triangle &tetra) {
	return in >> tetra.m_cornerA >> tetra.m_cornerB >> tetra.m_cornerC;
}

// public function: documentation in corresponding .h file
std::ostream &operator<<(std::ostream &out, triangle &point) {
	return out << point.m_cornerA << " " << point.m_cornerB << " " << point.m_cornerC;
}

// public function: documentation in corresponding .h file
line::line() {
}

// public function: documentation in corresponding .h file
line::line(int start, int end) {
	m_start = start;
	m_end = end;
}

// public function: documentation in corresponding .h file
std::ostream &operator<<(std::ostream &out, line &point) {
	return out << "(" << point.m_start << " -> " << point.m_end << ")";
}


bool lessDVector3D::operator() (const dVector3D& a, const dVector3D& b) const {
    return a.m_x < b.m_x || (a.m_x == b.m_x && (a.m_y < b.m_y || (a.m_y == b.m_y && a.m_z < b.m_z)));
}



// f***ing "separate compilation model" and compilers that don't support template-exports bully us
// to restrict the range of template-instantiations to a finite set:

#ifndef USE_EXPORT_KEYWORD

template std::istream &operator>>(std::istream &in, vector3D<double> &point);
template std::ostream &operator<<(std::ostream &out, vector3D<double> &point);
template class vector3D<double>;

// if your compiler does support the "export" keyword, then please define the USE_EXPORT_KEYWORD symbol

#endif // USE_EXPORT_KEYWORD

