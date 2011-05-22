/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "blProcrustesAlignmentFilter.h"
#include "blProcrustesAlignmentFilter.txx"

BASELIB_EXPORT template class blProcrustesAlignmentFilter<baselib::VnlMatrixType>;
BASELIB_EXPORT template class blProcrustesAlignmentFilter<baselib::UblasMatrixType>;
BASELIB_EXPORT template class blProcrustesAlignmentFilter<baselib::UblasDiskMatrixType>;
