/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _SandboxPluginResampleProcessor_H
#define _SandboxPluginResampleProcessor_H

#include "coreBaseProcessor.h"
#include "coreProcessorFactory.h"

#include "itkImage.h"
#include "itkScaleTransform.h"

namespace SandboxPlugin{

/**
Processor for Resampleing an image

\ingroup SandboxPlugin
\author Xavi Planes
\date 16 feb 2009
*/
class ResampleProcessor : public Core::BaseProcessor
{
public:
	typedef itk::Image<float,3> ImageType;
	typedef itk::ScaleTransform<double, 3> ScaleTransformType;

	typedef enum
	{
		INPUT_IMAGE,
		INPUT_POINT
	} INPUT_TYPE;

public:
	//!
	coreProcessor(ResampleProcessor, Core::BaseProcessor);

	//! Call library to perform operation
	void Update( );

	//!
	ScaleTransformType::Pointer GetScaleTransform() const;

private:
	//!
	ResampleProcessor();

	//!
	~ResampleProcessor();

	//! Purposely not implemented
	ResampleProcessor( const Self& );

	//! Purposely not implemented
	void operator = ( const Self& );

private:

	ScaleTransformType::Pointer m_ScaleTransform;
};
    
} // namespace SandboxPlugin{

#endif //_SandboxPluginResampleProcessor_H
