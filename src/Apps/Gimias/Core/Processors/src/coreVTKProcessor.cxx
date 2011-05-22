/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreVTKProcessor.h"
#include "vtkInformation.h"
#include "vtkWindowedSincPolyDataFilter.h"
#include "coreVTKPolyDataHolder.h"
#include "coreVTKImageDataHolder.h"

void GMPROCESSORS_EXPORT coreVTKProcessorFunc( )
{
	Core::VTKProcessor<vtkWindowedSincPolyDataFilter>::Pointer processor;
	processor = Core::VTKProcessor<vtkWindowedSincPolyDataFilter>::New( );
	processor->Update( );

	Core::vtkPolyDataPtr polyData;
	processor->GetProcessingData<Core::vtkPolyDataPtr>( 0, polyData, 0 );

}




