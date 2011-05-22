/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

//A logger for all command history in every Core procedure

#include "coreException.h"
#include "coreReportExceptionMacros.h"
#include "coreLogger.h"
#include <ctime>
#include <iostream>
#include <string.h>

using namespace Core::Runtime;

/** 
Initializer for Logger class. It creates the link to the specified log file and opens it for appending messages 
\param filename path and name of the log file
*/
Logger::Logger()
{
	this->m_FileName = "";
	this->_fp = NULL;
}

/** Destructor for Logger class. It closes the linked log file and clear its pointer */
Logger::~Logger(void)
{
	// Close any file pointer
	if(this->_fp != NULL) 
	{
		fclose(this->_fp);
		this->_fp = NULL;
	}
}

/** 
Specifies a new file for logging output. It closes the linked log file, links to the new one 
and opens it for append 
\param filename Name of the new file for logging messages 
*/
void Logger::SetFileName(const std::string& filename)
{
	// Close any file pointer before asigning a new file
	if(this->_fp != NULL) fclose(this->_fp);

	this->m_FileName = filename;

	this->_fp = fopen(this->m_FileName.c_str(), "a");

	// If file cannot be opened, throw a exception
	if(this->_fp == NULL)
	{
		Core::Exceptions::CannotOpenFileException e("Logger::SetFile");
		e.Append("File was: ");
		e.Append(filename.c_str());
		throw e;
	}

	// Log a new entry or easing the reading of the logfile
	AppendNewLogEntry();

	if(this->_fp != NULL) fclose(_fp);
	this->_fp = NULL;
}


/** Appends the message to the log file, adding the timestamp
*/
void Logger::LogMessage(const std::string& message)
{
	time_t hold_time;
	std::string timeString;

	// Determine time and inputs to a string without newline chars
	hold_time = time(NULL);
	timeString = ctime(&hold_time);

	// Open for append
	this->_fp = fopen(this->m_FileName.c_str(), "a");

	// Appends the message to the file
	if(fprintf(this->_fp, "%s> %s\n", timeString.c_str(), message.c_str()) < 0)
	{
		// If there was an error writing to the logfile, throw an exception
		fclose(this->_fp);
		this->_fp = NULL;
		Core::Exceptions::CannotWriteFileException e("Logger::LogMessage");
		e.Append("File was: ");
		e.Append(this->m_FileName.c_str());
		throw e;
	}

	// Close
	fclose(this->_fp);
	this->_fp = NULL;
}

/** Appends the exception message to the log file, adding the timestamp.
*/
void Logger::LogException(Core::Exceptions::Exception& e) throw()
{
	std::string message;
	try
	{
		// Logs the extended message
		message = e.whatExtended();
		message.append("\n---  Stack Trace --- \n");
		message.append(e.GetStackTrace());
		LogMessage(message);
	}
	catch(...)
	{
		// Do nothing and throw nothing (otherwise it will compromise stability 
		// if we throw new Exceptions inside the caller's try/catch statements)
	}

}

/** Clear all messages of the logfile */
void Logger::Clear(void)
{
	std::string message;
	try
	{
		if(this->_fp != NULL)
		{
			fclose(this->_fp);
			this->_fp = NULL;
		}
		remove(this->m_FileName.c_str());
		this->_fp = fopen(this->m_FileName.c_str(), "a");

		// If file cannot be opened, throw a exception
		if(this->_fp == NULL)
		{
			Core::Exceptions::CannotOpenFileException e("Logger::Clear");
			e.Append("File was: ");
			e.Append(this->m_FileName.c_str());
			throw e;
		}

		// Log a new entry or easing the reading of the logfile
		AppendNewLogEntry();

		if(this->_fp != NULL) fclose(this->_fp);
		this->_fp = NULL;
		
		message = "Log file was cleared";
		LogMessage(message);
	}
	coreCatchExceptionsAddTraceAndThrowMacro(Logger::Clear);
}

/** Appends a new entry message to the log file, easing the reading of the log */
void Logger::AppendNewLogEntry(void)
{
	time_t hold_time;
	std::string timeString;
	std::string message;
	try
	{
		if(this->_fp != NULL)
		{
			// Determines time and inputs to a string without newline chars
			hold_time = time(NULL);
			timeString = ctime(&hold_time);
			// Remove the '\n' char at the end
			timeString.erase(timeString.length()-1, 1);
			message = "-- New log entry on " + timeString + " ----------------------------------\n";
			// Appends the message to the file
			if(fprintf(this->_fp, "\n%s ", message.c_str()) < 0)
			{
				// If there was an error writing to the logfile, throw an exception
				Core::Exceptions::CannotWriteFileException e("Logger::AppendNewLogEntry");
				e.Append("File was: ");
				e.Append(this->m_FileName.c_str());
				throw e;
			}
		}
	}
	coreCatchExceptionsAddTraceAndThrowMacro(Logger::AppendNewLogEntry);
}

/** Appends a new entry message to the log file, easing the reading of the log */
std::string Logger::ReadLogFileGetFullText(void)
{
	std::string text = "";
	try
	{
		if(this->_fp != NULL)
		{
			fclose(this->_fp);
			this->_fp = NULL;
		}
		
		this->_fp = fopen(this->m_FileName.c_str(), "r");

		// If file cannot be opened, throw a exception
		if(this->_fp == NULL)
		{
			Core::Exceptions::CannotOpenFileException e("Logger::ReadLogFileGetFullText");
			e.Append("File was: ");
			e.Append(this->m_FileName.c_str());
			throw e;
		}

		// Get the length of the file
		fseek(this->_fp, 0, SEEK_END);
		long nread = ftell(_fp); 
		fseek(this->_fp, 0, SEEK_SET);

		if(nread > 0)
		{
			// Get the contents of the file
			char* buffer = new char[nread];
			memset(buffer, '\0', nread*sizeof(char));
			if(fread(buffer, nread, sizeof(char), this->_fp) < 0)
			{
				Core::Exceptions::CannotReadFileException e("Logger::ReadLogFileGetFullText");
				e.Append("File was: ");
				e.Append(this->m_FileName.c_str());
				throw e;
			}
			text = buffer;
			delete buffer;
		}

		if(this->_fp != NULL) fclose(this->_fp);
		this->_fp = NULL;
	}
	coreCatchExceptionsAddTraceAndThrowMacro(Logger::ReadLogFileGetFullText);

	return text;
}

/**
 */
std::string Core::Runtime::Logger::GetFileName() const
{
	return m_FileName;
}
