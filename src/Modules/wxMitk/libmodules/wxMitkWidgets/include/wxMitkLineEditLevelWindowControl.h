/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef wxMitkLineEditLevelWindowControl_H
#define wxMitkLineEditLevelWindowControl_H

#include "wxMitkWidgetsWin32Header.h"
#include <wx/panel.h>
#include <itkSmartPointer.h>

class wxTextCtrl;

namespace itk { class EventObject; }

namespace mitk
{
class LevelWindowManager;
class DataStorage;

/*
wxMitkLineEditLevelWindowControl provides a widget with two text edit 
fields, one used to change the window value of the current
2D geometry and one to change the level value, affecting the 
visualization as a Brightness and Contrast control.

\sa wxMitkLevelWindow

\ingroup wxMitkWidgets
\author Juan Antonio Moya
\date 19 Oct 2007
*/
class WXMITKWIDGETS_EXPORT wxMitkLineEditLevelWindowControl : public wxPanel
{
	
public:
	wxMitkLineEditLevelWindowControl(
		wxWindow* parent, 
		wxWindowID id, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = 0, 
		const wxString& name = wxT("wxMitkLineEditLevelWindowControl"));
	virtual ~wxMitkLineEditLevelWindowControl(void);
	void SetDataStorage(mitk::DataStorage* dataStorage);
	void SetLevelWindowManager(
		itk::SmartPointer<mitk::LevelWindowManager> levelWindowManager);

protected:
	virtual void OnLevelValueChanged(wxCommandEvent& event);	
	virtual void OnWindowValueChanged(wxCommandEvent& event);
	virtual void OnLevelWindowPropertyModified(const itk::EventObject& event);

	void SetLevelWindowProperty(mitk::LevelWindowProperty::Pointer levelWindowProperty);

	/// change notifications from mitkLevelWindowProperty
	void OnPropertyModified(const itk::EventObject& e);

private:
	itk::SmartPointer<mitk::LevelWindowManager> levelWindowManager;
	bool isObserverTagSet;
    unsigned long ObserverTag;
	//! pointer to the LevelWindowProperty of the current image
	LevelWindowProperty::Pointer m_LevelWindowProperty; 
	bool                         m_IsObserverTagSet;
	unsigned long                m_PropertyModifiedTag;
	bool                         m_IsPropertyModifiedTagSet;

	wxTextCtrl* txtWindow;
	wxTextCtrl* txtLevel;

	DECLARE_EVENT_TABLE();
};

} // namespace mitk


#endif // wxMitkLineEditLevelWindowControl
