/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreITKImageRenDataBuilder_H
#define coreITKImageRenDataBuilder_H

#include "corePluginMacros.h"
#include "coreRenDataBuilder.h"
#include <itkImage.h>
#include <mitkImage.h>

namespace Core
{
/** 
\brief A specialization of the RenDataBuilder class for ITK Image objects.

\ingroup gmDataHandling
\sa RenDataBuilder
\author Juan Antonio Moya
\date 28 Mar 2008
*/
class PLUGIN_EXPORT ITKImageRenDataBuilder : public RenDataBuilder
{
public:
	coreDeclareSmartPointerClassMacro( ITKImageRenDataBuilder, RenDataBuilder );

	//@{ 
	//! \name Overrides of parent implementation.
	void Update( );
	//@}

protected:
	ITKImageRenDataBuilder(void);

	//!
	virtual ~ITKImageRenDataBuilder(void);

	template <class itkImageType> 
	mitk::Image::Pointer GetMitkImageFromItkImage(
		AnyProcessingData processingData);

	mitk::BaseData::Pointer BuildSingleTimeStep(
		const Core::DataEntityImpl::Pointer timeStep);

private:
	coreDeclareNoCopyConstructors(ITKImageRenDataBuilder);
};

}

#endif
