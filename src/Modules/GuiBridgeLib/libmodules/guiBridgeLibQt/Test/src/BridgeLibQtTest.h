#ifndef BRIDGELIBQTTEST_H
#define BRIDGELIBQTTEST_H

#include <cxxtest/TestSuite.h>
#include "gblMockController.h"
#include "gblQtTestGUI.h"
#include "gblBridgeQt.h"
#include "QApplication.h"

namespace gbl
{

class BridgeLibQtTest : public CxxTest::TestSuite 
{
public:
	void testQtGUIAndController()
	{
		char* dummyArgv = "";
		int dummyArgc = 1;
		QApplication* qApp = new QApplication(dummyArgc, &dummyArgv);
		gbl::QtTestGUI* gui = new gbl::QtTestGUI();
		qApp->setMainWidget(gui);

		gbl::MockController controller;
		gbl::Bridge bridge;
		controller.SetBridge(bridge["gui"]);
		gui->SetBridge(bridge["gui"]);
		gui->EmulateClick(); // Should fire SetFilename in the controller
		gui->showMaximized();
		qApp->exec();
		int i = bridge["gui"].GetNumber("Type");
		std::string t = bridge["gui"].GetText("Type");
		TS_ASSERT_EQUALS(controller.GetFilename(), "Hello");
	}
};

}

using gbl::BridgeLibQtTest; // needed for the test runner

#endif //BRIDGELIBQTTEST_H
