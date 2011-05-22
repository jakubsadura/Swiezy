// Copyright 2008 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.


// own header-file
#include "blCommonHelpers.h"

// BaseLib-includes
#include "blMeshTypes.h"

// includes from other Toolkit-Libs
// ...none at the moment...

// Third Party Library includes
#include "boost/cstdint.hpp" // workaround because of missing "stdint.h" (C++ include) in some compilers

// C/C++ includes
#include <sstream>
#include <string>
#include <list>
#include <sys/stat.h>


using namespace base::meshio;


// if the compiler doesn't support the "export" keyword, then it is removed from the file
// and a set of pre-defined instances of the template defined (see end of this file)
#ifndef USE_EXPORT_KEYWORD
  #define export /*nothing*/
#endif


// public function: documentation in corresponding .h file
export template<typename T>
std::string base::meshio::stringify(std::list<T> x) {
	std::ostringstream o;
	o << "[ ";
	
	// first entry without preceded comma
    typename std::list<T>::const_iterator it = x.begin();
	if (it != x.end()) {
    	o << *it;
    	*it++;
    }
	
	// all other entries with preceded comma
	while (it != x.end()) {
    	o << ", " << *it;
    	it++;
    }
    
	o << " ]";
	return o.str();
}

// public function: documentation in corresponding .h file
export template<typename T>
std::string base::meshio::stringify(T x) {
	std::ostringstream o;
	o << x;
	return o.str();
}


// f***ing "separate compilation model" and compilers that don't support template-exports bully us
// to restrict the range of template-instantiations to a finite set:

#ifndef USE_EXPORT_KEYWORD

template std::string base::meshio::stringify(char);
template std::string base::meshio::stringify(short);
template std::string base::meshio::stringify(int);
template std::string base::meshio::stringify(unsigned int);
template std::string base::meshio::stringify(boost::int64_t);
template std::string base::meshio::stringify(boost::uint64_t);
template std::string base::meshio::stringify(float);
template std::string base::meshio::stringify(double);
template std::string base::meshio::stringify(dVector3D);

template std::string base::meshio::stringify(std::list<std::string>);

// if your compiler does support the "export" keyword, then please define the USE_EXPORT_KEYWORD symbol

#endif // USE_EXPORT_KEYWORD

