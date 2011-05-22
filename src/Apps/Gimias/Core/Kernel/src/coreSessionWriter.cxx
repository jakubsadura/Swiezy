/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreSessionWriter.h"
#include "coreDataEntityWriter.h"
#include "coreDataEntityHelper.h"

#include <boost/archive/xml_oarchive.hpp>
#include <boost/filesystem/operations.hpp>

using namespace Core::IO;

SessionWriter::SessionWriter()
{
	m_TreeManager = NULL;
}

SessionWriter::~SessionWriter()
{
}

void SessionWriter::Update()
{	
	if( m_filepath.size() == 0 )
		return;

	Core::DataEntityList::Pointer dataList = Core::Runtime::Kernel::GetDataContainer()->GetDataEntityList();
	if(dataList.IsNull())
		return;

	//get directory path
	boost::filesystem::path path(m_filepath);
	std::string dirPath = path.parent_path().string();

	//create session object
	Core::Session* session = new Core::Session();
	try
	{
		Core::DataEntityList::iterator it; 
		for(it = dataList->Begin(); it != dataList->End(); it++)
		{
			unsigned int id = dataList->GetId(it); 
			Core::DataEntity::Pointer dataEntity = dataList->GetDataEntity(id);
			if(dataEntity->GetFather().IsNull())
			{
				Core::LightData* ld = SaveDataEntity(dataEntity, dirPath);
				session->AddData(ld);
			}
		}

		std::ofstream saveSession(m_filepath.c_str());
		boost::archive::xml_oarchive xmlArchive(saveSession);
		using boost::serialization::make_nvp;
		xmlArchive << BOOST_SERIALIZATION_NVP(session);
		saveSession.close();

		delete session;
	}
	catch(...)
	{
		if (session != NULL)
			delete session;
		throw;
	}
}

Core::LightData* SessionWriter::SaveDataEntity(
	Core::DataEntity::Pointer dataEntity,
	std::string dirPath)
{
	Core::LightData* lightData = new Core::LightData();

	//set name
	lightData->SetName(dataEntity->GetMetadata()->GetName());

	//create filepath
	unsigned int id = dataEntity->GetId();
	std::ostringstream id_str;
	id_str << "_" << id; 
	std::string fileType = Core::IO::DataEntityWriter::GetDefaultFileTypeForWrite(dataEntity->GetType());
	std::string filepath = dirPath + Core::IO::SlashChar + dataEntity->GetMetadata()->GetName() + id_str.str() + fileType;
	
	// set number of TimeSteps
	wxString str = wxString::Format("%d", dataEntity->GetNumberOfTimeSteps() );
	lightData->SetNumberOfTimeSteps( std::string( str.mb_str( wxConvUTF8 ) ) );	

	//save data entity
	Core::DataEntityHelper::SaveDataEntity(filepath, dataEntity, m_TreeManager);
	lightData->SetFilepath(filepath);

	for(size_t i=0; i<dataEntity->GetChildrenList().size(); i++)
	{
		LightData* child = SaveDataEntity(dataEntity->GetChildrenList().at(i), dirPath);
		lightData->AddChild(child);
	}
	return lightData;
}

void Core::IO::SessionWriter::SetTreeManager( Core::RenderingTreeManager* val )
{
	m_TreeManager = val;
}
