/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef DicomConnectToPACSDialogWidget_H
#define DicomConnectToPACSDialogWidget_H

#include "DicomConnectToPacsDialogWidgetUI.h"
#include "DicomDataSetProcessor.h"
#include "CILabNamespaceMacros.h"
#include "coreProcessingWidget.h"

#include "pacsAPI.h"

namespace DicomPlugin{

#define wxID_DICOM_DATA_DOWNLOADED_MESSAGE wxID("DicomDataDownloadedMessage")

/** 
A class that defines all the graphical interface and user interaction for connecting to PACS and downloading data

\ingroup DicomPlugin
\author Jakub Lyko
\date 26 June 2009
*/

class ConnectToPacsDialogWidget : 
	public DicomConnectToPacsDialogWidgetUI,
	public Core::Widgets::ProcessingWidget
{
public:
	coreDefineBaseWindowFactory( ConnectToPacsDialogWidget )

	//!
	ConnectToPacsDialogWidget(wxWindow* parent, int id, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=0);

public:
	void OnInit( );

	DicomPlugin::DataSetProcessor::Pointer GetDataSetProcessor() const;
	void SetDataSetProcessor(DicomPlugin::DataSetProcessor::Pointer val);

	//!
	Core::BaseProcessor::Pointer GetProcessor( );;

private:

	//!
	void OpenDirectoryForSavingPacsData(wxCommandEvent& event);

	//!
	void OnRetrieve(wxCommandEvent& event);

	//!
	void OnQuery(wxCommandEvent& event);

	//!
	void OnSendToPacs(wxCommandEvent& event);

	//! Read the txt file of the query results and fill the tree
	void FillQueryResults( PACS::PacsAPI* pacsAPI );

	//!
	void OnPatientNameTextEnter( wxCommandEvent& WXUNUSED(event));

	//!
	void OnChkDateOfBirth( wxCommandEvent& event );

	//!
	void OnSaveConfig( wxCommandEvent& event);

	//!
	void UpdateWidget( );

	//!
	void FillNetworkParams( PACS::NetworkParams &networkParams );

	//! Fill patient name, id and date of birth from corresponding fields in textboxes
	void FillQueryParamsFromFields( PACS::QueryParams &queryParams );

	//! Fill patient id, study and series id form the tree
	void FillQueryParamsFromTree( PACS::QueryParams &queryParams );

private:

	DicomPlugin::DataSetProcessor::Pointer m_DataSetProcessor;

	//!
	DECLARE_EVENT_TABLE()
};

} // DicomPlugin

#endif //DicomConnectToPACSDialogWidget_H
