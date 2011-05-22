#ifndef TestWxMitkColorSelectorControl_H
#define TestWxMitkColorSelectorControl_H

#include <wxMitkApp.h>
class wxColourPickerEvent;

namespace mitk
{
	class wxMitkColorSelectorControl;
}
/*
This executable is used to test wxMitkColorSelectorControl.

\author Juan A. Moya
\date 12 Dec 2007
*/
class TestWxMitkColorSelectorControl : public mitk::wxMitkApp
{
public:
	// override base class virtuals
	virtual int OnRun(void);

private:
	void OnColorChange(wxColourPickerEvent& event);

	mitk::wxMitkColorSelectorControl* control;

	DECLARE_EVENT_TABLE();
};

DECLARE_APP(TestWxMitkColorSelectorControl)

#endif // TestWxMitkColorSelectorControl_H
