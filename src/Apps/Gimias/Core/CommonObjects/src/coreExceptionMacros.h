/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreExceptionMacros_H
#define coreExceptionMacros_H


#ifdef coreCatchExceptionsAddTraceAndThrowMacro
#undef coreCatchExceptionsAddTraceAndThrowMacro
#endif
/**
\def coreCatchExceptionsAddTraceAndThrowMacro(callingFunction)
coreCatchExceptionsAddTraceAndThrowMacro is a macro for catching exceptions 
thrown by a try { } block and 
propagating it (throwing and handling the stack trace). 
coreCatchExceptionsAddTraceAndThrowMacro always throw a
Core::Exception when catching any exception at the try { } block.
\param callingFunction Name of the function that throws the Exception
\ingroup gmCommonObjects
*/
#define coreCatchExceptionsAddTraceAndThrowMacro(callingFunction) \
	catch(Core::Exceptions::Exception& e) { \
	if ( e.GetCallingFunction() != #callingFunction ) \
	e.AddTrace(#callingFunction); \
	throw; \
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

#endif// coreExceptionMacros_H
