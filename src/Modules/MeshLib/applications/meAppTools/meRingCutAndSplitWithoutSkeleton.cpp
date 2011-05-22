/*=========================================================================

  Module    : MeshLib
  File      : meRingCutAndSplitWithoutSkeleton.cpp
  Copyright : (C)opyright (Pompeu Fabra University) 2005++
              See COPYRIGHT statement in top level directory.
  Authors   : Jose M. Pozo
  Modified  : $Author: jpozo $
  Purpose   : Demo for the automatic cut of a mesh perpendicularly to the vessel.
  Date      : 2008/09/23
  Version   : $Revision: $
  Changes   : $Locker:  $
              $Log: meRingCutAndSplitWithoutSkeleton.cpp,v $


=========================================================================*/
#include <meRingCut.h>
#include <meNormalPlaneToVessel.h>
#include <vtkPolyDataReader.h>
#include <vtkPolyDataWriter.h>
#include <vtkPolyDataMapper.h>
#include <vtkCamera.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkProperty.h>

using namespace std;

//Functions
void Usage(void);
string  itoa(int value, unsigned int base);

//----------------------------------------------------------------------------
int main(int argc, char **argv)
{

        if (argc < 4){
                Usage();
        }
        char *input_name=NULL;
        char *output_name=NULL;
	char *secondOutputName = NULL;
	char *ringName = NULL;


        input_name =argv[1];
        argc--;
        argv++;
        output_name =argv[1];
        argc--;
        argv++;

        int pointId,
	    point2Id;

        bool ok,
             verbose = false,
	     bRender = true,
	     secondPointSet = false;

        while (argc > 1)
        {
                ok = false;
                //Options:
                if (!ok && strcmp(argv[1], "-pointId")==0)
                {
                        argc--;
                        argv++;
                        pointId = atoi(argv[1]);
                        argc--;
                        argv++;
                        ok = true;
                }
                if (!ok && strcmp(argv[1], "-point2Id")==0)
                {
                        argc--;
                        argv++;
                        point2Id = atoi(argv[1]);
                        argc--;
                        argv++;
                        ok = true;
			secondPointSet = true;
                }
                if (!ok && strcmp(argv[1], "-secondOutput")==0)
                {
                        argc--;
                        argv++;
                        secondOutputName = argv[1];
                        argc--;
                        argv++;
                        ok = true;
                }
                 if (!ok && strcmp(argv[1], "-ring")==0)
                {
                        argc--;
                        argv++;
                        ringName = argv[1];
                        argc--;
                        argv++;
                        ok = true;
                }
               if (!ok && strcmp(argv[1], "-nr") == 0)
                {
                        argc--;
                        argv++;
                        bRender = false;
                        ok = true;
                }
                if (!ok && ( strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--verbose") == 0 ))
                {
                        argc--;
                        argv++;
                        verbose = true;
                        ok = true;
                }
                if (!ok)
		{
                        cerr << "Can not parse argument " << argv[1] << endl;
                        Usage();
                }
	}

	vtkPolyDataReader * readerMesh = vtkPolyDataReader::New();
	readerMesh->SetFileName(input_name);
	readerMesh->Update();

	meNormalPlaneToVessel::Pointer normalCorrector = meNormalPlaneToVessel::New();
	
// 	normalCorrector->SetSurfaceMesh(readerMesh->GetOutput());
// 	normalCorrector->SetOriginMeshId(pointId);
// 	normalCorrector->InitializeNormal();
// 	normalCorrector->Update();
// 	normalCorrector->GetNormal(normal);
// 	normalCorrector->GetCenter(center);

	meRingCut::Pointer ring = meRingCut::New();

	ring->SetInput(readerMesh->GetOutput());
	readerMesh->Delete();

	ring->SetOriginMeshId(pointId);
	if(secondPointSet) ring->SetSecondOriginMeshId(point2Id);
//	ring->SetPoint(center);
//	ring->SetNormal(normal);
	ring->Update();

cout << "Radius = " << ring->GetRadius() << endl;
cout << "Center = " << ring->GetCenter()[0] << " , " << ring->GetCenter()[1] << " , " << ring->GetCenter()[2] << endl;

	    //Writing top disk
        vtkPolyDataWriter *writer = vtkPolyDataWriter::New();
        writer->SetFileName(output_name);
        writer->SetInput(ring->GetFirstOutput());
        writer->Write();
        writer->Update();

	if( secondOutputName )
	{
		writer->SetFileName(secondOutputName);
		writer->SetInput(ring->GetSecondOutput());
		writer->Write();
		writer->Update();
	}

	if( ringName )
	{
		writer->SetFileName(ringName);
		writer->SetInput(ring->GetContourCrossSection());
		writer->Write();
		writer->Update();
	}

        if(bRender)
        {
                vtkPolyDataMapper*  mapper = vtkPolyDataMapper::New();
                mapper->SetInput(ring->GetFirstOutput());
                mapper->ScalarVisibilityOff();

                vtkActor* actor = vtkActor::New();
                actor->SetMapper(mapper);
                actor->GetProperty()->SetColor(1.0000, 0.3882, 0.2784);
                actor->RotateX(30.0);
                actor->RotateY(60.0);
                actor->RotateZ(30.0);

                vtkRenderer *ren = vtkRenderer::New();
                vtkRenderWindow *renWin = vtkRenderWindow::New();
                renWin->AddRenderer(ren);
                vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
                iren->SetRenderWindow(renWin);
                ren->AddActor(actor);
                ren->SetBackground(1., 1., 1.);
                renWin->SetSize(700, 700);

                // We'll zoom in a little by accessing the camera and invoking a "Zoom"
                // method on it.
                renWin->Render();

                // This starts the event loop and as a side effect causes an initial render.
                iren->Start();

                // Exiting from here, we have to delete all the instances that
                // have been created.
                mapper->Delete();
                actor ->Delete();
                ren   ->Delete();
                renWin->Delete();
                iren  ->Delete();
        }

        writer  ->Delete();

}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

void   Usage() //TODO: do
{
	exit(1);
}
////-----------------------------------------------------------------------------
