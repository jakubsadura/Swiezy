/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __blMITKUtils_h
#define __blMITKUtils_h


#include "TpExtLibMITKWin32Header.h"
#include <vector>
#include "mitkBaseData.h"
#include "mitkDataTreeNode.h"
#include "mitkSurface.h"
#include "blLookupTables.h"
#include "blShapeUtils.h"
#include "vtkPolyData.h"

class vtkLookupTable;
class vtkRenderer;

	/**
	 * \brief Static class with some utilities.
	 * \ingroup blUtilitiesMITK
	 */
class TPEXTLIBMITK_EXPORT blMITKUtils
{
public:
	typedef enum{
		LUT_SCALAR_MODE_POINT_DATA,
		LUT_SCALAR_MODE_CELL_DATA,
		LUT_SCALAR_MODE_NONE
	} LUT_SCALAR_MODE;

public:
	
	//! Update the "LookupTable" property according to "blLookupTablesType"
	static void UpdateLookupTableProperty( mitk::DataTreeNode::Pointer node );

	/**
	\brief Add a new LUT property to a generic node
	*/
	static void ApplyLookupTable(
		bool enable,
		mitk::DataTreeNode::Pointer node, 
		blLookupTables::LUT_TYPE lutType,
		blShapeUtils::ShapeUtils::VTKScalarType scalarType);

	/**
	\brief Add a new LUT property to this mitk::Image node
	*/
	static void ApplyLookupTableToImage(
		bool enable,
		mitk::DataTreeNode::Pointer node, 
		blLookupTables::LUT_TYPE lutType );
	/**
	\brief Add a new LUT property to this mitk::Surface node
	*/
	static void ApplyLookupTableToSurface(
		mitk::DataTreeNode::Pointer node, 
		blLookupTables::LUT_TYPE lutType,
		std::string name ,
		int mode );

	//! Get min and max scalars and call vtkLookupTable::SetRange( )
	static void SetVtkLutScalarRange( 
		mitk::DataTreeNode::Pointer node,
		vtkLookupTable* vtkLut );

	//! Call vtkLookupTable::SetRange( ) to set it to range
	static void SetVtkLutScalarRangeForMesh( 
		mitk::DataTreeNode::Pointer node,
		double range[2]);

	//! Create LUT and set properties blLookuptabletype and lookuptable to node
	static void CreateLut(
		mitk::DataTreeNode::Pointer node,
		blLookupTables::LUT_TYPE lutType);

	//! Set scalar mode and range
	static void SetScalarMode(
		mitk::DataTreeNode::Pointer node,
		std::string name,
		int mode);
	/**
	\brief Add three properties to this mitk::DataTree node
	*/
	static void ApplyMeasurementProperties(
		mitk::DataTreeNode::Pointer node,
		std::string name  = "distance");

	/**
	\brief Add eight properties to this mitk::DataTree node for a maskImage
	*/
	static void ApplyMaskImageProperties(
		mitk::DataTreeNode::Pointer node,
		std::string name  = "maskImage");

	/**
	\brief Compute point size for a polydata, return true if it finds the pointsize
	*/
	static bool ComputePointSize( 
		vtkPolyData* polyData, 
		double &pointSize, 
		double scaleFactor = 0.005 );

	//! Compute point size using bounding box diagonal length. If data == NULL -> pointSize = -1
	static bool ComputePointSize( 
		mitk::BaseData* data, 
		double &pointSize, 
		double scaleFactor = 0.1 );

	//! Compute bounding box with a small margin
	static bool ComputeBoundingBox( 
		mitk::BaseData* data, 
		double boundingBox[ 6 ]);

	/**
	Extract a 3D world point from the rendering data of the input data 
	entity that will be used for centering the view when the user select this data entity
	*/
	static bool ExtractImagePoint( 
		mitk::Image::Pointer image,
		int iTimeStep,
		mitk::Point3D &worldPoint );

	/** Perform pick operation with selection point provided. Normally the 
	 first two values for the selection point are x-y pixel coordinate, and
	 the third value is =0. Return non-zero if something was successfully picked.
	 Bug in VTK or MITK in vtkPicker because is not computing the intersection
	 with the mesh, so we need to compute the ray
	 */
	static void ComputeRay(
		double selectionX, double selectionY, double selectionZ, 
		vtkRenderer *renderer,
		double p1World[4], double p2World[4] );

	/** Compute closest point from mesh to pPoint or the
		stateEvent->GetDisplayPosition( ) if it is an skeleton
	*/
	static vtkIdType ComputeClosestPoint(
		mitk::StateEvent const* stateEvent,
		double pPoint[ 3 ],
		vtkPolyData* mesh);

};



#endif
