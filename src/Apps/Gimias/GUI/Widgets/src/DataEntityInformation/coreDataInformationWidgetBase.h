/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreDataInformationWidgetBase_H
#define coreDataInformationWidgetBase_H

#include "coreDataEntity.h"

namespace Core{
namespace Widgets{

/** 
\brief Base interface for all DataInformationWidget
\ingroup gmWidgets
\author Xavi Planes
\date Nov 2010
*/
class DataInformationWidgetBase : public BaseWindow
{
public:
	//!
	virtual bool IsValidData( Core::DataEntity::Pointer dataEntity ) = 0;
	//!
	virtual void SetInputDataEntity( Core::DataEntity::Pointer dataEntity ) = 0;
	//!
	virtual void Clear( ) = 0;
};

} // Widgets
} // Core

#endif // coreDataInformationWidgetBase_H
