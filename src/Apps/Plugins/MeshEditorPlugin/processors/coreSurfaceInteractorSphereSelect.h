/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreSurfaceInteractorSphereSelect_H
#define coreSurfaceInteractorSphereSelect_H

#include "coreSurfaceInteractor.h"

#include "blMitkSurfaceSelectInteractor.h"

namespace Core{

/**
\brief 
\ingroup MeshEditorPlugin
\author Chiara Riccobene
\date 14 Apr 2010
*/
class PLUGIN_EXPORT SurfaceInteractorSphereSelect 
	: public Core::SurfaceInteractor
{

// PUBLIC OPERATIONS
public:

	//!
	coreDeclareSmartPointerClassMacro3Param(
		SurfaceInteractorSphereSelect, 
		Core::SurfaceInteractor,
		Core::RenderingTree::Pointer,
		Core::DataEntityHolder::Pointer,
		Core::DataEntityHolder::Pointer );

	//! Redefined
	int GetDirection( );

	//! Redefined
	void SetBoundingBox( double bounds[ 6 ] );

	/**
	\brief Get the sphere of the selection 
	(only for surface select interactor)
	*/
	vtkSphere* GetSelectionSphere();

	//! Get input data entity types
	static Core::DataEntityType GetInputTypes( );

	//!
	int GetTimeStep();

	mitk::DataTreeNode::Pointer GetSphereNode();

	void SetSelectionSphereHolder( 
		Core::DataEntityHolder::Pointer sphereHolder);

	void SelectSurface();
	//!
	void DeselectSurface();

	void Execute(vtkObject* caller, unsigned long eventId, void* callData);

// PRIVATE OPERATIONS
private:
	//! 
	SurfaceInteractorSphereSelect(
		Core::RenderingTree::Pointer renderingTree,
		Core::DataEntityHolder::Pointer selectedPoints,
		Core::DataEntityHolder::Pointer selectedData );

	virtual ~SurfaceInteractorSphereSelect( );

	//! Redefined
	void CreateInteractor();

	//! Redefined
	void DestroyInteractor();

	//! Redefined
	void OnInteractorConnected();

	//! Return the interactor
	mitk::Interactor* GetInternalInteractor();

// ATTRIBUTES
private:

	//! Interactor to select a point of a mesh
	blMitkSurfaceSelectInteractor::Pointer m_surfaceSelectInteractor;
	//!SelectionSphere
	Core::DataEntityHolder::Pointer m_selectionSphere;
};

} // Core

#endif //coreSurfaceInteractorSphereSelect_H
