/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreVTKImageDataReader_H
#define coreVTKImageDataReader_H

#include "coreBaseDataEntityReader.h"

namespace Core
{
namespace IO
{
/** 
A specialization of the DataEntityReader class for VTK ImageData images.
If wraps the vtkImageReader class to be used as a DataEntityReader that 
can be registered by the Core.

\ingroup gmIO
\author Juan Antonio Moya & Martín Bianculli
\date 14 Apr 2008
*/
class GMIO_EXPORT VTKImageDataReader : public BaseDataEntityReader
{
public:
	coreDeclareSmartPointerClassMacro(
		VTKImageDataReader, 
		BaseDataEntityReader);

	void ReadData();

protected:
	VTKImageDataReader(void);
	virtual ~VTKImageDataReader(void);

	//!
	virtual boost::any ReadSingleTimeStep( 
		int iTimeStep, 
		const std::string &filename );

	//! "DataEntityType: ROI 4 [a (1)] [b (2)] [c (3)] [d (4)]"
	void ReadMultiROILevels( const std::string &fileName, std::map<int,std::string> &ROIlevels );

private:
	coreDeclareNoCopyConstructors(VTKImageDataReader);
};

}
}

#endif //coreVTKImageData
