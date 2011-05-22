/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreSimpleSimpleProgressDialog_H
#define coreSimpleSimpleProgressDialog_H

#include "gmWidgetsWin32Header.h"
#include "coreObject.h"
#include <wx/panel.h>
#include <string>
#include "coreSimpleProgressFrameUI.h"

namespace Core
{
namespace Widgets
{

/** 
\brief A dialog that shows processing message

\ingroup gmWidgets
\author Xavi Planes
\date 04 Mar 2008
*/
class GMWIDGETS_EXPORT SimpleProgressDialog 
	: public coreSimpleProgressFrameUI
{

public:
	SimpleProgressDialog(
		wxWindow* parent, 
		int id, 
		const wxString& title, 
		const wxPoint& pos=wxDefaultPosition, 
		const wxSize& size=wxDefaultSize, 
		long style=wxDEFAULT_DIALOG_STYLE);
	~SimpleProgressDialog(void);

private:
};

} // namespace Widgets
} // namespace Core

#endif // coreSimpleSimpleProgressDialog_H
