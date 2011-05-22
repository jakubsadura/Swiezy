/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef wxMitkSurfaceLightingControl_H
#define wxMitkSurfaceLightingControl_H

#include "wxMitkWidgetsWin32Header.h"
#include "wxMitkWidgetUpdateEvent.h"
#include <mitkMaterialProperty.h>
#include <wx/panel.h>

class wxPanel;
class wxChoice;
class wxColourPickerEvent;

namespace mitk
{
class wxMitkSliderDouble;
class wxMitkSliderChangeEvent;
class wxMitkColorSelectorControl;


/*
The wxMitkSurfaceLightingControl is a widget component used for selecting 
the lighting properties of a surface mesh, being it the specular and 
diffuse coefficients, opacity(translucency) and lighting interpolation for 
face normals (Phong Gouraud, Flat).

\ingroup wxMitkWidgets
\author Juan Antonio Moya
\date 18 Dec 2007
*/
class WXMITKWIDGETS_EXPORT wxMitkSurfaceLightingControl : public wxPanel
{
public:
	typedef enum
	{
		AMBIENT_COLOR_CONTROL,
		AMBIENT_COEFFICIENT_CONTROL,
		DIFFUSE_COLOR_CONTROL,
		DIFFUSE_COEFFICIENT_CONTROL,
		SPECULAR_COLOR_CONTROL,
		SPECULAR_COEFFICIENT_CONTROL,
		SPECULAR_POWER_CONTROL,
		OPACITY_CONTROL,
		LIGHTING_MODEL_CONTROL,
		MAX_CONTROLS,
	} WXMITKSURFACELIGHTINGCONTROL_CONTROL;

	typedef mitk::MaterialProperty::InterpolationType InterpolationType;

	wxMitkSurfaceLightingControl(
		wxWindow* parent, 
		wxWindowID id, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxBORDER_NONE, 
		const wxString& name = wxT("wxMitkSurfaceLightingControl"));
	virtual ~wxMitkSurfaceLightingControl(void);

	void SetMaterial(MaterialProperty::Pointer material);

	void SetInterpolation(InterpolationType interpolation);
	void SetInterpolationIndex(int index);
	InterpolationType GetInterpolation(void);
	int GetInterpolationIndex(void) const;
	double GetAmbientCoefficient(void) const;
	double GetDiffuseCoefficient(void) const;
	double GetSpecularCoefficient(void) const;
	double GetSpecularPower(void) const;
	double GetOpacity(void) const;
	void GetAmbientColor(float color[3]) const;
	void GetDiffuseColor(float color[3]) const;
	void GetSpecularColor(float color[3]) const;

	bool EnableControl( WXMITKSURFACELIGHTINGCONTROL_CONTROL whichControl, bool bEnable = true );

private:
	void CastWidgetUpdateEvent(void);
	void OnInterpolationSelected(wxCommandEvent& event);

	void OnAmbientCoefficientChange(wxMitkSliderChangeEvent& event);
	void OnDiffuseCoefficientChange(wxMitkSliderChangeEvent& event);
	void OnSpecularCoefficientChange(wxMitkSliderChangeEvent& event);
	void OnSpecularPowerChange(wxMitkSliderChangeEvent& event);
	void OnOpacityCoefficientChange(wxMitkSliderChangeEvent& event);

	void OnAmbientColorChange(wxColourPickerEvent& event);
	void OnDiffuseColorChange(wxColourPickerEvent& event);
	void OnSpecularColorChange(wxColourPickerEvent& event);

	void DoRemoveDisabledControl( wxWindow* pControl );
	void DoAddEnabledControl( WXMITKSURFACELIGHTINGCONTROL_CONTROL whichControl );
	void DoRemoveDisabledControl( WXMITKSURFACELIGHTINGCONTROL_CONTROL whichControl );


	InterpolationType currentInterpolation;

	bool interactiveApply;

	wxChoice* cmbInterpolation;
	wxMitkColorSelectorControl* ambientColorSelector;
	wxMitkColorSelectorControl* diffuseColorSelector;
	wxMitkColorSelectorControl* specularColorSelector;
	wxMitkSliderDouble* sldAmbient;
	wxMitkSliderDouble* sldDiffuse;
	wxMitkSliderDouble* sldSpecular;
	wxMitkSliderDouble* sldSpecularPower;
	wxMitkSliderDouble* sldOpacity;
	wxStaticText* m_plblAmbientColor;
	wxStaticText* m_plblAmbient;
	wxStaticText* m_plblDiffuseColor;
	wxStaticText* m_plblDiffuse;
	wxStaticText* m_plblSpecularColor;
	wxStaticText* m_plblSpecular;
	wxStaticText* m_plblSpecularPower;
	wxStaticText* m_plblOpacity;
	wxStaticText* m_plblInterpolation;
	wxFlexGridSizer* m_pGridSizer;

	bool m_ControlsEnableStatus[MAX_CONTROLS];

	DECLARE_EVENT_TABLE();
};


} // namespace mitk


#endif // wxMitkSurfaceLightingControl_H

