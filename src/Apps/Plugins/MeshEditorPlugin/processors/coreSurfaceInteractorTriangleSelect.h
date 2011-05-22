/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreSurfaceInteractorTriangleSelect_H
#define coreSurfaceInteractorTriangleSelect_H

#include "coreSurfaceInteractor.h"

#include "blMitkTriangleSelectInteractor.h"

namespace Core{

/**
\brief 
\ingroup MeshEditorPlugin
\author Chiara Riccobene
\date 14 Apr 2010
*/
class PLUGIN_EXPORT SurfaceInteractorTriangleSelect 
	: public Core::SurfaceInteractor
{

// PUBLIC OPERATIONS
public:

	//!
	coreDeclareSmartPointerClassMacro3Param(
		SurfaceInteractorTriangleSelect, 
		Core::SurfaceInteractor,
		Core::RenderingTree::Pointer,
		Core::DataEntityHolder::Pointer,
		Core::DataEntityHolder::Pointer );


	//! Get input data entity types
	static Core::DataEntityType GetInputTypes( );

	//!
	int GetTimeStep();


	void SetSelectionSphereHolder( 
		Core::DataEntityHolder::Pointer sphereHolder);

	//!
	void SelectSurface();
	//!
	void DeselectSurface();

// PRIVATE OPERATIONS
private:
	//! 
	SurfaceInteractorTriangleSelect(
		Core::RenderingTree::Pointer renderingTree,
		Core::DataEntityHolder::Pointer selectedPoints,
		Core::DataEntityHolder::Pointer selectedData );

	virtual ~SurfaceInteractorTriangleSelect( );

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
	blMitkTriangleSelectInteractor::Pointer m_surfaceSelectInteractor;
	//!SelectionSphere
	Core::DataEntityHolder::Pointer m_selectionSphere;

};

} // Core

#endif //coreSurfaceInteractorTriangleSelect_H
