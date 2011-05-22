/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "dcmPatient.h"

using namespace dcmAPI;

Patient::Patient()
{
	m_studies = new std::vector<Study::Pointer>();
	InitializeTagsMap();
}

Patient::~Patient()
{
	//TODO Iterate over the m_patientTags vector and delete all pointers. 
	//delete m_tags;
	delete m_studies;
}

void Patient::AddStudy(Study::Pointer _study)
{

	if ( !FindStudy( _study->GetTagAsText(tags::StudyId) ) )
	{
		m_studies->push_back(_study);
	}
}

void Patient::RemoveAllStudies()
{
	m_studies->clear();
}

StudyIdVectorPtr Patient::StudyIds() const{
	StudyIdVectorPtr studyIdVector(new StudyIdVector());

	for(unsigned int i=0; i < m_studies->size(); i++){
		Study::Pointer study = m_studies->at(i);
		studyIdVector->push_back(study->GetTagAsText(tags::StudyId));
	}

	return studyIdVector;
}

Study::Pointer Patient::Study(const StudyId& _studyId) const{
	Study::Pointer dcmStudy;

	for(unsigned int i=0; i < m_studies->size(); i++)
	{
		Study::Pointer study = m_studies->at(i);
		if (study->GetTagAsText(tags::StudyId) == _studyId)
		{
			dcmStudy = study;
			break;
		}
	}

	return dcmStudy;
}

void Patient::GetAllTags(TagsMap& tagsMap)
{
}

void Patient::InitializeTagsMap()
{
	m_tagsMap->insert(TagPair(tags::PatientId, ""));
	m_tagsMap->insert(TagPair(tags::PatientName, ""));
	m_tagsMap->insert(TagPair(tags::PatientSex, ""));
	m_tagsMap->insert(TagPair(tags::PatientAge, ""));
}

bool dcmAPI::Patient::FindStudy( const StudyId& _studyId ) const
{
	bool found = false;

	for(unsigned int i=0; i < m_studies->size(); i++)
	{
		Study::Pointer study = m_studies->at(i);
		if (study->GetTagAsText(tags::StudyId) == _studyId)
		{
			found = true;
			break;
		}
	}

	return found;
}
