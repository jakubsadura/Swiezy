/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreVTKUnstructuredGridWriter_H
#define coreVTKUnstructuredGridWriter_H

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
class GMIO_EXPORT VTKUnstructuredGridWriter : public BaseDataEntityWriter
{
public:
	coreDeclareSmartPointerClassMacro(
		VTKUnstructuredGridWriter, 
		BaseDataEntityWriter);

	void WriteData( );

protected:
	VTKUnstructuredGridWriter(void);
	virtual ~VTKUnstructuredGridWriter(void);
	
	//! Write Single data (volume mesh)
	void WriteSingleTimeStep( 
		const std::string& fileName, 
		Core::DataEntity::Pointer dataEntity,
		int iTimeStep );
private:
	coreDeclareNoCopyConstructors(VTKUnstructuredGridWriter);
};

}
}

#endif
