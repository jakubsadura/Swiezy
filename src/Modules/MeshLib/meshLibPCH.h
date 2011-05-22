#define _USE_MATH_DEFINES
#include <math.h>

#include "CILabAssertMacros.h"
#include "CILabItkMacros.h"

#include "blHistogram.h"

#include "vtkCellData.h"
#include "vtkPolyData.h"
#include "vtkPolyDataReader.h"
#include "vtkPolyDataWriter.h"

#include <assert.h>

#include <boost/format.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/graph_traits.hpp>

#include <float.h>
#include <fstream>

#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkLightObject.h>

#include <map>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <vector>

#include <vtkAppendPolyData.h>
#include <vtkBitArray.h>
#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkCleanPolyData.h>
#include <vtkDataArray.h>
#include <vtkDataSet.h>
#include <vtkEdgeTable.h>
#include <vtkFeatureEdges.h>
#include <vtkFieldData.h>
#include <vtkGenericCell.h>
#include <vtkIdList.h>
#include <vtkMath.h>
#include <vtkMergePoints.h>
#include <vtkMeshQuality.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkPolyDataNormals.h>
#include <vtkPolyDataReader.h>
#include <vtkPolyDataWriter.h>
#include <vtkPolyLine.h>
#include <vtkStripper.h>
#include <vtkTriangleFilter.h>
#include <vtkType.h>
#include <vtkTypeTraits.h>
#include <vtkstd/exception>
