/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _DicomMenuEventHandler_H
#define _DicomMenuEventHandler_H

#define wxID_ShowPACSWidget wxID("wxID_ShowPACSWidget")

namespace DicomPlugin{

class WorkingAreaPanelWidget;

/** 
Handler for MRU list and File menu entries

The wxEventHandler cannot be WorkingAreaPanelWidget because the parent is
the main window and creates an infinite loop

Is not possible to connect the menu event to a WorkingAreaPanelWidget
handler function because the handler function should be a member function
of wxEventHandler class

\ingroup DicomPlugin
\author Xavi Planes
\date 08 June 2010
*/
class DicomMenuEventHandler : public wxEvtHandler
{
public:
	DicomMenuEventHandler( WorkingAreaPanelWidget* workingArea );

	//!
	void Open( const std::string &filename );

	//!
	void AddFileToHistory( const std::string &filename );

protected:
	DECLARE_EVENT_TABLE( );

	//!
	void OnMRUFile(wxCommandEvent& event);

	//! Open directory with dicom files (2D images)
	void OnMenuOpenDirectory(wxCommandEvent& event);

	/** Open DICOMDIR file which contains information about dicom data
	or Open single dicom file
	*/
	void OnMenuOpenDicomFile(wxCommandEvent& event);

	//!
	void OnMenuShowPACSWidget(wxCommandEvent& event);

private:
	//!
	WorkingAreaPanelWidget* m_WorkingArea;
};

} // DicomPlugin

#endif //_DicomMenuEventHandler_H
