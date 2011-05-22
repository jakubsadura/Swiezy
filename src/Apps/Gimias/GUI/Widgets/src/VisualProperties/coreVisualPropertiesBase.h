/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreVisualPropertiesBase_H
#define coreVisualPropertiesBase_H

#include "coreDataEntity.h"
#include "coreBaseWindow.h"

namespace Core{
namespace Widgets{


/** 
Base class for all visual properties widgets

\ingroup gmWidgets
\author Xavi Planes
\date Nov 2010
*/
class VisualPropertiesBase : public BaseWindow
{
	public:
	//! Check if this widget can work with this DataEntity
	virtual bool IsValidData( Core::DataEntity::Pointer dataEntity ) = 0;
	//! Set input holder used to manage the DataEntity
	virtual void SetInputHolder( Core::DataEntityHolder::Pointer inputHolder ) = 0;
};

} // Widgets
} // Core

#endif // coreVisualPropertiesBase_H
