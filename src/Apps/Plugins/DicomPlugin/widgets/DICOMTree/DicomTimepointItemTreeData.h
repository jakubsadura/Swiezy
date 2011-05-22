/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef DicomTimepointItemTreeData_H
#define DicomTimepointItemTreeData_H

#include <wx/treectrl.h>

namespace  DicomPlugin{

/** 
A class that holds some information about timepoint in the tree item

\ingroup DicomPlugin
\author Jakub Lyko
\date 07 May 2008
*/

class TimepointItemTreeData : public wxTreeItemData
{
public:
	//!
	TimepointItemTreeData
	(
	std::string &timepointId,
	std::string &seriesId,
	std::string &studyId,
	std::string &patientId
	) :
	m_timepointId(timepointId),
	m_seriesId(seriesId),
	m_studyId(studyId),
	m_patientId(patientId)
	{ }

	//!
	~TimepointItemTreeData() {}

	//!
	const std::string& GetTiempointId() { return m_timepointId; }

	//!
	const std::string& GetSeriesId() { return m_seriesId; }

	//!
	const std::string& GetStudyId() { return m_studyId; }

	//!
	const std::string& GetPatientId() { return m_patientId; }

private:
	//!
	std::string m_timepointId;

	//!
	std::string m_seriesId;

	//!
	std::string m_studyId;

	//!
	std::string m_patientId;
};

} // DicomPlugin

#endif
