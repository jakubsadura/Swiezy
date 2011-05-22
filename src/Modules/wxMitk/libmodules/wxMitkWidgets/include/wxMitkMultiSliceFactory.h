/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _wxMitkMultiSliceFactory_H
#define _wxMitkMultiSliceFactory_H

#include "wxMitkWidgetsWin32Header.h"
#include "wxMitkMultiRenderWindow.h"

namespace mitk
{

/*
\brief Factory for wxMitkMultiRenderWindow

\ingroup wxMitkWidgets
\author Xavi Planes
\date 17 Oct 2007
*/
class WXMITKWIDGETS_EXPORT wxMitkMultiSliceFactory
{
public:

	//!
	static wxMitkMultiRenderWindow* Build( 
		wxWindow* parent, 
		wxWindowID id,
		mitk::SliceNavigationController::ViewDirection direction,
		int numSlices,
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize );

	//!
	static void InitRenderWindows( 
		mitk::wxMitkMultiRenderWindow* window,
		mitk::SliceNavigationController::ViewDirection direction,
		int numSlices );

private:

};
 
} // namespace mitk


#endif // _wxMitkMultiSliceFactory_H
