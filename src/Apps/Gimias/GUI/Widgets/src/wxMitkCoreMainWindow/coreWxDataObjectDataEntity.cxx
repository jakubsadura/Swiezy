/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreWxDataObjectDataEntity.h"

static const wxChar *wxDataFormatDataEntityString = wxT("wxDataObjectDataEntity");


Core::wxDataObjectDataEntity::wxDataObjectDataEntity()
{
	m_format.SetId( wxDataFormatDataEntityString );
}

wxDataFormat Core::wxDataObjectDataEntity::GetPreferredFormat( Direction WXUNUSED(dir) ) const
{
	return m_format;
}

size_t Core::wxDataObjectDataEntity::GetFormatCount( Direction dir ) const
{
	// our custom format is supported by both GetData() and SetData()
	size_t nFormats = 1;

	return nFormats;
}

bool Core::wxDataObjectDataEntity::SetData( const wxDataFormat& format, size_t len, const void *pBuf )
{
	if ( format == m_format )
	{
		// Get the raw pointer and build a new data entity
		Core::DataEntity* dataEntity = *((Core::DataEntity**) pBuf);

		m_DataEntity = dataEntity;

		return true;
	}

	return false;
}

Core::DataEntity::Pointer Core::wxDataObjectDataEntity::GetDataEntity()
{
	return m_DataEntity;
}

void Core::wxDataObjectDataEntity::SetDataEntity( Core::DataEntity::Pointer data )
{
	m_DataEntity = data;
}

void Core::wxDataObjectDataEntity::GetAllFormats( wxDataFormat *formats, Direction dir ) const
{
	formats[0] = m_format;
}

size_t Core::wxDataObjectDataEntity::GetDataSize( const wxDataFormat& format ) const
{
	if ( format == m_format )
	{
		// Size of the raw pointer
		return sizeof( m_DataEntity.GetPointer( ) );
	}
	return 0;
}

bool Core::wxDataObjectDataEntity::GetDataHere( const wxDataFormat& format, void *pBuf ) const
{
	if ( format == m_format )
	{
		// Copy the raw pointer
		Core::DataEntity* dataEntity = m_DataEntity.GetPointer( );
		memcpy( pBuf, &dataEntity, sizeof( m_DataEntity.GetPointer( ) ) );
		return true;
	}

	return false;
}

const wxChar * Core::wxDataObjectDataEntity::GetFormatId()
{
	return wxDataFormatDataEntityString;
}

