/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
	
#include "DicomWorkingAreaPanelWidget.h"
#include "DicomPatientItemTreeData.h"
#include "DicomStudyItemTreeData.h"
#include "DicomSeriesItemTreeData.h"
#include "DicomTimepointItemTreeData.h"
#include "DicomSliceItemTreeData.h"
#include "DicomConnectToPacsDialogWidget.h"
#include "DicomMenuEventHandler.h"

#include "dcmFile.h"
#include "dcmDataSetReader.h"
#include "dcmMultiSliceReader.h"
#include "dcmSearch.h"
#include "dcmStandardImageReader.h"

#include <boost/shared_ptr.hpp>

#include "coreKernel.h"
#include "coreWxMitkCoreMainWindow.h"
#include "coreMainMenu.h"
#include "coreWxMitkGraphicalInterface.h"
#include "coreDataEntity.h"
#include "coreSettings.h"
#include "coreDataContainer.h"
#include "coreToolbarIO.h"
#include "corePluginTab.h"
#include "corePluginTabFactory.h"

#include "mitkITKImageImport.h"
#include "mitkImage.h"
#include "blMitkUnicode.h"

#include "wx/dynarray.h"
#include "wx/busyinfo.h"
#include "wx/imaglist.h"
#include "wx/dnd.h"
#include "wxID.h"

#include <vtkImageFlip.h>
#include <vtkImageReslice.h>
#include <vtkImageClip.h>
#include <itkImageToVTKImageFilter.h>
#include "itkOrientImageFilter.h"
#include "itkImageFileWriter.h"
#include "itkResampleImageFilter.h"
#include "itkAffineTransform.h"
#include "itkChangeInformationImageFilter.h"

#include "itkImageFileReader.h"
#include <itkExtractImageFilter.h>
#include "itkGDCMImageIO.h"
#include "dcmFile.h"
#include "dcmImageUtilities.h"
#include "dcmIOUtils.h"

using namespace DicomPlugin;
using namespace mitk;


// Event the widget
BEGIN_EVENT_TABLE(WorkingAreaPanelWidget, DicomWorkingAreaPanelWidgetUI)
	EVT_BUTTON(wxID_DICOM_ADD_TO_DATALIST, WorkingAreaPanelWidget::AddToDataList)
	EVT_TREE_SEL_CHANGED(wxID_DICOM_TREE, WorkingAreaPanelWidget::OnTreeSelChanged)
	EVT_SLIDER(wxID_DICOM_IMG_SPACE_SLIDER, WorkingAreaPanelWidget::OnSpaceSliderChanged)
	EVT_SLIDER(wxID_DICOM_IMG_TIME_SLIDER, WorkingAreaPanelWidget::OnTimeSliderChanged)
END_EVENT_TABLE()

int WorkingAreaPanelWidget::m_DataEntityCounter = 0;

#if wxUSE_DRAG_AND_DROP
/**
\brief Drop target for Data entity list (i.e. user drags a file from 
explorer unto window and adds the file to entity list)

\author Xavi Planes
\date 05 Dec 2008
\ingroup gmWidgets
*/
class DICOMDropTarget : public wxFileDropTarget
{
public:
	DICOMDropTarget(DicomMenuEventHandler *dicomMenuEventHandler) {
		m_DicomMenuEventHandler = dicomMenuEventHandler;
	}
	~DICOMDropTarget(){}
	virtual bool OnDropFiles(wxCoord WXUNUSED(x), wxCoord WXUNUSED(y),
		const wxArrayString& files)
	{

		if ( files.size() == 0 )
		{
			return false;
		}

		m_DicomMenuEventHandler->Open( _U(files[ 0 ]) );

		m_DicomMenuEventHandler->AddFileToHistory( _U(files[ 0 ]) );

		return true;
	}
private:
	//!
	DicomMenuEventHandler *m_DicomMenuEventHandler;
};
#endif


WorkingAreaPanelWidget::WorkingAreaPanelWidget( wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style)
: DicomWorkingAreaPanelWidgetUI(parent, id, pos, size, style)
{
	try
	{
		m_DataSetProcessor = DataSetProcessor::New();
		m_DataSetProcessor->GetDataSetHolder()->AddObserver(
			this, 
			&WorkingAreaPanelWidget::OnModifiedDataSet );

		m_dcmImageDataHolder = Core::DataEntityHolder::New();

		m_RenderWindow->Init(m_dcmImageDataHolder);
		m_LevelWindowWidget->SetDataStorage(m_RenderWindow->GetDataTree()->GetDataStorage());
		m_LevelWindowWidget->SetLevelWindowManager(m_RenderWindow->GetLevelWindowManager());
		m_LevelWindowWidget->Show(true);

		Core::Runtime::wxMitkGraphicalInterface::Pointer gIface = Core::Runtime::Kernel::GetGraphicalInterface();
		m_DicomMenuEventHandler = new DicomMenuEventHandler( this );
		gIface->GetMainWindow( )->PushEventHandler( m_DicomMenuEventHandler );


		#if wxUSE_DRAG_AND_DROP
			// Drag & Drop
			SetDropTarget( new DICOMDropTarget( m_DicomMenuEventHandler ) );
		#endif


		wxArrayString aChoices;
		aChoices.push_back( "Default" );
		aChoices.push_back( "SOPInstanceUID" );
		aChoices.push_back( "PhaseNumberMR" );
		aChoices.push_back( "AcquisitionTime" );
		m_cmbTimeTag->Append( aChoices );
		m_cmbTimeTag->SetValue("Default");

	}
	coreCatchExceptionsReportAndNoThrowMacro(WorkingAreaPanelWidget::WorkingAreaPanelWidget)
}

DicomPlugin::WorkingAreaPanelWidget::~WorkingAreaPanelWidget()
{
	Core::Runtime::wxMitkGraphicalInterface::Pointer gIface;
	gIface = Core::Runtime::Kernel::GetGraphicalInterface();
	// TODO : Check that the event handler is the one that was pushed before
	//wxEvtHandler* eventHandler = gIface->GetMainWindow( )->PopEventHandler( );
	delete m_DicomMenuEventHandler;
}

void DicomPlugin::WorkingAreaPanelWidget::OnModifiedDataSet()
{
	try
	{
		ClearCachedObjects( );

		Core::Runtime::wxMitkGraphicalInterface::Pointer gIface;
		gIface = Core::Runtime::Kernel::GetGraphicalInterface();

		//load patients into tree
		m_treeDicomView->SetDcmData( GetDataSet() );
		m_treeDicomView->LoadPatientsIntoTree();

		bool orientationTag = dcmAPI::Search::CheckOrientationTag( GetDataSet() );

		if ( orientationTag )
		{
			gIface->GetMainWindow()->ReportMessage("DICOM file imported successfully", false);
		}
		else
		{
			gIface->GetMainWindow()->ReportMessage("DICOM file imported without orientation information", true);
		}
		
		/*if the data is a multislice (3D or 4D) it has to be loaded to cached memory, 
		it means that single file contains 3D or 4D image and will be represented as SeriesCollective object
		*/
		// build series collective map
		if (m_DataSetProcessor->GetDataSet()->GetStorageType() == dcmAPI::DataSet::MULTI_SLICE_PER_FILE )
		{
			if(m_DataSetProcessor->GetInputPathType() ==  INPUT_PATH_DCMFILE)
			{
				SeriesCollectivePtr seriesCollective = BuildSeriesCollectiveFromSeriesDataset(m_DataSetProcessor->GetPath());
				wxTreeItemIdValue cookie;
				wxTreeItemId seriesTreeId = m_treeDicomView->GetFirstSpecifiedTreeItem(DICOMTree::SERIES, cookie);
				//! Read the series data tree item
				SeriesItemTreeData* seriesDataItemTreeData = static_cast<SeriesItemTreeData*>(m_treeDicomView->GetItemData(seriesTreeId));
				if(seriesDataItemTreeData != NULL)
					m_cachedSeriesCollectiveMap[seriesDataItemTreeData->GetSeriesId()] = seriesCollective;

				//attach ECG signal if it was in the data
				if(seriesCollective->m_signalCollective->GetNumberOfSignals() > 0)
				{
					if(seriesTreeId.IsOk())
					{
						m_treeDicomView->AppendItem
						(
						seriesTreeId,
						_U("ECG"),
						5,
						5,
						NULL
						);
					}
				}
			}
			else
			{
				dcmAPI::DataSet::Pointer dataset = GetDataSet(); 
				dcmAPI::PatientIdVectorPtr patiendIdVector = dataset->GetPatientIds();
				for(size_t i = 0; i < patiendIdVector->size(); i++)
				{
					dcmAPI::Patient::Pointer patient = dataset->GetPatient(patiendIdVector->at(i));
					//reading studies
					dcmAPI::StudyIdVectorPtr studyIdVector = patient->StudyIds();
					for(size_t j=0; j < studyIdVector->size(); j++)
					{
						dcmAPI::Study::Pointer study = patient->Study(studyIdVector->at(j));
						//reading series
						dcmAPI::SeriesIdVectorPtr seriesIdVector = study->SeriesIds();
						for(size_t k=0; k < seriesIdVector->size(); k++)
						{
							dcmAPI::Series::Pointer series = study->Series(seriesIdVector->at(k));
							std::string seriesId = series->GetTagAsText(dcmAPI::tags::SeriesId);

							//go the the first slice and take filepath
							//1 file will be represented as 1 series
							dcmAPI::TimePointIdVectorPtr timepointIdVector = series->TimePointIds();
							if(timepointIdVector->size()>0)
							{
								dcmAPI::TimePoint::Pointer timepoint = series->TimePoint(timepointIdVector->at(0));
								dcmAPI::SliceIdVectorPtr sliceIdVector = timepoint->SliceIds();
								if(sliceIdVector->size()>0)
								{
									dcmAPI::Slice::Pointer slice = timepoint->Slice(sliceIdVector->at(0));
									std::string filePath = slice->GetTagAsText(dcmAPI::tags::SliceFilePath);
									if(filePath.size()>0 && seriesId.size()>0)
									{
										SeriesCollectivePtr seriesCollective = BuildSeriesCollectiveFromSeriesDataset(filePath);
										m_cachedSeriesCollectiveMap[seriesId] = seriesCollective;
									}		
								}
							}
							//check if ecg is attched
							/*if(seriesCollective->m_signalCollective->GetNumberOfSignals() > 0)
							{}*/
						}
					}
				}
			}
		}

		//show start preview
		DoStartStep();
	}
	coreCatchExceptionsReportAndNoThrowMacro(WorkingAreaPanelWidget::OnModifiedDataSet)
}

void WorkingAreaPanelWidget::AddToDataList(wxCommandEvent& event)
{
	bool value = m_chbCombineData->GetValue();
		
	wxBusyInfo info(wxT("Generating data, please wait..."), this);
	try
	{
		wxArrayTreeItemIds treeItemIdsArray;
		if(m_treeDicomView->GetSelections(treeItemIdsArray) < 1)
			return;

		Core::DataContainer::Pointer data = Core::Runtime::Kernel::GetDataContainer();
		Core::DataEntityList::Pointer list = data->GetDataEntityList();
		Core::DataEntity::Pointer imageDataEntity = NULL;
		Core::DataEntity::Pointer ecgDataEntity = NULL;
		
		if(m_chbCombineData->GetValue() == false || treeItemIdsArray.GetCount() == 1)
		{
			for(int i=0; i<treeItemIdsArray.GetCount(); i++)
			{
				wxTreeItemId treeItemId = treeItemIdsArray.Item(i);

				//checking if a series item was selected
				if ( dynamic_cast<SeriesItemTreeData*>(m_treeDicomView->GetItemData(treeItemId)) != NULL ) 
				{
					imageDataEntity = BuildImageDataEntityFromSelectedSerie(treeItemId);
					ecgDataEntity = BuildEcgDataEntityFromSelectedSerie(treeItemId);  
				}

				//checking if timepoint item was selected
				if(dynamic_cast<TimepointItemTreeData*>(m_treeDicomView->GetItemData(treeItemId)) != NULL)
				{
					imageDataEntity = BuildImageDataEntityFromSelectedTimepoint(treeItemId);
				}

				//checking if slice item was selected
				if(dynamic_cast<SliceItemTreeData*>(m_treeDicomView->GetItemData(treeItemId)) != NULL)
				{
					imageDataEntity = BuildImageDataEntityFromSelectedSlice(treeItemId);
				}

				// Add to the data entity list
				if( imageDataEntity.IsNotNull() )
				{
					AddInformationToDataEntity(imageDataEntity, treeItemId);
					list->Add( imageDataEntity );
					list->GetSelectedDataEntityHolder()->SetSubject( imageDataEntity );
					// if series contain ECG data add it as child data entity
					if( ecgDataEntity.IsNotNull() )
					{
						ecgDataEntity->SetFather(imageDataEntity);
						list->Add( ecgDataEntity );
					}
				}
			}
		}
		else if(m_chbCombineData->GetValue() == true && treeItemIdsArray.GetCount()>1)
		{
			//take the first selected item
			wxTreeItemId treeItemId = treeItemIdsArray.Item(0);
			
			//checking if timepoint item was selected
			if(dynamic_cast<TimepointItemTreeData*>(m_treeDicomView->GetItemData(treeItemId)) != NULL)
			{	
				imageDataEntity = BuildCombinedImageDataEntityFromSelectedTimepoints(treeItemIdsArray);
			}
			
			//checking if a slice item was selected
			if ( dynamic_cast<SliceItemTreeData*>(m_treeDicomView->GetItemData(treeItemId)) != NULL ) 
			{
				wxMessageDialog dialog
					(
					NULL, wxT("Slices cannot be combined"),
					wxT("Error message"),
					wxICON_ERROR
					);
				dialog.ShowModal();
				return;
			}

			// Add to the data entity list
			if( imageDataEntity.IsNotNull() )
			{
				AddInformationToDataEntity(imageDataEntity, treeItemId);
				list->Add( imageDataEntity );
				list->GetSelectedDataEntityHolder()->SetSubject( imageDataEntity );
				// if series contain ECG data add it as child data entity
				if( ecgDataEntity.IsNotNull() )
				{
					ecgDataEntity->SetFather(imageDataEntity);
					list->Add( ecgDataEntity );
				}
			}
		}
	}
	coreCatchExceptionsReportAndNoThrowMacro(WorkingAreaPanelWidget::AddToDataList)
}

void WorkingAreaPanelWidget::OnTreeSelChanged(wxTreeEvent& event)
{	
	try
	{
		wxArrayTreeItemIds treeItemIdsArray;
		if(m_treeDicomView->GetSelections(treeItemIdsArray) < 1)
			return;

		// get the last selected tree item
		wxTreeItemId treeItemId = treeItemIdsArray.Last();

		//checking if we selected tree item is a slice item
		if(dynamic_cast<SliceItemTreeData*>(m_treeDicomView->GetItemData(treeItemId)) != NULL)
		{
			//render slice
			RenderDicomSlice(treeItemId);
		}
		else if(dynamic_cast<TimepointItemTreeData*>(m_treeDicomView->GetItemData(treeItemId)) != NULL)
		{	
			//render the first slice in this timepoint
			if(m_treeDicomView->GetChildrenCount(treeItemId) > 0)
			{
				//select the middle slice
				wxTreeItemIdValue cookie;
				int middleSlicePos = m_treeDicomView->GetChildrenCount(treeItemId)/2;
				wxTreeItemId sliceItemId = m_treeDicomView->GetFirstChild(treeItemId, cookie);
				while(middleSlicePos > 0 && sliceItemId.IsOk())
				{
					sliceItemId = m_treeDicomView->GetNextChild(treeItemId, cookie);
					middleSlicePos--;
				}
				RenderDicomSlice(sliceItemId);
			}
		}
		else if(dynamic_cast<PatientItemTreeData*>(m_treeDicomView->GetItemData(treeItemId)) != NULL)
		{
			//DisplayPatientInfo(treeItemId);
		}
	}
	coreCatchExceptionsReportAndNoThrowMacro(WorkingAreaPanelWidget::OnTreeSelChanged)
}

Core::DataEntity::Pointer 
DicomPlugin::WorkingAreaPanelWidget::BuildImageDataEntityFromSelectedSlice(
	wxTreeItemId sliceTreeItemId )
{
	Core::DataEntity::Pointer dataEntity = NULL;
	try
	{	
		int sliceNr(0), timepointNr(0);

		//get slice data
		SliceItemTreeData* sliceItemTreeData = (SliceItemTreeData*)m_treeDicomView->GetItemData(sliceTreeItemId);
		if(sliceItemTreeData == NULL)
			return NULL;
		sliceNr = atoi(sliceItemTreeData->GetSliceId().c_str());

		wxTreeItemId timepointTreeItemId = m_treeDicomView->GetItemParent(sliceTreeItemId);
		if(timepointTreeItemId.IsOk())
		{
			TimepointItemTreeData* timepointItemTreeData =
				(TimepointItemTreeData*)m_treeDicomView->GetItemData(timepointTreeItemId);
			if(timepointItemTreeData != NULL)
				timepointNr = atoi(timepointItemTreeData->GetTiempointId().c_str());
		}
		
		//read the image
		Core::vtkImageDataPtr vtkImage = Core::vtkImageDataPtr::New();
		itk::ImageIOBase::IOComponentType pixelType;
		pixelType = dcmAPI::ImageUtilities::ParsePixelType(sliceItemTreeData->GetPixelType());
		if(GetDataSet()->GetStorageType() == dcmAPI::DataSet::SINGLE_SLICE_PER_FILE)
		{
			vtkImage = Read2DVtkImageFromSlice(sliceItemTreeData->GetSlicePath(), pixelType);
		}
		else if(GetDataSet()->GetStorageType() == dcmAPI::DataSet::MULTI_SLICE_PER_FILE
			&& m_cachedSeriesCollectiveMap.size() > 0)
		{
			std::string seriesId = sliceItemTreeData->GetSeriesId();
			std::map<std::string,SeriesCollectivePtr>::iterator it = m_cachedSeriesCollectiveMap.find(seriesId);
			if(it != m_cachedSeriesCollectiveMap.end())
			{
				SeriesCollectivePtr seriesCollective = it->second;
				if(seriesCollective.get() != NULL)
				{
					if(timepointNr > 0 && timepointNr <= seriesCollective->m_images.size())
					{	
						//extract the slice from cached image
						vtkImage = Extract2DVtkImageFrom3DVtkImage
						(
						seriesCollective->m_images.at(timepointNr-1),
						sliceNr
						);
					}
				}
			}
		}
		
		//check if internal image was read properly
		if(vtkImage.GetPointer() == NULL)
			return NULL;

		//build data entity
		dataEntity = Core::DataEntity::New( Core::ImageTypeId );
		dataEntity->AddTimeStep( vtkImage );
		dataEntity->GetMetadata( )->SetName( sliceItemTreeData->GetSliceName() );

		//set modality
		if(timepointTreeItemId.IsOk())
		{
			wxTreeItemId seriesTreeId = m_treeDicomView->GetItemParent(timepointTreeItemId);
			if(seriesTreeId.IsOk())
			{
				std::string modality = GetModalityFromSeriesItem(seriesTreeId);
				SetModalityForDataEntity(dataEntity, modality);
			}
		}
	}
	coreCatchExceptionsReportAndNoThrowMacro(WorkingAreaPanelWidget::BuildDataEntityFromSelectedSlice)


	return dataEntity;
}

Core::DataEntity::Pointer 
DicomPlugin::WorkingAreaPanelWidget::BuildImageDataEntityFromSelectedTimepoint(
	wxTreeItemId timepointTreeItemId )
{
	this->SetCursor(wxCursor(wxCURSOR_WAIT));
	Core::DataEntity::Pointer dataEntity = NULL;
	Core::vtkImageDataPtr vtkImage = Core::vtkImageDataPtr::New();
	blSliceImage::Pointer sliceImage = blSliceImage::New();

	try
	{
		//read timepoint tree item data
		TimepointItemTreeData* timepointItemTreeData  =
			(TimepointItemTreeData*)m_treeDicomView->GetItemData(timepointTreeItemId);
		
		if(timepointItemTreeData == NULL)
			return NULL;
		
		//check if slices are parallel or not 
		dcmAPI::TimePoint::Pointer timepoint = GetDataSet()->GetTimePoint(
			timepointItemTreeData->GetPatientId(),
			timepointItemTreeData->GetStudyId(),
			timepointItemTreeData->GetSeriesId(),
			timepointItemTreeData->GetTiempointId()
			);

		if ( ( dcmAPI::IOUtils::TimepointContainsNonParallelSlices(timepoint) || m_chbExportRawData->GetValue() ) 
			 && m_cmbTimeTag->GetValue( ) == "Default" 
			 && m_chbTaggedMR->GetValue( ) == false )
		{
			sliceImage = CreateSliceImageFromTimepoint(timepointTreeItemId);
			
			if(sliceImage == NULL)
				return NULL;

			//build  a data entity from the volume
			dataEntity = Core::DataEntity::New( Core::SliceImageTypeId );
			dataEntity->GetMetadata()->SetName( "timepoint" + timepointItemTreeData->GetTiempointId() );
			dataEntity->AddTimeStep( sliceImage );
		}
		else
		{
			if(GetDataSet()->GetStorageType() == dcmAPI::DataSet::SINGLE_SLICE_PER_FILE)
			{
				//create list of slice filepath that belongs to this timepoint, create a 3d image and build data entity
				vtkImage = CreateVtkVolumeImageFromTimepoint(timepointTreeItemId);
			}
			else if(GetDataSet()->GetStorageType() == dcmAPI::DataSet::MULTI_SLICE_PER_FILE)
			{
				if(m_cachedSeriesCollectiveMap.size() > 0)
				{
					std::string seriesId = timepointItemTreeData->GetSeriesId();
					std::map<std::string,SeriesCollectivePtr>::iterator it = m_cachedSeriesCollectiveMap.find(seriesId);
					if(it != m_cachedSeriesCollectiveMap.end())
					{
						SeriesCollectivePtr seriesCollective = it->second;
						if(seriesCollective.get() != NULL)
						{
							int timepointNr = atoi( timepointItemTreeData->GetTiempointId().c_str() );
							if(timepointNr > 0 && timepointNr <= seriesCollective->m_images.size())
								vtkImage = seriesCollective->m_images.at(timepointNr-1);
						}
					}
				}
			}
			
			if(vtkImage == NULL)
				return NULL;

			//build  a data entity from the volume
			dataEntity = Core::DataEntity::New( Core::ImageTypeId );
			dataEntity->GetMetadata()->SetName( "timepoint" + timepointItemTreeData->GetTiempointId() );
			dataEntity->AddTimeStep( vtkImage );
		}

		wxTreeItemId seriesTreeItemId = m_treeDicomView->GetItemParent(timepointTreeItemId);
		if(seriesTreeItemId.IsOk())
		{
			std::string modality = GetModalityFromSeriesItem(seriesTreeItemId);
			SetModalityForDataEntity(dataEntity, modality);
		}
	}
	coreCatchExceptionsReportAndNoThrowMacro(WorkingAreaPanelWidget::BuildDataEntityFromSelectedTimepoint)

	this->SetCursor(wxCursor(wxCURSOR_ARROW));
	return dataEntity;
}

Core::DataEntity::Pointer 
WorkingAreaPanelWidget::BuildCombinedImageDataEntityFromSelectedTimepoints(
	wxArrayTreeItemIds& treeItemIdsArray)
{
	//! Set the cursor to wait mode
	this->SetCursor(wxCursor(wxCURSOR_WAIT));
	Core::DataEntity::Pointer dataEntity = NULL;
	Core::vtkImageDataPtr vtkImage = NULL;

	//! A vector for storing all the timepoints of the series
	VtkImageVector imageVector; 

	try
	{
		for(int i=0; i<treeItemIdsArray.GetCount(); i++)
		{
			wxTreeItemId treeItemId = treeItemIdsArray.Item(i);
			
			//chce if this is timepoint
			TimepointItemTreeData* timepointItemTreeData = dynamic_cast<TimepointItemTreeData*>(m_treeDicomView->GetItemData(treeItemId));
			if(timepointItemTreeData == NULL)
				continue;

			if(GetDataSet()->GetStorageType() == dcmAPI::DataSet::SINGLE_SLICE_PER_FILE)
			{
				//create list of slice filepath that belongs to this timepoint, create a 3d image and build data entity
				vtkImage = CreateVtkVolumeImageFromTimepoint(treeItemId);
			}
			else if(GetDataSet()->GetStorageType() == dcmAPI::DataSet::MULTI_SLICE_PER_FILE)
			{
				if(m_cachedSeriesCollectiveMap.size() > 0)
				{
					std::string seriesId = timepointItemTreeData->GetSeriesId();
					std::map<std::string,SeriesCollectivePtr>::iterator it = m_cachedSeriesCollectiveMap.find(seriesId);
					if(it != m_cachedSeriesCollectiveMap.end())
					{
						SeriesCollectivePtr seriesCollective = it->second;
						if(seriesCollective.get() != NULL)
						{
							int timepointNr = atoi( timepointItemTreeData->GetTiempointId().c_str() );
							if(timepointNr > 0 && timepointNr <= seriesCollective->m_images.size())
								vtkImage = seriesCollective->m_images.at(timepointNr-1);
						}
					}
				}
			}

			if(vtkImage.GetPointer() != NULL)
				imageVector.push_back(vtkImage);
		}

		wxTreeItemId seriesTreeItemId = m_treeDicomView->GetItemParent(treeItemIdsArray.Item(0));
		std::string modality;
		if(seriesTreeItemId.IsOk())
		{
			modality = GetModalityFromSeriesItem(seriesTreeItemId);
		}

		//! Read the series id
		std::string seriesId;
		SeriesItemTreeData* seriesDataItemTreeData = static_cast<SeriesItemTreeData*>(m_treeDicomView->GetItemData(seriesTreeItemId));

		if(seriesDataItemTreeData != NULL)
			seriesId = seriesDataItemTreeData->GetSeriesId();

		//! Create a 3D+time data entity
		dataEntity = Core::DataEntity::New( Core::ImageTypeId );
		dataEntity->GetMetadata()->SetName( "Series" + seriesId );
		dataEntity->AddTimeSteps( imageVector );

		//set modality
		SetModalityForDataEntity(dataEntity, modality);

	}
	coreCatchExceptionsReportAndNoThrowMacro(WorkingAreaPanelWidget::BuildCombinedDataEntityFromSelectedTimepoints)

	//! Set the cursor to normal mode
	this->SetCursor(wxCursor(wxCURSOR_ARROW));
	return dataEntity;
}
/**
Build a  3d+t data entity from the selected series, which contains all the timepoints in the series
*/
Core::DataEntity::Pointer 
DicomPlugin::WorkingAreaPanelWidget::BuildImageDataEntityFromSelectedSerie(
	wxTreeItemId seriesItemTreeItemId )
{
	//! Set the cursor to wait mode
	this->SetCursor(wxCursor(wxCURSOR_WAIT));
	Core::DataEntity::Pointer dataEntity = NULL;

	try
	{
		//! Read the series data tree item
		SeriesItemTreeData* seriesDataItemTreeData = static_cast<SeriesItemTreeData*>(m_treeDicomView->GetItemData(seriesItemTreeItemId));
		
		if(seriesDataItemTreeData == NULL)
			return NULL;

		//! A vector for storing all the timepoints of the series
		std::vector< Core::vtkImageDataPtr > imageVector; 
		std::vector< blSliceImage::Pointer > sliceImageVector;
		//! vector for storing timestamps values connected to images
		std::vector < float > timestampsVector;
		wxTreeItemIdValue cookie;

		//check if the slices ar parallel or not
		wxTreeItemId treeItemId = m_treeDicomView->GetFirstChild(seriesItemTreeItemId, cookie);
		if(!treeItemId.IsOk())
			return NULL;

		TimepointItemTreeData* timepointItemTreeData = (TimepointItemTreeData*)m_treeDicomView->GetItemData(treeItemId);
		if(timepointItemTreeData == NULL)
			return NULL;

		dcmAPI::TimePoint::Pointer timepoint = GetDataSet()->GetTimePoint(
			timepointItemTreeData->GetPatientId(),
			timepointItemTreeData->GetStudyId(),
			timepointItemTreeData->GetSeriesId(),
			timepointItemTreeData->GetTiempointId()
			);

		if ( ( dcmAPI::IOUtils::TimepointContainsNonParallelSlices(timepoint) || m_chbExportRawData->GetValue() ) 
			 && m_cmbTimeTag->GetValue( ) == "Default" 
			 && m_chbTaggedMR->GetValue( ) == false )
		{
			wxTreeItemId treeItemId = m_treeDicomView->GetFirstChild(seriesItemTreeItemId, cookie);
			while(treeItemId.IsOk())
				{
					TimepointItemTreeData* timepointItemTreeData = (TimepointItemTreeData*)m_treeDicomView->GetItemData(treeItemId);
					if(timepointItemTreeData != NULL)
					{
						blSliceImage::Pointer image = CreateSliceImageFromTimepoint(timepointItemTreeData->GetId());
						sliceImageVector.push_back(image);
					}
					treeItemId = m_treeDicomView->GetNextChild(seriesItemTreeItemId, cookie);
				}
			
			//build  a 3d + t data entity from the series
			dataEntity = Core::DataEntity::New( Core::SliceImageTypeId );
			dataEntity->AddTimeSteps( sliceImageVector );
			dataEntity->GetMetadata()->SetName( "Series" + seriesDataItemTreeData->GetSeriesId() );
		}
		else
		{
			if(GetDataSet()->GetStorageType() == dcmAPI::DataSet::SINGLE_SLICE_PER_FILE)
			{
				wxTreeItemIdValue cookie;
				wxTreeItemId treeItemId = m_treeDicomView->GetFirstChild(seriesItemTreeItemId, cookie);
				while(treeItemId.IsOk())
				{
					TimepointItemTreeData* timepointItemTreeData = (TimepointItemTreeData*)m_treeDicomView->GetItemData(treeItemId);
					if(timepointItemTreeData != NULL)
					{
						Core::vtkImageDataPtr vtkImage = CreateVtkVolumeImageFromTimepoint(timepointItemTreeData->GetId());
						imageVector.push_back(vtkImage);
					}
					treeItemId = m_treeDicomView->GetNextChild(seriesItemTreeItemId, cookie);
				}
			}
			else if(GetDataSet()->GetStorageType() == dcmAPI::DataSet::MULTI_SLICE_PER_FILE)
			{	
				if(m_cachedSeriesCollectiveMap.size() > 0)
				{
					std::string seriesId = seriesDataItemTreeData->GetSeriesId();
					std::map<std::string,SeriesCollectivePtr>::iterator it = m_cachedSeriesCollectiveMap.find(seriesId);
					if(it != m_cachedSeriesCollectiveMap.end())
					{
						SeriesCollectivePtr seriesCollective = m_cachedSeriesCollectiveMap.find(seriesDataItemTreeData->GetSeriesId())->second;
						if(seriesCollective.get() != NULL)
						{
							imageVector = seriesCollective->m_images;
							timestampsVector = seriesCollective->m_timestamps;
						}
					}
				}
			}
			
			//! Create a 3D+t data entity
			dataEntity = Core::DataEntity::New( Core::ImageTypeId );
			dataEntity->AddTimeSteps( imageVector );
			dataEntity->GetMetadata()->SetName( "Series" + seriesDataItemTreeData->GetSeriesId() );
			
			if(timestampsVector.size() > 0)
				dataEntity->SetTimeForAllTimeSteps( timestampsVector );

		}


		//set modality
		std::string modality = GetModalityFromSeriesItem(seriesItemTreeItemId);
		SetModalityForDataEntity(dataEntity, modality);
	}
	coreCatchExceptionsReportAndNoThrowMacro(WorkingAreaPanelWidget::BuildDataEntityFromSelectedSerie)

	//! Set the cursor to normal mode
	this->SetCursor(wxCursor(wxCURSOR_ARROW));
	return dataEntity;
}

dcmAPI::DataSet::Pointer DicomPlugin::WorkingAreaPanelWidget::GetDataSet()
{
	return m_DataSetProcessor->GetDataSet();
}

Core::BaseProcessor::Pointer DicomPlugin::WorkingAreaPanelWidget::GetProcessor()
{
	return Core::BaseProcessor::Pointer(m_DataSetProcessor);
}

bool DicomPlugin::WorkingAreaPanelWidget::Enable( bool enable /*= true */ )
{
	m_DicomMenuEventHandler->SetEvtHandlerEnabled( enable );
	return DicomWorkingAreaPanelWidgetUI::Enable( enable );
}

void DicomPlugin::WorkingAreaPanelWidget::OnInit()
{
}

void DicomPlugin::WorkingAreaPanelWidget::OnTimeTag( wxCommandEvent &event )
{
	
	if ( m_cmbTimeTag->GetValue() == "SOPInstanceUID" )
	{
		m_DataSetProcessor->SetTimeTag( dcmAPI::tags::SOPInstanceUID );
	}
	else if ( m_cmbTimeTag->GetValue() == "PhaseNumberMR" )
	{
		m_DataSetProcessor->SetTimeTag( dcmAPI::tags::PhaseNumberMR );
	}
	else if ( m_cmbTimeTag->GetValue() == "AcquisitionTime" )
	{
		m_DataSetProcessor->SetTimeTag( dcmAPI::tags::AcquisitionTime );
	}
	else
	{
		dcmAPI::TagId emptyTag;
		m_DataSetProcessor->SetTimeTag( emptyTag );
	}

}

/**
Create a vtk image from a given timepoint tree item
*/
Core::vtkImageDataPtr WorkingAreaPanelWidget::CreateVtkVolumeImageFromTimepoint
(
	wxTreeItemId timempointTreeItemId
)
{
	Core::vtkImageDataPtr vtkImage = Core::vtkImageDataPtr::New();

	unsigned int sliceCount = m_treeDicomView->GetChildrenCount(timempointTreeItemId);
	if(sliceCount > 0)
	{
		wxTreeItemIdValue cookie;
		std::vector< std::string > sliceFilePaths;
		int pos = sliceCount - 1;
		wxTreeItemId treeItemId = m_treeDicomView->GetFirstChild(timempointTreeItemId, cookie);

		//get pixel type
		SliceItemTreeData* sliceItemTreeData = (SliceItemTreeData*)m_treeDicomView->GetItemData(treeItemId);
		itk::ImageIOBase::IOComponentType pixelType;
		pixelType = dcmAPI::ImageUtilities::ParsePixelType(sliceItemTreeData->GetPixelType());

		while(treeItemId.IsOk())
		{
			sliceItemTreeData = (SliceItemTreeData*)m_treeDicomView->GetItemData(treeItemId);
			if(sliceItemTreeData != NULL)
			{
				sliceFilePaths.push_back( sliceItemTreeData->GetSlicePath() );
			}
			treeItemId = m_treeDicomView->GetNextChild(timempointTreeItemId, cookie);
		}
		if(sliceFilePaths.size() > 0)
		{

			switch ( pixelType )
			{
				blVtkTemplateMacro( 
					( vtkImage = dcmAPI::ImageUtilities::ReadMultiSliceVtkImageFromFiles< PixelT, 3 >( 
						sliceFilePaths, 
						m_chbReorientData->GetValue(), m_chbTaggedMR->GetValue( ) ) ) 
					);
			}

			//check if vtkImage is not null
			if(vtkImage == NULL)
				return NULL;
		}
	}
	
	return vtkImage;
}


/**
Create a slice image from a given timepoint tree item
*/
blSliceImage::Pointer WorkingAreaPanelWidget::CreateSliceImageFromTimepoint
(
	wxTreeItemId timempointTreeItemId
)
{
	blSliceImage::Pointer sliceImage = blSliceImage::New();

	unsigned int sliceCount = m_treeDicomView->GetChildrenCount(timempointTreeItemId);

	if(sliceCount > 0)
	{
		wxTreeItemIdValue cookie;
		std::vector< std::string > sliceFilePaths;
		std::vector< std::string > slicePositions;
		std::vector< std::string > sliceOrientations;
		double orientation[6];
		double position[3];

		int pos = sliceCount - 1;
		wxTreeItemId treeItemId = m_treeDicomView->GetFirstChild(timempointTreeItemId, cookie);

		//get pixel type
		SliceItemTreeData* sliceItemTreeData = (SliceItemTreeData*)m_treeDicomView->GetItemData(treeItemId);
		itk::ImageIOBase::IOComponentType pixelType;
		pixelType = dcmAPI::ImageUtilities::ParsePixelType(sliceItemTreeData->GetPixelType());

		while(treeItemId.IsOk())
		{
			sliceItemTreeData = (SliceItemTreeData*)m_treeDicomView->GetItemData(treeItemId);
			dcmAPI::Slice::Pointer slice = GetDataSet()->GetSlice(
						sliceItemTreeData->GetPatientId(),
						sliceItemTreeData->GetStudyId(),
						sliceItemTreeData->GetSeriesId(),
						sliceItemTreeData->GetTiempointId(),
						sliceItemTreeData->GetSliceId()
						);
			if(sliceItemTreeData != NULL)
			{
				sliceFilePaths.push_back( sliceItemTreeData->GetSlicePath() );
				slicePositions.push_back(slice->GetTagAsText(dcmAPI::tags::ImagePositionPatient));
				sliceOrientations.push_back(slice->GetTagAsText(dcmAPI::tags::ImageOrientationPatient));
			}
			treeItemId = m_treeDicomView->GetNextChild(timempointTreeItemId, cookie);
		}
		if(sliceFilePaths.size() > 0)
		{

			switch ( pixelType )
			{
				blVtkTemplateMacro( 
					( sliceImage = dcmAPI::ImageUtilities::ReadMultiSliceImageSliceFromFiles< PixelT >( sliceFilePaths ) ) 
					);
			}
			//check if sliceImage is not null
			if(sliceImage == NULL)
				return NULL;

			if (sliceFilePaths.size() == slicePositions.size() &&
				sliceOrientations.size() == slicePositions.size())
			for (size_t id = 0; id < sliceFilePaths.size(); id++)
			{
				dcmAPI::ImageUtilities::StringToDouble(sliceOrientations.at(id).c_str(),orientation, 6);
				dcmAPI::ImageUtilities::StringToDouble(slicePositions.at(id).c_str(),position, 3);
				//fill sliceImage with the orientation and the position
				sliceImage->SetSliceOrientation( orientation, id);
				sliceImage->SetSlicePosition( position, id);
			}
		}
	}
	
	return sliceImage;
}

void WorkingAreaPanelWidget::SetSpaceSliderPosition(wxTreeItemId sliceTreeItemId)
{
	try
	{	
		//find out how many slices are in the current timepoint and set the range
		wxTreeItemId timepointTreeItemId = m_treeDicomView->GetItemParent(sliceTreeItemId);
		unsigned int sliceCount = m_treeDicomView->GetChildrenCount(timepointTreeItemId);
		m_dcmSpaceSlider->SetRange(0, sliceCount-1);
		
		//set proper position of the space slider
		wxTreeItemIdValue cookie;
		int pos = 0;
		wxTreeItemId treeItemId = m_treeDicomView->GetFirstChild(timepointTreeItemId, cookie);
		while(treeItemId.IsOk())
		{
			if(treeItemId == sliceTreeItemId)
			{
				m_dcmSpaceSlider->SetValue(pos);
				break;
			}
			treeItemId = m_treeDicomView->GetNextChild(timepointTreeItemId, cookie);
			pos++;
		}
		
	}
	coreCatchExceptionsReportAndNoThrowMacro(WorkingAreaPanelWidget::SetSpaceSliderPosition)
}

void WorkingAreaPanelWidget::SetTimeSliderPosition(wxTreeItemId sliceTreeItemId)
{
	try
	{	
		//find out how many timepoints are in the current series and set the range
		wxTreeItemId timepointTreeItemId = m_treeDicomView->GetItemParent(sliceTreeItemId);
		wxTreeItemId serieTreeItemId = m_treeDicomView->GetItemParent(timepointTreeItemId);
		unsigned int timepointCount = m_treeDicomView->GetChildrenCount(serieTreeItemId, false);
		m_dcmTimeSlider->SetRange(0, timepointCount-1);
		
		//set proper position of the time slider
		wxTreeItemIdValue cookie;
		int pos = 0;
		wxTreeItemId treeItemId = m_treeDicomView->GetFirstChild(serieTreeItemId, cookie);
		while(treeItemId.IsOk())
		{
			if(treeItemId == timepointTreeItemId)
			{
				m_dcmTimeSlider->SetValue(pos);
				break;
			}
			treeItemId = m_treeDicomView->GetNextChild(serieTreeItemId, cookie);
			pos++;
		}
	}
	coreCatchExceptionsReportAndNoThrowMacro(WorkingAreaPanelWidget::SetTimeSliderPosition)
}

void WorkingAreaPanelWidget::SpaceSliderChanged()
{
	try
	{
		//get new position of the space slider
		int newSpacePos  = m_dcmSpaceSlider->GetValue();

		unsigned int sliceCount = m_treeDicomView->GetCurrentSelectedSliceCount();
		if ( sliceCount > 0 )
		{
			m_dcmSpaceSlider->SetRange(0, sliceCount-1);
		}

		bool selectionChanged;
		selectionChanged = m_treeDicomView->SelectItemByIndex( newSpacePos );
		if ( selectionChanged )
		{
			RenderDicomSlice( m_treeDicomView->GetCurrentSliceTreeItemId( ) );
		}
	}
	coreCatchExceptionsReportAndNoThrowMacro(WorkingAreaPanelWidget::SpaceSliderChanged)
}

void WorkingAreaPanelWidget::OnSpaceSliderChanged(wxCommandEvent& event)
{
	SpaceSliderChanged();
}

void WorkingAreaPanelWidget::OnTimeSliderChanged(wxCommandEvent& event)
{
	TimeSliderChanged();
}

void WorkingAreaPanelWidget::TimeSliderChanged()
{
	try
	{
		////get new position of the time slider and current position of the space slider
		int newTimeSliderPos  = m_dcmTimeSlider->GetValue();
		int spaceSliderPos  = m_dcmSpaceSlider->GetValue();

		bool selectionChanged;
		selectionChanged = m_treeDicomView->SelectItemByIndex( newTimeSliderPos, spaceSliderPos );
		if ( selectionChanged )
		{
			RenderDicomSlice( m_treeDicomView->GetCurrentSliceTreeItemId( ) );
		}

	}
	coreCatchExceptionsReportAndNoThrowMacro(WorkingAreaPanelWidget::TimeSliderChanged)

}

void WorkingAreaPanelWidget::RenderDicomSlice(wxTreeItemId sliceTreeItemId)
{
	try
	{
		m_treeDicomView->SetCurrentSliceTreeItemId( sliceTreeItemId );

		//create data entity from selected slice
		Core::DataEntity::Pointer dataEntity = BuildImageDataEntityFromSelectedSlice(sliceTreeItemId);
		if(dataEntity.IsNull())
			return;
		
		//set time slider position
		SetTimeSliderPosition(sliceTreeItemId);

		//set space slider position
		SetSpaceSliderPosition(sliceTreeItemId);
		
		//save current level window property
		m_RenderWindow->SaveCurrentLevelWindow();

		//set slice data in DataHolder
		m_dcmImageDataHolder->SetSubject(dataEntity);

		//set filename as a label for currently rendered slice
		SliceItemTreeData* sliceItemTreeData = (SliceItemTreeData*)m_treeDicomView->GetItemData(sliceTreeItemId);
		if(sliceItemTreeData != NULL)
			m_sliceName->SetLabel(_U(sliceItemTreeData->GetSlicePath()));

		dcmAPI::Slice::Pointer slice = GetDataSet()->GetSlice(
			sliceItemTreeData->GetPatientId(),
			sliceItemTreeData->GetStudyId(),
			sliceItemTreeData->GetSeriesId(),
			sliceItemTreeData->GetTiempointId(),
			sliceItemTreeData->GetSliceId()
			);
	}
	coreCatchExceptionsReportAndNoThrowMacro(WorkingAreaPanelWidget::RenderDicomSlice)
}

void WorkingAreaPanelWidget::LoadDefaultLevelWindow(wxTreeItemId sliceTreeItemId)
{
	try
	{
		SliceItemTreeData* sliceItemTreeData = (SliceItemTreeData*)m_treeDicomView->GetItemData(sliceTreeItemId);
		if(sliceItemTreeData == NULL)
			return;
		
		mitk::LevelWindow levelWindow;
		int center = atoi(sliceItemTreeData->GetWindowCenter().c_str());	
		int width = atoi(sliceItemTreeData->GetWindowWidth().c_str());
		if ( width == 0 )
		{
			return;
		}

		levelWindow.SetWindowBounds(center - width / 2.0, center + width / 2.0);
		levelWindow.SetRangeMinMax(center - width / 2.0, center + width / 2.0);
		levelWindow.SetDefaultRangeMinMax(center - width / 2.0, center + width / 2.0);
		levelWindow.SetDefaultLevelWindow(center, width);
		m_RenderWindow->GetLevelWindowManager()->SetLevelWindow( levelWindow );
	}
	coreCatchExceptionsReportAndNoThrowMacro(WorkingAreaPanelWidget::LoadDefaultLevelWindow)
}

void WorkingAreaPanelWidget::DoStartStep()
{
	try
	{
		wxTreeItemIdValue cookie;
		
		// expand first Patient->Study->Series
		wxTreeItemId timepointId = m_treeDicomView->GetFirstSpecifiedTreeItem( DICOMTree::TIMEPOINT, cookie, true);
		if(!timepointId.IsOk())
			return;

		// render the middle slice in the first timepoint
		int middleSlicePos = m_treeDicomView->GetChildrenCount(timepointId)/2;
		wxTreeItemId sliceId = m_treeDicomView->GetFirstChild(timepointId, cookie);
		while(middleSlicePos > 0 && sliceId.IsOk())
		{
			sliceId = m_treeDicomView->GetNextChild(timepointId, cookie);
			middleSlicePos--;
		}

		//wxTreeItemId sliceId = m_treeDicomView->GetFirstChild(timepointId, cookie);
		if(!sliceId.IsOk())
			return;

		//select a slice and render it
		m_treeDicomView->UnselectAll();
		m_treeDicomView->SelectItem(timepointId);
		//m_treeDicomView->SelectItemByIndex(sliceId);
		//LoadDefaultLevelWindow(sliceId);
		m_RenderWindow->SetUseFixedLevelWindow(false);
		RenderDicomSlice(sliceId);
		m_RenderWindow->SetUseFixedLevelWindow(true);
	}
	coreCatchExceptionsReportAndNoThrowMacro(WorkingAreaPanelWidget::DoStartStep)
}


//void WorkingAreaPanelWidget::DisplayPatientInfo(wxTreeItemId patientTreeItemId)
//{
//	//get patient tree item data
//	PatientItemTreeData* patientItemTreeData = (PatientItemTreeData*)m_treeDicomView->GetItemData(patientTreeItemId);
//	if(patientItemTreeData == NULL)
//		return;
//
//	//read patient object
//	dcmAPI::Patient::Pointer dcmPatient = dcmData->GetPatient(patientItemTreeData->GetPatientId());
//	if(dcmPatient == NULL)
//		return;
//
//	dcmPatient->UpdateTagsMap();
//	//std::string tag = dcmPatient->GetTagAsText(dcmAPI::tags::PatientAge); 
//}

std::string WorkingAreaPanelWidget::GetModalityFromSeriesItem(wxTreeItemId seriesItemTreeId)
{
	std::string modality = "";
	if(seriesItemTreeId.IsOk())
	{
		SeriesItemTreeData* seriesItemTreeData =
			(SeriesItemTreeData*)m_treeDicomView->GetItemData(seriesItemTreeId);
		if(seriesItemTreeData != NULL)
		{
			dcmAPI::Series::Pointer seriesData = GetDataSet()->GetSeries
				(
				seriesItemTreeData->GetPatientId(),
				seriesItemTreeData->GetStudyId(),
				seriesItemTreeData->GetSeriesId()
				);
			modality = seriesData->GetTagAsText(dcmAPI::tags::Modality);
		}
	}
	return modality;
}

void WorkingAreaPanelWidget::SetModalityForDataEntity
(
 Core::DataEntity::Pointer dataEntity,
 const std::string& modality
)
{
	if(modality == "MR")
		dataEntity->GetMetadata()->SetModality(Core::MRI);
	else if(modality == "CT")
		dataEntity->GetMetadata()->SetModality(Core::CT);
	else if(modality == "NM")
		dataEntity->GetMetadata()->SetModality(Core::NM);
	else if(modality == "US")
		dataEntity->GetMetadata()->SetModality(Core::US);
	else if(modality == "XA")
		dataEntity->GetMetadata()->SetModality(Core::XA);
	else
		dataEntity->GetMetadata()->SetModality(Core::UnknownModality);
}


Core::vtkImageDataPtr WorkingAreaPanelWidget::Read2DVtkImageFromSlice
(
 std::string filePath,
 itk::ImageIOBase::IOComponentType pixelType
 )
{
	Core::vtkImageDataPtr vtkImage = Core::vtkImageDataPtr::New();
	try
	{
		switch(pixelType)
		{
			blVtkTemplateMacro(
				( vtkImage = dcmAPI::ImageUtilities::Read2DVtkImageFromFile< PixelT>( filePath.c_str() ) ) );
		}

	}
	coreCatchExceptionsReportAndNoThrowMacro(WorkingAreaPanelWidget::Read2DVtkImageFromSlice)
	return vtkImage;
}

Core::vtkImageDataPtr WorkingAreaPanelWidget::Extract2DVtkImageFrom3DVtkImage(
	Core::vtkImageDataPtr threeDInputImage, 
	int sliceNumber)
{
	Core::vtkImageDataPtr twoDVtkImage = Core::vtkImageDataPtr::New();
	try
	{
		// IMAGE RESLICE filter -------------------------
		//this crashes with spacing < 0
		/*vtkImageReslice* t = vtkImageReslice::New();
		t->SetInput(threeDInputImage.get());
		t->SetOutputDimensionality(2);

		double spacing[3], origin[3];
		threeDInputImage->GetSpacing(spacing);
		threeDInputImage->GetOrigin(origin);

		double cosines[] = {1, 0, 0, 0, 1, 0};
		double x[3], y[3], z[3];
		x[0] = 1; x[1] = 0; x[2] = 0;
		y[0] = 0; y[1] = 1; y[2] = 0;
		z[0] = x[1]*y[2] - x[2]*y[1];
		z[1] = x[2]*y[0] - x[0]*y[2];
		z[2] = x[0]*y[1] - x[1]*y[0];

		spacing[2] = 0.8;
		double slicePos = origin[2] + sliceNumber*(spacing[2]);
		t->SetResliceAxesDirectionCosines(x, y, z);
		t->SetResliceAxesOrigin(0,0, sliceNumber);
		t->Update();
		twoDVtkImage->DeepCopy(t->GetOutput());*/
		//-------------------------------------------------------------------
		
		//IMAGE CLIP---------------------------------------------------------
		// this works with spacing < 0
		vtkSmartPointer < vtkImageClip > imageClip = vtkSmartPointer< vtkImageClip >::New() ;
		int dim[3];
		threeDInputImage->GetDimensions(dim);
		imageClip->SetInput(threeDInputImage);
		imageClip->ClipDataOn();
		imageClip->SetOutputWholeExtent(0, dim[0], 0, dim[1], sliceNumber-1, sliceNumber-1);
		//imageClip->SetOutputWholeExtent(0, dim[0], sliceNumber-1, sliceNumber-1, 0, dim[2]);
		//imageClip->SetOutputWholeExtent(sliceNumber-1,sliceNumber-1, 0, dim[1], 0, dim[2]);
		imageClip->Update();
		twoDVtkImage->ShallowCopy(imageClip->GetOutput());
		//-------------------------------------------------------------------
	}
	coreCatchExceptionsReportAndNoThrowMacro(WorkingAreaPanelWidget::Extract2DVtkImageFrom3DVtkImage)
	return twoDVtkImage;
}

void WorkingAreaPanelWidget::ClearCachedObjects()
{
	try
	{
		m_cachedSeriesCollectiveMap.clear();
	}
	coreCatchExceptionsReportAndNoThrowMacro( WorkingAreaPanelWidget::ClearCachedObjects )
}

SeriesCollectivePtr WorkingAreaPanelWidget::BuildSeriesCollectiveFromSeriesDataset(std::string filePath)
{
	SeriesCollectivePtr seriesCollective(new SeriesCollective());
	try
	{
		dcmAPI::MultiSliceReader::Pointer reader = dcmAPI::MultiSliceReader::New();
		dcmAPI::StandardImageReader::Pointer standardReaderPtr( new dcmAPI::StandardImageReader() );
		reader->AddReader( standardReaderPtr );
		reader->SetPath( filePath );
		reader->SetDataSet( GetDataSet() );
		reader->Update( );

		seriesCollective->m_images = reader->GetOutput( );
		seriesCollective->m_timestamps = reader->GetVolumesTimestamps( );
		seriesCollective->m_signalCollective = reader->GetSignalCollectiveECG( );
	}
	coreCatchExceptionsReportAndNoThrowMacro( WorkingAreaPanelWidget::BuildSeriesCollectiveFromSeriesDataset )

	return seriesCollective;
}

Core::DataEntity::Pointer  WorkingAreaPanelWidget::BuildEcgDataEntityFromSelectedSerie(wxTreeItemId seriesItemTreeItemId)
{
	Core::DataEntity::Pointer ecgDataEntity = NULL;
	try
	{
		if(m_cachedSeriesCollectiveMap.size() > 0)
		{
			//! Read the series data tree item
			SeriesItemTreeData* seriesDataItemTreeData = static_cast<SeriesItemTreeData*>(m_treeDicomView->GetItemData(seriesItemTreeItemId));
			if(seriesDataItemTreeData == NULL)
				return NULL;

			std::string seriesId = seriesDataItemTreeData->GetSeriesId();
			std::map<std::string,SeriesCollectivePtr>::iterator it = m_cachedSeriesCollectiveMap.find(seriesId);
			if(it != m_cachedSeriesCollectiveMap.end())
			{
				SeriesCollectivePtr seriesCollective = it->second;
				if(seriesCollective.get() != NULL)
				{
					if(seriesCollective->m_signalCollective->GetNumberOfSignals() > 0 )
					{
						ecgDataEntity = Core::DataEntity::New( Core::SignalTypeId );
						ecgDataEntity->GetMetadata()->SetName( "ECG" );
						ecgDataEntity->AddTimeStep( seriesCollective->m_signalCollective );
					}
				}
			}
		}	
	}
	coreCatchExceptionsReportAndNoThrowMacro( WorkingAreaPanelWidget::BuildEcgDataEntityFromSelectedSerie )

	return ecgDataEntity;
}

//!
void WorkingAreaPanelWidget::AddInformationToDataEntity(Core::DataEntity::Pointer dataEntity, wxTreeItemId treeItemId)
{
	try
	{
		std::ostringstream name;
		name << dataEntity->GetMetadata()->GetName() + "_" << m_DataEntityCounter++;
		dataEntity->GetMetadata()->SetName( name.str( ) );

		PatientItemTreeData* patientData = m_treeDicomView->GetConnectedPatientTreeItem(treeItemId);
		if(patientData != NULL)
		{
			blTagMap::Pointer patientMetaData = dataEntity->GetMetadata()->GetTagValue<blTagMap::Pointer>( "Patient" );
			if ( patientMetaData.IsNull() )
			{
				patientMetaData = blTagMap::New( );
				dataEntity->GetMetadata()->AddTag( "Patient", patientMetaData );
			}

			patientMetaData->AddTag( "Patient name", patientData->GetPatientName() );
			patientMetaData->AddTag( "Patient sex", patientData->GetPatientSex() );
			std::string formatedData = dcmAPI::CreateDateFromRawDcmTagDate(patientData->GetPatientBirthDate() );
			patientMetaData->AddTag( "Patient birth date", formatedData );
		}
	}
	coreCatchExceptionsReportAndNoThrowMacro( WorkingAreaPanelWidget::AddInformationToDataEntity )
}
