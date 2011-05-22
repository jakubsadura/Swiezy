/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreSession_H
#define coreSession_H

#include "gmDataHandlingWin32Header.h"
#include "coreLightData.h"
#include "coreDataEntity.h"

namespace Core{

/** 
\brief A session is a class that stores list of LightData objects and
represents state of Data List in GIMIAS(with all parent-children
relations) that is going to be saved

\author Jakub Lyko
\date 10 Dec 2009
\ingroup gmDataHandling
*/

class GMDATAHANDLING_EXPORT Session
{
public:
	//!
	Session();

	//!
	virtual ~Session();

	//!
	std::vector<LightData*> GetDataList() {return m_dataList;}

	//!
	void AddData(LightData* data);

	//! give access to private members for serialization
	friend class boost::serialization::access;

private:
	//!
	std::vector<LightData*> m_dataList;

	//!
	template<class Archive> void serialize(Archive &ar, const unsigned int version);

};

template<class Archive> void Session::serialize(Archive &ar, const unsigned int version)
{
	using boost::serialization::make_nvp;
	ar & make_nvp("DataList", m_dataList);
}

} // end namespace Core

#endif

//LightData* CreateLightData(Core::DataEntity* dataEntity, std::string dirPath)
//{
//	LightData* lightData = new LightData();
//
//	//set name
//	lightData->SetName(dataEntity->GetMetadata()->GetName());
//	
//	//create filepath
//	unsigned int id = dataEntity->GetId();
//	std::ostringstream id_str;
//	id_str << "_" << id; 
//	//default file type
//	std::string fileType = Core::IO::DataEntityWriter::GetDefaultFileTypeForWrite(dataEntity->GetType());
//	std::string filepath = dirPath + Core::IO::SlashChar + dataEntity->GetMetadata()->GetName() + id_str.str() + fileType;
//	lightData->SetFilepath(filepath);
//
//	for(int i=0; i<dataEntity->GetChildrenList().size(); i++)
//	{
//		LightData* child = CreateLightData(dataEntity->GetChildrenList().at(i), dirPath);
//		lightData->AddChild(child);
//	}
//	return lightData;
//}
//
//void PrintLightData(LightData* ld)
//{
//	std::cout << ld->GetName();
//	std::cout << std::endl;
//	std::cout << "children of " << ld->GetName() << ":";
//	std::cout << std::endl;
//	for(int i=0; i<ld->GetChildren().size(); i++)
//	{
//		LightData* child = ld->GetChildren().at(i);
//		std::cout << child->GetName()<< "\t ";
//		PrintLightData(child);
//	}
//	std::cout << std::endl;
//}
//
//void PrintSession(Session* sesson)
//{
//	for(int i=0; i<sesson->GetDataList().size(); i++)
//	{
//		PrintLightData(sesson->GetDataList().at(i));
//	}
//}