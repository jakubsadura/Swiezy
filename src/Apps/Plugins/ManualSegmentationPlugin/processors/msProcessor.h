/*
* Copyright (c) 2010,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#ifndef ManualSegmentationProcessor_H
#define ManualSegmentationProcessor_H

#include "gmProcessorsWin32Header.h"
#include "itkImage.h"
#include "coreDataEntityHolder.h"
#include "coreSmartPointerMacros.h"
#include "coreObject.h"
#include "coreCommonDataTypes.h"
#include "corePluginMacros.h"
#include "coreDataEntityList.h"
#include "coreDataEntityHelper.h"
#include "coreRenderingTree.h"
#include "coreBaseProcessor.h"
#include "coreProcessorFactory.h"

#include <map>
#include <vector>


/**
\brief this is the processor for manual segmentation
\sa ManualSegmentationPanelWidget
\ingroup gmProcessors
\author Albert Sanchez
\date  June 2010
*/

class ManualSegmentationProcessor : public Core::BaseProcessor
{
public:
	//!
	coreProcessor(ManualSegmentationProcessor, Core::BaseProcessor);


	typedef enum
	{
		REFERENCE_IMAGE,
		INPUT_ROI,
		NUMBER_OF_INPUTS,
	} INPUT_TYPE;

	//!
	ManualSegmentationProcessor( );

	//!
	~ManualSegmentationProcessor( );


private:

	//! Purposely not implemented
	ManualSegmentationProcessor( const Self& );

	//! Purposely not implemented
	void operator = ( const Self& );

private:
	
};

#endif //ManualSegmentationProcessor_H

