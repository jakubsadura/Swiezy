/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreMultipleDataEntityImpl.h"
#include "coreDataEntityImplFactory.h"

using namespace Core;

/**
*/
Core::MultipleDataEntityImpl::MultipleDataEntityImpl( ) 
{
}

Core::MultipleDataEntityImpl::~MultipleDataEntityImpl()
{

}

boost::any Core::MultipleDataEntityImpl::GetDataPtr() const
{
	return m_Data;
}

bool Core::MultipleDataEntityImpl::SetData( 
	blTagMap::Pointer tagMap, ImportMemoryManagementType mem/* = gmCopyMemory*/ )
{
	blTag::Pointer tagDataEntitySetImplSet = tagMap->FindTagByName( "DataEntityImplSet" );
	if ( tagDataEntitySetImplSet.IsNull() )
	{
		return false;
	}

	DataEntityImplSetType setOfDataEntitySetImpl;
	setOfDataEntitySetImpl = tagDataEntitySetImplSet->GetValueCasted<DataEntityImplSetType>();

	ResetData( );

	DataEntityImplSetType::iterator it;
	for ( it = setOfDataEntitySetImpl.begin() ; it != setOfDataEntitySetImpl.end() ; it++)
	{
		// Create a new DataEntityImpl
		DataEntityImplFactory::Pointer factory;

		// If no factory defined, use the one of the input data
		if ( m_Factory.IsNull() )
		{
			factory = DataEntityImplFactory::FindFactory( (*it)->GetDataPtr().type() );
		}
		else
		{
			factory = m_Factory;
		}
		DataEntityImpl::Pointer dataEntityImpl = factory->NewData();
		// Copy data
		if ( !dataEntityImpl->DeepCopy( *it, mem ) )
		{
			return false;
		}

		// Add
		SetAt( GetSize(), dataEntityImpl, mem );
	}

	return true;
}

bool Core::MultipleDataEntityImpl::GetData( blTagMap::Pointer tagMap )
{
	tagMap->AddTag( "DataEntityImplSet", m_Data );

	return true;
}

void Core::MultipleDataEntityImpl::ResetData()
{
	m_Data.clear();
}

bool Core::MultipleDataEntityImpl::SwitchImplementation( const std::type_info &type )
{
	m_Factory = DataEntityImplFactory::FindFactory( type );
	DataEntityImplSetType::iterator it;
	for ( it = m_Data.begin() ; it != m_Data.end() ; it++)
	{
		if ( !(*it)->IsValidType( type ) )
		{
			DataEntityImpl::Pointer newData = m_Factory->NewData();
			if ( newData.IsNull() )
			{
				return false;
			}

			if ( (*it).IsNotNull() )
			{
				bool typeIsOk = newData->DeepCopy( (*it) );
				if ( !typeIsOk )
				{
					return false;
				}
			}

			(*it) = newData;
		}
	}

	return true;
}

size_t Core::MultipleDataEntityImpl::GetSize() const
{
	return m_Data.size();
}

DataEntityImpl::Pointer Core::MultipleDataEntityImpl::GetAt( size_t pos )
{
	return m_Data.at( pos );
}

bool Core::MultipleDataEntityImpl::SetAt( 
	size_t pos, 
	DataEntityImpl::Pointer data,
	ImportMemoryManagementType mem /*= gmCopyMemory*/ )
{
	if ( m_Data.size() )
	{
		const std::type_info &type1 = typeid( *data.GetPointer() );
		const std::type_info &type2 = typeid( *m_Data[ 0 ].GetPointer() );
		if ( type1 != type2 )
		{
			return false;
		}
	}

	m_Data.resize( pos + 1 );
	m_Data.at( pos ) = data;
	return true;
}

void Core::MultipleDataEntityImpl::SetAnyData( boost::any val )
{
	m_Data = boost::any_cast<DataEntityImplSetType> ( val );
}
