/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreStatusBar_H
#define coreStatusBar_H

#include "gmWidgetsWin32Header.h"

#include "wx/statusbr.h"

class wxMemoryUsageIndicator;

namespace Core
{
namespace Widgets
{

/** 
\brief Custom status bar

\ingroup gmWidgets
\author Xavi Planes
\date 23 Dec 2008
*/
class GMWIDGETS_EXPORT StatusBar : public wxStatusBar
{
public:
	StatusBar(wxWindow *parent);
	virtual ~StatusBar();

protected:
	void OnSize(wxSizeEvent& event);

	DECLARE_EVENT_TABLE()

protected:
	wxMemoryUsageIndicator *m_MemoryUsageWidget;
};

} // namespace Widgets
} // namespace Core

#endif // coreStatusBar_H
