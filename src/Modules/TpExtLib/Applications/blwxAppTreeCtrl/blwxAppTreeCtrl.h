// Copyright 2007 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef blwxAppTreeCtrl_H
#define blwxAppTreeCtrl_H

#include "wx/app.h"

namespace mitk { 
	class wxMitkTestRenderingTreeWidget; 
}

/**
\brief Test application to remove the memory leaks that appear when closing 
GIMIAS, test a crash, test the volume rendering and test LOD of volume
rendering.
\ingroup wxMitk
\author Xavi Planes
\date 13-05-08
*/
class blwxAppTreeCtrl : public wxApp
{
public:
	//! Called before OnRun(), this is a good place to do initialization
	bool OnInit( );

private:
	//! Create the layout of all windows and show main window
	void ShowMainWindow( );

	//! Create main window
	void CreateMainWindow( );

private:
	
	//! Man window
	wxFrame* m_mainWindow;

	DECLARE_EVENT_TABLE();
};

DECLARE_APP(blwxAppTreeCtrl)

#endif // blAppTreeCtrl_H
