/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "DicomConnectToPacsDialogWidget.h"
#include "DicomWorkingAreaPanelWidget.h"
#include "DicomPluginWidgetCollective.h"
#include "DicomMenuEventHandler.h"

#include "blMitkUnicode.h"
#include "blTextUtils.h"

#include "coreWxMitkCoreMainWindow.h"
#include "coreWxMitkGraphicalInterface.h"
#include "coreDirectory.h"
#include "coreSettings.h"
#include "coreKernel.h"
#include "corePluginTab.h"
#include "coreMainMenu.h"
#include "coreToolbarIO.h"
#include "corePluginTab.h"
#include "corePluginTabFactory.h"

#include "pacsAPI.h"

#include <wx/event.h>
#include <wx/process.h>

#include "dcmTypes.h"
#include "dcmPACSQueryFileReader.h"

#include "PACS.xpm"

using namespace DicomPlugin;

// Event the widget
BEGIN_EVENT_TABLE(ConnectToPacsDialogWidget, DicomConnectToPacsDialogWidgetUI)
	EVT_BUTTON(wxID_DICOM_OPEN_DIRECTORY_FOR_SAVING_PACS_DATA, ConnectToPacsDialogWidget::OpenDirectoryForSavingPacsData)
	EVT_BUTTON(wxID_DICOM_QUERY, ConnectToPacsDialogWidget::OnQuery)
	EVT_BUTTON(wxID_DICOM_RETRIEVE, ConnectToPacsDialogWidget::OnRetrieve)
	EVT_BUTTON(wxID_DICOM_SEND, ConnectToPacsDialogWidget::OnSendToPacs)
	EVT_BUTTON(wxID_DICOM_SAVECONFIG, ConnectToPacsDialogWidget::OnSaveConfig)
	EVT_TEXT_ENTER(wxID_DICOM_PATIENT_NAME, ConnectToPacsDialogWidget::OnPatientNameTextEnter)
	EVT_CHECKBOX(wxID_DICOM_PATIENT_DATE_OF_BIRTH, ConnectToPacsDialogWidget::OnChkDateOfBirth)
	EVT_CHECKBOX(wxID_DICOM_PATIENT_DATE_OF_BIRTH_FROM, ConnectToPacsDialogWidget::OnChkDateOfBirth)
	EVT_CHECKBOX(wxID_DICOM_PATIENT_DATE_OF_BIRTH_TO, ConnectToPacsDialogWidget::OnChkDateOfBirth)
END_EVENT_TABLE()

ConnectToPacsDialogWidget::ConnectToPacsDialogWidget( wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style)
: DicomConnectToPacsDialogWidgetUI(parent, id, pos, size, style)
{

	Core::Runtime::Settings::Pointer settings = Core::Runtime::Kernel::GetApplicationSettings();
	m_edSaveToDirectory->SetValue( _U(settings->GetDataSourcePath()  + "/" ) );

	std::list<std::string> list;
	std::string settingsPacsCalledAE(settings->GetPacsCalledAE());
	blTextUtils::ParseLine( settingsPacsCalledAE, ':', list );
	if ( list.size() )
	{
		m_edPACSCalledAETitle->SetValue( list.front() );
		list.pop_front();
	}
	if ( list.size() )
	{
		m_edPACSCalledAEIP->SetValue( list.front() );
		list.pop_front();
	}
	if ( list.size() )
	{
		m_edPACSCalledAEPort->SetValue( list.front() );
		list.pop_front();
	}

	list.clear();
	std::string settingsPacsCallingAE(settings->GetPacsCallingAE());
	blTextUtils::ParseLine( settingsPacsCallingAE, ':', list );
	if ( list.size() )
	{
		m_edPACSCallingAETitle->SetValue( list.front() );
		list.pop_front();
	}
	if ( list.size() )
	{
		m_edPACSCallingAEIP->SetValue( list.front() );
		list.pop_front();
	}
	if ( list.size() )
	{
		m_edPACSCallingAEPortRetrieve->SetValue( list.front() );
		list.pop_front();
	}
	if ( list.size() )
	{
		m_edPACSCallingAEPortSend->SetValue( list.front() );
		list.pop_front();
	}

	
	//a test to see if the parse works
	//std::string callingAE=settings->GetPacsCallingAE();
	//std::string calledAE=settings->GetPacsCalledAE();
	//ParseAEInfo(calledAE,true);
	//ParseAEInfo(callingAE,false);

	//callingAE="Title1";
	//calledAE="Title2";
	//ParseAEInfo(calledAE,true);
	//ParseAEInfo(callingAE,false);
	//std::cout << m_edPACSCalledAEIP->GetValue().c_str()<<std::endl;
	//std::cout << m_edPACSCallingAEIP->GetValue().c_str()<<std::endl;
	//std::cout << m_edPACSCalledAETitle->GetValue().c_str()<<std::endl;
	//std::cout << m_edPACSCallingAETitle->GetValue().c_str()<<std::endl;
	//std::cout << m_edPACSCalledAEPort->GetValue().c_str()<<std::endl;
	//std::cout << m_edCallingAEPort->GetValue().c_str()<<std::endl;

	//callingAE="Title1:124.5.5.2";
	//calledAE="Title2:123.2.2.2";
	//ParseAEInfo(calledAE,true);
	//ParseAEInfo(callingAE,false);
	//std::cout << m_edPACSCalledAEIP->GetValue().c_str()<<std::endl;
	//std::cout << m_edPACSCallingAEIP->GetValue().c_str()<<std::endl;
	//std::cout << m_edPACSCalledAETitle->GetValue().c_str()<<std::endl;
	//std::cout << m_edPACSCallingAETitle->GetValue().c_str()<<std::endl;
	//std::cout << m_edPACSCalledAEPort->GetValue().c_str()<<std::endl;
	//std::cout << m_edCallingAEPort->GetValue().c_str()<<std::endl;


	//callingAE="Title1:124.5.5.2:1233";
	//calledAE="Title2:123.2.2.2:1212";
	//ParseAEInfo(calledAE,true);
	//ParseAEInfo(callingAE,false);
	//std::cout << m_edPACSCalledAEIP->GetValue().c_str()<<std::endl;
	//std::cout << m_edPACSCallingAEIP->GetValue().c_str()<<std::endl;
	//std::cout << m_edPACSCalledAETitle->GetValue().c_str()<<std::endl;
	//std::cout << m_edPACSCallingAETitle->GetValue().c_str()<<std::endl;
	//std::cout << m_edPACSCalledAEPort->GetValue().c_str()<<std::endl;
	//std::cout << m_edCallingAEPort->GetValue().c_str()<<std::endl;

	//callingAE="Title1:124.5.5.2:1233:other";
	//calledAE="Title2:123.2.2.2:1212:other";
	//ParseAEInfo(calledAE,true);
	//ParseAEInfo(callingAE,false);
	//std::cout << m_edPACSCalledAEIP->GetValue().c_str()<<std::endl;
	//std::cout << m_edPACSCallingAEIP->GetValue().c_str()<<std::endl;
	//std::cout << m_edPACSCalledAETitle->GetValue().c_str()<<std::endl;
	//std::cout << m_edPACSCallingAETitle->GetValue().c_str()<<std::endl;
	//std::cout << m_edPACSCalledAEPort->GetValue().c_str()<<std::endl;
	//std::cout << m_edCallingAEPort->GetValue().c_str()<<std::endl;

	UpdateWidget();
}

void ConnectToPacsDialogWidget::OpenDirectoryForSavingPacsData(wxCommandEvent& event)
{
	try
	{
		Core::Runtime::wxMitkGraphicalInterface::Pointer gIface = Core::Runtime::Kernel::GetGraphicalInterface();
		coreAssertMacro(gIface.IsNotNull());

		Core::Runtime::Settings::Pointer settings = Core::Runtime::Kernel::GetApplicationSettings();
		std::string dataPath = settings->GetDataSourcePath();
		wxDirDialog* openDirectoryDialog = new wxDirDialog
			(
			this,
			wxT("Open Dicom directory"),
			wxT(""),
			wxDD_DIR_MUST_EXIST
			);
		
		openDirectoryDialog->SetPath( m_edSaveToDirectory->GetValue() );
		if(openDirectoryDialog->ShowModal() == wxID_OK)
		{
			std::string dirPath(openDirectoryDialog->GetPath().mb_str(wxConvUTF8));
			if(dirPath.compare("") == 0)
			{
				throw Core::Exceptions::Exception(
					"ConnectToPacsDialogWidget::OpenDirectoryForSavingPacsData",
					"Provide a valid directory path");
			}
			m_edSaveToDirectory->SetValue(wxString::FromAscii(dirPath.c_str()) + wxT("/") );
		}
		delete openDirectoryDialog;
	}
	coreCatchExceptionsReportAndNoThrowMacro(ConnectToPacsDialogWidget::OpenDirectoryForSavingPacsData)
}

void ConnectToPacsDialogWidget::OnQuery(wxCommandEvent& event)
{
	wxBusyInfo info(wxT("Querying data, please wait..."), this);
	PACS::PacsAPI* pacsAPI = NULL;
	try
	{

		pacsAPI = PACS::CreateObjectOfPacsAPI( );

		// Connect to server
		PACS::NetworkParams networkParams;
		FillNetworkParams( networkParams );
		pacsAPI->SetNetworkParams( networkParams );

		// Create query
		PACS::QueryParams queryParams;
		queryParams.retrieveLevel = PACS::SERIES;
		FillQueryParamsFromFields( queryParams );

		// Add other params to retrieve the information
		queryParams.query.push_back( dcmAPI::Tag( dcmAPI::tags::StudyDate ) );
		queryParams.query.push_back( dcmAPI::Tag( dcmAPI::tags::StudyTime ) );
		queryParams.query.push_back( dcmAPI::Tag( dcmAPI::tags::AccessionNumber ) );
		queryParams.query.push_back( dcmAPI::Tag( dcmAPI::tags::StudyRealID ) );
		queryParams.query.push_back( dcmAPI::Tag( dcmAPI::tags::StudyInstanceUID ) );
		queryParams.query.push_back( dcmAPI::Tag( dcmAPI::tags::StudyDescription ) );
		queryParams.query.push_back( dcmAPI::Tag( dcmAPI::tags::SeriesNumber ) );
		queryParams.query.push_back( dcmAPI::Tag( dcmAPI::tags::SeriesInstanceUID ) );
		//queryParams.query.push_back( dcmAPI::Tag( dcmAPI::tags::Modality ) );
		queryParams.query.push_back( dcmAPI::Tag( dcmAPI::tags::SeriesDescription ) );
		pacsAPI->SetQueryParams( queryParams );

		// Do query
		pacsAPI->Query();

		// Fill DICOM Tree
		FillQueryResults( pacsAPI );

	}
	coreCatchExceptionsReportAndNoThrowMacro(ConnectToPacsDialogWidget::OnRetrieve)

	PACS::DestroyObjectOfPacsAPI( pacsAPI );

	SetFocus( );
}

void ConnectToPacsDialogWidget::OnRetrieve(wxCommandEvent& event)
{
	wxBusyInfo info(wxT("Downloading data, please wait..."), this);
	PACS::PacsAPI* pacsAPI = NULL;
	try
	{
		pacsAPI = PACS::CreateObjectOfPacsAPI( );

		// Connect to server
		PACS::NetworkParams networkParams;
		FillNetworkParams( networkParams );
		pacsAPI->SetNetworkParams( networkParams );

		// Create query

		PACS::QueryParams queryParams;
		
		queryParams.retrieveLevel = PACS::SERIES;
		FillQueryParamsFromTree(queryParams);
		
		//queryParams.retrieveLevel = PACS::PATIENT;
		//FillQueryParams( queryParams );
		pacsAPI->SetQueryParams( queryParams );

		// Move params
		PACS::MoveParams moveParams;
		moveParams.retriveport=networkParams.clientPortRetrieve;
		moveParams.outputStorageFolder = (const char *)m_edSaveToDirectory->GetValue().c_str();
		Core::IO::Directory::Pointer homeDir;
		// Test if exists folder + create it if don't
		homeDir = Core::IO::Directory::New();
		homeDir->SetDirNameFullPath(moveParams.outputStorageFolder);
		homeDir->Create();
		pacsAPI->SetMoveParams( moveParams );

		// Do retrieve
		pacsAPI->Move();

		// Open the output data in the Dicom Plugin
		m_DataSetProcessor->SetPath( pacsAPI->GetMoveResultsFolder() );
		m_DataSetProcessor->Update( );

	}
	coreCatchExceptionsReportAndNoThrowMacro(ConnectToPacsDialogWidget::OnRetrieve)

	PACS::DestroyObjectOfPacsAPI( pacsAPI );

	SetFocus();
}

void DicomPlugin::ConnectToPacsDialogWidget::FillQueryResults( 
	PACS::PacsAPI* pacsAPI )
{

	dcmAPI::PACSQueryFileReader::Pointer reader = dcmAPI::PACSQueryFileReader::New();
	reader->SetDataSet( m_DICOMTree->GetDcmData() );
	reader->SetPath( pacsAPI->GetQueryResultsFileName() );
	reader->SetQuery( pacsAPI->GetQueryParams().query );
	reader->Update( );

	// Update the TREE
	m_DICOMTree->LoadPatientsIntoTree( );
	// expand first Patient->Study->Series
	wxTreeItemIdValue cookie;
	wxTreeItemId timepointId = m_DICOMTree->GetFirstSpecifiedTreeItem( 
		DICOMTree::TIMEPOINT, 
		cookie, 
		true);

}

DicomPlugin::DataSetProcessor::Pointer 
DicomPlugin::ConnectToPacsDialogWidget::GetDataSetProcessor() const
{
	return m_DataSetProcessor;
}

void DicomPlugin::ConnectToPacsDialogWidget::OnPatientNameTextEnter(
	wxCommandEvent& WXUNUSED(event))
{
	wxCommandEvent event;
	OnQuery( event );
}

void DicomPlugin::ConnectToPacsDialogWidget::OnChkDateOfBirth( wxCommandEvent& event )
{
	UpdateWidget();
}

void DicomPlugin::ConnectToPacsDialogWidget::UpdateWidget()
{
	m_dateBirth->Enable( m_chkDateOfBirth->IsChecked() );
	m_dateBirth_From->Enable( m_chkDateOfBirthFrom->IsChecked() );
	m_dateBirth_To->Enable( m_chkDateOfBirthTo->IsChecked() );
}

void DicomPlugin::ConnectToPacsDialogWidget::FillNetworkParams( 
	PACS::NetworkParams &networkParams )
{
	networkParams.CalledAETitle = m_edPACSCalledAETitle->GetValue( ).c_str( );
	networkParams.CallingAETitle = m_edPACSCallingAETitle->GetValue( ).c_str( );
	networkParams.serverIP = m_edPACSCalledAEIP->GetValue( ).c_str( );
	networkParams.clientIP = m_edPACSCallingAEIP->GetValue( ).c_str( );
	networkParams.serverPort = atoi( m_edPACSCalledAEPort->GetValue( ).c_str( ) );
	networkParams.clientPortSend = atoi( m_edPACSCallingAEPortSend->GetValue( ).c_str( ) );
	networkParams.clientPortRetrieve = atoi( m_edPACSCallingAEPortRetrieve->GetValue( ).c_str( ) );
	networkParams.timeout = 30;
}

void DicomPlugin::ConnectToPacsDialogWidget::FillQueryParamsFromFields( 
	PACS::QueryParams &queryParams )
{
	wxString patientName = m_edPatientName->GetValue();
	if(patientName.IsEmpty())
		patientName="*";
	queryParams.query.push_back( dcmAPI::Tag( dcmAPI::tags::PatientName, patientName.c_str() ) );

	wxString patientID = m_edPatientID->GetValue();
	queryParams.query.push_back( dcmAPI::Tag( dcmAPI::tags::PatientRealID, patientID.c_str() ) );

	wxString studyID = m_edStudyId->GetValue();
	queryParams.query.push_back( dcmAPI::Tag( dcmAPI::tags::StudyRealID, studyID.c_str() ) );

	wxString modality = m_edModality->GetValue();
	queryParams.query.push_back( dcmAPI::Tag( dcmAPI::tags::Modality, modality.c_str() ) );

	wxString birthDate;
	if ( m_chkDateOfBirth->IsChecked() )
	{
		wxDateTime dateTime = m_dateBirth->GetValue( );
		birthDate = dateTime.Format( wxDefaultDateTimeFormat );
	}
	queryParams.query.push_back( dcmAPI::Tag( dcmAPI::tags::PatientDate, birthDate.c_str() ) );

	wxString birthDateFrom;
	if ( m_chkDateOfBirthFrom->IsChecked() )
	{
		wxDateTime dateTime = m_dateBirth_From->GetValue( );
		birthDateFrom = dateTime.Format( wxDefaultDateTimeFormat );
	}
	queryParams.birthDateFrom = dcmAPI::Tag( dcmAPI::tags::PatientDate, birthDateFrom.c_str() );

	wxString birthDateTo;
	if ( m_chkDateOfBirthTo->IsChecked() )
	{
		wxDateTime dateTime = m_dateBirth_To->GetValue( );
		birthDateTo = dateTime.Format( wxDefaultDateTimeFormat );
	}
	queryParams.birthDateTo = dcmAPI::Tag( dcmAPI::tags::PatientDate, birthDateTo.c_str() );

}

void DicomPlugin::ConnectToPacsDialogWidget::FillQueryParamsFromTree( 
	PACS::QueryParams &queryParams )
{

	wxArrayTreeItemIds treeItemIdsArray;
	if(m_DICOMTree->GetSelections(treeItemIdsArray) < 1)
		return;
	
	std::string patientId, seriesId, studyId, sliceId;
	//for(int i=0; i<treeItemIdsArray.GetCount(); i++)
	if(treeItemIdsArray.GetCount())
	{
		//wxTreeItemId treeItemId = treeItemIdsArray.Item(i);
		wxTreeItemId treeItemId = treeItemIdsArray.Item(0);

		//checking if a patient item was selected
		PatientItemTreeData* patientData = dynamic_cast<PatientItemTreeData*>(m_DICOMTree->GetItemData(treeItemId));
		
		if ( patientData != NULL ) 
		{
			patientId=patientData->GetPatientId();
		}

		//checking if a study item was selected
		StudyItemTreeData* studyData = dynamic_cast<StudyItemTreeData*>(m_DICOMTree->GetItemData(treeItemId));
		if ( studyData != NULL ) 
		{
			patientId=studyData->GetPatientId();
			studyId=studyData->GetStudyId();
		}

		//checking if a series item was selected
		SeriesItemTreeData* seriesData = dynamic_cast<SeriesItemTreeData*>(m_DICOMTree->GetItemData(treeItemId));
		if ( seriesData != NULL ) 
		{
			patientId=seriesData->GetPatientId();
			studyId=seriesData->GetStudyId();
			seriesId=seriesData->GetSeriesId();
		}


		//checking if slice item was selected
		//SliceItemTreeData* sliceData = dynamic_cast<SliceItemTreeData*>(m_DICOMTree->GetItemData(treeItemId));
		//if(sliceData != NULL)
		//{
		//	patientId=sliceData->GetPatientId();
		//	studyId=sliceData->GetStudyId();
		//	seriesId=sliceData->GetSeriesId();
		//	sliceId=sliceData->GetSliceId();
		//}
	}

	queryParams.query.push_back( dcmAPI::Tag( dcmAPI::tags::PatientRealID, patientId.c_str() ) );	
	if(studyId.length()>0)
		queryParams.query.push_back( dcmAPI::Tag( dcmAPI::tags::StudyInstanceUID, studyId.c_str() ) );	
	if(seriesId.length()>0)
		queryParams.query.push_back( dcmAPI::Tag( dcmAPI::tags::SeriesInstanceUID, seriesId.c_str() ) );	

}

void DicomPlugin::ConnectToPacsDialogWidget::OnSaveConfig( wxCommandEvent& event)
{
	std::string strCalledAEinfo;
	strCalledAEinfo.append(m_edPACSCalledAETitle->GetValue().c_str());
	strCalledAEinfo.append(":");
	strCalledAEinfo.append(m_edPACSCalledAEIP->GetValue().c_str());
	strCalledAEinfo.append(":");
	strCalledAEinfo.append(m_edPACSCalledAEPort->GetValue().c_str());

	std::string strCallingAEinfo;
	strCallingAEinfo.append(m_edPACSCallingAETitle->GetValue().c_str());
	strCallingAEinfo.append(":");
	strCallingAEinfo.append(m_edPACSCallingAEIP->GetValue().c_str());
	strCallingAEinfo.append(":");
	strCallingAEinfo.append(m_edPACSCallingAEPortRetrieve->GetValue().c_str());
	strCallingAEinfo.append(":");
	strCallingAEinfo.append(m_edPACSCallingAEPortSend->GetValue().c_str());

	Core::Runtime::Settings::Pointer settings = Core::Runtime::Kernel::GetApplicationSettings();
	settings->SetPacsCalledAE(strCalledAEinfo);
	settings->SetPacsCallingAE(strCallingAEinfo);
	settings->SaveSettings();
}

void DicomPlugin::ConnectToPacsDialogWidget::OnSendToPacs(wxCommandEvent& event)
{

	PACS::PacsAPI* pacsAPI = NULL;
	try
	{
		pacsAPI = PACS::CreateObjectOfPacsAPI( );

		// Connect to server params
		PACS::NetworkParams networkParams;
		FillNetworkParams( networkParams );
		pacsAPI->SetNetworkParams( networkParams );

		PACS::StoreParams storeParams;

		Core::Runtime::wxMitkGraphicalInterface::Pointer gIface = Core::Runtime::Kernel::GetGraphicalInterface();
		coreAssertMacro(gIface.IsNotNull());
		std::string filetypes = "DICOM Files (dcm)|*.dcm|All files (*.*)|*.*";
		Core::Runtime::Settings::Pointer settings = Core::Runtime::Kernel::GetApplicationSettings();
		std::string dataPath = settings->GetLastOpenedPath(1);
		wxFileDialog* openFileDialog = new wxFileDialog
			(
			this,
			wxT("Specify DICOM files to send to PACS"),
			wxT(""), wxT(""), wxT(""),
			wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxMULTIPLE
			);

		if(openFileDialog== NULL)
		{
			gIface->GetMainWindow()->ReportError("Creating open file dialog failed", true);
			return;
		}

		openFileDialog->SetDirectory(_U(dataPath));
		openFileDialog->SetWildcard(_U(filetypes));
		if(openFileDialog->ShowModal() == wxID_OK)
		{

			dataPath = _U(openFileDialog->GetDirectory());

			wxArrayString wxFilenames;
			openFileDialog->GetFilenames( wxFilenames );
			for ( unsigned i = 0 ; i < wxFilenames.size() ; i++ )
			{
				storeParams.fullfilenames.push_back( dataPath + Core::IO::SlashChar + 
					_U(wxFilenames[ i ]) );
			}


			// Not needed any more
			openFileDialog->Destroy();
			wxTheApp->Yield(true);
		}

		delete openFileDialog;

		pacsAPI->SetStoreParams( storeParams );

		// Do retrieve
		pacsAPI->Store();

	}
	coreCatchExceptionsReportAndNoThrowMacro(ConnectToPacsDialogWidget::OnSendToPacs)

	PACS::DestroyObjectOfPacsAPI( pacsAPI );

	SetFocus( );

}

Core::BaseProcessor::Pointer DicomPlugin::ConnectToPacsDialogWidget::GetProcessor()
{
	return NULL;
}

void DicomPlugin::ConnectToPacsDialogWidget::OnInit()
{
	WorkingAreaPanelWidget* widget;
	GetPluginTab()->GetWidget( wxID_WorkingAreaPanelWidget, widget );

	if ( widget != NULL )
	{
		DicomPlugin::DataSetProcessor* processor;
		processor = dynamic_cast<DicomPlugin::DataSetProcessor*> 
			( widget->GetProcessor().GetPointer() );
		m_DataSetProcessor = processor;

	}
	else
	{
		m_DataSetProcessor = DicomPlugin::DataSetProcessor::New( );
	}

	Core::Widgets::ToolbarIO* toolbar;
	GetPluginTab()->GetWidget( wxID_IOToolbar, toolbar );
	if ( toolbar )
	{
		wxBitmap bitmap;
		bitmap = wxBitmap( pacs_xpm );
		toolbar->AddTool(wxID_ShowPACSWidget, _T("PACS"),
			bitmap, wxNullBitmap, wxITEM_NORMAL,
			_T("PACS"), _T("PACS"));
		toolbar->Realize();
	}

}

