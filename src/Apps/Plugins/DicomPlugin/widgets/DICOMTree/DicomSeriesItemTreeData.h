/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef DicomSeriesItemTreeData_H
#define DicomSeriesItemTreeData_H

#include <wx/treectrl.h>

namespace DicomPlugin{

/** 
A class that holds some information about series in the tree item

\ingroup DicomPlugin
\author Jakub Lyko
\date 07 May 2008
*/

class SeriesItemTreeData : public wxTreeItemData
{
public:
	//!
	SeriesItemTreeData(std::string &seriesId, std::string &studyId, std::string &patientId, bool ecgAttached = false) :
		m_seriesId(seriesId),
		m_studyId(studyId),
		m_patientId(patientId),
		m_ecgAttached(ecgAttached)
	{ }

	//!
	~SeriesItemTreeData() {}
	
	//!
	const std::string& GetSeriesId() { return m_seriesId; }

	//!
	const std::string& GetStudyId() { return m_studyId; }

	//!
	const std::string& GetPatientId() { return m_patientId; }

	//!
	bool IsEcgAttached() const { return m_ecgAttached; }

	//!
	void IsEcgAttached(bool val) { m_ecgAttached = val; }

private:
	//!
	std::string m_seriesId;

	//!
	std::string m_studyId;

	//!
	std::string m_patientId;

	//!
	bool m_ecgAttached;
};

} // DicomPlugin

#endif
