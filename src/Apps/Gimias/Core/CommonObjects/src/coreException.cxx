/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

//A generic Exception class for exception propagation and error reporting

#include "coreException.h"
#include <typeinfo>
#include <cstring>

using namespace Core::Exceptions;

/** 
*/
Exception::Exception(const char* callingFunction, const char* message) : std::exception()
{
	this->m_className = GetNameOfClass();
	this->m_callingFunction = callingFunction;
	this->m_message = message;
	this->AddTrace(callingFunction);
}

/**
*/
const char* Exception::what() const throw()
{
	return m_message.c_str();
}

/**
 */
std::string Exception::whatExtended() const throw()
{
	// Message constructed here because this->GetNameOfClass() has to be resolved by children classes
	// and this is not able by constructor

	//char* ret;
	std::string message;
	try
	{
		message = "Exception caught (";
		message += this->m_className;
		message += ") in ";
		message += this->m_callingFunction;
		message += ":\n "; 
		message += this->m_message;

		// Commented these lines because this generates memory leaks
		//ret = new char[message.length()];
		//strcpy(ret, message.c_str());
	}
	catch(...)
	{
		return ""; 
	}
	return message;
}

/** 
*/
void Exception::AddTrace(const char* currentFunction) throw()
{
	try
	{
		this->m_stackTrace.push_back(currentFunction);
	}
	catch(...){;}
}

/**
*/
void Exception::Append(const char* message) throw()
{
	this->m_message += message;
}

/**
 */
void Exception::Clone(Exception& e)
{
	this->m_stackTrace = e.m_stackTrace;
}

/**
*/
Exception::~Exception() throw()
{
}
			

/**
*/
std::string Exception::GetStackTrace(void) const throw()
{
	//char* ret;
	std::list<const char*>::const_iterator it;
	std::string outMessage = "";
	try
	{
		// Concatenate the list to a string
		for( it = this->m_stackTrace.begin(); it != this->m_stackTrace.end(); ++it )
		{
			outMessage += (*it);
			outMessage += "\n";
		}
		// Commented these lines because this generates memory leaks
		//ret = new char[outMessage.length() + 1];
		//strcpy(ret, outMessage.c_str());
	}
	catch(...)
	{
		return ""; 
	}
	return outMessage;
}

/**
*/
std::string Exception::GetCallingFunction(void) const throw()
{
	return this->m_callingFunction;
}

/**
*/
RuntimeException::RuntimeException(const char* callingFunction, const char* message) 
: Exception(callingFunction, message)
{
	this->m_className = GetNameOfClass();
}


/**
*/
CriticalException::CriticalException(const char* callingFunction, const char* message) 
: Exception(callingFunction, message)
{ 
	// Xavi: We don't need to terminate the program
	//this->m_message += " The program will be terminated.";
	this->m_className = GetNameOfClass();
}

/**
*/
StdException::StdException(const char* callingFunction, std::exception& exc) 
: CriticalException(callingFunction, exc.what())
{
	this->m_className = GetNameOfClass();
}
