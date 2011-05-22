/*=========================================================================

  Module    : MeshLib
  File      : $RCSfile: meAppSplit.cpp,v $
  Copyright : (C)opyright (Universitat Pompeu Fabra) 2007++
			  See COPYRIGHT statement in top level directory.
  Authors   : Xavier Mellado
  Modified  : $Author: xplanes $
  Purpose   : Split polydata executable.
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
#include <meVTKPolyDataSplitter.h>
#include <vtkCleanPolyData.h>
#include <vtkPolyDataWriter.h>
#include <vtkstd/exception>

using namespace std;


void usage( const char *appName )
{
    cerr << "Usage: " << appName << " [input] [cellId_List] [output_surrounding] [output_patch] <options>    \n";
    cerr << "The input/output are VTK PolyData meshes of triangles.	\n";
	cerr << "Options:                                               \n";
    cerr << "\t-verbose <- Show information                         \n";
//    cerr << "\t-readCellList <filename> <- Read from a file input cell ids to be splitted. \n";
    cerr << "\t-tag <- Read cell tag added to output file. It will overwrite -readCellList if present. \n";
    exit(1);
}


int main(int argc, char *argv[])
{
	const char *input_name = NULL, *surrounding_name = NULL, *patch_name = NULL, *app_name = NULL, *cellIdList_name = NULL;
	fstream file;
	vtkIdList *list = vtkIdList::New();

	app_name = argv[0];

    if (argc < 5){
      usage( app_name );
    }

    input_name = argv[1];
    argc--;
    argv++;
	cellIdList_name = argv[1];
	argc--;
	argv++;
    surrounding_name = argv[1];
    argc--;
    argv++;
    patch_name = argv[1];
    argc--;
    argv++;

	bool ok = false;
	bool bVerbose = false;
	bool readCellList = false;
	bool tag = false;

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
/*
		if ((ok == false) && (strcmp(argv[1], "-readCellList") == 0))
		{
			argc--;
			argv++;
			readCellList = true;
			cellIdList_name = argv[1];
			argc--;
			argv++;
			ok = true;
		}
*/
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


	//-----------------------------------------------------
	// Read cell id list from a file
	//-----------------------------------------------------
	vtkIdType numIds = 0;

	// Create new file
	file.open( cellIdList_name, ios::in );
	if ( !file )
		cout << " Error: File " << cellIdList_name << " can't be opened for reading." << endl;

	// Header
	if ( !( file >> numIds ) )
		cout << " Error: Can't read from file " << cellIdList_name << endl;;

	vtkIdType id = 0;

	for ( vtkIdType i = 0; i < numIds; i++ )
	{
		if ( !( file >> id ) )
			cout << " Error: Can't read from file " << cellIdList_name << endl;

		list->InsertNextId( id );
	}

	file.close();


	meVTKPolyDataSplitter::Pointer splitter = meVTKPolyDataSplitter::New();
	splitter->SetInput( reader->GetOutput() );
	splitter->SetCellIdList( list );
	splitter->SetTagging( tag );
	try
    {
		splitter->Update();
    }
    catch( vtkstd::exception& e )
    {
		cout << "ExceptionObject caught !" << endl;
		cout << e.what() << endl;
		return -1;
    }

	vtkPolyData *surrounding, *patch;
	splitter->GetOutput( surrounding, patch );


	if ( bVerbose )
		cout << "\n---------------------------------------------" << endl;
		cout<< "After processing surrounding surface has:\n\t"
			<< surrounding->GetNumberOfPoints()  << " Points\n\t"
			<< surrounding->GetNumberOfCells ()  << " Cells	 "
			<< "\n\t\tverts : " << surrounding->GetNumberOfVerts ()
			<< "\n\t\tlines : " << surrounding->GetNumberOfLines ()
			<< "\n\t\tpolys : " << surrounding->GetNumberOfPolys ()
			<< "\n\t\tstrips: " << surrounding->GetNumberOfStrips() << endl
			<< "And patch surface has:\n\t"
			<< patch->GetNumberOfPoints()  << " Points\n\t"
			<< patch->GetNumberOfCells ()  << " Cells	 "
			<< "\n\t\tverts : " << patch->GetNumberOfVerts ()
			<< "\n\t\tlines : " << patch->GetNumberOfLines ()
			<< "\n\t\tpolys : " << patch->GetNumberOfPolys ()
			<< "\n\t\tstrips: " << patch->GetNumberOfStrips() << endl;

	if ( bVerbose ) cout << " Writting to disk ..." << endl;
	vtkPolyDataWriter *writer = vtkPolyDataWriter::New();
	vtkPolyDataWriter *writer2 = vtkPolyDataWriter::New();

	writer->SetInput( surrounding );
	writer2->SetInput( patch );

	writer->SetFileName( surrounding_name );
	writer2->SetFileName( patch_name );
	try
    {
		writer->Write();
		writer2->Write();
    }
    catch( vtkstd::exception& e )
    {
		cout << "ExceptionObject caught !" << endl;
		cout << e.what() << endl;
		return -1;
    }

	list->Delete();
	reader->Delete();
	writer->Delete();
	writer2->Delete();
}
