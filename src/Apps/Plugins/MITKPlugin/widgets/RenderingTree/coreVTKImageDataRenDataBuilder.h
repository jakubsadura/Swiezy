/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreVTKImageDataRenDataBuilder_H
#define coreVTKImageDataRenDataBuilder_H

#include "corePluginMacros.h"
#include "coreRenDataBuilder.h"

namespace Core
{
/** 
\brief A specialization of the RenDataBuilder class for a vector of 
VTK ImageData objects (3D+t)
The vector can also contain only one image (3D)

\note The mitk data is using a reference to the buffer of the vtkImageData,
so please don't destroy the VTKImageData before destroying this object.

\ingroup gmDataHandling
\sa RenDataBuilder
\author Juan Antonio Moya 
\date 14 Apr 2008
*/
class PLUGIN_EXPORT VTKImageDataRenDataBuilder 
	: public RenDataBuilder
{
public:
	coreDeclareSmartPointerClassMacro( VTKImageDataRenDataBuilder, RenDataBuilder );

	//@{ 
	//! \name Overrides of parent implementation.
	void Update( );
	//@}

protected:
	VTKImageDataRenDataBuilder(void);
	//!
	virtual ~VTKImageDataRenDataBuilder(void);

private:
	coreDeclareNoCopyConstructors(VTKImageDataRenDataBuilder);

};

}

#endif
