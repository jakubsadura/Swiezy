/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreLoggerHelper_H
#define coreLoggerHelper_H

#include "coreObject.h"
#include "coreLogger.h"

#include <list>


namespace Core
{

/** 
LoggerHelper provides helper functions for working with Logger 

\ingroup gmKernel
\author Xavi Planes
\date 26 Mar 2008
*/
class GMKERNEL_EXPORT LoggerHelper : public Core::Object
{
// TYPES
public:
	/**
	\brief Type of message to log
	\ingroup Helpers
	*/
	typedef enum 
	{
		//! Log a message
		MESSAGE_TYPE_INFO,
		//! Report Warning
		MESSAGE_TYPE_WARNING,
		//! Report Error
		MESSAGE_TYPE_ERROR
	} MessageType;

public:

	//! Define virtual destructor to avoid warning
	virtual ~LoggerHelper( );

	/**
	\brief Add a message in the log
	*/
	static void ShowMessage( 
						std::string strMessage, 
						MessageType type, 
						bool showAlsoDialog = true );

};

} // Core

#endif
