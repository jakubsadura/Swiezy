#ifndef testbedTestLogger_h
#define testbedTestLogger_h

#include <cxxtest/TestSuite.h>
#include <coreLogger.h>
#include <coreException.h>

class TestLogger : public CxxTest::TestSuite 
{
public:

	/* <Story test> Checks that logger works with the filesystem */
	void test1_SettingFilenameAndWrittingAMessage( void )
	{
		std::string fileName = "testLogFile.txt";
		std::string message = "Testing the Logger";
		std::string gotFileName;
		std::string inputStr;
		FILE* fp = NULL;
		Core::Runtime::Logger::Pointer logger;

		//
		// Step1. Create it
		//
		try
		{
			logger = Core::Runtime::Logger::New();
			TS_ASSERT(logger.IsNotNull());
		}
		catch(...)
		{
			TS_FAIL("An exception was caught on Step 1");
		}

		//
		// Step2. Assign a filename, log a message and check the file exists
		//
		try
		{
			// Set a filename "testLogFile.txt"
			TS_ASSERT_THROWS_NOTHING(logger->SetFileName(fileName));
			// Check that filename is correctly set 
			gotFileName = logger->GetFileName();
			TS_ASSERT_EQUALS(fileName, gotFileName);

			// Log the message "Testing the Logger"
			TS_ASSERT_THROWS_NOTHING(logger->LogMessage(message));

			// Check the file exists
			fp = fopen(fileName.c_str(), "r");
			TS_ASSERT(fp != NULL);
			fclose(fp);
		}
		catch(...)
		{
			TS_FAIL("An exception was caught on Step 2");
		}


		//
		// Step3. Log an exception and check the file contents
		//
		try
		{
			// Log an exception
			Core::Exceptions::RuntimeException e("DummyRuntimeException", "Exception provided as test");
			TS_ASSERT_THROWS_NOTHING(logger->LogException(e));

			// Check that the contents are well written
			fp = fopen(fileName.c_str(), "r");
			TS_ASSERT(fp != NULL);
			TS_ASSERT(fscanf(fp, "%s", &inputStr) > 0);
			fclose(fp);
		}
		catch(...)
		{
			TS_FAIL("An exception was caught on Step 3");
		}


		//
		// Step4. Clear the logfile
		//
		try
		{
			TS_ASSERT_THROWS_NOTHING(logger->Clear());
		}
		catch(...)
		{
			TS_FAIL("An exception was caught on Step 4");
		}

		//
		// CLEANUP: kill the logfile
		//
		try
		{
			TS_ASSERT(remove(fileName.c_str()) == 0);
		}
		catch(...)
		{
			TS_FAIL("An exception was caught on cleanup");
		}
		
	}
};

#endif // testbedTestLogger_h
