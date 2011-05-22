// Copyright 2008 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef coreNumericDataWriter_H
#define coreNumericDataWriter_H

#include "coreBaseDataEntityWriter.h"

namespace Core
{
namespace IO
{
/** 
A specialization of the DataEntityWriter class for VTK PolyData objects.
If wraps the SignalWriter class to be used as a DataEntityWriter that can 
be registered by the Core.

\ingroup gmIO
\author Martin Bianculli
\date Oct 2009
*/
class GMIO_EXPORT NumericDataWriter : public BaseDataEntityWriter
{
public:
	coreDeclareSmartPointerClassMacro(NumericDataWriter, BaseDataEntityWriter);

	//!
	void WriteData( );

protected:
	NumericDataWriter(void);
	virtual ~NumericDataWriter(void);

	//! Write Single data (image)
	void WriteSingleTimeStep( 
		const std::string& fileName, 
		Core::DataEntity::Pointer dataEntity,
		int iTimeStep );

private:
	coreDeclareNoCopyConstructors(NumericDataWriter);
};

}
}

#endif
