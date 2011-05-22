#ifndef testbedTestSettingsIO_h
#define testbedTestSettingsIO_h

#include <cxxtest/TestSuite.h>
#include <coreSettingsIO.h>
#include <coreException.h>
#include <coreXMLSubsystem.h>

class TestSettingsIO : public CxxTest::TestSuite 
{
public:
	/* <Unit test> Checks that SettingsIO works with the filesystem and creates proper settings */
	void test1_CreatingAConfigFile( void )
	{
		Core::IO::SettingsIO::Pointer settingsIO;
		Core::IO::ConfigVars configVars;
		std::string configFileName = "testConfigFile.xml";
		std::string projectFolderName = "TestBed";
		std::string fullConfigFileName;
		std::string inputStr;

		FILE* fp;
		//
		// Step1. Create the object and set a file
		//
		try
		{
			// First thing to do: Initialize the XML subsystem (settings object is using the XML subsystem)
			Core::IO::XML::XMLSubsystem::Initialize();

			settingsIO = Core::IO::SettingsIO::New();
			TS_ASSERT(settingsIO.IsNotNull());
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
			TS_ASSERT_THROWS_NOTHING(settingsIO->TestCreateConfigFile(projectFolderName, configFileName));

			// Check the file exists and contains data
			fp = fopen(fullConfigFileName.c_str(), "r");
			TS_ASSERT(fp != NULL);
			TS_ASSERT(fscanf(fp, "%s", &inputStr) > 0);
			fclose(fp);
		}
		catch(...)
		{
			TS_FAIL("An exception was caught on Step 2");
		}

		//
		// Step3: Check that the contents can be read
		//
		try
		{
			TS_ASSERT_THROWS_NOTHING(settingsIO->ReadConfigFromFile(configFileName, configVars));
			TS_ASSERT(configVars.m_profile.IsNotNull());
			TS_ASSERT(configVars.m_profile->ProfileHasType("Common functionalities"));
			TS_ASSERT(!configVars.m_startedOnce);
		}
		catch(...)
		{
			TS_FAIL("An exception was caught on Step 3");
		}
		
		//
		// CLEANUP: Kill the config file and terminate xml subsystem
		//
		try
		{
			// Deinitialize the XML subsystem
			TS_ASSERT(remove(fullConfigFileName.c_str()) == 0);
			Core::IO::XML::XMLSubsystem::Terminate();
		}
		catch(...)
		{
			TS_FAIL("An exception was caught on cleanup");
		}
		
	}




	/* <Unit test> Checks that SettingsIO writes the configuration properly */
	void test1_WritingToAConfigFile( void )
	{
		Core::IO::SettingsIO::Pointer settingsIO;
		Core::IO::ConfigVars configVars;
		Core::IO::ConfigVars readConfigVars;
		Core::Profile::Pointer profile;
		std::string configFileName = "testConfigFile.xml";
		std::string projectFolderName = "TestBed";
		std::string fullConfigFileName;

		FILE* fp;
		//
		// Step1. Create the object and set a file
		//
		try
		{
			// First thing to do: Initialize the XML subsystem (settings object is using the XML subsystem)
			Core::IO::XML::XMLSubsystem::Initialize();

			settingsIO = Core::IO::SettingsIO::New();
			TS_ASSERT(settingsIO.IsNotNull());
		}
		catch(...)
		{
			TS_FAIL("An exception was caught on Step 1");
		}

		//
		// Step2: Create a default config file
		//
		try
		{
			TS_ASSERT_THROWS_NOTHING(settingsIO->TestCreateConfigFile(projectFolderName, configFileName));
		}
		catch(...)
		{
			TS_FAIL("An exception was caught on Step 2");
		}

		//
		// Step3: Write some configuration
		//
		try
		{
			configVars.m_profile = Core::Profile::New();
			configVars.m_profile->AddToProfile("TestWorkingSet1");
			configVars.m_profile->AddToProfile("TestWorkingSet2");
			configVars.m_profile->AddToProfile("TestWorkingSet3");
			configVars.m_startedOnce = true;
			std::string dtdFileFullPath = "";
			TS_ASSERT_THROWS_NOTHING(settingsIO->WriteConfigToFile(configFileName, dtdFileFullPath, "MyProject",configVars));

			
		}
		catch(...)
		{
			TS_FAIL("An exception was caught on Step 2");
		}


		//
		// Step4: Check the written configuration is readable and correct
		//
		try
		{
			TS_ASSERT_THROWS_NOTHING(settingsIO->ReadConfigFromFile(configFileName, readConfigVars));
			TS_ASSERT(configVars.m_profile->ProfileIsOfType(readConfigVars.m_profile));
			TS_ASSERT_EQUALS(configVars.m_startedOnce, readConfigVars.m_startedOnce);
		}
		catch(...)
		{
			TS_FAIL("An exception was caught on Step 2");
		}
		
		//
		// CLEANUP: Kill the config file and terminate xml subsystem
		//
		try
		{
			// Deinitialize the XML subsystem
			TS_ASSERT(remove(fullConfigFileName.c_str()) == 0);
			Core::IO::XML::XMLSubsystem::Terminate();
		}
		catch(...)
		{
			TS_FAIL("An exception was caught on cleanup");
		}
		
	}
};

#endif // testbedTestSettingsIO_h
