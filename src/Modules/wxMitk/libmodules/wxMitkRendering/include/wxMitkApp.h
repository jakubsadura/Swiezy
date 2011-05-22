/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef wxMitkApp_H
#define wxMitkApp_H

#include "wxMitkRenderingWin32Header.h"
#include "wx/app.h"
#include "mitkCoreExtObjectFactory.h"

namespace mitk
{

/**
A version of wxApp is provided for wxMitk. When you create an application 
of wxMitk, you might subclass this class for your application, instead 
of using the wxApp provided by wxWidgets.

The reason behind this relies in specific routines and event filtering 
needed for the correct behavior of the mitk rendering engine. Read more 
about the event filtering in wxMitkAbortEventFilter class.

\sa wxMitkAbortEventFilter

\ingroup wxMitkCore
\author Juan Antonio Moya
\date 25 Oct 2007
*/
class WXMITKRENDERING_EXPORT wxMitkApp : public wxApp
{
public:
	/** Tries to load "StateMachine.xml" from the current working folder.
	 Try to avoid using this function; instead, use the OnInit function 
	 that accepts an (absolute) path to StateMachine.xml, because this is 
	 more robust!
	 */
	virtual bool OnInit();

	//! Loads state machine from \a stateMachinePath.
	static bool Initialize( const std::string& stateMachinePath );

	//!
	virtual int FilterEvent(wxEvent& event);

	/**
	\brief Clean static memory allocated inside MITK
	\note This function is not called in GIMIAS, so we will use OnExit( )
	 */
	virtual void CleanUp();

	/**
	\brief Clean static memory allocated inside MITK
	 */
	virtual int OnExit(void);

	//!
	static void CleanUpMITK();
private:
	//!
	static mitk::CoreExtObjectFactory::Pointer m_CoreExtObjectFactory;

};
 
}

#endif // wxMitkApp_H
