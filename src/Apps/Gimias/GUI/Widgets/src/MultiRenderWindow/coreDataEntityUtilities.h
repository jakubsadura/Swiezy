/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreDataEntityUtilities_H
#define coreDataEntityUtilities_H

#include "CILabNamespaceMacros.h"
#include "CILabAssertMacros.h"
#include "coreDataEntity.h"
#include "coreDataContainer.h"

namespace Core{
/**
\brief This is a helper function that calls a filter to adapt a vtkImage 
data inside a DataEntity to an
itkImage representation

\ingroup gmKernel
\author Martín Bianculli
\date 10/04/2007
*/
template <typename outputItkImageType>
typename outputItkImageType::Pointer AdaptToItkImage
(
	Core::DataEntity::Pointer inputImageDataEntity,
	int pixelType
);

/**
\brief This is a helper function that calls a filter to adapt a vtkImage 
data inside a DataEntity to an
itkImage representation

\ingroup gmKernel
\author Xavi Planes
\date 22/05/2008
*/
template <typename outputItkImageType>
typename outputItkImageType::Pointer AdaptVtkToItkImage( 
	Core::DataEntityHolder::Pointer inputImageDataEntityHolder,
	int iTimeStep = 0 );

} // Core

#endif //coreDataEntityUtilities_H


