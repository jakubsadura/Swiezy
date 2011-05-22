/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef corePointInteractorPointSelect_H
#define corePointInteractorPointSelect_H

#include "corePointInteractor.h"

#include "blMitkPointSelectInteractor.h"

namespace Core{

/**
\brief Select a single point
\ingroup gmInteractors
\author Xavi Planes
\date 03-04-09
*/
class PLUGIN_EXPORT PointInteractorPointSelect 
	: public Core::PointInteractor
{

// PUBLIC OPERATIONS
public:
	//!
	coreDeclareSmartPointerClassMacro3Param(
		PointInteractorPointSelect, 
		Core::PointInteractor,
		Core::RenderingTree::Pointer,
		Core::DataEntityHolder::Pointer,
		Core::DataEntityHolder::Pointer );

	/**
	\brief Set the cutting_type 
	(only for point select interactor)
	*/
	void SetCuttingType(CUT_TYPE cutting_type );

	//! Get (one) cell that the selected point belongs to
	int GetCellId();

	//! Redefined
	int GetDirection( );

	//! Redefined
	void SetBoundingBox( double bounds[ 6 ] );

	//! Redefined
	void SetBoundingBox( Core::DataEntity::Pointer dataEntity );

	//! Get input data entity types
	static Core::DataEntityType GetInputTypes( );

	//!
	int GetTimeStep();

// PRIVATE OPERATIONS
private:
	//! 
	PointInteractorPointSelect(
		Core::RenderingTree::Pointer renderingTree,
		Core::DataEntityHolder::Pointer selectedPoints,
		Core::DataEntityHolder::Pointer selectedData );

	virtual ~PointInteractorPointSelect( );

	//! Redefined
	void CreateInteractor();

	//! Redefined
	void DestroyInteractor();

	//! Return the interactor
	mitk::Interactor* GetInternalInteractor();

	//! Change opacity of the father of the skeleton
	void OnInteractorConnected( );

// ATTRIBUTES
private:

	//! Interactor to select a point of a mesh
	blMitkPointSelectInteractor::Pointer m_pointSelectInteractor;

	//!
	CUT_TYPE m_CuttingType;

	//!
	double m_BoundingBox[ 6 ];
};

} // Core

#endif //corePointInteractorPointSelect_H
