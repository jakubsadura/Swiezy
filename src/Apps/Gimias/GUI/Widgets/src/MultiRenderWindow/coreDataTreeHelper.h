/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreDataTreeHelper_H
#define coreDataTreeHelper_H

#include "gmWidgetsWin32Header.h"
#include "coreObject.h"
#include "coreDataEntity.h"
#include "coreRenderingTree.h"

namespace Core{


/**
\brief Helper functions for mitk DataStorage
\ingroup gmKernel
\author Xavi Planes
\date 07-05-08
*/
class GMWIDGETS_EXPORT DataTreeHelper : public Core::Object
{
// OPERATIONS
public:

	/** 
	\brief Add dataEntityHolderOutput to the DataEntityList, render it and
	set it as selected
	*/
	static void PublishOutput(  
						Core::DataEntityHolder::Pointer dataEntityHolderOutput,
						Core::RenderingTree::Pointer tree,
						bool bInitializeViews = true,
						bool bSetSelected = true,
						bool bShow = true);

};

} // Core

#endif //coreDataTreeHelper_H

