/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreConfigVars.h"

using namespace Core::IO;

ConfigVars::ConfigVars( )
{
	m_startedOnce = false;
	m_ImportConfiguration = false;
	m_profile = Core::Profile::New();
	m_profile->ResetProfileToDefault();
	m_pacsCalledAE= "ANYSCP:127.0.0.1:1234";
	m_pacsCallingAE= "g127.0.0.1:127.0.0.1:1235";
	m_Perspective = "Plugin";
	m_ActiveWorkflow = "";
	m_ShowRegistrationForm = true;
	m_UserRegistered = false;
}


Core::IO::ConfigVarsPlugin::ConfigVarsPlugin()
{
	
}

