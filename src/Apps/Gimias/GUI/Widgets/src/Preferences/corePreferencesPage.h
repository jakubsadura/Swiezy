/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _corePreferencesPage_H
#define _corePreferencesPage_H

#include "gmWidgetsWin32Header.h"
#include "coreBaseWindow.h"

namespace Core
{
namespace Widgets
{
/** 
\brief Interface for a page that will be added to preferences dialog

\ingroup gmWidgets
\author Xavi Planes
\date Jan 2011
*/
class PreferencesPage : public Core::BaseWindow
{
public:
	//! When Ok is pressed
	virtual void UpdateData( ) = 0;

	//! When the dialog is shown
	virtual void UpdateWidget( ) = 0;

protected:

private:

};
}
}

#endif // _corePreferencesPage_H
