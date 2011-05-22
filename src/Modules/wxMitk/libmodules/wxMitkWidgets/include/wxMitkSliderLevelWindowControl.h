/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef wxMitkSliderLevelWindowControl_H
#define wxMitkSliderLevelWindowControl_H

#include "wxMitkWidgetsWin32Header.h"
#include "wxMitkRangeSliderControl.h"
#include <itkSmartPointer.h>

class wxTextCtrl;

namespace itk
{
class EventObject;
}

namespace mitk
{

class LevelWindowManager;
class DataTree;
class DataStorage;

/*
wxMitkSliderLevelWindowControl provides a widget with two text edit fields,
one used to change the window value of the current 2D geometry and one to 
change the level value, affecting the visualization as a Brightness and 
Contrast control.

\sa wxMitkLevelWindow

\ingroup wxMitkWidgets
\author Juan Antonio Moya
\date 23 Oct 2007
*/
class WXMITKWIDGETS_EXPORT wxMitkSliderLevelWindowControl 
	: public wxMitkRangeSliderControl
{
	
public:
	wxMitkSliderLevelWindowControl(
		wxWindow* parent, 
		wxWindowID id, 
		int level = 0, 
		unsigned int window = 5, 
		int minValue = -10, 
		int maxValue = 10, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxSL_VERTICAL, 
		const wxValidator& validator = wxDefaultValidator, 
		const wxString& name = wxT("wxMitkSliderLevelWindowControl"));
	virtual ~wxMitkSliderLevelWindowControl(void);
	void SetDataStorage(mitk::DataStorage* dataStorage);
	void SetLevelWindowManager(
		itk::SmartPointer<mitk::LevelWindowManager> levelWindowManager);

protected:
	virtual void OnLevelWindowPropertyModified(const itk::EventObject& event);

private:
	void RangeChangedByUser(wxMitkRangeChangedEvent& event);

	itk::SmartPointer<mitk::LevelWindowManager> levelWindowManager;
	bool isObserverTagSet;
    unsigned long ObserverTag;

	DECLARE_EVENT_TABLE();
};

} // namespace mitk


#endif // wxMitkSliderLevelWindowControl
