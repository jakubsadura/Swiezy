#ifndef BRIDGELIBTEST_H
#define BRIDGELIBTEST_H

#include <cxxtest/TestSuite.h>
#include "gblBridge.h"
#include "gblMockController.h"
#include "gblException.h"
#include <limits>

namespace gbl
{

gblDeclareExceptionMacro(TestControllerException, std::exception)

class MockGUI
{
public:
	MockGUI()
	{
		txtFilename.value = "Hello";
	}

	void SetBridge( boost::shared_ptr<Bridge> _bridge, const std::string& _prefix )
	{
		this->bridge = _bridge;
		if( !this->bridge.get() )
			return;

		AddBridgeValue(this->bridge, _prefix + "Filename", &this->txtFilename);
	}

	//! Simulate pressing the button "SetFilename".
	void PressButtonSetFilename()
	{
		this->bridge->CallEvent("gui.SetFilename");
	}

private:
	boost::shared_ptr<Bridge> bridge;
	MockTextBox txtFilename;
};

class BridgeLibTest : public CxxTest::TestSuite 
{
public:
	void testMockTextBoxProxy()
	{
		MockTextBox value;
		MockTextBoxValueProxy proxy(&value);

		proxy.SetText("Yes");
		TS_ASSERT_EQUALS( proxy.GetText(), "Yes" );
		TS_ASSERT_EQUALS( proxy.GetFlag(), true );
		TS_ASSERT_THROWS(proxy.GetNumber(), std::bad_cast); // should throw, as "Yes" is not a number

		proxy.SetText("NO");
		TS_ASSERT_EQUALS( proxy.GetFlag(), false );

		proxy.SetText("Maybe");
		TS_ASSERT_THROWS(proxy.GetFlag(), std::bad_cast);

		// Create a number that is more precise than can be stored in a double
		double preciseNumber = 0.12345678901234567890;
		TS_ASSERT(std::string("12345678901234567890").size() > std::numeric_limits<double>::digits10);

		// Check that this precise number can be converted to a string and back stored in a double
		proxy.SetNumber(preciseNumber);
		proxy.SetText(proxy.GetText());
		TS_ASSERT_EQUALS( proxy.GetNumber(), preciseNumber );
	}

	void testBridge()
	{
		gbl::BridgePtr bridge(new gbl::Bridge);
		MockTextBox value1, value2;
		gbl::AddBridgeValue(bridge, "value1", &value1);
		TS_ASSERT_THROWS(gbl::AddBridgeValue(bridge, "value1", &value2), gbl::BridgeException);

		value1.value = "1.234";
		TS_ASSERT_EQUALS(bridge->GetNumber("value1"), 1.234);
	}

	void testSetSameValueTwice()
	{
		Bridge bridge;
		MockTextBox mockTextBox;
		gbl::ValueProxyPtr p(new MockTextBoxValueProxy(&mockTextBox));
		bridge.SetValueProxy("test", p);
		TS_ASSERT_THROWS_NOTHING(bridge.SetValueProxy("test", p));
	}

	void testMockController()
	{
		BridgePtr bridge(new Bridge);
		MockController controller;
		controller.SetBridge(bridge);
		MockGUI gui;
		gui.SetBridge(bridge, "gui.");

		// Simulate the "SetFilename" event. This will raise the "SetFilename" event, calling controller->SetFilename, which
		// will obtain the filename from the bridge and store it in a copyOfFilename buffer.
		gui.PressButtonSetFilename();

		// controller.GetFilename() will return the value of the copyOfFilename buffer (see above). Check that this value
		// is equal to the value which is stored in the GUI: "Hello"
		TS_ASSERT_EQUALS(controller.GetFilename(), "Hello");
	}
};

}

using gbl::BridgeLibTest; // needed for the test runner

#endif //BRIDGELIBTEST_H
