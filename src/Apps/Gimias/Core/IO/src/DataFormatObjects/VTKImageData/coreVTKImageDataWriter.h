/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreVTKImageDataWriter_H
#define coreVTKImageDatariter_H

#include "coreBaseDataEntityWriter.h"

namespace Core
{
namespace IO
{
/** 
A specialization of the DataEntityWriter class for VTK ImageData images.
If wraps the vtkImageDataWriter class to be used as a DataEntityWriter 
that can be registered by the Core.

\ingroup gmIO
\author Juan Antonio Moya & Martín Bianculli
\date 14 Apr 2008
*/
class GMIO_EXPORT VTKImageDataWriter : public BaseDataEntityWriter
{
public:
	coreDeclareSmartPointerClassMacro(
		VTKImageDataWriter, 
		BaseDataEntityWriter);

	void WriteData( );

protected:
	VTKImageDataWriter(void);
	virtual ~VTKImageDataWriter(void);

	//! Write Single data (image)
	void WriteSingleTimeStep( 
		const std::string& fileName, 
		Core::DataEntity::Pointer dataEntity,
		int iTimeStep );

private:
	coreDeclareNoCopyConstructors(VTKImageDataWriter);
};

}
}

#endif
