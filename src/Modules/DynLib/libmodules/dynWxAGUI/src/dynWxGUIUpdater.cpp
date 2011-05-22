/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "dynWxGUIUpdater.h"

// For compilers that don't support precompilation, include "wx/wx.h"
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
       #include "wx/wx.h"
#endif

dynWxGUIUpdater::dynWxGUIUpdater()
{
	m_Module = NULL;
}

dynWxGUIUpdater::~dynWxGUIUpdater()
{

}

ModuleDescription *dynWxGUIUpdater::GetModule() const
{
	return m_Module;
}

void dynWxGUIUpdater::SetModule( ModuleDescription *val )
{
	m_Module = val;
}

void dynWxGUIUpdater::UpdateData()
{
	ControlUpdaterMapType::iterator it;
	for ( it = m_ControlUpdaters.begin(); it != m_ControlUpdaters.end( ) ; it++ )
	{
		m_Module->SetParameterDefaultValue( it->first, it->second->GetValue( ) );
	}
}

void dynWxGUIUpdater::UpdateWidget()
{
	ControlUpdaterMapType::iterator it;
	for ( it = m_ControlUpdaters.begin(); it != m_ControlUpdaters.end( ) ; it++ )
	{
		std::string paramValue = m_Module->GetParameterDefaultValue( it->first );
		it->second->SetValue( paramValue );
	}
}

void dynWxGUIUpdater::AddControlUpdater( gbl::ControlUpdaterBase* controlUpdater )
{
	gbl::ControlUpdaterBase* updater = FindUpdater( controlUpdater->GetName( ) );
	if ( updater )
	{
		delete updater;
	}

	m_ControlUpdaters[controlUpdater->GetName( )] = controlUpdater;
}

gbl::ControlUpdaterBase* dynWxGUIUpdater::FindUpdater( const std::string &name )
{
	ControlUpdaterMapType::iterator it;
	it = m_ControlUpdaters.find( name );
	if ( it == m_ControlUpdaters.end() )
	{
		return NULL;
	}

	return it->second;
}

dynWxGUIUpdater::ControlUpdaterMapType & dynWxGUIUpdater::GetControlUpdaters()
{
	return m_ControlUpdaters;
}
