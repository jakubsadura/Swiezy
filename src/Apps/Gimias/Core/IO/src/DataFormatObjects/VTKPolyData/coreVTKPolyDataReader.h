/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreVTKPolyDataReader_H
#define coreVTKPolyDataReader_H

#include "coreBaseDataEntityReader.h"

namespace Core
{
namespace IO
{
/** 
A specialization of the DataEntityReader class for VTK PolyData objects.
If wraps the VTKPolyDataReader class to be used as a DataEntityReader that 
can be registered by the Core.

\ingroup gmIO
\author Juan Antonio Moya
\date 20 Mar 2008
*/
class GMIO_EXPORT VTKPolyDataReader : public BaseDataEntityReader
{
public:
	coreDeclareSmartPointerClassMacro(
		VTKPolyDataReader, 
		BaseDataEntityReader);

	void ReadData( );

protected:
	VTKPolyDataReader(void);
	virtual ~VTKPolyDataReader(void);

	//!
	virtual boost::any ReadSingleTimeStep( 
		int iTimeStep, 
		const std::string &filename );

private:
	coreDeclareNoCopyConstructors(VTKPolyDataReader);

	//!
	Core::DataEntityType m_DataEntityType;
};

}
}

#endif
