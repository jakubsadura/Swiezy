/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreDataEntityImplFactory_H
#define _coreDataEntityImplFactory_H

#include "gmDataHandlingWin32Header.h"
#include "coreObject.h"
#include <string>
#include <typeinfo>
#include <vector>
#include <boost/any.hpp>
#include "blTagMap.h"
#include "coreDataEntityImpl.h"
#include "coreDataEntityTypes.h"

namespace Core{

/**
Stores the processing data at a certain time step. 
It is mutable (so that one client can change the Processing 
Data instance of another client).

\author Xavi Planes
\date 07 sept 2010
\ingroup gmDataHandling
*/
class GMDATAHANDLING_EXPORT DataEntityImplFactory : public Core::SmartPointerObject
{
public:
	coreDeclareSmartPointerTypesMacro(DataEntityImplFactory,Core::SmartPointerObject) 
	coreClassNameMacro(DataEntityImplFactory)

public:

	typedef std::list<DataEntityImplFactory::Pointer > DataEntityImplFactoryListType;

	//!
	virtual DataEntityImpl::Pointer NewData( ) = 0;

	//!
	virtual const std::type_info& GetDataType( ) = 0;

	//! Find a factory
	static DataEntityImplFactory::Pointer FindFactory( blTagMap::Pointer tagMap );

	//! Find a factory
	static DataEntityImplFactory::Pointer FindFactory( const std::type_info &type );

	//! Find a factory using similar type name like "vtkPolyData"
	static DataEntityImplFactory::Pointer FindFactory( const std::string &name );

	//! Register a class to a TimeStep type. For example VtkPolyDataImpl
	static void RegisterFactory( DataEntityImplFactory::Pointer factory );

	//!
	static void UnRegisterAll( );

	//! Check if tagMap values matches m_Properties
	bool CheckProperties( blTagMap::Pointer tagMap );

	//!
	blTagMap::Pointer GetProperties() const;
protected:
	//!
	DataEntityImplFactory( );

protected:
	//! Map of factories
	static DataEntityImplFactoryListType m_FactoryList;
	//! List of properties like valid data types
	blTagMap::Pointer m_Properties;
};

} // end namespace Core

#define coreDefineDataFactoryBegin( DataEntityImplType, type1 ) \
	class Factory : public Core::DataEntityImplFactory \
	{ \
	public: \
		coreDeclareSmartPointerClassMacro(Factory, DataEntityImplFactory)  \
		static DataEntityImplFactory::Pointer NewBase( )\
		{\
			Pointer p = New( );\
			return p.GetPointer( );\
		}\
		const std::type_info& GetDataType( )\
		{\
			return typeid( type1 );\
		}\
	private: \
		DataEntityImpl::Pointer NewData( )\
		{\
			DataEntityImplType::Pointer data = DataEntityImplType::New( );\
			return data.GetPointer( );\
		}\
		Factory( ) \
		{ \

#define coreDefineDataFactoryAddTypeInfo( type1 ) \
		m_Properties->AddTag( typeid( type1 ).name( ), true );  \

#define coreDefineDataFactoryAddSingle( ) \
		m_Properties->AddTag( "single", true );  \

#define coreDefineDataFactoryAddMultiple(  ) \
		m_Properties->AddTag( "multiple", true );  \

#define coreDefineDataFactoryAddDefaultDataEntityType( typeN ) \
		m_Properties->AddTag( "DefaultDataEntityType", typeN );  \

#define coreDefineDataFactoryEnd( ) \
		} \
	}; \

#define coreDefineSingleDataFactory( DataEntityImplType, type1, dataEntityType ) \
	coreDefineDataFactoryBegin( DataEntityImplType, type1 ) \
	coreDefineDataFactoryAddTypeInfo( type1 ) \
	coreDefineDataFactoryAddSingle( ) \
	coreDefineDataFactoryAddDefaultDataEntityType( dataEntityType ) \
	coreDefineDataFactoryEnd( ) \

#define coreDefineMultipleDataFactory( DataEntityImplType, type1, dataEntityType ) \
	coreDefineDataFactoryBegin( DataEntityImplType, type1 ) \
	coreDefineDataFactoryAddTypeInfo( type1 ) \
	coreDefineDataFactoryAddMultiple( ) \
	coreDefineDataFactoryAddDefaultDataEntityType( dataEntityType ) \
	coreDefineDataFactoryEnd( ) \

#define coreDefineSingleDataFactory2Types( DataEntityImplType, type1, type2, dataEntityType ) \
	coreDefineDataFactoryBegin( DataEntityImplType, type1 ) \
	coreDefineDataFactoryAddTypeInfo( type1 ) \
	coreDefineDataFactoryAddTypeInfo( type2 ) \
	coreDefineDataFactoryAddSingle( ) \
	coreDefineDataFactoryAddDefaultDataEntityType( dataEntityType ) \
	coreDefineDataFactoryEnd( ) \

#define coreDefineMultipleDataFactory2Types( DataEntityImplType, type1, type2, dataEntityType ) \
	coreDefineDataFactoryBegin( DataEntityImplType, type1 ) \
	coreDefineDataFactoryAddTypeInfo( type1 ) \
	coreDefineDataFactoryAddTypeInfo( type2 ) \
	coreDefineDataFactoryAddMultiple( ) \
	coreDefineDataFactoryAddDefaultDataEntityType( dataEntityType ) \
	coreDefineDataFactoryEnd( ) \

#endif // _coreDataEntityImplFactory_H

