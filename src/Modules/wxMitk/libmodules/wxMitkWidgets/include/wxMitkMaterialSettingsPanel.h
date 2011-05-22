/*
* Copyright (c) 2010,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef wxMitkMaterialSettingsPanel_H
#define wxMitkMaterialSettingsPanel_H

#include "wxMitkWidgetsWin32Header.h"
#include <mitkMaterialProperty.h>
#include <wx/panel.h>

namespace mitk
{
class wxMitkWidgetUpdateEvent;
class wxMitkSurfaceLightingControl;
class wxMitkSurfaceRepresentationControl;
/*
The wxMitkMaterialSettingsPanel is a widget that allows the user to
easily change the material properties of a mitk surface, or some
volume properties of a volume image.

\ingroup wxMitkWidgets
\author David Lucena González
\date 07 Dec 2010
*/

DECLARE_EXPORTED_EVENT_TYPE( WXMITKWIDGETS_EXPORT, wxEVT_MATERIALSETTINGS_INTERACTIVEAPPLY_CHANGED, wxID_ANY )
DECLARE_EXPORTED_EVENT_TYPE( WXMITKWIDGETS_EXPORT, wxEVT_MATERIALSETTINGS_INTERACTIVEAPPLY_PRESSED, wxID_ANY )
DECLARE_EXPORTED_EVENT_TYPE( WXMITKWIDGETS_EXPORT, wxEVT_MATERIALSETTINGS_REPRESENTATION_CHANGED, wxID_ANY )

class wxMitkMaterialSettingsPanel : public wxPanel
{
//! Public static members
public:
	// There are several approach to get an event from
	// a control. One can be just, get an event from the
	// id of that control, or just create a new event and
	// listen to that event. I have decided to expose the
	// ids of the contained controls to make it similar
	// of how it was
	// But I would rather prefer the approach of creating
	// new events for this container control
	static const wxWindowID wxID_representation;
	static const wxWindowID wxID_lighting;

	typedef enum
	{
		LIGHTING_CONTROL,
		SURFACE_REPRESENTATION_CONTROL,
		WXMITKMATERIALSETTINGSPANEL_MAX_CONTROLS,
	} WXMITKMATERIALSETTINGSPANEL_CONTROL;
	

//! Public methods
public:
	wxMitkMaterialSettingsPanel( wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER,
		const wxString& name = wxPanelNameStr );

	virtual ~wxMitkMaterialSettingsPanel();


	void OnInteractiveApplyChanged( wxCommandEvent& event );
	void OnLightingChange( wxMitkWidgetUpdateEvent& event );
	void OnRepresentationChange( wxMitkWidgetUpdateEvent& event );

	void UpdateMaterial();
	MaterialProperty::Pointer GetMaterialProperty();
	void SetMaterialProperty( MaterialProperty::Pointer material );
	bool EnableControl( WXMITKMATERIALSETTINGSPANEL_CONTROL whichControl, bool bEnable = true );

	const wxMitkSurfaceLightingControl& GetLightingControl() const;
	wxMitkSurfaceLightingControl* GetLightingControlPointer();
	const wxMitkSurfaceRepresentationControl& GetRepresentationControl() const;

//! Private methods
private:
	void DoRemoveDisabledControl( wxWindow* pControl, wxSizer* pSizer = NULL );
	void DoAddEnabledControl( WXMITKMATERIALSETTINGSPANEL_CONTROL whichControl );
	void DoRemoveDisabledControl( WXMITKMATERIALSETTINGSPANEL_CONTROL whichControl );


//! Private members
private:
	wxBoxSizer* m_pInteractiveApplySizer;
	wxBoxSizer* m_SphereAndCoSizer;
	wxBoxSizer *m_vLayoutLighting;

	wxMitkSurfaceLightingControl* m_lighting;
	wxMitkSurfaceRepresentationControl* m_representation;

	MaterialProperty::Pointer m_MaterialProperty;

	bool m_ControlsEnableStatus[WXMITKMATERIALSETTINGSPANEL_MAX_CONTROLS];

	DECLARE_EVENT_TABLE();
};
 

} // namespace mitk


#endif // #ifndef wxMitkMaterialSettingsPanel_H


