/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef wxMitkApplicationCursorImplementation_H
#define wxMitkApplicationCursorImplementation_H

#include "wxMitkRenderingWin32Header.h"

#include "mitkApplicationCursor.h"

#include <list>
#include <wx/cursor.h>

class wxWindow;

namespace mitk
{

/**
\brief Manager of the mouse cursor icon

Use a vtkRenderWindow to register this instance. MITK will use this
vtkRenderWindow to access to this registered instance.
This vtkRenderWindow is contained inside a wxWindow. This wxWindow
will be used to change the cursor.

\ingroup wxMitkCore
\author Xavi Planes
\date 18 09 2008
*/
class WXMITKRENDERING_EXPORT wxMitkApplicationCursorImplementation 
	: public ApplicationCursorImplementation
{
public:

	//!
	wxMitkApplicationCursorImplementation( );

	//!
	~wxMitkApplicationCursorImplementation( );

	/// Change the current application cursor
	void PushCursor(const char* XPM[], int hotspotX, int hotspotY);

	/// Restore the previous cursor
	void PopCursor();

	/// Register this instance using vtkWindow and the container wxWindow window
	void SetWindow( 
		vtkRenderWindow *vtkWindow,
		wxWindow *wxwindow );

protected:
	//!
	vtkRenderWindow *m_vtkWindow;

	//! window used to change the cursor
	wxWindow *m_wxwindow;

	//!
	std::list<wxCursor> m_CursorList;
};
 
}

#endif // wxMitkApplicationCursorImplementation_H
