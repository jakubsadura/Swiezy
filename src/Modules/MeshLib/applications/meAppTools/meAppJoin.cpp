/*=========================================================================

  Module    : MeshLib
  File      : $RCSfile: meAppJoin.cpp,v $
  Copyright : (C)opyright (Universitat Pompeu Fabra) 2007++
			  See COPYRIGHT statement in top level directory.
  Authors   : Xavier Mellado
  Modified  : $Author: xplanes $
  Purpose   : Join polydata executable.
  Date      : $Date: 2008-01-09 08:17:19 $
  Version   : $Revision: 1.1 $
  Changes   : $Locker:  $
			  $Log: not supported by cvs2svn $
			  Revision 1.1  2007/12/18 07:52:56  xplanes
			  - Changed the name of the Applications to meApp... and the name of the demos to meDemo...
			

=========================================================================*/

#include <fstream>

// VTK
#include <vtkPolyDataReader.h>
#include <meVTKPolyDataJoiner.h>
#include <vtkCleanPolyData.h>
#include <vtkPolyDataWriter.h>
#include <vtkstd/exception>

using namespace std;


void usage( const char *appName )
{
    cerr << "Usage: " << appName << " [input_surrounding] [input_patch] [output] <options>    \n";
    cerr << "The input/output are VTK PolyData meshes of triangles.	\n";
	cerr << "Options:                                               \n";
    cerr << "\t-verbose <- Show information                         \n";
    cerr << "\t-writeCellList <filename> <- Write to a file output cell ids of joined cells. \n";
    cerr << "\t-tag <- Tag cells added to output file.              \n";
    exit(1);
}


int main(int argc, char *argv[])
{
	const char *surrounding_name = NULL, *patch_name = NULL, *output_name = NULL, *app_name = NULL, *cellIdList_name = NULL;
	fstream file;


	app_name = argv[0];

    if (argc < 5){
      usage( app_name );
    }

    surrounding_name = argv[1];
    argc--;
    argv++;
    patch_name = argv[1];
    argc--;
    argv++;
    output_name = argv[1];
    argc--;
    argv++;


	bool ok = false;
	bool bVerbose = false;
	bool tag = false;
	bool writeCellList = false;


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

		if ((ok == false) && (strcmp(argv[1], "-writeCellList") == 0))
		{
			argc--;
			argv++;
			writeCellList = true;
			cellIdList_name = argv[1];
			argc--;
			argv++;
			ok = true;
		}

		if ((ok == false) && (strcmp(argv[1], "-tag") == 0))
		{
			argc--;
			argv++;
			tag = true;
			ok = true;
		}

		if (ok == false)
		{
			cerr << "Can not parse argument " << argv[1] << endl;
			usage( app_name );
		}
	}

	if ( bVerbose ) cout << "Reading PolyData inputs:  " << surrounding_name << " " << patch_name << endl;
	vtkPolyDataReader *reader = vtkPolyDataReader::New();
	vtkPolyDataReader *reader2 = vtkPolyDataReader::New();
	reader->SetFileName(surrounding_name);
	reader2->SetFileName(patch_name);
	try
    {
		reader->Update();
		reader2->Update();
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
			<< "\n\t\tstrips: " << reader->GetOutput()->GetNumberOfStrips()
			<< "\nPatch surface has:\n\t"
			<< reader2->GetOutput()->GetNumberOfPoints()  << " Points\n\t"
			<< reader2->GetOutput()->GetNumberOfCells ()  << " Cells	 "
			<< "\n\t\tverts : " << reader2->GetOutput()->GetNumberOfVerts ()
			<< "\n\t\tlines : " << reader2->GetOutput()->GetNumberOfLines ()
			<< "\n\t\tpolys : " << reader2->GetOutput()->GetNumberOfPolys ()
			<< "\n\t\tstrips: " << reader2->GetOutput()->GetNumberOfStrips()<<endl;


	meVTKPolyDataJoiner::Pointer joiner = meVTKPolyDataJoiner::New();
	joiner->SetInput( reader->GetOutput(), reader2->GetOutput() );
	joiner->SetTagging( tag );
	try
    {
		joiner->Update();
    }
    catch( vtkstd::exception& e )
    {
		cout << "ExceptionObject caught !" << endl;
		cout << e.what() << endl;
		return -1;
    }

	if ( bVerbose )
		cout << "\n---------------------------------------------" << endl;
		cout<< "After processing surface has:\n\t"
			<< joiner->GetOutput()->GetNumberOfPoints()  << " Points\n\t"
			<< joiner->GetOutput()->GetNumberOfCells ()  << " Cells	 "
			<< "\n\t\tverts : " << joiner->GetOutput()->GetNumberOfVerts ()
			<< "\n\t\tlines : " << joiner->GetOutput()->GetNumberOfLines ()
			<< "\n\t\tpolys : " << joiner->GetOutput()->GetNumberOfPolys ()
			<< "\n\t\tstrips: " << joiner->GetOutput()->GetNumberOfStrips() << endl;

	if ( bVerbose ) cout << " Writting to disk ..." << endl;
	vtkPolyDataWriter *writer = vtkPolyDataWriter::New();
	writer->SetInput( joiner->GetOutput() );
	writer->SetFileName( output_name );
	try
    {
		writer->Write();
    }
    catch( vtkstd::exception& e )
    {
		cout << "ExceptionObject caught !" << endl;
		cout << e.what() << endl;
		return -1;
    }

	if ( writeCellList )
	{
		//-----------------------------------------------------
		// Write cell id list to a file
		//-----------------------------------------------------

		vtkIdList *list = joiner->GetCellList();

		// Create new file
		file.open( cellIdList_name, ios::out );
		if ( !file )
			cout << " Error: File " << cellIdList_name << " can't be opened for writing." << endl;

		// Header
		if ( !( file << list->GetNumberOfIds() << endl ) )
			cout << " Error: Can't write to file " << cellIdList_name << endl;;

		for ( vtkIdType i = 0; i < list->GetNumberOfIds(); i++ )
		{
			if ( !( file << list->GetId( i ) << endl ) )
				cout << " Error: Can't write to file " << cellIdList_name << endl;;
		}

		file.close();
	}

	reader->Delete();
	reader2->Delete();
	writer->Delete();
}
