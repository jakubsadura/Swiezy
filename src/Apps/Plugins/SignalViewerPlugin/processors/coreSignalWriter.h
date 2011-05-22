// Copyright 2008 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef coreSignalWriter_H
#define coreSignalWriter_H

#include "coreBaseDataEntityWriter.h"

namespace Core
{
namespace IO
{
/** 
A specialization of the DataEntityWriter class for VTK PolyData objects.
If wraps the SignalWriter class to be used as a DataEntityWriter that can 
be registered by the Core.

\ingroup SignalViewerPlugin
\author Juan Antonio Moya
\date 20 Mar 2008
*/
class SignalWriter : public BaseDataEntityWriter
{
public:
	coreDeclareSmartPointerClassMacro(SignalWriter, BaseDataEntityWriter);

	//!
	void Update( );

protected:
	SignalWriter(void);
	virtual ~SignalWriter(void);

	//! Write Single data (image)
	void WriteSingleTimeStep( 
		const std::string& fileName, 
		Core::DataEntity::Pointer dataEntity,
		int iTimeStep );

private:
	coreDeclareNoCopyConstructors(SignalWriter);
};

}
}

#endif
