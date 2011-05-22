/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include <wx/wx.h>
#include <wx/image.h>

#ifndef WXMITKMEASUREMENTINFORMATIONWIDGET_H
#define WXMITKMEASUREMENTINFORMATIONWIDGET_H

#include "wxMitkWidgetsWin32Header.h"
#include <mitkProperties.h>
#include <wx/panel.h>

#include "vtkSmartPointer.h"
#include "vtkPolyData.h"

#include "vtkCommand.h"


namespace mitk
{

class wxMitkMeasurementInformationWidget;

class vtkUpdateWidgetCommand	: public vtkCommand
{
public:
	static vtkUpdateWidgetCommand *New();

	void SetWidget( wxMitkMeasurementInformationWidget* widget );

	//!
	void Execute(vtkObject* caller, unsigned long eventId, void* callData);

protected:
	wxMitkMeasurementInformationWidget* m_widget;

};

/**
\brief Widget for displaying information about measurement dataentity
\group wxMitkWidgets
\author Chiara Riccobene
\date 07-06-09
*/

class WXMITKWIDGETS_EXPORT wxMitkMeasurementInformationWidget: public wxPanel {
public:
   
    wxMitkMeasurementInformationWidget(wxWindow* parent, 
										int id, 
										const wxPoint& pos=wxDefaultPosition, 
										const wxSize& size=wxDefaultSize, 
										long style=0);

	~wxMitkMeasurementInformationWidget( );

	//! Update the current data to display
	void UpdateData(vtkSmartPointer<vtkPolyData> measurementInput);

	//!
	void ResetWidget( );

	//! Update the controls
	void UpdateWidget( );

	//!
	void Execute(vtkObject* caller, unsigned long eventId, void* callData);

private:
   
    void set_properties();
    void do_layout();
  

protected:

	wxStaticBox* sizer_2_staticbox;
    wxStaticText* label_9;
    wxStaticText* label_6;
	wxStaticText* label_7;
    wxStaticText* label_3;
    wxTextCtrl* m_point1_X;
    wxTextCtrl* m_point2_X;
    wxStaticText* label_4;
    wxTextCtrl* m_point1_Y;
    wxTextCtrl* m_point2_Y;
    wxStaticText* label_5;
    wxTextCtrl* m_point1_Z;
    wxTextCtrl* m_point2_Z;
    wxStaticText* label_1;
    wxTextCtrl* m_measure;
    
	//!
	vtkSmartPointer<vtkPolyData> m_MeasurementInput;

	//!
	unsigned long m_MeasurementObserverTag;

	//!
	vtkSmartPointer<vtkUpdateWidgetCommand> m_UpdateWidgetCommand;
}; 

}// mitk
#endif // WXMITKMeasurementINFORMATIONWIDGET_H
