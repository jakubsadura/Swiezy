/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include <wx/wx.h>
#include <wx/image.h>

#ifndef WXMITKPolyDataINFORMATIONWIDGET_H
#define WXMITKPolyDataINFORMATIONWIDGET_H

#include "wxMitkWidgetsWin32Header.h"
#include <mitkProperties.h>
#include <wx/panel.h>

#include "vtkSmartPointer.h"
#include "vtkPolyData.h"


namespace mitk
{


/**
\brief Widget for displaying information about Polydata dataentity
\group wxMitkWidgets
\author Chiara Riccobene
\date 07-06-09
*/

class WXMITKWIDGETS_EXPORT wxMitkPolyDataInformationWidget: public wxPanel {
public:
   
    wxMitkPolyDataInformationWidget(wxWindow* parent, 
										int id, 
										const wxPoint& pos=wxDefaultPosition, 
										const wxSize& size=wxDefaultSize, 
										long style=0);

	/** Display number of Cells, Points , type of data and whatever 
	information of the polydata
	*/
	void UpdateData(vtkSmartPointer<vtkPolyData> PolyDataInput);
	//!
	void ResetWidget( );

private:
   
    void set_properties();
    void do_layout();
  

protected:

   wxStaticText* label_1;
    wxTextCtrl* m_vertices;
    wxStaticText* label_2;
    wxTextCtrl* m_triangles;
    wxStaticText* label_3;
    wxStaticText* label_3_copy_1;
    wxStaticText* label_3_copy_2;
    wxStaticText* label_4;
    wxTextCtrl* m_Xminmax;
    wxTextCtrl* m_Xrange;
    wxStaticText* label_5;
    wxTextCtrl* m_Yminmax;
    wxTextCtrl* m_Yrange;
    wxStaticText* label_6;
    wxTextCtrl* m_Zminmax;
    wxTextCtrl* m_Zrange;
	wxStaticText* label_7;
	wxTextCtrl* m_Volume;
    
}; 

}// mitk
#endif // WXMITKPolyDataINFORMATIONWIDGET_H
