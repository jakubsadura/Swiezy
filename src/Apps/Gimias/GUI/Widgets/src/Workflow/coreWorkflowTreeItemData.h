/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreWorkflowTreeItemData_H
#define _coreWorkflowTreeItemData_H

#include "coreWorkflow.h"

namespace Core
{

namespace Widgets
{

/** 
\brief item data to store factoryName
\ingroup gmWidgets
\author Xavi Planes
\date 1 June 2010
*/
class WindowItemData : public wxTreeItemData
{
public:
	WindowItemData( const std::string factoryName )
	{
		m_FactoryName = factoryName;
	}

	std::string m_FactoryName;
};


} // namespace Core
} // namespace Widgets

#endif // _coreWorkflowTreeItemData_H
