/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __blMatlabIO_h
#define __blMatlabIO_h

#include "blVector.h"
#include "blMatrix.h"


/// \ingroup blUtilities
void ToMatlab( const double* vector, int length, const char* filename, const char* var_name );

/// \ingroup blUtilities
void ToMatlab( const double* matrix, int rows, int cols, const char* filename, const char* var_name );

/// \note be careful since these reading functions can handle only simple
///  structure, look at the file created by the above writing functions

/// \ingroup blUtilities
void FromMatlab( blVector<double>::VectorType* vector, const char* filename );

/// \ingroup blUtilities
void FromMatlab( vnl_matrix<double>* matrix, const char* filename );

#endif
