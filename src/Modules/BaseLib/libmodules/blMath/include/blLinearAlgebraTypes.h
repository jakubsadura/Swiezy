/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef BLLINEARALGEBRATYPES_H
#define BLLINEARALGEBRATYPES_H

#include "CILabNamespaceMacros.h"
#include "blUblasConvertibleArray.h"
#include <vnl/vnl_matrix.h>
#include <vnl/vnl_vector.h>
#include <vnl/vnl_vector_ref.h>
#include "blUblasMatrix.h"
#include "boost/numeric/ublas/vector.hpp"
#include "boost/shared_ptr.hpp"
#include "blStoragePolicy.h"

namespace Ublas = boost::numeric::ublas;

/**
This namespace declares standard linear algebra types.

\author Maarten Nieber
\date 09 jun 2008
*/

CILAB_BEGIN_NAMESPACE(baselib)

typedef vnl_matrix<double> VnlMatrixType;
typedef vnl_vector<double> VnlVectorType;
typedef vnl_vector_ref<double> VnlVectorRefType;
typedef Ublas::matrix<double, boost::numeric::ublas::row_major, blUblasConvertibleArray<double> > UblasMatrixType;
typedef Ublas::matrix<double, boost::numeric::ublas::row_major, blDiskStoragePolicy<double> > UblasDiskMatrixType;
typedef Ublas::vector<double, blUblasConvertibleArray<double> > UblasVectorType;
typedef boost::shared_ptr<VnlMatrixType> VnlMatrixPointer;
typedef boost::shared_ptr<VnlVectorType> VnlVectorPointer;
typedef boost::shared_ptr<const VnlMatrixType> ConstVnlMatrixPointer;
typedef boost::shared_ptr<const VnlVectorType> ConstVnlVectorPointer;

CILAB_END_NAMESPACE(baselib)

#endif //BLLINEARALGEBRATYPES_H
