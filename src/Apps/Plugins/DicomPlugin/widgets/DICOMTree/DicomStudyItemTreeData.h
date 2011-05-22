/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef DicomStudyItemTreeData_H
#define DicomStudyItemTreeData_H

#include <wx/treectrl.h>

namespace DicomPlugin{

/** 
A class that holds some information about study in the tree item

\ingroup DicomPlugin
\author Jakub Lyko
\date 07 May 2008
*/

class StudyItemTreeData : public wxTreeItemData
{
public:
	//!
	StudyItemTreeData(std::string& studyId, std::string& patientId) :
		m_studyId(studyId),
		m_patientId(patientId)
	{ }

	//!
	~StudyItemTreeData() {}
	
	//!
	const std::string& GetStudyId() { return m_studyId; }

	//!
	const std::string& GetPatientId() { return m_patientId; }

private:
	//!
	std::string m_studyId;

	//!
	std::string m_patientId;
};

} // DicomPlugin

#endif
