/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreExceptions_H
#define coreExceptions_H

#include <exception>
#include <string>
#include <list>
#include "gmCommonObjectsWin32Header.h"
#include "coreCommonMacros.h"

namespace Core
{
namespace Exceptions
{

/** 
\brief A generic Exception class for exception propagation and error reporting

\section Examples Examples

To throw a generic exception:
\code
throw Core::Exceptions::Exception( 
	"RegisterChild", 
	"Children is already in the list" );
\endcode

To create a new exception class:
\code
coreCreateCoreRuntimeExceptionClassMacro(CannotRetrieveSettingsException, \
	"the program was unable to retrieve the application settings. ");
\endcode

To catch an exception
\code
try
{
	(...)
}
coreCatchExceptionsAddTraceAndThrowMacro(File::Append)
\endcode


\ingroup gmCommonObjects
\author Juan Antonio Moya
\date 03 Jan 2008
*/
class GMCOMMONOBJECTS_EXPORT Exception : public std::exception
{
public:
	coreClassNameMacro(Exception);

	/**
	* Constructor for the Exception class
	* @param callingFunction Name of the function and the class that 
	* originated the exception
	* @param message Message for display to the user describing the error
	*/
	Exception(const char* callingFunction, const char* message);

	virtual ~Exception() throw();

	//! Show only m_message
	virtual const char* what() const throw();

	//! Show comple information
	virtual std::string whatExtended() const throw();

	/** 
	* Adds current routine name to stack trace. That will help to collect the
	* stack trace that displays the error propagation. 
	* @param currentFunction Name of the function and the class that cought 
	* the exception
	*/
	void AddTrace(const char* currentFunction) throw();

	//! Append message to m_message
	void Append(const char* message) throw();

	/** 
	Returns the stack trace of the exception in order to trace the error 
	propagation from its source down to	the last exception handler (catch)
	*/
	std::string GetStackTrace(void) const throw();	

	/**
	Returns the calling function name where it was thrown originally
	*/
	std::string GetCallingFunction(void) const throw();

	/** 
	* Copies the stacktrace for Exception
	* @param Exception object to clone
	*/
	void Clone(Exception& e);

protected:
	std::string m_className;
	std::string m_message;
	std::string m_callingFunction;
	#ifdef _MSC_VER
	#	pragma warning(push)
		// Disables the warning "needs to have dll-interface to be used by clients of class"
	#	pragma warning( disable: 4251 ) 
	#endif
	std::list<const char*> m_stackTrace;
	#ifdef _MSC_VER
	#	pragma warning( pop )
	#endif
};

/** 
* \brief A generic exception class for exception propagation and error reporting, that
* models a runtime exception. When a RuntimeException is thrown, the program
* has not to be terminated, and it can still be recovered from the problem.
\ingroup gmCommonObjects
*/
class GMCOMMONOBJECTS_EXPORT RuntimeException : public Exception
{
public: 
	coreClassNameMacro(RuntimeException);
	RuntimeException(const char* callingFunction, const char* message);
};

/** 
* \brief A generic exception class for exception propagation and error reporting, that
* models a critical exception. When a CriticalException is thrown, the program
* has to be terminated
\ingroup gmCommonObjects
*/
class GMCOMMONOBJECTS_EXPORT CriticalException : public Exception
{
public: 
	coreClassNameMacro(CriticalException);
	CriticalException(const char* callingFunction, const char* message);
};

/** 
* \brief A generic std::exception class for exception propagation and error reporting, that
* models a critical exception. When a StdException is thrown, the program
* has to be terminated
\ingroup gmCommonObjects
*/
class GMCOMMONOBJECTS_EXPORT StdException : public CriticalException
{
public: 
	coreClassNameMacro(StdException);
	StdException(const char* callingFunction, std::exception& exc);
};


} // namespace Exceptions
} // namespace Core

// Macros
#include "coreCreateExceptionMacros.h"
// Include the rest of the exceptions
#include "coreBaseExceptions.h"
//
#include "coreExceptionMacros.h"

#endif
