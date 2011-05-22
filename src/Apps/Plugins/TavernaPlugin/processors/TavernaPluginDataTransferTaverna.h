/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _TavernaPluginDataTransferTaverna_H
#define _TavernaPluginDataTransferTaverna_H

// CoreLib
#include "gmProcessorsWin32Header.h"
#include "coreDynDataTransferCLP.h"

namespace TavernaPlugin{

/**
\brief The inputs/outputs passed to Taverna through disk.

\ingroup TavernaPlugin
\author Xavi Planes
\date 05 Jan 2011
*/
class DataTransferTaverna : public Core::DynDataTransferCLP
{
public:
	//!
	coreDeclareSmartPointerClassMacro(DataTransferTaverna, Core::DynDataTransferBase)

protected:
	//!
	DataTransferTaverna(void);

	//!
	virtual ~DataTransferTaverna(void);

	//!
	void UpdateAllOutputs();

	//!
	void UpdateProcessorOutput( ModuleParameter* param, int num );

	//!
	void CleanTemporaryFiles( );

private:
	//!
	DataTransferTaverna( const Self& );

	//!
	void operator=(const Self&);

protected:
	//!
	std::string m_OutputDirectory;
};

} // namespace TavernaPlugin{


#endif // _TavernaPluginDataTransferTaverna_H


