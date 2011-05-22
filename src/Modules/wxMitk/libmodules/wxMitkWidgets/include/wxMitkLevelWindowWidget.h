/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef wxMitkLevelWindowWidget_H
#define wxMitkLevelWindowWidget_H

#include "wxMitkWidgetsWin32Header.h"
#include <wx/panel.h>
#include <itkSmartPointer.h>

namespace mitk
{

class wxMitkLineEditLevelWindowControl;
class wxMitkSliderLevelWindowControl;
class LevelWindowManager;
class DataStorage;

/*
wxMitkLevelWindowWidget provides a complete widget set for handling 
LevelWindowProperties on the current image being displayed. It shows a 
vertical slider and two checkboxes that ease the user interaction for 
changing that property. The slider is namely a 
wxMitkSliderLevelWindowControl, while the checkboxes are a single 
wxMitkLineEditLevelWindowControl.

\sa wxMitkLineEditLevelWindowControl
\sa wxMitkSliderLevelWindowControl

\ingroup wxMitkWidgets
\author Juan Antonio Moya
\date 19 Oct 2007
*/
class WXMITKWIDGETS_EXPORT wxMitkLevelWindowWidget : public wxPanel
{
	
public:
	wxMitkLevelWindowWidget(
		wxWindow* parent, 
		wxWindowID id, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = 0, 
		const wxString& name = wxT("wxMitkLevelWindowWidget"));
	virtual ~wxMitkLevelWindowWidget(void);
	void SetDataStorage(mitk::DataStorage* dataStorage);
	void SetLevelWindowManager(
		itk::SmartPointer<mitk::LevelWindowManager> levelWindowManager);
	itk::SmartPointer<mitk::LevelWindowManager> GetLevelWindowManager();


private:

	wxMitkLineEditLevelWindowControl* lineEdit;
	wxMitkSliderLevelWindowControl* slider;
	itk::SmartPointer<mitk::LevelWindowManager> levelWindowManager;

};

} // namespace mitk


#endif // wxMitkLevelWindowWidget
