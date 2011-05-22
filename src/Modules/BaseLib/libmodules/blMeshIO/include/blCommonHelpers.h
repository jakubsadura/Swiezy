// Copyright 2008 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.


#ifndef BLCommonHelpers_H
#define BLCommonHelpers_H


// BaseLib-includes
// ...none at the moment...

// includes from other Toolkit-Libs
// ...none at the moment...

// Third Party Library includes
// ...none at the moment...

// C/C++ includes
#include <string>
#include <list>


namespace base {
namespace meshio {


//! Creates a human-readable string representation using the << operator
template<typename T>
std::string stringify(std::list<T> x);

//! Creates a human-readable string representation using the << operator
template<typename T>
std::string stringify(T x);


} // meshio
} // base


#endif // BLCommonHelpers_H


