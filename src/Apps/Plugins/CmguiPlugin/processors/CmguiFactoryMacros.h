/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _cmguiFactoryMacros_H
#define _cmguiFactoryMacros_H

#include "corePluginMacros.h"
#include "coreDataEntityImplFactory.h"

#define coreDefineDataFactoryNewData( DataEntityImplType ) \
	DataEntityImpl::Pointer NewData( ) \
	{ \
		blTag::Pointer tag = m_Properties->FindTagByName( "Name" ); \
		std::string name; \
		if ( tag.IsNotNull() ) \
		{ \
			tag->GetValue( name ); \
		} \
		DataEntityImplType::Pointer data = DataEntityImplType::New( m_RootRegion, name ); \
		return data.GetPointer( ); \
	}


/**

Factory for all DataEntity implementations

\note You need to set the name of the DataEntity before creating this object
because the name will be used to set region name

\author Xavi Planes
\date Oct 2010
\ingroup CMGUIPlugin
*/
#define coreDefineCMGUIDataFactory( DataEntityImplType, type1, dataEntityType ) \
	class Factory : public Core::DataEntityImplFactory \
	{ \
	public: \
		coreDeclareSmartPointerClassMacro1Param(Factory, DataEntityImplFactory, Cmiss_region *) \
		const std::type_info& GetDataType( ) \
		{ \
			return typeid( type1 ); \
		} \
		coreDefineDataFactoryNewData( DataEntityImplType ) \
	private: \
		Factory( Cmiss_region *rootRegion )  \
		{ \
			m_RootRegion = rootRegion; \
			coreDefineDataFactoryAddTypeInfo( type1 ) \
			coreDefineDataFactoryAddSingle( ) \
			coreDefineDataFactoryAddDefaultDataEntityType( dataEntityType ) \
		} \
		Cmiss_region *m_RootRegion; \
	};

#endif //_cmguiFactoryMacros_H
