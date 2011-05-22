/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef DicomSliceItemTreeData_H
#define DicomSliceItemTreeData_H

#include <wx/treectrl.h>

namespace DicomPlugin{

/** 
A class that holds some information about slice in the tree item

\ingroup DicomPlugin
\author Jakub Lyko
\date 07 May 2008
*/

class SliceItemTreeData : public wxTreeItemData
{
public:
	//!
	SliceItemTreeData
	(
	const std::string &sliceId,
	const std::string &timepointId,
	const std::string &seriesId,
	const std::string &studyId,
	const std::string &patientId,
	const std::string &slicePath,
	const std::string &sliceName,
	const std::string &windowCenter,
	const std::string &windowWidth,
	const std::string &pixelType
	) :
	m_sliceId(sliceId),
	m_timepointId(timepointId),
	m_seriesId(seriesId),
	m_studyId(studyId),
	m_patientId(patientId),
	m_slicePath(slicePath),
	m_sliceName(sliceName),
	m_windowCenter(windowCenter),
	m_windowWidth(windowWidth),
	m_pixelType(pixelType)
	{ }
	
	//!
	~SliceItemTreeData() {}
	
	//!
	const std::string& GetSliceId() { return m_sliceId; }

	//!
	const std::string& GetTiempointId() { return m_timepointId; }

	//!
	const std::string& GetSeriesId() { return m_seriesId; }

	//!
	const std::string& GetStudyId() { return m_studyId; }

	//!
	const std::string& GetPatientId() { return m_patientId; }

	//!
	const std::string& GetSlicePath() { return m_slicePath; }

	//!
	const std::string& GetSliceName() { return m_sliceName; }

	//!
	const std::string& GetWindowCenter() { return m_windowCenter; }

	//!
	const std::string& GetWindowWidth() { return m_windowWidth; }

	//!
	const std::string& GetPixelType() { return m_pixelType; }

private:
	//!
	std::string m_sliceId;

	//!
	std::string m_timepointId;

	//!
	std::string m_seriesId;

	//!
	std::string m_studyId;

	//!
	std::string m_patientId;

	//!
	std::string m_slicePath;

	//!
	std::string m_sliceName;

	//!
	std::string m_windowCenter;

	//!
	std::string m_windowWidth;

	//!
	std::string m_pixelType;
};

} // DicomPlugin

#endif
