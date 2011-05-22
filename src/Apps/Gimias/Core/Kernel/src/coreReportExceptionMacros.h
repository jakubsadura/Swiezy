/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreExceptionsMacros_H
#define coreExceptionsMacros_H

#include "gmKernelWin32Header.h"
#include "coreException.h"
#include "CILabNamespaceMacros.h"

namespace Core{

/**
\brief Shows a message box with the exception description
 */
void GMKERNEL_EXPORT LogExceptionWithLoggerAndGraphicalInterface(
	Core::Exceptions::Exception& e);

/**
\brief Log an exception and log into the main window without message box
*/
void GMKERNEL_EXPORT LogException(Core::Exceptions::Exception& e);


} // Core

//////////////////////////////////////////////////////////
// Macros that ease exception handling and unify the	//
// behaviour for their proper reporting and propagation //
//////////////////////////////////////////////////////////
/** 
\author Juan Antonio Moya
\date 12 Jan 2007
*/

#ifdef coreCatchExceptionsReportAndNoThrowMacro
#undef coreCatchExceptionsReportAndNoThrowMacro
#endif
/**
\def coreCatchExceptionsReportAndNoThrowMacro(callingFunction)
coreCatchExceptionsReportAndNoThrowMacro is a macro for catching exceptions 
thrown by a try { } block. 
It do not propagate the exception, but logs it and reports it to the 
standard error channels and graphical interface.

\param callingFunction Name of the function that throws the Exception
\ingroup gmKernel
*/
#define coreCatchExceptionsReportAndNoThrowMacro(callingFunction) \
	catch(Core::Exceptions::Exception& e) { \
	if( e.GetCallingFunction() != #callingFunction ) \
	e.AddTrace(#callingFunction); \
	Core::LogExceptionWithLoggerAndGraphicalInterface(e); \
	} \
	catch(std::exception& stdEx) \
	{ \
	Core::Exceptions::StdException e(#callingFunction, stdEx); \
	Core::LogExceptionWithLoggerAndGraphicalInterface(e); \
	} \
	catch(...) \
	{ \
	Core::Exceptions::UnhandledException e(#callingFunction); \
	Core::LogExceptionWithLoggerAndGraphicalInterface(e); \
	} 


/**
\brief One new parameter for restoring state after an exception
\ingroup gmKernel
*/
#define coreCatchExceptionsReportAndNoThrowMacro2(callingFunction, callbackFunction) \
	catch(Core::Exceptions::Exception& e) { \
	if( e.GetCallingFunction() != #callingFunction ) \
	e.AddTrace(#callingFunction); \
	Core::LogExceptionWithLoggerAndGraphicalInterface(e); \
	callbackFunction;\
	} \
	catch(std::exception& stdEx) \
	{ \
	Core::Exceptions::StdException e(#callingFunction, stdEx); \
	Core::LogExceptionWithLoggerAndGraphicalInterface(e); \
	callbackFunction;\
	} \
	catch(...) \
	{ \
	Core::Exceptions::UnhandledException e(#callingFunction); \
	Core::LogExceptionWithLoggerAndGraphicalInterface(e); \
	callbackFunction;\
	} 



#ifdef coreCatchExceptionsLogAndNoThrowMacro
#undef coreCatchExceptionsLogAndNoThrowMacro
#endif
/**
\def coreCatchExceptionsLogAndNoThrowMacro(callingFunction)
coreCatchExceptionsReportAndNoThrowMacro is a macro for catching exceptions 
thrown by a try { } block. 
It do not propagate the exception, but logs it and reports it to the 
log file.

\param callingFunction Name of the function that throws the Exception
\ingroup gmKernel
*/
#define coreCatchExceptionsLogAndNoThrowMacro(callingFunction) \
	catch(Core::Exceptions::Exception& e) { \
	if( e.GetCallingFunction() != #callingFunction ) \
	e.AddTrace(#callingFunction); \
	Core::LogException(e); \
	} \
	catch(std::exception& stdEx) \
	{ \
	Core::Exceptions::StdException e(#callingFunction, stdEx); \
	Core::LogException(e); \
	} \
	catch(...) \
	{ \
	Core::Exceptions::UnhandledException e(#callingFunction); \
	Core::LogException(e); \
	} 



#ifdef coreCatchOnlyRuntimeExceptionsReportAndNoThrowMacro
#undef coreCatchOnlyRuntimeExceptionsReportAndNoThrowMacro
#endif
/**
\def coreCatchOnlyRuntimeExceptionsReportAndNoThrowMacro(callingFunction)
coreCatchOnlyRuntimeExceptionsReportAndNoThrowMacro is a macro for catching 
exceptions thrown by a try { } block. 
It do not propagate the runtime exception, but logs it and reports it to 
the standard error channels. Not that this
macro only catches RuntimeExceptions. Other types of exception are not caught.
coreCatchOnlyRuntimeExceptionsReportAndNoThrowMacro do not throw exceptions
\param callingFunction Name of the function that throws the Exception
\ingroup gmKernel
*/
#define coreCatchOnlyRuntimeExceptionsReportAndNoThrowMacro(callingFunction) \
	catch(Core::Exceptions::RuntimeException& e) { \
	if(  e.GetCallingFunction() != #callingFunction ) \
	e.AddTrace(#callingFunction); \
	Core::LogExceptionWithLoggerAndGraphicalInterface(e); \
	}


#ifdef coreCatchExceptionsAddTraceAndThrowCriticalExceptionMacro
#undef coreCatchExceptionsAddTraceAndThrowCriticalExceptionMacro
#endif
/**
\def coreCatchExceptionsAddTraceAndThrowCriticalExceptionMacro(callingFunction)
coreCatchExceptionsAddTraceAndThrowCriticalExceptionMacro is a macro 
for catching exceptions thrown by a try { } block and 
propagating it (throwing and handling the stack trace). 
coreCatchExceptionsAddTraceAndThrowCriticalExceptionMacro always throw a
Core::CriticalException when catching any exception at the try { } block. 
It essentially casts any
exception to Core::CriticalException.
\param callingFunction Name of the function that throws the Exception
\ingroup gmKernel
*/
#define coreCatchExceptionsAddTraceAndThrowCriticalExceptionMacro(callingFunction) \
	catch(Core::Exceptions::Exception& e) { \
	Core::Exceptions::CriticalException eCast(#callingFunction, e.what()); \
	eCast.Clone(e); \
	throw eCast; \
	} \
	catch(std::exception& stdEx) \
	{ \
	Core::Exceptions::StdException e(#callingFunction, stdEx); \
	throw e; \
	} \
	catch(...) \
	{ \
	Core::Exceptions::CriticalException eCrit("UnhandledException", #callingFunction); \
	throw eCrit; \
	}



#ifdef coreCatchExceptionsCastToNewTypeAndThrowMacro
#undef coreCatchExceptionsCastToNewTypeAndThrowMacro
#endif
/**
\def coreCatchExceptionsCastToNewTypeAndThrowMacro(classname, callingFunction)
coreCatchExceptionsCastToNewTypeAndThrowMacro is a macro for catching 
exceptions thrown by a try { } block and 
propagating it (throwing and handling the stack trace). 
coreCatchExceptionsCastToNewTypeAndThrowMacro always throw a
Core::Exception of type 'classname' when catching any exception at the try { } 
block. It essentially 
casts any exception to Core::Exception::'classname'.
\param classname Give a name for the exception class. 'classname' cannot 
be a variable, it is expecting a string without " "
\param callingFunction Name of the function that throws the Exception
\ingroup gmKernel
*/
#define coreCatchExceptionsCastToNewTypeAndThrowMacro(classname, callingFunction) \
	catch(Core::Exceptions::Exception& e) { \
	classname eExc(#callingFunction); \
	eExc.Clone(e); \
	throw eExc; \
	} \
	catch(std::exception& stdEx) \
	{ \
	classname e(#callingFunction); \
	e.Append("\nDescription: "); \
	e.Append(stdEx.what()); \
	throw e; \
	} \
	catch(...) \
	{ \
	classname eExc(#callingFunction); \
	throw eExc; \
	}


#ifdef coreCatchExceptionsCastToNewTypeAndNoThrowMacro
#undef coreCatchExceptionsCastToNewTypeAndNoThrowMacro
#endif
/**
\def coreCatchExceptionsCastToNewTypeAndNoThrowMacro(classname, callingFunction)
coreCatchExceptionsCastToNewTypeAndNoThrowMacro is a macro for catching 
exceptions thrown by a try { } block.
It do not propagate the runtime exception, but logs it and reports it to 
the standard error channels. Note that
this macro only catches RuntimeExceptions. Other types of exception are 
not caught.
coreCatchOnlyRuntimeExceptionsReportAndNoThrowMacro do not throw exceptions
\param callingFunction Name of the function that throws the Exception
\ingroup gmKernel
*/
#define coreCatchExceptionsCastToNewTypeAndNoThrowMacro(classname, callingFunction) \
	catch(Core::Exceptions::Exception& e) { \
	classname eExc(#callingFunction); \
	eExc.Clone(e); \
	Core::LogExceptionWithLoggerAndGraphicalInterface(eExc); \
	} \
	catch(std::exception& stdEx) \
	{ \
	classname eExc(#callingFunction); \
	eExc.Append("\nDescription: "); \
	eExc.Append(stdEx.what()); \
	Core::LogExceptionWithLoggerAndGraphicalInterface(eExc); \
	} \
	catch(...) \
	{ \
	classname eExc(#callingFunction); \
	Core::LogExceptionWithLoggerAndGraphicalInterface(eExc); \
	}

#endif
