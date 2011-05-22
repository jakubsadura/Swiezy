// Copyright 2008 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef coreSignalReader_H
#define coreSignalReader_H

#include "coreBaseDataEntityReader.h"

namespace Core
{
namespace IO
{
/** 
A specialization of the DataEntityReader class for signal objects.

\ingroup SignalViewerPlugin
\author Xavi Planes
\date 24 April 2009
*/
class SignalReader : public BaseDataEntityReader
{
public:
	coreDeclareSmartPointerClassMacro(SignalReader, BaseDataEntityReader);

	//!
	virtual void Update( );

	//!
	bool ReadHeader( );

protected:
	SignalReader(void);
	virtual ~SignalReader(void);

	//!
	virtual boost::any ReadSingleTimeStep( 
		int iTimeStep, 
		const std::string &filename );

private:
	coreDeclareNoCopyConstructors(SignalReader);
};

}
}

#endif
