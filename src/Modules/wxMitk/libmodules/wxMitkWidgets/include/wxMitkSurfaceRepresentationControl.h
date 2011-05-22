/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef wxMitkSurfaceRepresentationControl_H
#define wxMitkSurfaceRepresentationControl_H

#include "wxMitkWidgetsWin32Header.h"
#include "wxMitkWidgetUpdateEvent.h"
#include <mitkMaterialProperty.h>
#include <wx/panel.h>

class wxChoice;
class wxSpinCtrl;
class wxSpinEvent;

class wxWidgetStackControl;

namespace mitk
{

/*
The wxMitkSurfaceRepresentationControl is a widget component used for 
selecting the representation of a surface mesh, being it wireframe, 
points or solid surface. It is also responsible for displaying additional 
options that may be interesting to set depending on the chosen representation.

It throws a wxMitkWidgetUpdateEvent when its values have been updated

\ingroup wxMitkWidgets
\author Juan Antonio Moya
\date 14 Dec 2007
*/
class WXMITKWIDGETS_EXPORT wxMitkSurfaceRepresentationControl 
	: public wxPanel
{
public:
	typedef mitk::MaterialProperty::RepresentationType RepresentationType;

	wxMitkSurfaceRepresentationControl(
		wxWindow* parent, 
		wxWindowID id, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxBORDER_NONE, 
		const wxString& name = wxT("wxMitkSurfaceRepresentationControl"));
	virtual ~wxMitkSurfaceRepresentationControl(void);

	void SetMaterial(MaterialProperty::Pointer material);

	void SetRepresentation(RepresentationType representation);
	void SetRepresentationIndex(int index);
	RepresentationType GetRepresentation(void) const;
	int GetRepresentationIndex(void) const;
	float GetLineWidth(void) const;
	float GetPointSize(void) const;

private:
	void OnRepresentationSelected(wxCommandEvent& event);
	void OnLineWidthChange(wxSpinEvent& event);
	void OnPointSizeChange(wxSpinEvent& event);

	wxWidgetStackControl* widgetStack;
	wxChoice* cmbRepresentation;
	wxPanel* wireframeOptions;
	wxPanel* pointOptions;
	wxPanel* emptyPanel;
	wxSpinCtrl* udcLineWidth;
	wxSpinCtrl* udcPointSize;
	RepresentationType currentRepresentation;
	
	DECLARE_EVENT_TABLE();
};


} // namespace mitk


#endif // wxMitkSurfaceRepresentationControl_H

