// Copyright 2008 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef _coreITKTransformReader_H
#define _coreITKTransformReader_H

#include "coreBaseDataEntityReader.h"

namespace Core
{
namespace IO
{

/** 

\sa DataEntityReader
\author Martín Bianculli
\date 30 Oct 2008
*/
class  ITKTransformReader : public Core::IO::BaseDataEntityReader
{
public:
	coreDeclareSmartPointerClassMacro(ITKTransformReader, BaseDataEntityReader);

	virtual void ReadData();

protected:
	ITKTransformReader(void);
	virtual ~ITKTransformReader(void);

	//!
	boost::any ReadSingleTimeStep( 
		int iTimeStep, 
		const std::string &filename );

private:
	coreDeclareNoCopyConstructors(ITKTransformReader);
};

}
}

#endif //_coreITKTransformReader_H
