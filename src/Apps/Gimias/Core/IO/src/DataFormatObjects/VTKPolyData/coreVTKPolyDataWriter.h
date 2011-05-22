/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreVTKPolyDataWriter_H
#define coreVTKPolyDataWriter_H

#include "coreBaseDataEntityWriter.h"

namespace Core
{
namespace IO
{
/** 
A specialization of the DataEntityWriter class for VTK PolyData objects.
If wraps the VTKPolyDataWriter class to be used as a DataEntityWriter 
that can be registered by the Core.

\ingroup gmIO
\author Juan Antonio Moya
\date 20 Mar 2008
*/
class GMIO_EXPORT VTKPolyDataWriter : public BaseDataEntityWriter
{
public:
	coreDeclareSmartPointerClassMacro(
		VTKPolyDataWriter, 
		BaseDataEntityWriter);

	void WriteData( );

protected:
	VTKPolyDataWriter(void);
	virtual ~VTKPolyDataWriter(void);

	//! Write Single data (image)
	void WriteSingleTimeStep( 
		const std::string& fileName, 
		Core::DataEntity::Pointer dataEntity,
		int iTimeStep );

private:
	coreDeclareNoCopyConstructors(VTKPolyDataWriter);
};

}
}

#endif
