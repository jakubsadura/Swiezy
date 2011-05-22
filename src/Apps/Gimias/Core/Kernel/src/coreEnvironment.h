/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreEnvironment_H
#define coreEnvironment_H

/*
This header file and corresponding source file are also used as an example 
of documenting code. 
Therefore, some documentation instructions can be found in these files.
In this example, the documentation of the functions is in the cpp file. 
It is also okay to place the documentation of the functions in the header 
file (it is up to your judgement what is most 
appropriate for the particular source file). 
Some functions and variables are commented with an empty doxygen statement (//!). 
This indicates that the documentation follows trivially from the function 
or variable name.
*/

#include "gmKernelWin32Header.h"
#include "coreObject.h"
#include "coreDataEntity.h"
#include "coreDataHolder.h"
#include "coreProgressTicket.h"
#include <string>

#include <wx/wx.h>

namespace Core 
{
namespace Runtime 
{

//! Stores the mode in which the application is running.
typedef enum Mode { Console, Graphical } AppRunMode;

//! State of the application
typedef enum{
	//! The application is processing an algorithm
	APP_STATE_PROCESSING,
	//! The application is idle state
	APP_STATE_IDLE
} APP_STATE;
typedef Core::DataHolder<APP_STATE> AppStateHolderType;

/** 
\brief The Environment class is the backbone of the application. It is 
responsible for managing the main application loop (the loop that
listens for events and calls the handler for these events). 

\todo This class has a protected section but no private section. The use 
of private sections should be preferred over 
the use of protected sections. When refactoring a class, changing 
protected sections maybe almost impossible (or at best:
dangerous) because it cannot be anticipated how child classes rely on 
these protected sections.

\note You should access and get the instance for this class through the 
runtime high-end object: Kernel

\sa Core::Runtime::Kernel
\ingroup gmKernel
\author Juan Antonio Moya
\date 31 May 2007
*/

class GMKERNEL_EXPORT Environment : public Core::SmartPointerObject
{
public:
	coreDeclareSmartPointerClassMacro(Environment, Core::SmartPointerObject);

	bool OnAppExit(void) const;
	void Exec(void);
	void ExitApplication(void);
	void RestartApplication(void);
	bool IsAppRunning(void) const;
	void InitApplication(
		int& argc, 
		wxChar ** argv, 
		Core::Runtime::AppRunMode mode);

	AppStateHolderType::Pointer GetAppStateHolder() const;

	/**
	\brief Change the state of the application. In the APP_STATE_PROCESSING
	a info window will appear with the message "Processing... Please wait".
	*/
	void SetAppState( 
		APP_STATE val, 
		Core::ProgressTicketHolderType::Pointer progressTicket = NULL );

	//!
	Core::ProgressTicketHolderType::Pointer GetProgressTicketHolder() const;

protected:
	Environment(void);
	virtual ~Environment(void);

private:
	coreDeclareNoCopyConstructors(Environment);

protected:
	//! A boolean used for breaking the app loop on exit.
	bool exit;							
	//! data node count for assigning an unique identifier to each rendering node
	int dataNodeCount;

	//! Current state of the app
	AppStateHolderType::Pointer m_AppStateHolder;

	//! Current progress ticket
	Core::ProgressTicketHolderType::Pointer m_ProgressTicketHolder;

};

} // namespace Runtime
} // namespace Core

#endif
