/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreMainContextMenu_H
#define coreMainContextMenu_H

#include "gmWidgetsWin32Header.h"
#include "coreObject.h"

namespace Core
{
namespace Widgets
{

/** 
\brief Main context menu for the Core application

\ingroup gmWidgets
\author Xavi Planes
\date 8 April 2010
*/
class GMWIDGETS_EXPORT MainContextMenu : public wxMenu
{

public:
	coreClassNameMacro(MainContextMenu)

	//!
	MainContextMenu( );

protected:
	
	//!
	void UpdateMenu( );
private:

};

} // namespace Core
} // namespace Widgets

#endif
