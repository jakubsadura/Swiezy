/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreDataEntityInfoHelper_H
#define coreDataEntityInfoHelper_H

#include "gmDataHandlingWin32Header.h"
#include "coreObject.h"
#include "coreDataEntity.h"
#include <list>

namespace Core
{

/** 
\brief DataEntityHelper provides helper functions for working with 
DataEntities and to retrieve info from them:
 - Modality
 - Type

\ingroup gmDataHandling
\author Juan A. Moya
\date 26 Mar 2008
*/
class GMDATAHANDLING_EXPORT DataEntityInfoHelper : public Core::Object
{
public:
	typedef Core::DataEntityType DataEntityType;
	typedef std::list<DataEntityType> EntityList;
	typedef Core::ModalityType ModalityType;
	typedef std::list<ModalityType> ModalityList;

public:
	//! Define virtual destructor to avoid warning
	virtual ~DataEntityInfoHelper( );

	//! Returns the current type of entity in its std:string representation 
	static std::string GetEntityTypeAsString(
		const Core::DataEntityType &type);
	static std::string GetModalityTypeAsString(
		const DataEntityInfoHelper::ModalityType &modality);
	static EntityList GetAvailableEntityTypes(void);
	static ModalityList GetAvailableModalityTypes(void);

	//! Returns a std:string with a short description of what the DataEntity is holding 
	static std::string GetShortDescription(
		Core::DataEntity::Pointer dataEntity);
	static ModalityType GetModalityType( const std::string &strModality );
	static Core::DataEntityType GetDataEntityType( 
		const std::string &strDataEntityType );

	//! Get "Modality: <name>"
	static std::string GetModalityDescription( 
						Core::DataEntity::Pointer dataEntity );
	
	//! Load modality from file header using the format "Modality: <name>"
	static Core::ModalityType LoadModalityDescription( 
						const std::string &fileName );

	//! Get "DataEntityType: <name>"
	static std::string GetDataEntityTypeDescription( 
						Core::DataEntity::Pointer dataEntity );

	//! Load modality from file header using the format "DataEntityType: <name>"
	static Core::DataEntityType ReadDataEntityTypeDescription( 
						const std::string &fileName);

private:
	//!
	static void BuildMaps( );

private:
	//! names for each data entity type
	static std::map<Core::DataEntityType,std::string> m_MapDataEntityType;

	//! names for each modality type
	static std::map<Core::ModalityType,std::string> m_MapModalityType;
};

}

#endif
