/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "vtkRegularPolygonSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkProperty.h"
#include "vtkCamera.h"
#include "vtkSmartPointer.h"
#include "vtkPieChartActor.h"
#include "vtkFloatArray.h"
#include "blVtkBullsEyeActor.h"
#include "vtkProperty2D.h"
#include "vtkLegendBoxActor.h"
#include "vtkTextProperty.h"
#include "vtkScalarBarActor.h"

int main()
{


	//------------------------------------------------------------------
	int numTuples = 17;

	// Create the array of values to display in the bulls eye plot
	vtkSmartPointer<vtkFloatArray> values = vtkSmartPointer<vtkFloatArray>::New();
	values->SetNumberOfTuples(numTuples);
	for (int i=0; i<numTuples; i++)
	{
		values->SetTuple1( i, vtkMath::Random(0,100));
	}
	vtkSmartPointer<vtkDataObject> dobj = vtkSmartPointer<vtkDataObject>::New();
	dobj->GetFieldData()->AddArray(values);

	
	// Create the actor
	vtkSmartPointer<blVtkBullsEyeActor> bullsEyeActor = vtkSmartPointer<blVtkBullsEyeActor>::New();
	bullsEyeActor->SetInput(dobj);
	bullsEyeActor->SetTitle("Bull's Eye Plot");
	bullsEyeActor->GetPositionCoordinate()->SetValue(0.05,0.1,0.0);
	bullsEyeActor->GetPosition2Coordinate()->SetValue(0.95,0.85,0.0);
	bullsEyeActor->GetProperty()->SetColor(1,1,1);
	bullsEyeActor->SetLegendVisibility( false );

	// Set text colors 
	bullsEyeActor->GetTitleTextProperty()->SetColor(1,1,0);
	bullsEyeActor->GetLabelTextProperty()->SetColor(1,1,1);


  // Create the graphics structure. The renderer renders into the 
  // render window. The render window interactor captures mouse events
  // and will perform appropriate camera or bullsEyeActor manipulation
  // depending on the nature of the events.
  //
  vtkSmartPointer<vtkRenderer> ren1 = vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
  renWin->AddRenderer(ren1);
  vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  iren->SetRenderWindow(renWin);

  // Add the actors to the renderer, set the background and size
  //
  ren1->AddActor(bullsEyeActor);
  ren1->SetBackground(0.1, 0.2, 0.4);
  renWin->SetSize(200, 200);

  // We'll zoom in a little by accessing the camera and invoking a "Zoom"
  // method on it.
  ren1->ResetCamera();
  ren1->GetActiveCamera()->Zoom(1.5);
  renWin->Render();
  
  // This starts the event loop and as a side effect causes an initial render.
  iren->Start();

  return 0;
}




