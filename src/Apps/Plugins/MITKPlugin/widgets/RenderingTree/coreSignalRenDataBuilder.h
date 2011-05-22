/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreSignalRenDataBuilder_H
#define _coreSignalRenDataBuilder_H

#include "corePluginMacros.h"
#include "coreRenDataBuilder.h"

namespace Core
{
/** 
\brief A specialization of the RenDataBuilder class for VTK PolyData objects.

\ingroup gmDataHandling
\sa RenDataBuilder
\author Juan Antonio Moya
\date 28 Mar 2008
*/
class PLUGIN_EXPORT SignalRenDataBuilder : public RenDataBuilder
{
public:
	coreDeclareSmartPointerClassMacro( SignalRenDataBuilder, RenDataBuilder );

	//@{ 
	//! \name Overrides of parent implementation.
	void Update( );
	//@}

protected:
	SignalRenDataBuilder(void);

	//!
	virtual ~SignalRenDataBuilder(void);

	//! Check processing data is OK
	bool CheckProcessingDataIsOk( const Core::DataEntityImpl::Pointer timeStep );

private:
	coreDeclareNoCopyConstructors(SignalRenDataBuilder);

};

}

#endif
