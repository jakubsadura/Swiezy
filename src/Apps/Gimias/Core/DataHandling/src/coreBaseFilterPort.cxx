/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreBaseFilterPort.h"


Core::BaseFilterPort::BaseFilterPort()
{
	m_DataEntityHolder = Core::DataEntityHolder::New();
	m_DataEntityType = Core::UnknownTypeId;
	m_Optional = false;
	m_Active = false;
}

Core::BaseFilterPort::~BaseFilterPort()
{

}

Core::DataEntityHolder::Pointer Core::BaseFilterPort::GetDataEntityHolder() const
{
	return m_DataEntityHolder;
}

std::string Core::BaseFilterPort::GetName() const
{
	return m_Name;
}

void Core::BaseFilterPort::SetName( std::string val )
{
	m_Name = val;
	m_DataEntityHolder->SetName( m_ProcessorName + ": " + m_Name );
}

Core::DataEntity::Pointer Core::BaseFilterPort::GetDataEntity()
{
	return m_DataEntityHolder->GetSubject();
}

bool Core::BaseFilterPort::SetDataEntity( Core::DataEntity::Pointer val )
{
	if ( !CheckDataEntityRestrictions( val ) )
	{
		return false;
	}

	// Always notify observers because when automatic is on, we
	// need to update the views sometimes
	// Modifictaion: In cardiac initialization, when the selection changes
	// and the input is the same, we need to update the observers (volume
	// computation), so we force the notification
	// Xavi: Changed with the new pipeline
	m_DataEntityHolder->SetSubject( val );

	return true;
}

std::string Core::BaseFilterPort::GetProcessorName() const
{
	return m_ProcessorName;
}

void Core::BaseFilterPort::SetProcessorName( std::string val )
{
	m_ProcessorName = val;
	m_DataEntityHolder->SetName( m_ProcessorName + ": " + m_Name );
}

Core::DataEntityType Core::BaseFilterPort::GetDataEntityType() const
{
	return m_DataEntityType;
}

void Core::BaseFilterPort::SetDataEntityType( int val )
{
	m_DataEntityType = Core::DataEntityType( val );
}

bool Core::BaseFilterPort::GetOptional() const
{
	return m_Optional;
}

void Core::BaseFilterPort::SetOptional( bool val )
{
	m_Optional = val;
}

bool Core::BaseFilterPort::GetActive() const
{
	return m_Active;
}

void Core::BaseFilterPort::SetActive( bool val )
{
	m_Active = val;
}
