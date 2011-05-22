/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreITKTensorRenDataBuilder_H
#define coreITKTensorRenDataBuilder_H

#include "corePluginMacros.h"
#include "coreRenDataBuilder.h"
#include <itkImage.h>
#include <mitkImage.h>

namespace Core
{
/** 
\brief A specialization of the RenDataBuilder class for ITK Tensor objects.

\ingroup gmDataHandling
\sa RenDataBuilder
\author Chiara Riccobene
\date 10 Jun 2010
*/
class PLUGIN_EXPORT ITKTensorRenDataBuilder : public RenDataBuilder
{
public:
	coreDeclareSmartPointerClassMacro( ITKTensorRenDataBuilder, RenDataBuilder );

	//@{ 
	//! \name Overrides of parent implementation.
	void Update( );
	//@}

protected:
	ITKTensorRenDataBuilder(void);

	//!
	virtual ~ITKTensorRenDataBuilder(void);


	mitk::BaseData::Pointer BuildSingleTimeStep(
		const Core::DataEntityImpl::Pointer timeStep);

	mitk::Image::Pointer GetMitkImageFromItkImage(
		AnyProcessingData processingData);

private:
	coreDeclareNoCopyConstructors(ITKTensorRenDataBuilder);
};

}

#endif
