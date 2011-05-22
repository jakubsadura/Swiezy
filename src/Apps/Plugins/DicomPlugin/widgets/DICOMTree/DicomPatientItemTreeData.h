/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef DicomPatientItemTreeData_H
#define DicomPatientItemTreeData_H

#include <wx/treectrl.h>

namespace DicomPlugin{

/** 
A class that holds some information about patient in the tree item

\ingroup DicomPlugin
\author Jakub Lyko
\date 07 May 2008
*/

class PatientItemTreeData : public wxTreeItemData
{
public:
	//!
	PatientItemTreeData
	(
	std::string& patientId,
	std::string& patientName,
	std::string& patientBirthDate,
	std::string& patientSex
	) :
	m_patientId(patientId),
	m_patientName(patientName),
	m_patientBirthDate(patientBirthDate),
	m_patientSex(patientSex)
	{ }

	//!
	~PatientItemTreeData() {}

	//!
	std::string GetPatientId() const { return m_patientId; }
	//!
	std::string GetPatientName() const { return m_patientName; }
	//!
	std::string GetPatientBirthDate() const { return m_patientBirthDate; }
	//!
	std::string GetPatientSex() const { return m_patientSex; }

private:
	//!
	std::string m_patientId;
	//!
	std::string m_patientName;
	//!
	std::string m_patientBirthDate;
	//!
	std::string m_patientSex;
};

} // DicomPlugin

#endif
