/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: vtkPolyDataSingleSourceShortestPath.h,v $
  Language:  C++
  Date:      $Date: 2004/05/10 18:34:41 $
  Version:   $Revision: 1.13 $
  
  Made by Rasmus Paulsen
  email:  rrp(a)imm.dtu.dk
  web:    www.imm.dtu.dk/~rrp/VTK

  This class is not mature enough to enter the official VTK release.
=========================================================================*/
// .NAME vtkPolyDataSingleSourceShortestPath - Single source shortest path on a polygonal mesh
// .SECTION Description
//
// vtkPolyDataSingleSourceShortestPath is a filter that takes as input
// a polygonal mesh and performs a single source shortest path
// calculation. Dijkstra's algorithm is used. The implementation is
// similar to the one described in Introduction to Algorithms (Second
// Edition) by Thomas H. Cormen, Charles E. Leiserson, Ronald
// L. Rivest, and Cliff Stein, published by MIT Press and McGraw-Hill.
// Some minor enhancement are added though. All vertices are not pushed
// on the heap at start, instead a front set is maintained.  The heap
// is implemented as a binary heap.
//
// The output of the filter is a set of lines describing the shortest path
// from StartVertex to EndVertex.
//
// Thanks to Alexandre Gouaillard and Shoaib Ghias for bug fixes and enhancements

#ifndef __vtkPolyDataSingleSourceShortestPath_h__
#define __vtkPolyDataSingleSourceShortestPath_h__

#include "BaseLibVTKWin32Header.h"
#include "vtkPolyDataToPolyDataFilter.h"

class vtkDoubleArray;
class vtkIntArray;
class vtkIdList;
class vtkFloatArray;

class BASELIBVTK_EXPORT vtkPolyDataSingleSourceShortestPath : public vtkPolyDataToPolyDataFilter
{
public:
	vtkTypeRevisionMacro(vtkPolyDataSingleSourceShortestPath,vtkPolyDataToPolyDataFilter);
	
	static vtkPolyDataSingleSourceShortestPath *New();
	
	void PrintSelf(ostream& os, vtkIndent indent);
	
	// Description:
	// The vertex ids (of the input polydata) on the shortest path
	vtkGetObjectMacro(IdList, vtkIdList);
	
	// Description:
	// Get the summed weight for all vertices
	vtkGetObjectMacro(d, vtkFloatArray);
	
	// Description:
	// The vertex at the start of the shortest path
	vtkGetMacro(StartVertex, vtkIdType);
	vtkSetMacro(StartVertex, vtkIdType);
	
	// Description:
	// The vertex at the end of the shortest path
	vtkGetMacro(EndVertex, vtkIdType);
	vtkSetMacro(EndVertex, vtkIdType);
	
	// Description: 
	// Stop when the end vertex is reached 
	// or calculate shortest path to all vertices
	vtkSetMacro(StopWhenEndReached, int);
	vtkGetMacro(StopWhenEndReached, int);
	vtkBooleanMacro(StopWhenEndReached, int);
	
	// Description: 
	// Use scalar values in the edge weight (experimental)
	vtkSetMacro(UseScalarWeights, int);
	vtkGetMacro(UseScalarWeights, int);
	vtkBooleanMacro(UseScalarWeights, int);
	
	// Description:
	unsigned long GetMTime();
	
protected:
	vtkPolyDataSingleSourceShortestPath();
	~vtkPolyDataSingleSourceShortestPath();
	vtkPolyDataSingleSourceShortestPath(const vtkPolyDataSingleSourceShortestPath&);
	void operator=(const vtkPolyDataSingleSourceShortestPath&);
	
	// Usual data generation method
	void Execute();
	
	// Build a graph description of the mesh
	void BuildAdjacency(vtkPolyData *pd);
	
	void DeleteAdjacency();
	
	// The cost going from vertex u to v
	// TODO: should be implemented as a user supplied
	// callback function
	double EdgeCost(vtkPolyData *pd, vtkIdType u, vtkIdType v);
	
	void init();
	
	// structure the heap
	void Heapify(int i);
	
	// insert vertex v in heap. Weight is in d(v)
	void HeapInsert(int v);
	
	// extract vertex with min d(v)
	int HeapExtractMin();
	
	// Update heap when key d(v) has been decreased
	void HeapDecreaseKey(int v);
	
	void InitSingleSource(int startv);
	
	// Calculate shortest path from vertex startv to vertex endv
	void ShortestPath(int startv, int endv);
	
	// Relax edge u,v with weight w
	void Relax(int u, int v, double w);
	
	// Backtrace the shortest path
	void TraceShortestPath(vtkPolyData *inPd, vtkPolyData *outPd,
							 vtkIdType startv, vtkIdType endv);
	
	// the number of vertices
	int n;
	
	// d(v) current summed weight for path to vertex v
	vtkFloatArray *d;
	
	// pre(v) predecessor of v
	vtkIntArray *pre;
	
	// f is the set of vertices wich has not a shortest path yet but has a path
	// ie. the front set (f(v) == 1 means that vertex v is in f)
	vtkIntArray *f;
	
	// s is the set of vertices with allready determined shortest path
	// s(v) == 1 means that vertex v is in s
	vtkIntArray *s;
	
	// the priority que (a binary heap) with vertex indices
	vtkIntArray *H;
	
	// The real number of elements in H != H.size()
	int Hsize;
	
	// p(v) the position of v in H (p and H are kindoff inverses)
	vtkIntArray *p;
	
	// The vertex ids on the shortest path
	vtkIdList *IdList;
	
	// The vertex at the start of the shortest path
	vtkIdType StartVertex;
	
	// The vertex at the end of the shortest path
	vtkIdType EndVertex;
	
	// Adjacency representation
	vtkIdList **Adj;
	
	int StopWhenEndReached;
	
	int UseScalarWeights;

	// Used to remember the size of the graph. If the filter is re-used.
	int AdjacencyGraphSize;
};

#endif

