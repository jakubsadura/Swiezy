/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreVTKUnstructuredGridRenDataBuilder_H
#define coreVTKUnstructuredGridRenDataBuilder_H

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
class PLUGIN_EXPORT VTKUnstructuredGridRenDataBuilder 
	: public RenDataBuilder
{
public:
	coreDeclareSmartPointerClassMacro( VTKUnstructuredGridRenDataBuilder, RenDataBuilder );

	//@{ 
	//! \name Overrides of parent implementation.
	void Update( );
	//@}

protected:
	VTKUnstructuredGridRenDataBuilder(void);
	//!
	virtual ~VTKUnstructuredGridRenDataBuilder(void);

private:
	coreDeclareNoCopyConstructors(VTKUnstructuredGridRenDataBuilder);
};

}

#endif
