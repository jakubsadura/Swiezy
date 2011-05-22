/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
	
#include "DicomTree.h"
#include "blMitkUnicode.h"
#include "wx/imaglist.h"

#include "coreReportExceptionMacros.h"
#include "coreKernel.h"
#include "coreSettings.h"

BEGIN_EVENT_TABLE(DicomPlugin::DICOMTree, wxTreeCtrl)
END_EVENT_TABLE()

using namespace DicomPlugin;

DICOMTree::DICOMTree( 
	wxWindow *parent, 
	wxWindowID id /*= wxID_ANY*/, 
	const wxPoint& pos /*= wxDefaultPosition*/, 
	const wxSize& size /*= wxDefaultSize*/, 
	long style /*= wxTR_HAS_BUTTONS | wxTR_LINES_AT_ROOT*/, 
	const wxValidator& validator /*= wxDefaultValidator*/, 
	const wxString& name /*= wxTreeCtrlNameStr*/ )
	: wxTreeCtrl( parent, id, pos, size, style, validator, name )
{

	m_dcmData = dcmAPI::DataSet::New( );

	m_currentDcmPatientId = "";
	m_currentDcmStudyId = "";
	m_currentDcmSeriesId = "";
	m_currentDcmTimepointId = "";
	m_currentDcmSliceId = "";

	SetIcons();
}

//assign icons for items in tree
void DICOMTree::SetIcons()
{
	//set tree icons
	wxImageList* imageList = new wxImageList(16,16);
	imageList->Add(GetIcon("patients_list_16.ico"));
	imageList->Add(GetIcon("patient_16.ico"));
	imageList->Add(GetIcon("study_16.ico"));
	imageList->Add(GetIcon("series_16.ico"));
	imageList->Add(GetIcon("timepoint_16.ico"));
	imageList->Add(GetIcon("ecg_16.ico"));
	imageList->Add(GetIcon("slice_16.ico"));

	AssignImageList(imageList);
}

dcmAPI::DataSet::Pointer DICOMTree::GetDcmData() const
{
	return m_dcmData;
}

void DICOMTree::SetDcmData( dcmAPI::DataSet::Pointer val )
{
	m_dcmData = val;
}

void DICOMTree::LoadPatientsIntoTree()
{
	//first clean the tree
	DeleteAllItems();

	dcmAPI::PatientIdVectorPtr patiendIdVector = m_dcmData->GetPatientIds();
	unsigned int numberOfPatients = patiendIdVector->size();

	if(numberOfPatients == 0 )
	{
		throw Core::Exceptions::Exception( 
			"LoadPatientsIntoTree", 
			"Specified DICOM data does not contain any patients" );
	}

	if(numberOfPatients > 200 )
	{
		wxMessageDialog* question = new wxMessageDialog(
			this, 
			_U( "Specified DICOM data contains more than 200 patients Do you want to load it?" ),
			_U( "DICOM data too large" ),
			wxYES_NO | wxICON_QUESTION | wxSTAY_ON_TOP);
		switch(question->ShowModal())
		{
		case wxID_YES: break; 
		case wxID_NO: return;break;
		}
	}

	wxTreeItemId rootPatients = AddRoot(wxT("Patients list"), 0, 0);

	//reading patients
	for(unsigned i=0; i < patiendIdVector->size(); i++)
	{
		m_currentDcmPatientId = patiendIdVector->at(i);
		dcmAPI::Patient::Pointer patientData = m_dcmData->GetPatient(m_currentDcmPatientId);
		std::string patientName = patientData->GetTagAsText(dcmAPI::tags::PatientName);
		std::string patientBirthDate = patientData->GetTagAsText(dcmAPI::tags::PatientDate);
		std::string patientSex = patientData->GetTagAsText(dcmAPI::tags::PatientSex);
		wxTreeItemId patientItem = AppendItem
			(
			rootPatients,
			_U("Patient: " +  patientName),
			1,
			1,
			new PatientItemTreeData(m_currentDcmPatientId, patientName, patientBirthDate, patientSex)
			);
		//reading studies for the patient
		LoadStudiesIntoTree(patientData, patientItem);
	}
}

wxTreeItemId DICOMTree::GetFirstSpecifiedTreeItem(
	TreeItemType type, 
	wxTreeItemIdValue& cookie, 
	bool expand)
{
	wxTreeItemId emptyId;
	try
	{
		//get tree root 
		wxTreeItemId patientListId = GetRootItem();
		if(!patientListId.IsOk())
			return emptyId;

		if(expand)
			Expand(patientListId);

		//first patient
		wxTreeItemId patientId = GetFirstChild(patientListId, cookie);
		if( !patientId.IsOk() )
			return emptyId;
		if(type == PATIENT)
			return patientId;
		if(expand)
			Expand(patientId);

		//first study
		wxTreeItemId studyId = GetFirstChild(patientId, cookie);
		if( !studyId.IsOk() )
			return emptyId;
		if( type == STUDY)
			return studyId;
		if(expand)
			Expand(studyId);


		//first series
		wxTreeItemId seriesId = GetFirstChild(studyId, cookie);
		if( !seriesId.IsOk() )
			return emptyId;
		if ( type == SERIES)
			return seriesId;
		if(expand)
			Expand(seriesId);

		//first timepoint
		wxTreeItemId timepointId = GetFirstChild(seriesId, cookie);
		if( !timepointId.IsOk() )
			return emptyId;
		if (type == TIMEPOINT)
			return timepointId;
		if(expand)
			Expand(timepointId);

		//first slice
		wxTreeItemId sliceId = GetFirstChild(timepointId, cookie);
		if( !sliceId.IsOk() )
			return emptyId;
		else if (type == SLICE)
			return sliceId;
	}
	coreCatchExceptionsReportAndNoThrowMacro(DicomPlugin::DICOMTree::GetFirstSpecifiedTreeItem)

	return emptyId;
}

void DICOMTree::LoadStudiesIntoTree
	(
	dcmAPI::Patient::Pointer patientData,
	const wxTreeItemId& patientItem
	)
{
	try
	{
		if(patientData.get() == NULL)
			return;

		//reading studies
		dcmAPI::StudyIdVectorPtr studiesIdVector = patientData->StudyIds();
		for(unsigned i=0; i < studiesIdVector->size(); i++)
		{
			m_currentDcmStudyId = studiesIdVector->at(i);
			dcmAPI::Study::Pointer studyData = patientData->Study(m_currentDcmStudyId);
			std::string rawDate = studyData->GetTagAsText(dcmAPI::tags::StudyDate);
			const std::string studyDescription = studyData->GetTagAsText(dcmAPI::tags::StudyDescription);
			const std::string date = dcmAPI::CreateDateFromRawDcmTagDate(rawDate);
			wxTreeItemId studyItem = AppendItem
				(
				patientItem,
				_U("Study: " + date + "(" + studyDescription + ")"),
				2,
				2,
				new StudyItemTreeData(m_currentDcmStudyId, m_currentDcmPatientId)
				);
			//reading series for the study
			LoadSeriesIntoTree(studyData, studyItem);
		}
	}
	coreCatchExceptionsReportAndNoThrowMacro(DicomPlugin::DICOMTree::LoadStudiesIntoTree)
}

void DICOMTree::LoadSeriesIntoTree(dcmAPI::Study::Pointer studyData,wxTreeItemId& studyItem)
{
	try
	{
		if(studyData.get() == NULL)
			return;

		//reading series
		dcmAPI::SeriesIdVectorPtr seriesIdVector = studyData->SeriesIds();
		for(unsigned i=0; i < seriesIdVector->size(); i++)
		{
			m_currentDcmSeriesId = seriesIdVector->at(i);
			std::string nr = _U(wxString::Format(wxT("%d"), i+1));
			dcmAPI::Series::Pointer seriesData = studyData->Series(m_currentDcmSeriesId);
			std::string seriesId = seriesData->GetTagAsText(dcmAPI::tags::SeriesId);
			std::string seriesModality = seriesData->GetTagAsText(dcmAPI::tags::Modality);
			std::string seriesDescription = seriesData->GetTagAsText(dcmAPI::tags::SeriesDescription);

			wxTreeItemId seriesItem = AppendItem
				(
				studyItem,
				_U("Series" + nr + ", "  + seriesModality + "(" +  seriesDescription + ")"),
				3,
				3,
				new SeriesItemTreeData(m_currentDcmSeriesId, m_currentDcmStudyId, m_currentDcmPatientId)
				);

			//reading timepoints for the series
			LoadTimepointsIntoTree(seriesData, seriesItem);
		}
	}
	coreCatchExceptionsReportAndNoThrowMacro(DicomPlugin::DICOMTree::LoadSeriesIntoTree)
}

void DICOMTree::LoadTimepointsIntoTree(dcmAPI::Series::Pointer seriesData, wxTreeItemId& seriesItem)
{
	try
	{
		if(seriesData.get() == NULL)
			return;

		//reading timepoints
		dcmAPI::TimePointIdVectorPtr timePointIdVector = seriesData->TimePointIds();
		for(unsigned i=0; i < timePointIdVector->size(); i++)
		{
			m_currentDcmTimepointId = timePointIdVector->at(i);
			std::string nr = _U(wxString::Format(wxT("%d"), i+1));
			dcmAPI::TimePoint::Pointer timePointData = seriesData->TimePoint(m_currentDcmTimepointId);
			wxTreeItemId timePointItem = AppendItem
				(
				seriesItem,
				_U("Timepoint" + nr),
				4,
				4,
				new TimepointItemTreeData(m_currentDcmTimepointId, m_currentDcmSeriesId, m_currentDcmStudyId, m_currentDcmPatientId)
				);
			//reading slices for the timepoint
			LoadSlicesIntoTree(timePointData, timePointItem);
		}
	}
	coreCatchExceptionsReportAndNoThrowMacro(DicomPlugin::DICOMTree::LoadTimepointsIntoTree)
}

void DICOMTree::LoadSlicesIntoTree(dcmAPI::TimePoint::Pointer timePointData, wxTreeItemId& timePointItem)
{
	try
	{
		if(timePointData.get() == NULL)
			return;

		//reading slices
		dcmAPI::SliceIdVectorPtr sliceIdVector = timePointData->SliceIds();
		for(unsigned i=0; i < sliceIdVector->size(); i++)
		{
			m_currentDcmSliceId = sliceIdVector->at(i);
			std::string nr = _U(wxString::Format(wxT("%d"), i+1));
			dcmAPI::Slice::Pointer sliceData = timePointData->Slice(m_currentDcmSliceId);
			if(sliceData.get() != NULL)
			{
				SliceItemTreeData *sliceItemData = new SliceItemTreeData
					(
					m_currentDcmSliceId, 
					m_currentDcmTimepointId,
					m_currentDcmSeriesId,
					m_currentDcmStudyId,
					m_currentDcmPatientId,
					sliceData->GetTagAsText(dcmAPI::tags::SliceFilePath),
					sliceData->GetTagAsText(dcmAPI::tags::SliceFileName),
					sliceData->GetTagAsText(dcmAPI::tags::WindowCenter),
					sliceData->GetTagAsText(dcmAPI::tags::WindowWidth),
					sliceData->GetTagAsText(dcmAPI::tags::PxType)
					);
				wxTreeItemId imageItem = AppendItem
					(
					timePointItem,
					_U("Slice" + nr),
					6,
					6,
					sliceItemData
					);
			}
		}
	}
	coreCatchExceptionsReportAndNoThrowMacro(DicomPlugin::DICOMTree::LoadSlicesIntoTree)
}

wxIcon DICOMTree::GetIcon( const std::string& filename )
{
	std::string fullPath = Core::Runtime::Kernel::GetApplicationSettings()->GetCoreResourceForFile(filename);
	return wxIcon(_U(fullPath), wxBITMAP_TYPE_ANY);
}

unsigned int DICOMTree::GetCurrentSelectedSliceCount()
{
	unsigned int sliceCount = 0;

	if(m_currentSliceTreeItemId.IsOk())
	{
		//find appropriate slice in timepoint
		wxTreeItemId timepointTreeItemId = GetItemParent(m_currentSliceTreeItemId);
		if(timepointTreeItemId.IsOk())
		{
			sliceCount = GetChildrenCount(timepointTreeItemId);
		}
	}

	return sliceCount;
}

bool DICOMTree::SelectItemByIndex( unsigned int slicePos )
{
	wxTreeItemId selectedItem;
	
	if( !m_currentSliceTreeItemId.IsOk() )
	{
		return false;
	}

	//find appropriate slice in timepoint
	wxTreeItemId timepointTreeItemId = GetItemParent(m_currentSliceTreeItemId);
	if( !timepointTreeItemId.IsOk() )
	{
		return false;
	}

	// find item timePoint
	wxTreeItemId sliceTreeItemId = FindChildItemByIndex( timepointTreeItemId, slicePos );

	// If found
	bool selectionChanged = false;
	if( sliceTreeItemId.IsOk() )
	{
		selectionChanged = SetCurrentSliceTreeItemId( sliceTreeItemId );
	}


	return selectionChanged;
}

bool DICOMTree::SelectItemByIndex( 
	unsigned int timePoint, 
	unsigned int slicePos )
{
	wxTreeItemId selectedItem;
	if( !m_currentSliceTreeItemId.IsOk() )
	{
		return false;
	}

	//find appropriate timepoint in the series
	wxTreeItemId timepointTreeItemId = GetItemParent(m_currentSliceTreeItemId);
	wxTreeItemId seriesTreeItemId = GetItemParent(timepointTreeItemId);

	wxTreeItemId newTimepointTreeItemId;
	newTimepointTreeItemId = FindChildItemByIndex( seriesTreeItemId, timePoint );
	if ( !newTimepointTreeItemId.IsOk() )
	{
		return false;
	}

	selectedItem = FindChildItemByIndex( newTimepointTreeItemId, slicePos );


	// If found
	bool selectionChanged = false;
	if( selectedItem.IsOk() )
	{
		selectionChanged = SetCurrentSliceTreeItemId( selectedItem );
	}

	return selectionChanged;

}

wxTreeItemId DICOMTree::FindChildItemByIndex( 
	wxTreeItemId treeItemId,
	unsigned int index )
{
	// find item timePoint
	wxTreeItemIdValue cookie;
	unsigned treePos = 0;
	wxTreeItemId childItemId = GetFirstChild(treeItemId, cookie);
	while ( childItemId.IsOk() && treePos != index )
	{
		childItemId = GetNextChild(treeItemId, cookie);
		treePos++;
	}

	return childItemId;
}

wxTreeItemId DICOMTree::GetCurrentSliceTreeItemId() const
{
	return m_currentSliceTreeItemId;
}

bool DICOMTree::SetCurrentSliceTreeItemId( wxTreeItemId val )
{
	if ( m_currentSliceTreeItemId == val )
	{
		return false;
	}

	//select item and render it
	wxTreeItemId timepointTreeItemId = GetItemParent( val );
	m_currentSliceTreeItemId = val;

	if( IsExpanded( timepointTreeItemId ) == true )
		SelectItem( m_currentSliceTreeItemId );
	else
		SelectItem( timepointTreeItemId );

	return true;
}

PatientItemTreeData* DICOMTree::GetConnectedPatientTreeItem( wxTreeItemId anyTreeItem )
{
	PatientItemTreeData* patientData = NULL;//dynamic_cast<PatientItemTreeData*>(this->GetItemData(anyTreeItem));

	/*if ( patientData != NULL ) 
		return patientData;*/
	
	wxTreeItemId currentId = anyTreeItem;
	wxTreeItemId previousId = anyTreeItem;
	int maxLoopCount = 5;
	while(maxLoopCount > 0)
	{
		patientData = dynamic_cast<PatientItemTreeData*>(this->GetItemData(currentId));
		if ( patientData != NULL )
		{
			break;
		}
		else
		{
			previousId = currentId;
			currentId = this->GetItemParent(previousId);
		}
		maxLoopCount--;
	}
	return patientData;
}