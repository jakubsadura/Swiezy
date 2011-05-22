#ifndef BRIDGELIBWXWIDGETSTEST_H
#define BRIDGELIBWXWIDGETSTEST_H

#include <cxxtest/TestSuite.h>
#include "wx/app.h"
#include "gblMockController.h"
#include "gblWxWidgetsTestGUI.h"
#include "gblBridgeWxWidgets.h"

namespace gbl
{

//! Define a new application type, each program should derive a class from wxApp
class MyApp : public wxApp
{
public:
	MyApp()
	{
		frame = new WxWidgetsTestGUI(NULL, -1);
	}

	// override base class virtuals
	virtual bool OnInit()
	{
		frame->Show(true);
		return true;
	}

	WxWidgetsTestGUI *frame;
};

IMPLEMENT_APP(MyApp)

class BridgeLibWxWidgetsTest : public CxxTest::TestSuite 
{
public:
	void testWxWidgetsGUIAndController()
	{
		MyApp* app = new MyApp;
		gbl::MockController controller;
		gbl::BridgePtr bridge(new Bridge);
		controller.SetBridge(bridge);
		app->frame->SetBridge(bridge, "gui.");
		app->frame->EmulateClick(); // Should fire SetFilename in the controller
		app->OnRun();
		TS_ASSERT_EQUALS(controller.GetFilename(), "Hello");
	}
};

}

using gbl::BridgeLibWxWidgetsTest; // needed for the test runner

#endif //BRIDGELIBWXWIDGETSTEST_H
