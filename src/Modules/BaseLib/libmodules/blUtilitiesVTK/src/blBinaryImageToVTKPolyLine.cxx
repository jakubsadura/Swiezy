// Copyright 2006 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "blBinaryImageToVTKPolyLine.h"

//ITK
#include <itkNeighborhoodIterator.h>

// VTK
#include <vtkPoints.h>
#include <vtkPolyLine.h>

//boost
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>

// STD
#include <queue>
#include <math.h>

//--------------------------------------------------
blBinaryImageToVTKPolyLine::blBinaryImageToVTKPolyLine()
//--------------------------------------------------
{
	// Input initialized to do DeepCopy at SetInput function
	this->outputPolyData = vtkPolyData::New();
}

//--------------------------------------------------
blBinaryImageToVTKPolyLine::~blBinaryImageToVTKPolyLine()
//--------------------------------------------------
{
	this->outputPolyData->Delete();
}

//--------------------------------------------------
void blBinaryImageToVTKPolyLine::Update()
//--------------------------------------------------
{
	using namespace boost;
	//////////////////////////////////////////////////////////////////////////
	// Convert the binary image to a graph
	//////////////////////////////////////////////////////////////////////////
	// Some typedefs
	typedef itk::Image<unsigned long, kDIM_3D>	AuxImageType;
	typedef itk::ImageRegionIterator<AuxImageType >  AuxImageIteratorType;
	typedef itk::NeighborhoodIterator<AuxImageType > AuxImageNeighborhoodIteratorType;
	typedef itk::Point< double, kDIM_3D> WorldCoordsPointType;
	typedef AuxImageType::Pointer										AuxImagePointer;
	typedef std::pair< unsigned long, unsigned long > E;
	typedef std::list<E> EdgesList;
	typedef std::list<double> WeightsList;
	typedef std::list<WorldCoordsPointType> WorldCoordsPointList;

	// Create an auxiliary image for designing the node ids
	AuxImagePointer graphNodesIds= AuxImageType::New();
	graphNodesIds->SetRegions( this->itkCenterline->GetRequestedRegion() );
	graphNodesIds->SetSpacing( this->itkCenterline->GetSpacing() );
	graphNodesIds->SetOrigin( this->itkCenterline->GetOrigin() );
	graphNodesIds->Allocate();

	// This list is used for storing the points of the centerline
	WorldCoordsPointList pointList;

	// Compute an Id for all the points in the centerline, auxId has the total number of centerline points+1 at the end
	unsigned long auxId = 1;
	WorldCoordsPointType aPoint;
	BinaryImageType::IndexType pt, ptNeig;
	BinaryImageIteratorType it( itkCenterline, this->itkCenterline->GetRequestedRegion() );
	// count the total number of points in the skeleton
	for (it.GoToBegin();!it.IsAtEnd();++it){
		graphNodesIds->SetPixel( it.GetIndex(), 0);
		// visit all the image looking for the pixels with value 1
		if (it.Get()) {
			graphNodesIds->SetPixel( it.GetIndex(), auxId);
			graphNodesIds->TransformIndexToPhysicalPoint(it.GetIndex(),aPoint);
			// save the position of the center of the pixel for posterior use
			pointList.push_back(aPoint);
			auxId++;
		}
	}
	// As I started from 1 and my numbering will be from 0, decrease one (this is the total 
	// number of points in the centerline)
	auxId--;

	// Compute the Edges and their weights.
	EdgesList edges;
	WeightsList weights;
	// radius of the graph, connecting with one neighbor is enough
	AuxImageNeighborhoodIteratorType::RadiusType radius;
	radius.Fill(1);
	itk::Offset<kDIM_3D> o;

	// count, for each pixel, the number neighbors that are in the centerline
	AuxImageNeighborhoodIteratorType walker( radius, graphNodesIds, graphNodesIds->GetRequestedRegion() );
	AuxImageIteratorType itGraph( graphNodesIds, graphNodesIds->GetRequestedRegion() );
	// recover some information from the image
	BinaryImageType::RegionType region;
	BinaryImageType::SizeType sizeR;
	BinaryImageType::SpacingType spacing;
	region = graphNodesIds->GetRequestedRegion();
	sizeR = region.GetSize();
	spacing = graphNodesIds->GetSpacing();

	// Go over all the image creating the graph structure. I will connect each 
	// centerline node with the 8 neighbors. For this I use a neighborhood iterator.
	unsigned long nodeId, nodeNeigId;
	double dist;
	unsigned long edgesCounter = 0;
	for (itGraph.GoToBegin(); !(itGraph.IsAtEnd()); ++itGraph){
		walker.SetLocation(itGraph.GetIndex());
		pt = itGraph.GetIndex();
		nodeId = graphNodesIds->GetPixel( pt );
		// if the pixel is in the centerline
		if (nodeId)
		{
			// look for all the neighbors and connect to them
			for (unsigned char n=0; n<27; n++) {
				if (n!=13)
				{
					// get the offset of that neighbor
					o = walker.GetNeighborhood().GetOffset(n);
					ptNeig = itGraph.GetIndex();
					ptNeig += o;
					// if the pixels is in the image and is 
					if ((ptNeig[0]>=0) && (ptNeig[0]<sizeR[0]) && 
						(ptNeig[1]>=0) && (ptNeig[1]<sizeR[1]) && 
						(ptNeig[2]>=0) && (ptNeig[2]<sizeR[2])){
							nodeNeigId = graphNodesIds->GetPixel( ptNeig );
							// and if is part of the centerline
							if (nodeNeigId){
								// add the edge between nodeId and nodeNeigId
								edges.push_back(E(nodeId-1,nodeNeigId-1));
								// compute the distance as weight and add it to the weights list
								dist = ((pt[0]-ptNeig[0])*spacing[0])*((pt[0]-ptNeig[0])*spacing[0]) +
									((pt[1]-ptNeig[1])*spacing[1])*((pt[1]-ptNeig[1])*spacing[1]) +
									((pt[2]-ptNeig[2])*spacing[2])*((pt[2]-ptNeig[2])*spacing[2]);
								dist = sqrt(dist);
								weights.push_back(dist);
								// increase the edge counter
								edgesCounter++;
							}
						}
				}
			}
		}
	}
	////////////////////////////////////////////////////////////////////////////////////
	// use Boost Prim's algorithm implementation for computing the minimum spanning tree
	////////////////////////////////////////////////////////////////////////////////////
	// convert to a data format that can be handled by boost
	E *edgesVec = new E[edgesCounter];
	double *weightsVec = new double[edgesCounter];
	for (unsigned long i=0; i<edgesCounter;i++ ){
		edgesVec[i] = edges.front();
		edges.pop_front();
		weightsVec[i] = weights.front();
		weights.pop_front();
	}
	// create the graph representation as an adjacency list for computing the minimum spanning tree
	typedef adjacency_list < vecS, vecS, directedS,
		property<vertex_distance_t, unsigned long>, property < edge_weight_t, double> > Graph;
	// auxId holds the number of nodes in the graph
	long num_nodes = auxId;

#if defined(BOOST_MSVC) && BOOST_MSVC <= 1300
	Graph g(num_nodes);
	property_map<Graph, edge_weight_t>::type weightmap = get(edge_weight, g); 
	for (std::size_t j = 0; j < edgesCounter; ++j) {
		graph_traits<Graph>::edge_descriptor e; bool inserted;
		tie(e, inserted) = add_edge(edgesVec[j].first, edgesVec[j].second, g);
		weightmap[e] = weightsVec[j];
	}
#else
	Graph g(edgesVec, edgesVec + edgesCounter, weightsVec, num_nodes);
	property_map<Graph, edge_weight_t>::type weightmap = get(edge_weight, g);
#endif
	std::vector < graph_traits < Graph >::vertex_descriptor > p(num_vertices(g));

#if defined(BOOST_MSVC) && BOOST_MSVC <= 1300
	property_map<Graph, vertex_distance_t>::type distance = get(vertex_distance, g);
	property_map<Graph, vertex_index_t>::type indexmap = get(vertex_index, g);
	prim_minimum_spanning_tree
		(g, *vertices(g).first, &p[0], distance, weightmap, indexmap, 
		default_dijkstra_visitor());
#else
	prim_minimum_spanning_tree(g, &p[0]);
#endif

	////////////////////////////////////////////////////////////////////////////////////
	// Convert the minimum spanning tree into a vtkPolydata of vtkPolyline cells
	////////////////////////////////////////////////////////////////////////////////////
	// classify all the nodes of the image to know how to treat them
	typedef enum NodeType {leaf, intermediate, crossing, rt};
	std::vector<unsigned char> nodesTypes(num_nodes);
	std::queue<unsigned long> polylinesStarts;
	unsigned long count;
	for	(unsigned long nd = 0; nd<num_nodes; nd++){
		count = 0;
		for (unsigned long j = 0; j<num_nodes; j++){
			if ((j!=nd) && (p[j]==nd)){
				count++;
			}
		}
		// this is the root
		if (p[nd]==nd){
			nodesTypes[nd] = rt;
		} 
		// this is a leaf
		else if (count==0){
			nodesTypes[nd] = leaf;
			polylinesStarts.push(nd);
		}
		// this is an intermediate node
		else if (count==1){
			nodesTypes[nd] = intermediate;
			// otherwise, is a crossing of centerlines
		}else{
			nodesTypes[nd] = crossing;
			polylinesStarts.push(nd);
		}
	}

	// create the points container and store in it all the points of the centerline
	vtkPoints *points = vtkPoints::New();
	points->SetNumberOfPoints(num_nodes);
	points->Allocate(num_nodes);
	for (unsigned long ptId = 0; ptId<num_nodes;ptId++){
		aPoint = pointList.front();
		pointList.pop_front();
		points->InsertNextPoint(aPoint.GetVnlVector().data_block());
	}

	// Create the polydata
	vtkPolyData *tmpOutput = vtkPolyData::New();
	tmpOutput->Allocate(num_nodes);
	tmpOutput->SetPoints(points);

	// run though all the graph to convert it
	std::list<unsigned long> polyline;
	unsigned long startId, cellCount = 0;
	// while it still exists any starting point
	while (!polylinesStarts.empty()){
		unsigned long auxId;
		// recover the point
		startId = polylinesStarts.front();
		polylinesStarts.pop();
		bool endPolyline = false;
		auxId = startId;
		polyline.clear();
		// and run though the branch untill the next end of branch
		while(!endPolyline){
			polyline.push_back(auxId);
			auxId = p[auxId];
			if ((nodesTypes[auxId]==crossing) || (nodesTypes[auxId]==rt)){
				polyline.push_back(auxId);
				endPolyline = true;
			}
		}
		// Create the cell
		vtkPolyLine *plCell = vtkPolyLine::New();
		if (polyline.front()==polyline.back()){
			polyline.pop_front();
		}
		unsigned long pointsCount = polyline.size();
		plCell->GetPointIds()->SetNumberOfIds(pointsCount);
		unsigned long i = 0;
		// add al its points to it
		while (!polyline.empty()){
			plCell->GetPointIds()->SetId(i++,polyline.front());
			polyline.pop_front();
		}
		// add it to the polydata
		tmpOutput->InsertNextCell(plCell->GetCellType(),plCell->GetPointIds());
		plCell->Delete();
		cellCount++;
	}
	this->outputPolyData = tmpOutput;
}
