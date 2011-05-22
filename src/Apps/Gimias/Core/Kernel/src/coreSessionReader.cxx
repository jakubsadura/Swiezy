/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreSessionReader.h"
#include "coreDataEntityReader.h"
#include "coreDataEntityHelper.h"

#include "blTextUtils.h"

#include <boost/archive/xml_iarchive.hpp>
#include <boost/filesystem/operations.hpp>

using namespace Core::IO;

SessionReader::SessionReader()
{
}

SessionReader::~SessionReader()
{
}

void SessionReader::Update()
{	
	if( m_filepath.size() == 0 )
		return;

	Core::DataEntityList::Pointer dataList = Core::Runtime::Kernel::GetDataContainer()->GetDataEntityList();
	if(dataList.IsNull())
		return;

	dataList->RemoveAll();

	//get directory path
	boost::filesystem::path path(m_filepath);
	std::string dirPath = path.parent_path().string();

	//create session object
	Core::Session* session = new Core::Session();
	std::ifstream readSession(m_filepath.c_str());
	boost::archive::xml_iarchive xmlArchive(readSession);
	using boost::serialization::make_nvp;
	xmlArchive >> BOOST_SERIALIZATION_NVP(session);

	for(int i = 0; i < session->GetDataList().size(); i++)
	{
		LoadLightData(session->GetDataList().at(i));
	}

	delete session;
}

void SessionReader::LoadLightData( Core::LightData* lightData, Core::DataEntity::Pointer father )
{
	Core::DataEntity::Pointer dataEntity = NULL;

	//create DataEntity
	boost::filesystem::path path(m_filepath);
	std::string dirPath = path.parent_path().string();
	std::vector< std::string > paths;
	std::string filePath;
	boost::filesystem::path pathFile(lightData->GetFilepath());
	if (lightData->GetNumOfTimeSteps() > 1)
	{
		for (size_t time = 0; time < lightData->GetNumOfTimeSteps(); time++)
		{
			filePath = dirPath +"/"+ lightData->GetRelativePath();
			paths.push_back(AddFileNumber(filePath, time ));
		}
	}
	else
	{
		filePath = dirPath +"/"+ lightData->GetRelativePath();
		paths.push_back(filePath);
	}

	dataEntity = Core::DataEntityHelper::LoadDataEntity( paths );
	dataEntity->GetMetadata()->SetName(lightData->GetName());
	dataEntity->SetFather(father);

	Core::DataEntityHelper::AddDataEntityToList( dataEntity, false );

	//load children
	for(int i=0; i<lightData->GetChildren().size(); i++)
	{
		LoadLightData(lightData->GetChildren().at(i), dataEntity);
	}
}
