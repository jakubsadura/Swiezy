#ifndef wxMitkVisor_H
#define wxMitkVisor_H

#include <wxMitkApp.h>

/**
The VisorApp class
*/
class wxMitkSurfaceVisor : public mitk::wxMitkApp
{
public:
	virtual int OnRun(void);
};

DECLARE_APP(wxMitkSurfaceVisor)

#endif // wxMitkVisor_H
