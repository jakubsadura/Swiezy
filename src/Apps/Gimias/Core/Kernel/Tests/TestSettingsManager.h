#ifndef testbedTestSettingsManager_h
#define testbedTestSettingsManager_h

#include <cxxtest/TestSuite.h>
#include <coreSettings.h>
#include <coreException.h>
#include <coreXMLSubsystem.h>

class TestSettings : public CxxTest::TestSuite 
{
public:
	/* <Unit test> Checks that settings works with the filesystem and creates proper settings */
	void test1_CreatingAConfigFile( void )
	{
		Core::Runtime::Settings::Pointer settings;
		std::string appName = "TestSuite";
		std::string fileName;
		std::string inputStr;
		FILE* fp;
		//
		// Step1. Create the object
		//
		try
		{
			// First thing to do: Initialize the XML subsystem (settings object is using the XML subsystem)
			Core::IO::XML::XMLSubsystem::Initialize();

			settings = Core::Runtime::Settings::New(appName);
			TS_ASSERT(settings.IsNotNull());
		}
		catch(...)
		{
			TS_FAIL("An exception was caught on Step 1");
		}

		//
		// Step2: Check that the config file is created
		//
		try
		{
			// Check the file exists and contains data
			fileName = settings->GetConfigFileFullPath();
			fp = fopen(fileName.c_str(), "r");
			TS_ASSERT(fp != NULL);
			TS_ASSERT(fscanf(fp, "%s", &inputStr) > 0);
			fclose(fp);
		}
		catch(...)
		{
			TS_FAIL("An exception was caught on Step 2");
		}
		
		//
		// CLEANUP
		//
		try
		{
			// Deinitialize the XML subsystem
			Core::IO::XML::XMLSubsystem::Terminate();
		}
		catch(...)
		{
			TS_FAIL("An exception was caught on cleanup");
		}
		
	}
};

#endif // testbedTestSettingsManager_h
