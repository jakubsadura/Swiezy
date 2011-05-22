/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreSurfaceInteractor_H
#define coreSurfaceInteractor_H

#include "corePluginMacros.h"
#include "coreRenderingTree.h"
#include "coreObject.h"
#include "coreDataEntityHolder.h"
#include "coreCommonDataTypes.h"

// MITK
#include "mitkDataTree.h"
#include "mitkPointSet.h"

//Core
#include "coreDataEntity.h"
#include "coreDataHolder.h"


namespace Core{


/**
\brief Adapter for the blmitkSurfaceSelectInteractor
and blmitkTriangleSelectInteractor (Convert the interface 
of a class into another interface clients expect)
\ingroup MeshEditorPlugin
\author Chiara Riccobene
\date 14 Apr 10

Provides a new interface to GIMIAS that uses Core::DataHolder

missing information about interactor
*/

class PLUGIN_EXPORT SurfaceInteractor : public Core::SmartPointerObject
{

// TYPE DEFINITIONS
public:
	coreDeclareSmartPointerTypesMacro(SurfaceInteractor,Core::SmartPointerObject);
	coreClassNameMacro(SurfaceInteractor);

	enum INTERACTOR_EVENT_TYPE{
		INTERACTOR_EVENT_ADD,
		INTERACTOR_EVENT_REMOVE,
		INTERACTOR_EVENT_MOVE,
		INTERACTOR_EVENT_MAX
	};

	enum INTERACTOR_TYPE{
		TRIANGLE,
		SPHERE,
		BRUSH
	};

// PUBLIC OPERATIONS
public:

	//! Connect the interactor to the selectedData tree node
	void ConnectToDataTreeNode( );

	//! Disconnect the interactor from the rendering tree
	void DisconnectFromDataTreeNode();

	//! Returns the direction of the point ( pushing right mouse button )
	virtual int GetDirection( );

	//! Set the bounding box to select the interactor
	virtual void SetBoundingBox( double bounds[ 6 ] );

	//! Return the first selected point ID and position, else return -1
	int GetSelectedPoint( mitk::Point3D &point );

	//!
	Core::RenderingTree::Pointer GetRenderingTree() const;

	//!
	void SetRenderingTree(Core::RenderingTree::Pointer val);

	//!
	Core::DataEntityHolder::Pointer GetSelectedPointsHolder() const;

	//!
	void SetSelectedPointsHolder(Core::DataEntityHolder::Pointer val);

	//!
	Core::DataEntityHolder::Pointer GetSelectedData() const;

	//!
	void SetSelectedData(Core::DataEntityHolder::Pointer val);

	//!
	Core::DataEntity::Pointer GetSelectedPointsDataEntity( );

	//!
	Core::DataEntity::Pointer GetSelectedDataEntity( );

	//!
	mitk::DataTreeNode::Pointer GetSelectedPointsNode( );

	//!
	mitk::PointSet::Pointer GetSelectedPointsRenderingData( );

	//!
	mitk::DataTreeNode::Pointer GetSelectedDataEntityNode( );

	//! Set bounding box of the father data entity
	void SetBoundingBox( Core::DataEntity::Pointer dataEntity );

	//!
	bool GetHideNode() const;
	void SetHideNode(bool val);

	//!
	void UpdateMitkToVtkPoints();

	//!
	virtual void SelectSurface()= 0;
	//!
	virtual void DeselectSurface()= 0;

	vtkIdList* SelectCells(vtkImplicitFunction* implicitFunction);

	//! set/get for m_value
	void SetValue(double val);
	double GetValue ();

	//! set/get for m_selectFlag
	void SetFlagSelection(bool val){m_selectFlag = val;}
	bool GetFlagSelection(){return m_selectFlag;}



// PRIVATE OPERATIONS
protected:
	//! 
	SurfaceInteractor(
		Core::RenderingTree::Pointer renderingTree,
		Core::DataEntityHolder::Pointer selectedPoints,
		Core::DataEntityHolder::Pointer selectedData );

	virtual ~SurfaceInteractor( );

	//@{ 
	//! \name Event handlers

	//! 
	void OnPointSetAddEvent( );

	//! 
	void OnPointSetRemoveEvent( );

	//! 
	void OnPointSetMoveEvent( );

	/**
	 * \brief m_selectedPointsHolder has changed, so we need to update
	 * m_MitkPointSet
	 */
	void OnVtkDataPointModified();

	void OnNewSelectedPoints();

	//!
	void OnModifiedTimeStep( );

	//@}


	/**
	 * \brief m_MitkPointSet has changed so we need to update
	 * m_selectedPointsHolder points
	 */
	void UpdateVtkPointSet();

	//! Update VTK PointData of m_selectedPointsHolder from m_MitkPointSet
	void UpdateVtkPointData( );

	//! Connect m_pointSetNode to the m_renderingTree
	void ConnectNodeToTree( );

	//! Disconnect m_pointSetNode to the m_renderingTree
	void DisconnectNodeFromTree( );

	//! Create a new interactor
	virtual void CreateInteractor( ) = 0;

	//! Destroy the interactor
	virtual void DestroyInteractor( ) = 0;

	//! Connect the interactors to the global instance
	void ConnectInteractors();

	//! Disconnect all interactors to the global instance
	void DisconnectInteractors( );

	//!
	virtual mitk::Interactor* GetInternalInteractor( ) = 0;

	//! After the interactor is connected and before updating the MITK data
	virtual void OnInteractorConnected( );

	//! Check if on this event from MITK we need to update the data
	virtual bool CheckUpdateDataOnEvent( INTERACTOR_EVENT_TYPE );

	//! Update labels of the rendering node
	void UpdateRenderingNodeLabels();

	//!
	void SetSelectedPoints( 
		Core::DataEntity::Pointer pointsDataEntity );

	//!
	virtual int GetTimeStep( ) = 0;

	mitk::PointSet* GetPointSet();

// ATTRIBUTES
protected:

	//! Updating vtkPointSet from mitk::PointSet 
	bool m_bUpdatingVtkPointSet;

private:

	//! Updating mitk::PointSet from vtkPointSet
	bool m_bUpdatingMitkPointSet;

	//! Rendering data tree.
	Core::RenderingTree::Pointer m_renderingTree;
	
	//@{ 
	//! \name Data used from data container

	//! Current selected point of m_selectedData
	Core::DataEntityHolder::Pointer m_selectedPointsHolder;

	//! Surface mesh of the selected point
	Core::DataEntityHolder::Pointer m_selectedData;
	//@}

	//! 
	unsigned long m_OnPointSetAddEventObserverTag;

	//! 
	unsigned long m_OnPointSetRemoveEventObserverTag;

	//! 
	unsigned long m_OnPointSetMoveEventObserverTag;

	/** Hide the node or disconnect it from the rendering tree when the 
	interactor is disabled
	*/
	bool m_hideNode;

	/** Rendering data of the m_selectedPointsHolder
	When the subject changes, we need to remove observers to old data
	*/
	Core::DataEntity::Pointer m_PointsDataEntity;

	mitk::PointSet::Pointer m_pointSet;

	//! value to use for the brush selector to set the radius
	double m_value;
	
	//! flag to select or to unselect
	bool m_selectFlag;
};

} // Core

#endif //coreSurfaceInteractor_H
