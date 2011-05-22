/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "meVTKExtractMainSurfaceFilter.h"
#include "vtkPolyDataConnectivityFilter.h"
#include "meMeshTypes.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"

#include "blShapeUtils.h"

vtkStandardNewMacro(meVTKExtractMainSurfaceFilter);


meVTKExtractMainSurfaceFilter::meVTKExtractMainSurfaceFilter()
{
}


meVTKExtractMainSurfaceFilter::~meVTKExtractMainSurfaceFilter() 
{
}


int meVTKExtractMainSurfaceFilter::RequestData(
	vtkInformation *vtkNotUsed(request),
	vtkInformationVector **inputVector,
	vtkInformationVector *outputVector)
{

	// -------------------------------------------  
	// get the info objects
	vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
	vtkInformation *outInfo = outputVector->GetInformationObject(0);

	// get the input and ouptut
	vtkPolyData *input = vtkPolyData::SafeDownCast(
		inInfo->Get(vtkDataObject::DATA_OBJECT()));

	vtkPolyData *output = vtkPolyData::SafeDownCast(
		outInfo->Get(vtkDataObject::DATA_OBJECT()));

	try
	{
		// Split output, to get connected surfaces one by one
		vtkSmartPointer<vtkPolyDataConnectivityFilter> connectivity = 
				vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
		connectivity->SetInput( input );
		connectivity->SetExtractionModeToLargestRegion();
		connectivity->Update();
		unsigned int numSurfaces = connectivity->GetNumberOfExtractedRegions();

		int iPos = 0;
		
		if ( numSurfaces > 0 && iPos != -1 )
		{
			connectivity->InitializeSpecifiedRegionList();
			connectivity->AddSpecifiedRegion( iPos );
			connectivity->Update();
		}

 		vtkSmartPointer<vtkCleanPolyData> clean = vtkSmartPointer<vtkCleanPolyData>::New();
 		clean->SetInput(connectivity->GetOutput());
 		clean->Update();

		output->DeepCopy(clean->GetOutput() );

	}
	catch( vtkstd::exception& e )
	{
		cout << "ExceptionObject caught !" << endl;
		cout << e.what() << endl;
		return 0;
	}

	return 1;
	
}
