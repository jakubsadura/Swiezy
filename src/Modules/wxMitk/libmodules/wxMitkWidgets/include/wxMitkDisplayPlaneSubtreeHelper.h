/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef wxMitkDisplayPlaneSubtreeHelper_H
#define wxMitkDisplayPlaneSubtreeHelper_H

#include "wxMitkWidgetsWin32Header.h"
#include "mitkDataStorage.h"

namespace mitk
{
class wxMitkMultiRenderWindow;
class wxMitkSelectableGLWidget;

/**
This class provides extra helper functions for managing DataTrees.


\ingroup wxMitkRendering
\author Juan Antonio Moya
\date 23 Oct 2007
*/
class WXMITKWIDGETS_EXPORT wxMitkDisplayPlaneSubtreeHelper
{
public:
	static const char* PLANE_SUB_TREE_ROOT_NODE_NAME;
	static const char* PLANE_SUB_TREE_NODE_NAME;

	//!
	static void AddDisplayPlaneSubTree( mitk::wxMitkMultiRenderWindow* renderWindow );

	//!
	static void SetDisplayPlaneSubTreeVisibility(
		const mitk::DataStorage::Pointer dataStorage, 
		bool visible);

	//!
	static bool HasDisplayPlaneSubTree(
		const mitk::DataStorage::Pointer dataStorage);

	//!
	static bool RemoveDisplayPlaneSubTree(
		const mitk::DataStorage::Pointer dataStorage);

	//! Change color multiplied by a factor
	static void ChangeColorToDisplayPlanes( 
		float colorFactor[3],
		mitk::wxMitkMultiRenderWindow* renderWindow );

	//! Change color multiplied by a factor
	static void ChangeColorToDisplayPlanes( 
		bool enable,
		mitk::wxMitkMultiRenderWindow* renderWindow );

	/** Update node of plane
	- Add node if root node is present, remove node if root node is not present
	- Update color of node
	- Update visibility depending on root node
	*/
	static mitk::DataTreeNode::Pointer UpdateDisplayPlaneSubTree( 
		wxMitkSelectableGLWidget* widget );

	//!
	static void RemoveDisplayPlaneSubTree( 
		wxMitkSelectableGLWidget* widget );

	//!
	static mitk::DataTreeNode::Pointer AddDisplayPlaneSubTreeRoot(
		mitk::DataStorage* dataStorage );

	//!
	static mitk::DataTreeNode::Pointer GetRootNode(
		const mitk::DataStorage::Pointer dataStorage );

private:

	//!
	wxMitkDisplayPlaneSubtreeHelper(void);

	//!
	virtual ~wxMitkDisplayPlaneSubtreeHelper(void);

};
 
}


#endif // wxMitkDisplayPlaneSubtreeHelper_H
