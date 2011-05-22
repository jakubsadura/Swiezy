/*=========================================================================

  Copyright : (C)opyright (Pompeu Fabra University) 2007++
              See COPYRIGHT statement in top level directory.
  Authors   : Nacho Larrabide
  Modified  : $Author: mbianculli $ $Author: mbianculli $
  Purpose   : Demo application for the centerlineAPI.
  Date      : $Date: 2008-01-30 16:44:11 $
  Version   : $Revision: 1.2 $

=========================================================================*/

//ITK
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
// VTK
#include "vtkPolyData.h"
#include "vtkPolyDataReader.h"
#include "vtkPolyDataWriter.h"
//MeshLib
#include "meVTKPolyLineCombine.h"
#include "meVTKPolyLineDelete.h"
#include "meVTKPolyLineSmooth.h"
#include "meVTKPolyLineShortestPath.h"
#include "meVTKPolyLineClean.h"
#include "meVTKPolyLineTopologyAnalysis.h"

using namespace std;

//Usage example: 

void usage( char *app_name )
{
	cerr << "- This application allows to edit a polydata with many polylines. -" << std::endl;
	cerr << "Usage: " << app_name << " [input polydata] [output polydata]  <parameters>               " << endl;
	cerr << "    -combineCells <int> <int>				<- combine the polylines with the especified ids  " << endl;
	cerr << "    -deleteCell <int>						<- delete cell with specified id                  " << endl;
	cerr << "    -deleteCellsWithPoint <int>			<- delete cells containing point id specified     " << endl;
	cerr << "    -smoothCells <int> <int>				<- smooth a polyline cell (smooth all if -1), specifying cellId and number of iterations" << endl;
	cerr << "    -extractPathBetweenCells <int> <int>   <- compute the path between the especified ids  " << endl;
	cerr << "    -cleanPolyline	<int>					<- cleans (beautifies) the polyline, the integer specifies the max number of points in terminals to be removed" << endl;
	cerr << "    -topologyAnalysis   					<- analyzes the topology of the centerline indicating if the cells are terminals or not and what number of neighbors they have" << endl;
	cerr << "NOTE: please specify only one operation in each execution.						    " << endl;
	exit(1);
}

int main( int argc, char **argv )
{    
	char *input_name = NULL, *output_name = NULL;
	int cell1Id, cell2Id, cellDel, pointId, iters;
	cell1Id = -1;
	cell2Id = -1;
	cellDel = -1;
	pointId = -1;
	iters = -1;
	unsigned int terminal = 0;
	bool extractPathBetweenCells = false;
	bool smoothCells = false;
	bool cleanPolyline = false;
	bool topologyAnalysis = false;
	bool readOk;

	if ( argc < 3 )
		usage( argv[0] );

	input_name  = argv[1];
	argc--;
	argv++;
	output_name = argv[1];
	argc--;
	argv++;

	int ok;
	
	// Parse input arguments
	//while ( argc > 1 )
	//{
	ok = false;

	if ( argc <= 1 )
	{
		ok = true;
	}

	if ( ( ok == false ) && ( strcmp( argv[1], "-combineCells" ) == 0 ) )
	{
		argc--;
		argv++;
		cell1Id = atoi( argv[1] );
		argc--;
		argv++;
		cell2Id = atoi( argv[1] );
		argc--;
		argv++;
		extractPathBetweenCells = false;
		ok = true;
	}

	if ( ( ok == false ) && ( strcmp( argv[1], "-deleteCell" ) == 0 ) )
	{
		argc--;
		argv++;
		cellDel = atoi( argv[1] );
		argc--;
		argv++;
		ok = true;
	}

	
	if ( ( ok == false ) && ( strcmp( argv[1], "-topologyAnalysis" ) == 0 ) )
	{
		argc--;
		argv++;
		topologyAnalysis = true;
		argc--;
		argv++;
		ok = true;
	}


	if ( ( ok == false ) && ( strcmp( argv[1], "-deleteCellsWithPoint" ) == 0 ) )
	{
		argc--;
		argv++;
		pointId = atoi( argv[1] );
		argc--;
		argv++;
		ok = true;
	}

	if ( ( ok == false ) && ( strcmp( argv[1], "-smoothCells" ) == 0 ) )
	{
		argc--;
		argv++;
		cell1Id = atoi( argv[1] );
		argc--;
		argv++;
		iters = atoi( argv[1] );
		argc--;
		argv++;
		smoothCells = true;
		ok = true;
	}

	//centerline
	if ( ( ok == false ) && ( strcmp( argv[1], "-extractPathBetweenCells" ) == 0 ) ) 
	{
		argc--;
		argv++;
		cell1Id = atoi( argv[1] );  
		argc--;
		argv++;
		cell2Id = atoi( argv[1] );	
		argc--;
		argv++;
		extractPathBetweenCells = true;
		ok = true;
	}

	//centerline
	if ( ( ok == false ) && ( strcmp( argv[1], "-cleanPolyline" ) == 0 ) ) 
	{
		argc--;
		argv++;
		terminal = atoi( argv[1] );	
		argc--;
		argv++;
		cleanPolyline = true;
		ok = true;
	}
	
	if ( ok == false )
	{
		cerr << "Can not parse argument " << argv[1] << endl;
		usage( argv[0] );
	}
	//}

	vtkPolyDataReader *polydataReader = vtkPolyDataReader::New();
	polydataReader->SetFileName( input_name );
	try
	{
		polydataReader->Update();
		readOk = true;
	}
	catch (vtkstd::exception& e)
	{
		cout << "ExceptionObject caught!" << endl;
		cout << e.what() << endl;
		readOk = false;
	}

	vtkPolyDataWriter *pdWriter = vtkPolyDataWriter::New();
	pdWriter->SetFileName( output_name );

	if (topologyAnalysis){
		meVTKPolyLineTopologyAnalysis::Pointer topol = meVTKPolyLineTopologyAnalysis::New();
		topol->SetInput( polydataReader->GetOutput() );
		topol->Update();
		pdWriter->SetInput(topol->GetOutput());
	} else if (cleanPolyline) {
		meVTKPolyLineClean::Pointer clean = meVTKPolyLineClean::New();
		clean->SetInput(polydataReader->GetOutput());
		clean->SetTerminalSize(terminal);
		clean->Update();
		//
		// IMPORTANT: Always copy VTK outputs to a new variable when using EvoAPI in DLL mode.
		// Boundary memory problems may happen.
		pdWriter->SetInput( clean->GetOutput() );
	}else if (cell1Id!=-1 && cell2Id!=-1 && !extractPathBetweenCells)	{
		meVTKPolyLineCombine::Pointer combine = meVTKPolyLineCombine::New();
		combine->SetInput(polydataReader->GetOutput());
		combine->CombineByCellId(cell1Id,cell2Id);
		combine->Update();
		// IMPORTANT: Always copy VTK outputs to a new variable when using EvoAPI in DLL mode.
		// Boundary memory problems may happen.
		pdWriter->SetInput( combine->GetOutput() );

	}else if (cell1Id!=-1 && cell2Id!=-1 && extractPathBetweenCells)	{
		meVTKPolyLineShortestPath::Pointer shortestPathExtractor = meVTKPolyLineShortestPath::New(); 
		shortestPathExtractor->SetInput(polydataReader->GetOutput());
		shortestPathExtractor->SetCellsIds(cell1Id,cell2Id);
		shortestPathExtractor->Update();
		// IMPORTANT: Always copy VTK outputs to a new variable when using EvoAPI in DLL mode.
		// Boundary memory problems may happen.
		pdWriter->SetInput( shortestPathExtractor->GetOutput() );

	}else if (smoothCells && iters!=-1)	{
		meVTKPolyLineSmooth::Pointer smooth = meVTKPolyLineSmooth::New();
		smooth->SetInput(polydataReader->GetOutput());
		smooth->SetCellId(cell1Id);
		smooth->SetIterations(iters);
		smooth->Update();
		// IMPORTANT: Always copy VTK outputs to a new variable when using EvoAPI in DLL mode.
		// Boundary memory problems may happen.
		pdWriter->SetInput( smooth->GetOutput() );
	}else if (cellDel!=-1) {
		meVTKPolyLineDelete::Pointer del = meVTKPolyLineDelete::New();
		del->SetInput(polydataReader->GetOutput());
		del->DeleteCellWithId(cellDel);
		del->Update();
		//
		// IMPORTANT: Always copy VTK outputs to a new variable when using EvoAPI in DLL mode.
		// Boundary memory problems may happen.
		pdWriter->SetInput( del->GetOutput() );
	}else if (pointId!=-1){
		meVTKPolyLineSmooth::Pointer smooth = meVTKPolyLineSmooth::New();
		smooth->SetInput(polydataReader->GetOutput());
		smooth->SetCellId(cell1Id);
		smooth->SetIterations(iters);
		smooth->Update();
		// IMPORTANT: Always copy VTK outputs to a new variable when using EvoAPI in DLL mode.
		// Boundary memory problems may happen.
		pdWriter->SetInput( smooth->GetOutput() );
	}else{
		pdWriter->SetInput( polydataReader->GetOutput() );
		cerr << "Parameters set are not valid." << endl;
		usage( argv[0] );
	}

	try
	{
		pdWriter->Update();
	}
	catch (vtkstd::exception& e)
	{
		cout << "ExceptionObject caught !" << endl;
		cout << e.what() << endl;
		return -1;
	}

	return 0;	
}




