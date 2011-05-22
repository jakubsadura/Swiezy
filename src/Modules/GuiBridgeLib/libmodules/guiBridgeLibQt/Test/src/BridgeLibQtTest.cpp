/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/ErrorPrinter.h>

int main() {
 return CxxTest::ErrorPrinter().run();
}
#include "BridgeLibQtTest.h"

static BridgeLibQtTest suite_BridgeLibQtTest;

static CxxTest::List Tests_BridgeLibQtTest = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_BridgeLibQtTest( "BridgeLibQtTest.h", 13, "BridgeLibQtTest", suite_BridgeLibQtTest, Tests_BridgeLibQtTest );

static class TestDescription_BridgeLibQtTest_testQtGUIAndController : public CxxTest::RealTestDescription {
public:
 TestDescription_BridgeLibQtTest_testQtGUIAndController() : CxxTest::RealTestDescription( Tests_BridgeLibQtTest, suiteDescription_BridgeLibQtTest, 16, "testQtGUIAndController" ) {}
 void runTest() { suite_BridgeLibQtTest.testQtGUIAndController(); }
} testDescription_BridgeLibQtTest_testQtGUIAndController;

#include <cxxtest/Root.cpp>
