#ifndef MAIN_H
#define MAIN_H

/*
This executable can be used to "test" wxMitkDataTreeListView.

\note wxWidgets reports some memory leaks, but I'm not sure if they are actually leaks.
\author Maarten Nieber
\date 25 Oct 2007
*/

#include <wxMitkApp.h>
#include "wx/window.h"

wxWindow* CreateWidget();

//! Define a new application type, each program should derive a class from wxApp
class MyApp : public mitk::wxMitkApp
{
public:
	MyApp()
	{
		this->mainWindow = NULL;
	}

	~MyApp()
	{
	}

	// override base class virtuals
	virtual bool OnInit();
	wxWindow* mainWindow;
};

DECLARE_APP(MyApp)

#endif //MAIN_H
