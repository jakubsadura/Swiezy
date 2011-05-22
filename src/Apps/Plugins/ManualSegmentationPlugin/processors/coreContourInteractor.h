/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreContourInteractor_H
#define coreContourInteractor_H


#include "coreRenderingTree.h"
#include "coreObject.h"
#include "coreDataEntityHolder.h"
#include "coreCommonDataTypes.h"

// MITK
#include "mitkDataTree.h"
#include "mitkContourInteractor.h"
#include "mitkContour.h"
#include "mitkContourTool.h"
#include "mitkToolManager.h"
#include "mitkImage.h"

//Core
#include "coreDataEntity.h"
#include "coreDataHolder.h"



namespace Core{

/**
\brief 
\ingroup gmInteractors
\author Chiara Riccobene
\date 29-05-09
*/
class ContourInteractor 
	: public Core::SmartPointerObject
{

// PUBLIC OPERATIONS
public:

	//!
	coreDeclareSmartPointerClassMacro3Param(
		ContourInteractor, 
		Core::SmartPointerObject,
		Core::RenderingTree::Pointer,
		Core::DataEntityHolder::Pointer,
		Core::DataEntityHolder::Pointer );

	//! Connect the interactor to the selectedData tree node
	void ConnectToDataTreeNode( );

	//! Disconnect the interactor from the rendering tree
	void DisconnectFromDataTreeNode();

	//!
	Core::RenderingTree::Pointer GetRenderingTree() const;

	//!
	void SetRenderingTree(Core::RenderingTree::Pointer val);

	//!
	Core::DataEntityHolder::Pointer GetSelectedContourHolder() const;

	//!
	void SetSelectedContourHolder(Core::DataEntityHolder::Pointer val);

	//!
	Core::DataEntityHolder::Pointer GetSelectedData() const;

	//!
	void SetSelectedData(Core::DataEntityHolder::Pointer val);

	bool IsConnectedToRenderingTree(Core::DataEntity::Pointer dataEntity);

	//!
	void SetPixelValue(int n);

	//!
	int GetPixelValue();

	//!
	void SetEraseOnlySelectedColor(bool b);

	//!
	bool GetEraseOnlySelectedColor();

// PRIVATE OPERATIONS
private:
	//! 
	ContourInteractor(
		Core::RenderingTree::Pointer renderingTree,
		Core::DataEntityHolder::Pointer selectedContour,
		Core::DataEntityHolder::Pointer selectedData );

	virtual ~ContourInteractor( );


		//! Connect m_pointSetNode to the m_renderingTree
	void ConnectNodeToTree( );

	//! Disconnect m_pointSetNode to the m_renderingTree
	void DisconnectNodeFromTree( );

	//! Connect the interactors to the global instance
	void ConnectInteractors();

	//! Disconnect all interactors to the global instance
	void DisconnectInteractors( );

	//! Redefined
	void CreateInteractor();

	//! Redefined
	void DestroyInteractor();

	//! Redefined
	void OnInteractorConnected();

	//!
	Core::DataEntity::Pointer GetSelectedContourDataEntity( );

	//!
	Core::DataEntity::Pointer GetSelectedDataEntity( );

	//!
	mitk::DataTreeNode::Pointer GetSelectedContourNode( );

	//!
	mitk::Image::Pointer GetSelectedContourRenderingData( );

	//!
	mitk::DataTreeNode::Pointer GetSelectedDataEntityNode( );

	//!
	void OnContourModified( );

	void CreateEmptyWorkingData( );

	//!
	mitk::ContourTool* GetContourTool( );


// ATTRIBUTES
private:

	//! Rendering data tree.
	Core::RenderingTree::Pointer m_renderingTree;
	
	//@{ 
	//! \name Data used from data container

	//! Current selected point of m_selectedData
	Core::DataEntityHolder::Pointer m_selectedContourHolder;

	//! Surface mesh of the selected point
	Core::DataEntityHolder::Pointer m_selectedData;
	//@}

	//! Interactor to select a VOI of an image
	mitk::ToolManager::Pointer m_toolManager;

	//!
	int m_pixelValue;

	//!
	bool m_bEraseOnlySelectedColor;
	
};

} // Core

#endif //corePointInteractorSurfaceSelect_H
