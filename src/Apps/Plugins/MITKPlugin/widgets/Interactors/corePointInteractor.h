/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef corePointInteractor_H
#define corePointInteractor_H

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
\brief Adapter for the mitk::PointSelectInteractor (Convert the interface 
of a class into another interface clients expect)

Provides a new interface to GIMIAS that uses Core::DataHolder

The rendering data of the input selectedPoints will be added and removed from the 
mitk::DataDataStorage calling the functions ConnectToDataTreeNode( )
and DisconnectFromDataTreeNode(). 

The selectedPoints node will be 
added as a child of selectedData node. If selectedData is NULL, the 
pointSet node will be added as a child of the root. selectedData contains
the restricted points that the user can select if using POINT SELECT

The selectedPoints param of the constructor will be updated
automatically when the user modifies the points with the interactor.
There are two events that will notify the observers of selectedPoints:
- The user adds a new point
- The user moves a point and release the mouse button (INTERACTOR_POINTSET)

When the interactor points change, the selectedPoints are updated and the 
observers are notified. The observers can modify the position of the 
selectedPoints but is not allowed to change the number of points. 
After that, the interactor points are updated again.

Updates the interactor points when the user changes the selectedPoints.
To update the interactor points, if there's more than one time step, 
we need to pass the render to the interactor to get the time step.

The selectedPoints data entity will be created automatically if it is empty.

\note You can only modify the current timeStep.

Max number of selected points can be configured for INTERACTOR_POINTSET.

\ingroup gmInteractors
\author Xavi Planes
\date 07-05-08
*/
class PLUGIN_EXPORT PointInteractor : public Core::SmartPointerObject
{

// TYPE DEFINITIONS
public:
	coreDeclareSmartPointerTypesMacro(PointInteractor,Core::SmartPointerObject);
	coreClassNameMacro(PointInteractor);

	enum INTERACTOR_EVENT_TYPE{
		INTERACTOR_EVENT_ADD,
		INTERACTOR_EVENT_REMOVE,
		INTERACTOR_EVENT_MOVE,
		INTERACTOR_EVENT_MAX
	};

	enum INTERACTOR_POINT_TYPE{
		POINT_SET,
		POINT_SELECT
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

// PRIVATE OPERATIONS
protected:
	//! 
	PointInteractor(
		Core::RenderingTree::Pointer renderingTree,
		Core::DataEntityHolder::Pointer selectedPoints,
		Core::DataEntityHolder::Pointer selectedData );

	virtual ~PointInteractor( );

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

	//!
	void SetSelectedPoints( 
		Core::DataEntity::Pointer pointsDataEntity );

	//!
	virtual int GetTimeStep( ) = 0;

// ATTRIBUTES
protected:

	//! Updating vtkPointSet from mitk::PointSet 
	bool m_bUpdatingVtkPointSet;

private:

	//! Updating mitk::PointSet from vtkPointSet
	bool m_bDisableMitkEvents;

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
};

} // Core

#endif //corePointInteractor_H
