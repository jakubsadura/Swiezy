// Copyright 2008 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef coreNumericDataReader_H
#define coreNumericDataReader_H

#include "coreBaseDataEntityReader.h"

namespace Core
{
namespace IO
{
/** 
A specialization of the DataEntityReader class for signal objects.

\ingroup gmIO
\author Xavi Planes
\date 24 April 2009
*/
class GMIO_EXPORT NumericDataReader : public BaseDataEntityReader
{
public:
	coreDeclareSmartPointerClassMacro(NumericDataReader, BaseDataEntityReader);

	//!
	virtual void ReadData( );

protected:
	NumericDataReader(void);
	virtual ~NumericDataReader(void);

	//!
	virtual boost::any ReadSingleTimeStep( 
		int iTimeStep, 
		const std::string &filename );

private:
	coreDeclareNoCopyConstructors(NumericDataReader);
};

}
}

#endif
