/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
// For compilers that don't support precompilation, include "wx/wx.h"
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
       #include <wx/wx.h>
#endif

#include "wxMitkApp.h"
#include "wxMitkAbortEventFilter.h"
#include "mitkStateMachineFactory.h"
#include "mitkGlobalInteraction.h"
#include "mitkDataStorage.h"
#include "mitkRenderingManager.h"
#include "mitkCoreObjectFactory.h"

#include "wx/xrc/xmlres.h"

using namespace mitk;

mitk::CoreExtObjectFactory::Pointer wxMitkApp::m_CoreExtObjectFactory;

bool wxMitkApp::OnInit()
{
	return Initialize("StateMachine.xml");
}

bool wxMitkApp::Initialize(const std::string& stateMachinePath )
{
	// We need to initialize wxXmlResource before use it
	wxXmlResource::Get()->InitAllHandlers();

	// Register mitkExt factory
	m_CoreExtObjectFactory = mitk::CoreExtObjectFactory::New();
	mitk::CoreObjectFactory::GetInstance()->RegisterExtraFactory( 
		 m_CoreExtObjectFactory.GetPointer() );

	// Load specific StateMachine.xml
	// This will construct a new mitk::GlobalInteractor global instance
	return mitk::GlobalInteraction::GetInstance()->Initialize( "global", stateMachinePath.c_str() );
}

/**
Filters all events captured by the application. This is an important routine.
Read the documentation of wxMitkAbortEventFilter
\sa wxMitkAbortEventFilter
*/
int wxMitkApp::FilterEvent(wxEvent& event)
{
	return mitk::wxMitkAbortEventFilter::GetInstance()->eventFilter(event);
}

/**
 */
int wxMitkApp::OnExit(void)
{
	CleanUp();
	return true;
}


/**
 */
void wxMitkApp::CleanUp()
{
	CleanUpMITK( );
}


void mitk::wxMitkApp::CleanUpMITK()
{
	// NEWMITK
	//// Destroy rendering tree
	//mitk::DataStorage::DestroyAllInstances();

	//// Destroy all state machines for interactors
	//mitk::StateMachineFactory::RemoveAllStates( );

	//// Destroy rendering manager
	//mitk::RenderingManager::DestroyInstance( );

	mitk::CoreObjectFactory::GetInstance()->UnRegisterExtraFactory( 
		m_CoreExtObjectFactory.GetPointer() );

	// We cannot do this, because there could be other
	// factories from other plugins registered
	//itk::ObjectFactoryBase::UnRegisterAllFactories();

}
