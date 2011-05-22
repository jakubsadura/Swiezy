/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreBaseFilterInputPort.h"



Core::BaseFilterInputPort::BaseFilterInputPort()
{
	m_Modality = Core::UnknownModality;
	m_NotValidDataEntityType = Core::DataEntityType( 0 );
}

Core::BaseFilterInputPort::~BaseFilterInputPort()
{

}

void Core::BaseFilterInputPort::SetDataEntityType( int val )
{
	BaseFilterPort::SetDataEntityType( val );

	if ( !CheckDataEntityRestrictions( GetDataEntity() ) )
	{
		Superclass::SetDataEntity( NULL );
	}
}

Core::ModalityType Core::BaseFilterInputPort::GetModality() const
{
	return m_Modality;
}

void Core::BaseFilterInputPort::SetModality( Core::ModalityType val )
{
	m_Modality = val;

	if ( !CheckDataEntityRestrictions( GetDataEntity() ) )
	{
		Superclass::SetDataEntity( NULL );
	}
}

bool Core::BaseFilterInputPort::CheckDataEntityRestrictions( 
	Core::DataEntity::Pointer val )
{
	if ( val.IsNull() )
	{
		return true;
	}

	if ( !CheckAllowedInputDataTypes( val ) )
	{
		return false;
	}

	if ( GetDataEntityType() == UnknownTypeId )
	{
		return true;
	}

	if ( ( GetDataEntityType() & val->GetType() ) == 0 ||
		 ( m_NotValidDataEntityType == val->GetType() ) )
	{
		return false;
	}

 	return true;
}

bool Core::BaseFilterInputPort::CheckAllowedInputDataTypes( 
	Core::DataEntity::Pointer val )
{
	bool validModality = 
		m_Modality == Core::UnknownModality || 
		m_Modality == val->GetMetadata()->GetModality();

	bool validTagList = true;
	std::list<DataEntityTag::Pointer>::iterator it;
	for ( it = m_ValidTagList.begin() ; it != m_ValidTagList.end() ; it++)
	{
		DataEntityTag::Pointer validTag = *it;
		DataEntityTag::Pointer inputTag = val->GetMetadata()->FindTagByName( validTag->GetName() );
		if ( inputTag.IsNotNull() )
		{
			validTagList = validTagList && validTag->Compare( inputTag );
		}
	}

	return validModality && validTagList;
}

Core::DataEntityType Core::BaseFilterInputPort::GetNotValidDataEntityType() const
{
	return m_NotValidDataEntityType;
}

void Core::BaseFilterInputPort::SetNotValidDataEntityType( int val )
{
	m_NotValidDataEntityType = Core::DataEntityType( val );

	if ( !CheckDataEntityRestrictions( GetDataEntity() ) )
	{
		Superclass::SetDataEntity( NULL );
	}
	
}

void Core::BaseFilterInputPort::AddValidDataEntityTag( DataEntityTag::Pointer tag )
{
	m_ValidTagList.push_back( tag );

	if ( !CheckDataEntityRestrictions( GetDataEntity() ) )
	{
		Superclass::SetDataEntity( NULL );
	}
	
}
