/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreITKTransformWriter_H
#define coreITKTransformWriter_H

#include "coreBaseDataEntityWriter.h"


namespace Core
{
namespace IO
{

/** 

\sa DataEntityWriter
\author Chiara Riccobene
\date 03 Mar 2010
*/
class  ITKTransformWriter : public Core::IO::BaseDataEntityWriter
{
public:
	coreDeclareSmartPointerClassMacro(ITKTransformWriter, BaseDataEntityWriter);

	virtual void WriteData();

protected:
	ITKTransformWriter(void);
	virtual ~ITKTransformWriter(void);

private:
	coreDeclareNoCopyConstructors(ITKTransformWriter);
};

}
}

#endif //coreITKTransformWriter.h_H
