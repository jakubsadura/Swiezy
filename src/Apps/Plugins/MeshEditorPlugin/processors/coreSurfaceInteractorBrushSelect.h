/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreSurfaceInteractorBrushSelect_H
#define coreSurfaceInteractorBrushSelect_H

#include "coreSurfaceInteractor.h"

#include "blMitkTriangleSelectInteractor.h"

namespace Core{

/**
\brief 
\ingroup MeshEditorPlugin
\author Chiara Riccobene
\date 14 Apr 10
*/
class PLUGIN_EXPORT SurfaceInteractorBrushSelect 
	: public Core::SurfaceInteractor
{

// PUBLIC OPERATIONS
public:

	//!
	coreDeclareSmartPointerClassMacro3Param(
		SurfaceInteractorBrushSelect, 
		Core::SurfaceInteractor,
		Core::RenderingTree::Pointer,
		Core::DataEntityHolder::Pointer,
		Core::DataEntityHolder::Pointer );


	//! Get input data entity types
	static Core::DataEntityType GetInputTypes( );

	//!
	int GetTimeStep();

	//!
	void SelectSurface();
	//!
	void DeselectSurface();
	//!
	void SetRadius(double val);
	double GetRadius ();

// PRIVATE OPERATIONS
private:
	//! 
	SurfaceInteractorBrushSelect(
		Core::RenderingTree::Pointer renderingTree,
		Core::DataEntityHolder::Pointer selectedPoints,
		Core::DataEntityHolder::Pointer selectedData );

	virtual ~SurfaceInteractorBrushSelect( );

	//! Redefined
	void CreateInteractor();

	//! Redefined
	void DestroyInteractor();

	//! Redefined
	void OnInteractorConnected();

	//! Return the interactor
	mitk::Interactor* GetInternalInteractor();

	/**
	\brief Get the sphere of the selection 
	(only for surface select interactor)
	*/
	vtkSphere* GetSelectionSphere();

// ATTRIBUTES
private:

	//! Interactor to select a point of a mesh
	blMitkTriangleSelectInteractor::Pointer m_surfaceSelectInteractor;

};

} // Core

#endif //coreSurfaceInteractorBrushSelect_H
