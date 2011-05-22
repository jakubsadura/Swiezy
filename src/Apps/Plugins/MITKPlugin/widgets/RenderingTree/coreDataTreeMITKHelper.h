/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreDataTreeMITKHelper_H
#define coreDataTreeMITKHelper_H

#include "gmWidgetsWin32Header.h"
#include "coreObject.h"
#include "coreDataEntity.h"
#include "coreVTKPolyDataHolder.h"
#include "coreRenderingTree.h"

#include "blLookupTables.h"
#include "blMITKUtils.h"

#include "vtkPolyData.h"

#include "mitkDataTreeNode.h"

class vtkDataSetAttributes;

namespace Core{


/**
\brief Helper functions for mitk DataStorage
\ingroup gmKernel
\author Xavi Planes
\date 07-05-08
*/
class PLUGIN_EXPORT DataTreeMITKHelper : public Core::Object
{
// OPERATIONS
public:

	/**
	\brief Set the properties to the node to be able to colorize the mesh.
	A default LUT will be used, but you can add a parameter to change it.
	It will use the default active scalars array of PointData.
	mode -> set mode to Point Data (0) or to Cell Data (1)
	 */
	static void ApplyLookupTableToMesh(
		mitk::DataTreeNode::Pointer meshNode, 
		blMITKUtils::LUT_SCALAR_MODE mode = blMITKUtils::LUT_SCALAR_MODE_NONE );

	/**
	\brief An overloaded function of ApplyLookUpTableToNMImage which also takes
	the LUT to be applied to
	*/
	static void ApplyLookupTableToNMImage(
		mitk::DataTreeNode::Pointer imageNode, 
		blLookupTables::LUT_TYPE lutType = blLookupTables::LUT_TYPE_RAINBOW_GE);


	/** 
	\brief If processing data of the data entity is vtkPolyData, call
	GetPointData( )->SetActiveScalars( strName )
	*/
	static void SetActiveScalars(  
						Core::DataEntityHolder::Pointer dataEntityHolder,
						std::string strName,
						int iTimeStep = 0 );

	/** 
	\brief If there's no active array, select the first one and return
	the available scalar data to be used for the LUT
	*/
	static blMITKUtils::LUT_SCALAR_MODE SetDefaultActiveScalars( 
		Core::DataEntity::Pointer dataEntity );


	//! Set default active scalars if none is set and return true if data is available
	static bool SetDefaultActiveScalars( vtkDataSetAttributes* data );

	//! Change the property "show labels"
	static void ChangeShowLabelsProperty(  
						Core::DataEntity::Pointer dataEntity,
						Core::RenderingTree::Pointer tree,  
						bool show );

	//! Get the property "show labels"
	static void GetShowLabelsProperty(  
		Core::DataEntity::Pointer dataEntity,
		Core::RenderingTree::Pointer tree,
		bool &showLabels );

	//!
	static void UpdateRenderingNodeLabels( 
		mitk::DataTreeNode::Pointer node );

};

} // Core

#endif //coreDataTreeMITKHelper_H

