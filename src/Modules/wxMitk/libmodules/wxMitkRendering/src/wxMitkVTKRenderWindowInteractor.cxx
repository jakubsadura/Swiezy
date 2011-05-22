/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#include "wxMitkVTKRenderWindowInteractor.h"
#include <vtkCommand.h>
#include <vtkObjectFactory.h>
//#include <mitkVtkRenderWindow.h>
#include <mitkRenderingManager.h>

using namespace mitk;

vtkStandardNewMacro(wxMitkVTKRenderWindowInteractor);

//!
wxMitkVTKRenderWindowInteractor::wxMitkVTKRenderWindowInteractor(void) : 
	 vtkRenderWindowInteractor()
{
}

//!
wxMitkVTKRenderWindowInteractor::~wxMitkVTKRenderWindowInteractor(void)
{
}
