#ifndef MAIN_H
#define MAIN_H

/*
This executable can be used to "test" wxMitkDataTreeListView.

\note wxWidgets reports some memory leaks, but I'm not sure if they are actually leaks.
\author Juan A. Moya
\date 25 Oct 2007
*/

#include "wxMitkApp.h"

wxWindow* CreateWidget();

class MyApp : public mitk::wxMitkApp
{
public:
	// override base class virtuals
	virtual bool OnInit();
};

DECLARE_APP(MyApp)

#endif //MAIN_H
