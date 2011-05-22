/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef COREVTKUNSTRUCTUREDGRIDHOLDER_H
#define COREVTKUNSTRUCTUREDGRIDHOLDER_H

#include "gmDataHandlingWin32Header.h"
#include "coreDataHolder.h"
#include "vtkUnstructuredGrid.h"
#include "vtkSmartPointer.h"

namespace Core{

typedef vtkSmartPointer<vtkUnstructuredGrid> vtkUnstructuredGridPtr;
typedef Core::DataHolder< vtkUnstructuredGridPtr > vtkUnstructuredGridHolder;

}

#endif //COREVTKUNSTRUCTUREDGRIDHOLDER_H
