/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef DicomTree_H
#define DicomTree_H

#include <wx/wx.h>
#include <wx/treectrl.h>

#include "dcmDataSet.h"

#include "DicomPatientItemTreeData.h"
#include "DicomStudyItemTreeData.h"
#include "DicomSeriesItemTreeData.h"
#include "DicomTimepointItemTreeData.h"
#include "DicomSliceItemTreeData.h"

namespace DicomPlugin{

/** 
DICOM Tree with icons and dcmAPI structures

\ingroup DicomPlugin
\author Xavi Planes
\date 20 July 2009
*/
class DICOMTree : public wxTreeCtrl
{
public:
	enum TreeItemType { PATIENT,  STUDY, SERIES, TIMEPOINT, SLICE};
    
	DICOMTree(wxWindow *parent, wxWindowID id = wxID_ANY,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = wxTR_HAS_BUTTONS | wxTR_LINES_AT_ROOT,
               const wxValidator& validator = wxDefaultValidator,
			   const wxString& name = wxTreeCtrlNameStr);

	//!
	dcmAPI::DataSet::Pointer GetDcmData() const;
	void SetDcmData(dcmAPI::DataSet::Pointer val);

	//!
	wxTreeItemId GetCurrentSliceTreeItemId() const;

	//! \return true if selection changed
	bool SetCurrentSliceTreeItemId(wxTreeItemId val);

	//! Fill the tree with the info in dcmAPI::DataSet
	void LoadPatientsIntoTree();

	//! Find the first item of type type
	wxTreeItemId GetFirstSpecifiedTreeItem(
		TreeItemType type, 
		wxTreeItemIdValue& cookie, 
		bool expand = false);

	//! Get number of slices of the current selected time point
	unsigned int GetCurrentSelectedSliceCount( );

	/** Select a item using slice index
	\return true if selection changed
	*/
	bool SelectItemByIndex( unsigned int slicePos );

	/** Select an item using timePoint index and slice index
	\return true if selection changed
	*/
	bool SelectItemByIndex( unsigned int timePoint, unsigned int slicePos );

	//! get patient data for selected (any) tree item
	PatientItemTreeData* GetConnectedPatientTreeItem (wxTreeItemId anyTreeItem);

private:
	//!
	void SetIcons();

	//!
	void LoadStudiesIntoTree(
		dcmAPI::Patient::Pointer patientData, 
		const wxTreeItemId& patientItem);

	//!
	void LoadSeriesIntoTree(
		dcmAPI::Study::Pointer studyData, 
		wxTreeItemId& studyItem);

	//!
	void LoadTimepointsIntoTree(
		dcmAPI::Series::Pointer seriesData, 
		wxTreeItemId& seriesItem);

	//!
	void LoadSlicesIntoTree(
		dcmAPI::TimePoint::Pointer timePointData, 
		wxTreeItemId& timePointItem);

	//! Returns an icon based on the GIMIAS resource path and \a filename
	wxIcon GetIcon(const std::string& filename);

	//! Find the child of the treeItemId using the index
	wxTreeItemId FindChildItemByIndex( 
		wxTreeItemId treeItemId,
		unsigned int index );

private:
	//! contains structure of the dicom data (from patient to slice)
	dcmAPI::DataSet::Pointer m_dcmData;

	//!
	wxTreeItemId m_currentSliceTreeItemId;

	//!
	std::string m_currentDcmPatientId;

	//!
	std::string m_currentDcmStudyId;

	//!
	std::string m_currentDcmSeriesId;

	//!
	std::string m_currentDcmTimepointId;

	//!
	std::string m_currentDcmSliceId;

	DECLARE_EVENT_TABLE()
};

} // DicomPlugin

#endif //DicomTree_H
