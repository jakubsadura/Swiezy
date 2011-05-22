/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
#define _CXXTEST_HAVE_EH
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/ErrorPrinter.h>

int main() {
 return CxxTest::ErrorPrinter().run();
}
#include "BridgeLibTest.h"

static BridgeLibTest suite_BridgeLibTest;

static CxxTest::List Tests_BridgeLibTest = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_BridgeLibTest( "BridgeLibTest.h", 43, "BridgeLibTest", suite_BridgeLibTest, Tests_BridgeLibTest );

static class TestDescription_BridgeLibTest_testMockTextBoxProxy : public CxxTest::RealTestDescription {
public:
 TestDescription_BridgeLibTest_testMockTextBoxProxy() : CxxTest::RealTestDescription( Tests_BridgeLibTest, suiteDescription_BridgeLibTest, 46, "testMockTextBoxProxy" ) {}
 void runTest() { suite_BridgeLibTest.testMockTextBoxProxy(); }
} testDescription_BridgeLibTest_testMockTextBoxProxy;

static class TestDescription_BridgeLibTest_testBridge : public CxxTest::RealTestDescription {
public:
 TestDescription_BridgeLibTest_testBridge() : CxxTest::RealTestDescription( Tests_BridgeLibTest, suiteDescription_BridgeLibTest, 72, "testBridge" ) {}
 void runTest() { suite_BridgeLibTest.testBridge(); }
} testDescription_BridgeLibTest_testBridge;

static class TestDescription_BridgeLibTest_testSetSameValueTwice : public CxxTest::RealTestDescription {
public:
 TestDescription_BridgeLibTest_testSetSameValueTwice() : CxxTest::RealTestDescription( Tests_BridgeLibTest, suiteDescription_BridgeLibTest, 83, "testSetSameValueTwice" ) {}
 void runTest() { suite_BridgeLibTest.testSetSameValueTwice(); }
} testDescription_BridgeLibTest_testSetSameValueTwice;

static class TestDescription_BridgeLibTest_testMockController : public CxxTest::RealTestDescription {
public:
 TestDescription_BridgeLibTest_testMockController() : CxxTest::RealTestDescription( Tests_BridgeLibTest, suiteDescription_BridgeLibTest, 92, "testMockController" ) {}
 void runTest() { suite_BridgeLibTest.testMockController(); }
} testDescription_BridgeLibTest_testMockController;

#include <cxxtest/Root.cpp>
