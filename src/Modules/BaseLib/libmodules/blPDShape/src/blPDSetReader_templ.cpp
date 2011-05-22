/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

/*
brief   : This class allow to create a new PDSet from a file that 
*/

#include "blPDSetReader_templ.h"
#include "blPDSetReader_templ.txx"

BASELIB_EXPORT template class blPDSetReader_templ<baselib::VnlMatrixType>;
BASELIB_EXPORT template class blPDSetReader_templ<baselib::UblasMatrixType>;
BASELIB_EXPORT template class blPDSetReader_templ<baselib::UblasDiskMatrixType>;
