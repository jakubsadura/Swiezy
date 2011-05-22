/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreBaseWindowFactory.h"


void Core::BaseWindowFactory::SetParent( wxWindow* parent )
{
	m_Parent = parent;
}

wxWindow* Core::BaseWindowFactory::GetParent() const
{
	return m_Parent;
}

void Core::BaseWindowFactory::SetWindowId( int id )
{
	m_WindowId = id;
}

int Core::BaseWindowFactory::GetWindowId() const
{
	return m_WindowId;
}

std::string Core::BaseWindowFactory::GetWindowName() const
{
	return m_WindowName;
}

void Core::BaseWindowFactory::SetWindowName( std::string val )
{
	m_WindowName = val;
}

wxPoint Core::BaseWindowFactory::GetPosition() const
{
	return m_Position;
}

void Core::BaseWindowFactory::SetPosition( wxPoint val )
{
	m_Position = val;
}

wxSize Core::BaseWindowFactory::GetSize() const
{
	return m_Size;
}

void Core::BaseWindowFactory::SetSize( wxSize val )
{
	m_Size = val;
}

Core::BaseWindowFactory::BaseWindowFactory()
{
	m_Parent = NULL;
	m_WindowId = wxID_ANY;
	m_Position = wxDefaultPosition;
	m_Size = wxDefaultSize;
}

std::string Core::BaseWindowFactory::GetBitmapFilename() const
{
	return m_BitmapFilename;
}

void Core::BaseWindowFactory::SetBitmapFilename( std::string val )
{
	m_BitmapFilename = val;
}

ModuleDescription &Core::BaseWindowFactory::GetModule()
{
	return m_Module;
}

void Core::BaseWindowFactory::SetModule( ModuleDescription val )
{
	m_Module = val;
}

Core::BaseWindowReaderFactory::BaseWindowReaderFactory()
{
	m_DataEntitytype = Core::UnknownTypeId;
	m_Extension = "";
}

bool Core::BaseWindowReaderFactory::CheckType( 
	Core::DataEntityType type, const std::string &ext )
{
	
	return type == m_DataEntitytype && ext == m_Extension;
}

