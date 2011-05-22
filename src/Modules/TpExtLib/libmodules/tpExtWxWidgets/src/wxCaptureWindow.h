/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef wxCatpureWindow_H
#define wxCatpureWindow_H

#include "TpExtLibWxWidgetsWin32Header.h"
#include <string>
#include <vector>

class wxWindow;

/**
Capture a screen region defined by a window and save it to a filename

\ingroup TpExtLibWxWidgets
\author Xavi Planes
\date 16 April 2009
*/
void TPEXTLIBWXWIDGETS_EXPORT wxCaptureWindow( const wxWindow *inputWindow, const std::string filename );
void TPEXTLIBWXWIDGETS_EXPORT wxCaptureWindowThumbnail( const wxWindow *inputWindow, const std::string filename, int width, int height, bool proportional );
void TPEXTLIBWXWIDGETS_EXPORT wxCaptureWindowsAndMerge( std::vector<wxWindow *> inputWindows, 
															const std::string filename,
															bool rescale,
															int width, 
															int height, 
															bool proportional );
#endif //wxCatpureWindow_H
