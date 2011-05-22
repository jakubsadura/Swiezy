/*=========================================================================
	Library   : Applications
  	Module    : $RCSfile: meHoleFiller.cxx,v $
  	Authors   : Jose M. Pozo 
  	Copyright : Pompeu Fabra University, 2005
  	Purpose   : Demo for closing holes in a vtkPolyData mesh using 
	            the class CloseHoles, able to apply different algorithms.
  	Date      : $Date: 2009/10/01 $
  	Version   : $Revision: 1.1 $
  	Changes   : $Locker:  $
=========================================================================*/

//VTK lib
#include <vtkstd/exception>
#include <vtkPolyDataReader.h>
#include <vtkPolyDataWriter.h>

#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkProperty.h>
#include <vtkCamera.h>

#include <vtkPoints.h>
#include <vtkCellArray.h>

// meshlib
#include <meCloseHoles.h>

//Standard
#include <iostream>
#include <vector>

using namespace std;

void usage()
{
	cerr << "Usage: closeholes [input] [output] <option>" 				<< endl;
	cerr << "The input/output are Poly Data meshes of triangles" 			<< endl;
	cerr << "Options:" 								<< endl;
	cerr << " -v, --verbose    Details the readed/writed file proporties" 		<< endl;
	cerr << " -nr              No Rendering" 					<< endl;
	cerr << " -method <int>:\n" 							<< endl;
	cerr << "\t 0: A single point is added in the center." 				<< endl;
	cerr << "\t 1: A series of points are added in each radius from the center." 	<< endl;
	cerr << "\t 2: A series of linearly distributed concentric coronas \n" <<
		"\t\t  with the same number of points." 				<< endl;
	cerr << "\t 3: A series of linearly distributed concentric coronas \n" <<
		"\t\t  decreasing the number of points towards the center." 		<< endl;
	cerr << "\t 4: A series of roundedly distributed concentric coronas \n" <<
		"\t\t  decreasing the number of points towards the center (default)."	<< endl;
	exit(1); 
}


int main(int argc, char *argv[])
{
    if (argc < 3){
      usage();
    }

	const char * input_name = NULL, * output_name=NULL;  
    input_name = argv[1];
    argc--;
    argv++;
    output_name = argv[1];
    argc--;
    argv++;
  
    bool bRender 	= true;
	bool ok;
	bool verbose = false;
	unsigned int method=4;
    
	while (argc > 1)
	{
		ok = false;   
		//Option:
		if (!ok && strcmp(argv[1],"-nr")==0)
		{
			argc--;
			argv++;
			bRender = false;
			ok = true;
		}
		if (!ok && (strcmp(argv[1],"-v")==0 || strcmp(argv[1],"--verbose") == 0) )
		{
			argc--;
			argv++;
			verbose = true;
			ok = true;
		}
		if (!ok && strcmp(argv[1], "-method")==0)
		{
			argc--;
			argv++;
			if(argv[1] == NULL)
			{
				cout << "ERROR:: method IS NULL"<< endl;
				usage();
			}
			method = atoi(argv[1]);
			argc--;
			argv++;
			ok = true;
		}
		if (!ok)
		{
			cerr << "Can not parse argument " << argv[1] << endl;
			usage();
		}
	} 

	vtkPolyDataReader *reader = vtkPolyDataReader::New();
	reader->SetFileName(input_name);
	try
	{
		reader->Update();
	}
	catch (vtkstd::exception& e)
	{
		cout << "ExceptionObject caught !" << endl;
		cout << e.what() << endl;
		return -1;
	}

	if(verbose)
	{
		cout << "Reading PolyData input:  " << input_name << endl;
		cout << "Original surface has:\n\t" 
			 << reader->GetOutput()->GetNumberOfPoints()  << " Points\n\t"
			 << reader->GetOutput()->GetNumberOfCells ()  << " Cells	 "
			 << "\n\t\tverts : " << reader->GetOutput()->GetNumberOfVerts () 
			 << "\n\t\tlines : " << reader->GetOutput()->GetNumberOfLines () 
			 << "\n\t\tpolys : " << reader->GetOutput()->GetNumberOfPolys ()  
			 << "\n\t\tstrips: " << reader->GetOutput()->GetNumberOfStrips()<<endl;
	}


	meCloseHoles::Pointer meshcloser = meCloseHoles::New();
	meshcloser->SetAlgorithm((meCloseHoles::closeHoleAlgorithmEnum)method);
	meshcloser->SetInput(reader->GetOutput());
	meshcloser->Update();
	

	vtkPolyDataWriter *writer = vtkPolyDataWriter::New();
	writer->SetInput(meshcloser->GetOutput());
	writer->SetFileName(output_name);
	writer->Write();
	writer->Update();

//  meshcloser->Delete();

    if(bRender) 
    {
        vtkPolyDataMapper*  mapper = vtkPolyDataMapper::New();
        mapper->SetInput(meshcloser->GetOutput());
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
        actor->Delete();
        ren->Delete();
        renWin->Delete();
        iren->Delete();
    } 
    reader 	->Delete();
    writer 	->Delete();
}
