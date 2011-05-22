/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _wxEventHandlerHelper_H
#define _wxEventHandlerHelper_H

#include "TpExtLibWxWidgetsWin32Header.h"

/**
This function removes an event handler from the stack

\ingroup TpExtLibWxWidgets
\author Xavi Planes
\date 26 August 2010
*/

void TPEXTLIBWXWIDGETS_EXPORT wxPopEventHandler(wxWindow* win, wxEvtHandler* handler);

#endif //_wxEventHandlerHelper_H
