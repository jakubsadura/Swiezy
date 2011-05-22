/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef corePointInteractorPointSet_H
#define corePointInteractorPointSet_H

#include "corePointInteractor.h"

#include "blMitkPointSetInteractor.h"

#include "vtkCommand.h"

namespace Core
{
	class PointInteractorPointSet;
}


class vtkScalarValueCommand	: public vtkCommand
{
public:
	static vtkScalarValueCommand *New();

	void SetInteractor( Core::PointInteractorPointSet* interactor );

	//!
	void Execute(vtkObject* caller, unsigned long eventId, void* callData);

protected:
	Core::PointInteractorPointSet* m_interactor;

};

namespace Core{

/**
\brief Select a set of points

m_bClickAndDragLeftMouseButton will be true by default, because it is
handling the EDTIMECHANGED

\ingroup gmInteractors
\author Xavi Planes
\date 03-04-09
*/
class PLUGIN_EXPORT PointInteractorPointSet 
	: public Core::PointInteractor
{

// PUBLIC OPERATIONS
public:
	//!
	coreDeclareSmartPointerClassMacro3Param(
		PointInteractorPointSet, 
		Core::PointInteractor,
		Core::RenderingTree::Pointer,
		Core::DataEntityHolder::Pointer,
		Core::DataEntityHolder::Pointer );

	//! Get scalar value attached to the current point using the keys A and Z
	int GetScalarValue( );

	//!
	void OnScalarValueModified( );

	//! Check if current action is add point (only for pointSet)
	virtual bool IsCurrentActionAddPoint();

	//! Check if current action is move point (only for pointSet)
	bool IsCurrentActionMovePoint();

	//! Restrict point position when adding a point
	void SetRestrictAddPointPosition( bool bVal );

	//! Max number of points
	int GetMaxPoints() const;

	//! Max number of points
	void SetMaxPoints(int val);

	/**
	\brief Get the normal and the origin of the last clicked 2D Slice plane 
	(only for point set interactor)
	*/
	bool GetLastClickedPlane( mitk::Vector3D &normal, mitk::Point3D &origin );

	//! When adding a point, add it to the end or use a fixed position in the pointset
	void SetIncrementalAdd( bool val );

	//! Update the data when the user is moving a point
	void SetEnableContinuousMoveEvent( bool bVal );

	//! 
	bool GetClickAndDragLeftMouseButton() const;

	/**
	\brief Set the state machine that will be used: 
	- false: standard one
	- true: click and drag with one left mouse button click
	\note Set this before calling ConnectToDataTreeNode( )
	*/
	void SetClickAndDragLeftMouseButton(bool val);

	//! Set some properties to the point set node in order to display the distance
	void SetMeasurementPointSet(bool val);

	//! Get input data entity types
	static Core::DataEntityType GetInputTypes( );

	//! Set line contour color
	void SetContourColor(double color[3]);

	//! Get line color
	double *GetContourColor();

// PRIVATE OPERATIONS
private:
	//! 
	PointInteractorPointSet(
		Core::RenderingTree::Pointer renderingTree,
		Core::DataEntityHolder::Pointer selectedPoints,
		Core::DataEntityHolder::Pointer selectedData );

	virtual ~PointInteractorPointSet( );

	//! Redefined
	void CreateInteractor();

	//! Redefined
	void DestroyInteractor();

	//! Return the interactor
	mitk::Interactor* GetInternalInteractor();

	//!
	void OnInteractorConnected();

	//! Redefined
	bool CheckUpdateDataOnEvent( INTERACTOR_EVENT_TYPE eventType );

	//!
	int GetTimeStep( );


// ATTRIBUTES
private:

	//! Interactor to select several points in the space
	blMitkPointSetInteractor::Pointer m_pointSetInteractor;

	//!
	unsigned long m_ScalarVectorObserverTag;

	//!
	vtkScalarValueCommand *m_ScalarValueCommand;

	//! Restrict point position when adding a point to the mesh
	bool m_bRestrictAddPointPosition;

	//! Enable update of the points while the user is moving a point
	bool m_EnableContinuousMoveEvent;

	/** When adding a point, add it to the end of the pointset or use 
	a fixed position in the pointset (default is 0)
	*/
	bool m_bIncrementalAdd;

	//! Max number of points
	int m_iMaxPoints;

	//! Use specific state machine
	bool m_bClickAndDragLeftMouseButton;

};

} // Core

#endif //corePointInteractorPointSet_H
