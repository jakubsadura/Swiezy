/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreMHDVectorFieldReader_H
#define coreMHDVectorFieldReader_H

#include "coreBaseDataEntityReader.h"

namespace Core
{
namespace IO
{
/** 

\ingroup gmIO
\author Martín Bianculli
\date 08 Aug 2008
*/
class GMIO_EXPORT MHDVectorFieldReader : public BaseDataEntityReader
{
public:
	coreDeclareSmartPointerClassMacro(
		MHDVectorFieldReader, 
		BaseDataEntityReader);

	virtual void ReadData( );

protected:
	MHDVectorFieldReader(void);
	virtual ~MHDVectorFieldReader(void);

	//!
	virtual boost::any ReadSingleTimeStep( 
		int iTimeStep, 
		const std::string &filename );

private:
	coreDeclareNoCopyConstructors(MHDVectorFieldReader);
};

}
}

#endif //coreVTKImageData
