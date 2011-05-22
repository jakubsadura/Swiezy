/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreDataEntityInfoHelper.h"
#include "coreAssert.h"
#include "blVTKUtils.h"

#include <sstream>
#include <stdio.h>

using namespace Core;

std::map<Core::DataEntityType,std::string> Core::DataEntityInfoHelper::m_MapDataEntityType;
std::map<Core::ModalityType,std::string> Core::DataEntityInfoHelper::m_MapModalityType;

Core::DataEntityInfoHelper::~DataEntityInfoHelper()
{

}

std::string DataEntityInfoHelper::GetEntityTypeAsString(
	const Core::DataEntityType &type)
{
	BuildMaps( );
	std::map<Core::DataEntityType,std::string>::iterator it;
	it = m_MapDataEntityType.find( type );
	if ( it != m_MapDataEntityType.end() )
	{
		return it->second;
	}
	return "";
}

std::string DataEntityInfoHelper::GetModalityTypeAsString(
	const DataEntityInfoHelper::ModalityType &modality)
{
	BuildMaps( );
	std::map<Core::ModalityType,std::string>::iterator it;
	it = m_MapModalityType.find( modality );
	if ( it != m_MapModalityType.end() )
	{
		return it->second;
	}
	return "";
}

DataEntityInfoHelper::ModalityType DataEntityInfoHelper::GetModalityType( 
	const std::string &strModality )
{
	DataEntityInfoHelper::ModalityType modality = Core::UnknownModality;

	BuildMaps( );

	static std::map<Core::ModalityType,std::string>::iterator it;
	it = m_MapModalityType.begin();
	while ( it != m_MapModalityType.end() )
	{
		if ( strModality == it->second )
		{
			modality = it->first;
		}
		it++;
	}

	return modality;
}

Core::DataEntityType 
Core::DataEntityInfoHelper::GetDataEntityType( 
	const std::string &strDataEntityType )
{
	Core::DataEntityType dataEntityType = Core::UnknownTypeId;

	BuildMaps( );

	std::map<Core::DataEntityType,std::string>::iterator it;
	it = m_MapDataEntityType.begin();
	while ( it != m_MapDataEntityType.end() )
	{
		if ( strDataEntityType.substr(0,it->second.size()) == it->second )
		{
			dataEntityType = it->first;
		}
		it++;
	}

	return dataEntityType;
}

DataEntityInfoHelper::ModalityList DataEntityInfoHelper::GetAvailableModalityTypes(void)
{
	ModalityList list;

	BuildMaps( );

	static std::map<Core::ModalityType,std::string>::iterator it;
	it = m_MapModalityType.begin();
	while ( it != m_MapModalityType.end() )
	{
		list.push_back( it->first );
		it++;
	}

	return list;
}

DataEntityInfoHelper::EntityList DataEntityInfoHelper::GetAvailableEntityTypes(void)
{
	EntityList list;

	BuildMaps( );

	static std::map<Core::DataEntityType,std::string>::iterator it;
	it = m_MapDataEntityType.begin();
	while ( it != m_MapDataEntityType.end() )
	{
		list.push_back( it->first );
		it++;
	}

	return list;
}

std::string DataEntityInfoHelper::GetShortDescription(Core::DataEntity::Pointer dataEntity)
{
	std::ostringstream oss;
	oss << dataEntity->GetId() << "#" 
		<< dataEntity->GetMetadata( )->GetName() << " (" 
		<< GetEntityTypeAsString(dataEntity->GetType()) << ", " 
		<< GetModalityTypeAsString(dataEntity->GetMetadata( )->GetModality()) << ")";
	return oss.str();
}

void Core::DataEntityInfoHelper::BuildMaps()
{
	if ( m_MapDataEntityType.size() == 0 )
	{
		m_MapDataEntityType[ ImageTypeId ] = "Volume image";
		m_MapDataEntityType[ DataEntityType( ImageTypeId | ROITypeId ) ] = "ROI";
		m_MapDataEntityType[ SurfaceMeshTypeId ] = "Surface mesh";
		m_MapDataEntityType[ DataEntityType( SurfaceMeshTypeId | SkeletonTypeId ) ] = "Skeleton";
		m_MapDataEntityType[ SignalTypeId ] = "Signal";
		m_MapDataEntityType[ VolumeMeshTypeId ] = "Volume mesh";
		m_MapDataEntityType[ TransformId ] = "Transform file";
		m_MapDataEntityType[ PointSetTypeId ] = "Point set";
		m_MapDataEntityType[ DataEntityType( PointSetTypeId | MeasurementTypeId ) ] = "Measurement";
		m_MapDataEntityType[ SignalTypeId ] = "Signal";
		m_MapDataEntityType[ ContourTypeId ] = "Contour";
		m_MapDataEntityType[ UnknownTypeId ] = "-";
		m_MapDataEntityType[ TensorTypeId ] = "Tensor";
	}

	if ( m_MapModalityType.size() == 0 )
	{
		m_MapModalityType[ Core::US ] = "US";
		m_MapModalityType[ Core::MRI ] = "MRI";
		m_MapModalityType[ Core::CT ] = "CT";
		m_MapModalityType[ Core::NM ] = "NM";
		m_MapModalityType[ Core::XA ] = "XA";
		m_MapModalityType[ Core::THREE_D_RA ] = "3DRA";
		m_MapModalityType[ Core::UnknownModality ] = "-";
	}
}


std::string DataEntityInfoHelper::GetModalityDescription( 
	Core::DataEntity::Pointer dataEntity )
{
	// Create header
	std::ostringstream	osHeader;
	Core::ModalityType modality = dataEntity->GetMetadata( )->GetModality( );
	std::string strModality = Core::DataEntityInfoHelper::GetModalityTypeAsString( modality );
	osHeader << "Modality: " << strModality;
	std::string strHeader = osHeader.str();

	return strHeader;
}


Core::ModalityType DataEntityInfoHelper::LoadModalityDescription( 
	const std::string &fileName )
{
	// Read the header to get the modality
	Core::ModalityType modality = Core::UnknownModality;
	std::string strHeader;
	blVTKHelperTools::blVTKUtils::ReadHeader( fileName.c_str(), strHeader );
	if ( !strHeader.empty() )
	{
		char cModality[ 10 ];
		sscanf( strHeader.c_str(), "Modality: %10s", cModality );
		modality = DataEntityInfoHelper::GetModalityType( cModality );
	}

	return modality;
}

std::string DataEntityInfoHelper::GetDataEntityTypeDescription( 
	Core::DataEntity::Pointer dataEntity )
{
	// Create header
	std::ostringstream	osHeader;
	Core::DataEntityType DataEntityType = dataEntity->GetType( );
	std::string strEntityType = Core::DataEntityInfoHelper::GetEntityTypeAsString( DataEntityType );
	osHeader << "DataEntityType: " << strEntityType;
	std::string strHeader = osHeader.str();

	return strHeader;
}

Core::DataEntityType DataEntityInfoHelper::ReadDataEntityTypeDescription( 
	const std::string &fileName )
{
	// Read the header to get the modality
	Core::DataEntityType DataEntityType = Core::UnknownTypeId;
	std::string strHeader;
	blVTKHelperTools::blVTKUtils::ReadHeader( fileName.c_str(), strHeader );
	if ( !strHeader.empty() )
	{
		std::stringstream headerStream( strHeader );
		std::string strDataEntityType;
		headerStream.ignore( std::string( "DataEntityType: " ).size() );
		std::getline(headerStream, strDataEntityType);
		//char cBuffer[ 32 ];
		//sscanf( strHeader.c_str(), "DataEntityType: %32s", cBuffer );
		DataEntityType = DataEntityInfoHelper::GetDataEntityType( strDataEntityType.c_str() );
	}

	return DataEntityType;
}
