// Copyright 2007 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef _blwxAppEmbeddedWindow_H
#define _blwxAppEmbeddedWindow_H

#include "wx/app.h"

/**
\brief Embed a calc.exe application under Windows
\ingroup TpExtLib
\author Xavi Planes
\date 7 Jan 2011
*/
class blwxAppEmbeddedWindow : public wxApp
{
public:
	//! Called before OnRun(), this is a good place to do initialization
	bool OnInit( );

private:
	
	//! Man window
	wxFrame* m_mainWindow;

	DECLARE_EVENT_TABLE();
};

DECLARE_APP(blwxAppEmbeddedWindow)

#endif // _blwxAppEmbeddedWindow_H
