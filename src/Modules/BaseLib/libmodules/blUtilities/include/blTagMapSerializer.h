/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _blTagMapSerializer_H
#define _blTagMapSerializer_H

#include "blTagMap.h"
#include <boost/serialization/string.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/list.hpp>
#include <boost/foreach.hpp>

BOOST_CLASS_VERSION(blTagMap::Pointer, 1)

//!
template<class Archive> 
void save(Archive &ar, blTagMap::Pointer tagMapInstance, const unsigned int version)
{
    using boost::serialization::make_nvp;

    blTagMap::TagMapType tagMap = tagMapInstance->GetTagMap();
    size_t size = tagMap.size();
    ar << make_nvp("PropertiesSize", size );

    blTagMap::TagMapType::iterator it;
    for ( it = tagMap.begin() ; it != tagMap.end() ; it++ )
    {
		blTag::Pointer tag = it->second;
        unsigned long id = tag->GetId();
        ar << make_nvp("PropertyId", id );

        std::string propertyName(tag->GetName());
        ar << make_nvp("PropertyName", propertyName );

		std::string valueTypeName = tag->GetTypeName();
		ar << make_nvp("PropertyValueType", valueTypeName );

		if ( tag->GetValue().type() == typeid( blTagMap::Pointer ) )
		{
			blTagMap::Pointer tagMapProperty;
			tag->GetValue<blTagMap::Pointer>( tagMapProperty );
			ar << BOOST_SERIALIZATION_NVP( tagMapProperty );
		}
		else
		{
			std::string valueAsString = tag->GetValueAsString();
			ar << make_nvp("PropertyValue", valueAsString );
		}
    }
}



//!
template<class Archive> 
void load(Archive &ar, blTagMap::Pointer tagMapInstance, const unsigned int version)
{
    using boost::serialization::make_nvp;

    size_t size;
    ar >> make_nvp("PropertiesSize", size );

    for ( unsigned int i = 0 ; i < size ; i++ )
    {
        unsigned long id;
        ar >> make_nvp("PropertyId", id );

        std::string name;
        ar >> make_nvp("PropertyName", name );

        std::string valueTypeName;
        ar >> make_nvp("PropertyValueType", valueTypeName );

		// Changed due to incompatibility problems with cross platform
		if ( version == 0 )
		{
			if ( valueTypeName == "class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >" )
			{
				const std::type_info& info = typeid( std::string );
				valueTypeName = blTag::GetTypeName( info );
			}
			else if ( valueTypeName == "class blSmartPointer<class blTagMap>" )
			{
 				const std::type_info& info = typeid( blTagMap::Pointer );
				valueTypeName = blTag::GetTypeName( info );
			}
		}

		blTag::Pointer tag = blTag::New( id, name, NULL );
		if ( valueTypeName == blTag::GetTypeName( typeid( blTagMap::Pointer ) ) )
		{
			blTagMap::Pointer tagMapProperty = blTagMap::New();
			ar >> BOOST_SERIALIZATION_NVP( tagMapProperty );
			tag->SetValue( tagMapProperty );
		}
		else
		{
			std::string valueAsString;
			ar >> make_nvp("PropertyValue", valueAsString );

			tag->SetValueAsString( valueTypeName, valueAsString );
		}

        tagMapInstance->AddTag( tag );
    }
}

BOOST_SERIALIZATION_SPLIT_FREE( blTagMap::Pointer )

#endif // _blTagMapSerializer_H