/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef DicomWorkingAreaPanelWidget_H
#define DicomWorkingAreaPanelWidget_H

#include "DicomWorkingAreaPanelWidgetUI.h"
#include "DicomPluginTypes.h"
#include "DicomProcessorCollective.h"

#include "blSignalCollective.h"
#include "blSliceImage.h"

#include "CILabNamespaceMacros.h"
#include "coreDataHolder.h"
#include "coreDataEntity.h"
#include "coreVTKImageDataHolder.h"
#include "coreReportExceptionMacros.h"
#include "coreProcessingWidget.h"

#include "dcmDataSet.h"
#include "dcmTypes.h"

#include "itkImageSeriesReader.h"
#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"

#include "vtkSmartPointer.h"
#include "vtkImageFlip.h"


namespace DicomPlugin{

	class DicomMenuEventHandler;


/** 
A class that defines all the graphical interface and user interaction with working area of Dicom Plugin

\ingroup DicomPlugin
\author Jakub Lyko
\date 07 April 2008
*/
class WorkingAreaPanelWidget : 
	public DicomWorkingAreaPanelWidgetUI,
	public Core::Widgets::ProcessingWidget
{
public:
	coreDefineBaseWindowFactory( WorkingAreaPanelWidget )

	WorkingAreaPanelWidget(wxWindow* parent, int id, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=0);

	//!
	~WorkingAreaPanelWidget( );

	//!
	void SpaceSliderChanged();

	//!
	void TimeSliderChanged();

	//!
	void SetProcessorCollective(ProcessorCollective::Pointer val);

	//!
	Core::BaseProcessor::Pointer GetProcessor( );

	//!
	bool Enable(bool enable = true );

private:

	//!
	void OnTimeTag(wxCommandEvent &event);

	//!
	void OnInit( );

	//! add selected tree data to GIMIAS data list
	void AddToDataList(wxCommandEvent& event);

	//! 
	void OnTreeSelChanged(wxTreeEvent& event);

	//! 
	void OnSpaceSliderChanged(wxCommandEvent& event);

	//!
	void OnTimeSliderChanged(wxCommandEvent& event);
	
	//!
	void AddInformationToDataEntity(Core::DataEntity::Pointer dataEntity, wxTreeItemId treeItemId);

	//!
	void OnModifiedDataSet( );

	//!
	dcmAPI::DataSet::Pointer GetDataSet( );

	//! apply level window that is stored in dicom file
	void LoadDefaultLevelWindow(wxTreeItemId sliceTreeItemId);

	//!
	void SetSpaceSliderPosition(wxTreeItemId sliceTreeItemId);

	//!
	void SetTimeSliderPosition(wxTreeItemId sliceTreeItemId);

	//!
	void RenderDicomSlice(wxTreeItemId sliceTreeItemId);

	//! expand the tree to the default position, and render a slice
	void DoStartStep();

	//! Create a volume image from a given timepoint tree item
	Core::vtkImageDataPtr CreateVtkVolumeImageFromTimepoint( wxTreeItemId timempointTreeItemId );
	
	//! Create a slice image from a given timepoint tree item
	blSliceImage::Pointer CreateSliceImageFromTimepoint( wxTreeItemId timempointTreeItemId);

	//! Build 2d image data entity from selected  slice
	Core::DataEntity::Pointer BuildImageDataEntityFromSelectedSlice(wxTreeItemId sliceTreeItemId);

	//! Build 3d image data entity from selected timepont
	Core::DataEntity::Pointer BuildImageDataEntityFromSelectedTimepoint(wxTreeItemId tiempointTreeItemId);

	//! Build a  3d + time image data entity from selected timepoints
	Core::DataEntity::Pointer BuildCombinedImageDataEntityFromSelectedTimepoints(wxArrayTreeItemIds& treeItemIdsArray);

	//! 
	//void DisplayPatientInfo(wxTreeItemId patientTreeItemId);

	//! Build a image 3d+time data entity from the selected serie, which contains all the timepoints in the serie
	Core::DataEntity::Pointer BuildImageDataEntityFromSelectedSerie(wxTreeItemId seriesItemTreeItemId);

	//! Build a ECG data entity 
	Core::DataEntity::Pointer BuildEcgDataEntityFromSelectedSerie(wxTreeItemId seriesItemTreeItemId);

	//!
	SeriesCollectivePtr BuildSeriesCollectiveFromSeriesDataset(std::string filePath);

	//!
	std::string GetModalityFromSeriesItem(wxTreeItemId seriesItemTreeItemId);

	//!
	void SetModalityForDataEntity(Core::DataEntity::Pointer dataEntity, const std::string& modality);

	//!
	Core::vtkImageDataPtr Read2DVtkImageFromSlice(
		std::string filePath , 
		itk::ImageIOBase::IOComponentType pixelType);

	//!
	Core::vtkImageDataPtr Extract2DVtkImageFrom3DVtkImage(
		Core::vtkImageDataPtr threeDInputImage, 
		int sliceNumber);

	//!
	blSignal::Pointer CreateSignalFromEcgData(
		std::vector< float > ecgSamples,
		double ecgStartTime,
		double ecgIncrementTime,
		int scaleEcgSamplesByValue = 1
		);

	//!
	void ClearCachedObjects();

public:
	//! Holds the dicom Image that is currently selected.
	Core::DataEntityHolder::Pointer m_dcmImageDataHolder;

private:
	//!
	std::map <std::string, SeriesCollectivePtr> m_cachedSeriesCollectiveMap;

	//!
	DataSetProcessor::Pointer m_DataSetProcessor;

	//! Counter to append to data entity name
	static int m_DataEntityCounter;

	//!
	DicomMenuEventHandler *m_DicomMenuEventHandler;

	DECLARE_EVENT_TABLE()
};

} // DicomPlugin

#endif //DicomWorkingAreaPanelWidget_H
