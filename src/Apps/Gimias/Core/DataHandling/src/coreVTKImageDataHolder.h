// Copyright 2007 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef COREVTKIMAGEDATAHOLDER_H
#define COREVTKIMAGEDATAHOLDER_H

#include "gmDataHandlingWin32Header.h"
#include "coreDataHolder.h"
#include "vtkImageData.h"
#include "vtkSmartPointer.h"

namespace Core{

typedef vtkSmartPointer<vtkImageData> vtkImageDataPtr;
typedef Core::DataHolder< vtkImageData > vtkImageDataHolder;

}

#endif //COREVTKIMAGEDATAHOLDER_H
