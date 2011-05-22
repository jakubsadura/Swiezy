/*=========================================================================

  Module    : MeshLib
  File      : $RCSfile: meAppHoleFilling.cpp,v $
  Copyright : (C)opyright (Universitat Pompeu Fabra) 2007++
			  See COPYRIGHT statement in top level directory.
  Authors   : Xavier Mellado
  Modified  : $Author: xplanes $
  Purpose   : Hole filling for VTK polydata.
  Date      : $Date: 2008-01-09 08:17:19 $
  Version   : $Revision: 1.1 $
  Changes   : $Locker:  $
			  $Log: not supported by cvs2svn $
			  Revision 1.1  2007/12/18 07:52:56  xplanes
			  - Changed the name of the Applications to meApp... and the name of the demos to meDemo...
			

=========================================================================*/

// VTK
#include <vtkPolyDataReader.h>
#include <meHoleFiller.h>
#include <vtkCleanPolyData.h>
#include <vtkPolyDataWriter.h>
#include <vtkstd/exception>

using namespace std;


void usage( const char *appName )
{
    cerr << "Usage: " << appName << " [input] [output] <options>    \n";
    cerr << "The input/output are VTK PolyData meshes of triangles.	\n";
	cerr << "Options:                                               \n";
    cerr << "\t-alpha <- Alpha value controlling density of polygons. Default [sqrt(2)].  \n";
    cerr << "\t-angle <- Angle between 2 triangles to allow edge swapping. Default[0.0].  \n";
    cerr << "\t          Only coplanar triangles will be swapped.   \n";
    cerr << "\t-minEdges <- Minimum number of edges swapped in one iteration to do another one. Default[10]. \n";
    cerr << "\t-order <- Umbrella operator order when fairing. Default[2]. \n";
    cerr << "\t          0 - None.                                  \n";
    cerr << "\t          1 - Smoothing.                             \n";
    cerr << "\t          2 - Curvature preserving.                  \n";
    cerr << "\t-percentage <- Percentage of first iteration average change to stop iterating when fairing. Default [0.001] (0.01%).  \n";
    cerr << "\t-verbose <- Show information                         \n";
    exit(1);
}


int main(int argc, char *argv[])
{
	const char *input_name = NULL, *output_name = NULL, *app_name = NULL;
	double alpha = 0.0, angle = 0.0, percentage = 0.0001;
	unsigned int minEdges = 10;
	unsigned int order = 10;


	app_name = argv[0];

    if (argc < 3){
      usage( app_name );
    }

    input_name = argv[1];
    argc--;
    argv++;
    output_name = argv[1];
    argc--;
    argv++;

	int   ok;
	bool bVerbose = false;

	while (argc > 1)
	{
		ok = false;

		//Option:
		if ((ok == false) && (strcmp(argv[1], "-verbose") == 0))
		{
			argc--;
			argv++;
			bVerbose = true;
			ok = true;
		}

		if ((ok == false) && (strcmp(argv[1], "-alpha") == 0))
		{
			argc--;
			argv++;
			alpha = atof( argv[1] );
			argc--;
			argv++;
			ok = true;
		}

		if ((ok == false) && (strcmp(argv[1], "-angle") == 0))
		{
			argc--;
			argv++;
			angle = atof( argv[1] );
			argc--;
			argv++;
			ok = true;
		}

		if ((ok == false) && (strcmp(argv[1], "-percentage") == 0))
		{
			argc--;
			argv++;
			percentage = atof( argv[1] );
			argc--;
			argv++;
			ok = true;
		}

		if ((ok == false) && (strcmp(argv[1], "-minEdges") == 0))
		{
			argc--;
			argv++;
			minEdges = atoi( argv[1] );
			argc--;
			argv++;
			ok = true;
		}

		if ((ok == false) && (strcmp(argv[1], "-order") == 0))
		{
			argc--;
			argv++;
			order = atoi( argv[1] );
			argc--;
			argv++;
			ok = true;
		}

		if (ok == false)
		{
			cerr << "Can not parse argument " << argv[1] << endl;
			usage( app_name );
		}
	}

	if ( bVerbose ) cout << "Reading PolyData input:  " << input_name << endl;
	vtkPolyDataReader *reader = vtkPolyDataReader::New();
	reader->SetFileName(input_name);
	try
    {
		reader->Update();
    }
    catch( vtkstd::exception& e )
    {
		cout << "ExceptionObject caught !" << endl;
		cout << e.what() << endl;
		return -1;
    }

	if ( bVerbose )
		cout << "Original surface has:\n\t"
			<< reader->GetOutput()->GetNumberOfPoints()  << " Points\n\t"
			<< reader->GetOutput()->GetNumberOfCells ()  << " Cells	 "
			<< "\n\t\tverts : " << reader->GetOutput()->GetNumberOfVerts ()
			<< "\n\t\tlines : " << reader->GetOutput()->GetNumberOfLines ()
			<< "\n\t\tpolys : " << reader->GetOutput()->GetNumberOfPolys ()
			<< "\n\t\tstrips: " << reader->GetOutput()->GetNumberOfStrips()<<endl;


	meHoleFiller::Pointer holeFilling = meHoleFiller::New();
	holeFilling->SetInput( reader->GetOutput() );
	holeFilling->SetDensityFactor( alpha );
	holeFilling->SetAngle( angle );
	holeFilling->SetOrder( order );
	holeFilling->SetPercentage( percentage );
	holeFilling->SetMinimumEdgesSwapped( minEdges );
	try
    {
		holeFilling->Update();
    }
    catch( vtkstd::exception& e )
    {
		cout << "ExceptionObject caught !" << endl;
		cout << e.what() << endl;
		return -1;
    }

	//vtkCleanPolyData *clean = vtkCleanPolyData::New();
	//clean->SetInput( holeFilling->GetOutput() );
	//clean->Update();

	//if ( bVerbose )
	//	cout << "\n---------------------------------------------" << endl;
	//	cout<< "After of the final cleaning the surface has:\n\t"
	//		<< clean->GetOutput()->GetNumberOfPoints()  << " Points\n\t"
	//		<< clean->GetOutput()->GetNumberOfCells ()  << " Cells	 "
	//		<< "\n\t\tverts : " << clean->GetOutput()->GetNumberOfVerts ()
	//		<< "\n\t\tlines : " << clean->GetOutput()->GetNumberOfLines ()
	//		<< "\n\t\tpolys : " << clean->GetOutput()->GetNumberOfPolys ()
	//		<< "\n\t\tstrips: " << clean->GetOutput()->GetNumberOfStrips() <<endl;

	if ( bVerbose ) cout << " Writting to disk ..." << endl;
	vtkPolyDataWriter *writer = vtkPolyDataWriter::New();
//	writer->SetInput( clean->GetOutput() );
	writer->SetInput( holeFilling->GetOutput() );
	writer->SetFileName( output_name );
	try
    {
		writer->Write();
		//writer->Update();
    }
    catch( vtkstd::exception& e )
    {
		cout << "ExceptionObject caught !" << endl;
		cout << e.what() << endl;
		return -1;
    }

	reader->Delete();
//	clean->Delete();
	writer->Delete();
}
