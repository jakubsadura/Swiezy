/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreSkeletonCutProcessor_H
#define _coreSkeletonCutProcessor_H

#include "corePluginMacros.h"
#include "coreDataEntityHolder.h"
#include "coreSmartPointerMacros.h"
#include "coreBaseFilter.h"
#include "coreBaseProcessor.h"

#include "coreDirectionalPlane.h"

#include "meMeshTypes.h"
#include "meRingCut.h"

namespace Core{

/**
Cut the surface using using SkeletonAPI.

\ingroup MeshEditorPlugin
\author Chiara Riccobene
\date 6 nov 2009
*/

class PLUGIN_EXPORT SkeletonCutProcessor : public Core::BaseProcessor
{
public:
	coreDeclareSmartPointerClassMacro(SkeletonCutProcessor, Core::BaseProcessor);

	typedef boost::shared_ptr<DirectionalPlane> DirectionalPlanePtr;
	//!
	void Update();

	meRingCut::Pointer GetRingCut() const;

	//!
	void UpdatePlane();

	//!
	bool CheckInputs();

private:
	/**
	*/
	SkeletonCutProcessor( );	
	//!
	meRingCut::Pointer	m_meRingCut;
	//!
	DirectionalPlanePtr m_plane;
};

} // Core

#endif //_coreSkeletonCutProcessor_H
