/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include <wx/wx.h>
#include <wx/image.h>

#ifndef WXMITKVOLUMEIMAGEINFORMATIONWIDGET_H
#define WXMITKVOLUMEIMAGEINFORMATIONWIDGET_H

#include "wxMitkWidgetsWin32Header.h"
#include <mitkProperties.h>
#include <wx/panel.h>
#include "vtkSmartPointer.h"
#include "vtkImageData.h"
#include "itkImage.h"

namespace mitk
{

/**
\brief Widget for displaying information about volumeimage dataentity
\group wxMitkWidgets
\author Chiara Riccobene
\date 07-06-09
*/

class WXMITKWIDGETS_EXPORT wxMitkVolumeImageInformationWidget: public wxPanel {
public:
   
	typedef itk::Image< double, 3 > ImageType;
    wxMitkVolumeImageInformationWidget(wxWindow* parent, 
										int id, 
										const wxPoint& pos=wxDefaultPosition, 
										const wxSize& size=wxDefaultSize, 
										long style=0);

	//! Compute origin, size, dimension and data type of the image									
	void UpdateData(vtkSmartPointer<vtkImageData> imageInput);
	void ResetWidget( );

private:
   
    void set_properties();
    void do_layout();
  

protected:

    wxStaticBox* sizer_2_staticbox;
    wxStaticText* label_9;
    wxStaticText* label_6;
    wxStaticText* label_7;
    wxStaticText* label_8;
    wxStaticText* label_3;
    wxTextCtrl* m_dimension_X;
    wxTextCtrl* m_size_X;
    wxTextCtrl* m_origin_X;
    wxStaticText* label_4;
    wxTextCtrl* m_dimension_Y;
    wxTextCtrl* m_size_Y;
    wxTextCtrl* m_origin_Y;
    wxStaticText* label_5;
    wxTextCtrl* m_dimension_Z;
    wxTextCtrl* m_size_Z;
    wxTextCtrl* m_origin_Z;
    wxStaticText* label_10;
    wxTextCtrl* m_txt_dataType;
 
}; 

}// mitk
#endif // WXMITKVOLUMEIMAGEINFORMATIONWIDGET_H
