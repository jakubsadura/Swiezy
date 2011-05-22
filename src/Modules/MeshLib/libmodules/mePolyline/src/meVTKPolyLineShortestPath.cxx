#include "meVTKPolyLineShortestPath.h"

// Copyright 2007 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

using namespace itk;

#include "CILabAssertMacros.h"

// VTK
#include <vtkPoints.h>
#include <vtkPolyLine.h>
#include <vtkGenericCell.h>
#include <vtkIdList.h>

// BoostGraphLib

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/format.hpp>

using namespace boost;

typedef adjacency_list < listS, vecS, undirectedS,
no_property, property < edge_weight_t, int > > graph_t;
typedef graph_traits < graph_t >::vertex_descriptor vertex_descriptor;
typedef graph_traits < graph_t >::edge_descriptor edge_descriptor;
typedef std::pair<int, int> Edge;

meVTKPolyLineShortestPath::meVTKPolyLineShortestPath()
{
	// Input initialized to do DeepCopy at SetInput function
	this->input = NULL;
	this->output = NULL;
	this->cellId1 = -1;
	this->cellId2 = -1;
}

//--------------------------------------------------
meVTKPolyLineShortestPath::~meVTKPolyLineShortestPath()
//--------------------------------------------------
{
}

//--------------------------------------------------
void meVTKPolyLineShortestPath::Update()
//--------------------------------------------------
{
	//determine the total number of cells in the PolyLine file
	vtkIdType nrCells = this->input->GetNumberOfCells();

	if( this->input == NULL )
		return;

	cilabAssertMacro(this->cellId1 >= 0);
	cilabAssertMacro(this->cellId2 >= 0);
	cilabAssertMacro(this->cellId1 < nrCells);
	cilabAssertMacro(this->cellId2 < nrCells);

	//std::cout << "meVTKPolyLineShortestPath::Update()" << std::endl;
	this->output = vtkPolyData::New();

	//structure of the graph in accordance with boostGraphLib
	///the numbers of nodes is equal to the number of cells in the file
	edge_descriptor e; bool inserted;
	const int nrNodes = nrCells;
	graph_t g(nrNodes);	
	property_map<graph_t, edge_weight_t>::type weightmap = get(edge_weight, g);

	///////////////////////////////////////////////////////////////
	//create the graph
	///////////////////////////////////////////////////////////////
	for ( int i=0; i<nrCells; i++)
	{
		vtkGenericCell *cell1 = vtkGenericCell::New();
		vtkGenericCell *cell2 = vtkGenericCell::New();

		this->input->GetCell(i, cell1);	//the cells to be combined

		if (cell1!=NULL && cell2!=NULL)
		{
			if ((cell1->GetCellType()!=VTK_POLY_LINE)||(cell1->GetCellType()!=VTK_LINE) && 
				(cell2->GetCellType()!=VTK_POLY_LINE)||(cell2->GetCellType()!=VTK_LINE))
			{
				vtkIdType NPCell1 = cell1->GetNumberOfPoints(); //number of points of cell1
				vtkIdType cell1Id0 = cell1->GetPointId(0);	//the first point of the cell1
				vtkIdType cell1IdN = cell1->GetPointId(NPCell1-1);	//the last point of the cell1

				//compare with the first and last point of the remaining cells
				for ( int j=i+1; j<nrCells; j++ )
				{
					//find the points of cell2, and the total number of points of it
					this->input->GetCell(j, cell2);
					vtkIdType NPCell2 = cell2->GetNumberOfPoints();	//number of points of cell2
					vtkIdType cell2Id0 = cell2->GetPointId(0);	//the first point of the cell2
					vtkIdType cell2IdN = cell2->GetPointId(NPCell2-1);	//the last point of the cell2

					///Compare de first and last point of the cells in order to test connectivity
					if (  cell1Id0 == cell2Id0 || cell1Id0 == cell2IdN ||  cell1IdN == cell2Id0 ||
						cell1IdN == cell2IdN )
					{
						/// Fill graph. 
						/// The Id of the cell to the graph. Te line number corresponds to the cellID
						tie(e, inserted) = add_edge(i, j, g); 
						weightmap[e] = 1.0; // all weights in the graph are 1.0
					}
				}
			}
			else
			{
				std::string message = (boost::format
				(
					"Cell type not line(3) or polyline(4). Cell 1 type : %1%. Cell 2 type : %2%"
				)
				% this->input->GetCellType(cellId1) % this->input->GetCellType(cellId2)).str();
				throw message;
			}
		}
	}

	// the map p stores the predecessor (p[i] returns the predecessor of i in the shortest path to the source
	std::vector<vertex_descriptor> p(num_vertices(g));

	// d contains the distance (you can ignore it)
	std::vector<int> d(num_vertices(g));

	// set the source node: this->cellId1
	int source = this->cellId1;
	vertex_descriptor s = vertex(source, g); // start the search in node cellId1

	// run dijkstra
	property_map<graph_t, vertex_index_t>::type indexmap = get(vertex_index, g);
	dijkstra_shortest_paths(g, s, &p[0], &d[0], weightmap, indexmap, 
		std::less<int>(), closed_plus<int>(), 
		(std::numeric_limits<int>::max)(), 0,
		default_dijkstra_visitor());


	///////////////////////////////////////////////////////////////
	//create the output file   
	///////////////////////////////////////////////////////////////
	int vertex = this->cellId2;
	int currentNode = vertex; //the searh is stared from the second point
	std::vector<int> path;
	while( currentNode != source)
	{
		path.push_back(currentNode);

		currentNode = p[currentNode]; // go to the predecessor
	}
	path.push_back(source);

	this->output = vtkPolyData::New();
	vtkPolyLine *cellNew = vtkPolyLine::New(); //to store the points ids of the shortest path

	//copy the points from the input
	vtkPoints *InputPoints = vtkPoints::New();	//represent and manipulate 3D points
	InputPoints->DeepCopy(this->input->GetPoints());

	vtkPoints *OutputPoints = vtkPoints::New(); //the list of points of the path
	vtkCell *cellAux1 = vtkPolyLine::New();
	vtkCell *cellAux2 = vtkPolyLine::New();

	
	vtkIdType newCellPtId = 0;	//the first ID of the points of the polyLine 

	for( unsigned i = 0; i<path.size()-1; i++ ) 
	{
		vtkIdType auxId;

		int lastNode = path.size()-1;
		cellAux1->DeepCopy(this->input->GetCell(path[i]));
		cellAux2->DeepCopy(this->input->GetCell(path[i+1]));
		vtkIdType NPCell1 = cellAux1->GetNumberOfPoints();
		vtkIdType NPCell2 = cellAux2->GetNumberOfPoints();

		// we add all the points of the path but not the last
		if( cellAux1->GetPointId(NPCell1-1) == cellAux2->GetPointId(0) ||
			cellAux1->GetPointId(NPCell1-1) == cellAux2->GetPointId(NPCell2-1))
		{
			for (int j=0; j<NPCell1-1; j++)	
			{
				auxId = cellAux1->GetPointId(j);
				double *aux = InputPoints->GetPoint(auxId);
				OutputPoints->InsertNextPoint( aux );
				cellNew->GetPointIds()->InsertNextId(newCellPtId++);
			}
		}
		else 
		{	
			for (int j=NPCell1-1; j>0; j--) 
			{
				auxId = cellAux1->GetPointId(j);
				double *aux = InputPoints->GetPoint(auxId);
				OutputPoints->InsertNextPoint( aux );
				cellNew->GetPointIds()->InsertNextId(newCellPtId++);
			}
		}

		//add the last element, 
		if (i==lastNode-1 && ( cellAux1->GetPointId(0) == cellAux2->GetPointId(0) || 
			cellAux1->GetPointId(NPCell1-1) == cellAux2->GetPointId(0) )  )
		{
			for (int j=0; j<NPCell2; j++)	
			{
				auxId = cellAux2->GetPointId(j);
				double *aux = InputPoints->GetPoint(auxId);
				OutputPoints->InsertNextPoint( aux );
				cellNew->GetPointIds()->InsertNextId(newCellPtId++);
			}
		}

		else if( i==lastNode-1 && ( (cellAux1->GetPointId(0) == cellAux2->GetPointId(NPCell2-1)
			|| cellAux1->GetPointId(NPCell1-1) == cellAux2->GetPointId(NPCell2-1)) ) )
		{
			for (int j=NPCell2-1; j>=0; j--) 
			{
				auxId = cellAux2->GetPointId(j);
				double *aux = InputPoints->GetPoint(auxId);
				OutputPoints->InsertNextPoint( aux );
				cellNew->GetPointIds()->InsertNextId(newCellPtId++);
			}
		}
	}

	// clean up
	cellAux1->Delete();
	cellAux2->Delete();
	InputPoints->Delete();

	output->Allocate(OutputPoints->GetNumberOfPoints());
	output->SetPoints(OutputPoints); //Associate the points with the dataset
	OutputPoints->Delete();
	output->InsertNextCell(cellNew->GetCellType(),cellNew->GetPointIds());//en path cell tienen que estar la ids de los puntos
	cellNew->Delete();
}


