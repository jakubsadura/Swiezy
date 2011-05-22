/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef COREVTKPOLYDATAHOLDER_H
#define COREVTKPOLYDATAHOLDER_H

#include "gmDataHandlingWin32Header.h"
#include "coreDataHolder.h"
#include "vtkPolyData.h"
#include "vtkSmartPointer.h"

namespace Core{

typedef vtkSmartPointer<vtkPolyData> vtkPolyDataPtr;
typedef Core::DataHolder< vtkPolyDataPtr > vtkPolyDataHolder;

}

#endif //COREVTKPOLYDATAHOLDER_H
