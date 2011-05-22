/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef wxMitkLookupTableWidget_H
#define wxMitkLookupTableWidget_H

#include "wxMitkWidgetsWin32Header.h"
#include <wx/panel.h>

#include "mitkLookupTableProperty.h"

#include "mitkDataTreeNode.h"
#include <wx/clrpicker.h>

#include "mitkProperties.h"

#include "blShapeUtils.h"

namespace mitk
{

class wxMitkRenderWindow;
class wxMitkColorSelectorControl;

/**
\brief Select a lookup table and show or hide the legend

\ingroup wxMitkWidgets
\author Xavi Planes
\date 03 Feb 2009
*/
class WXMITKWIDGETS_EXPORT wxMitkLookupTableWidget : public wxPanel
{
public:

	wxMitkLookupTableWidget(
		wxWindow* parent, 
		wxWindowID id, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = 0, 
		const wxString& name = wxT("wxMitkLookupTableWidget"));
	virtual ~wxMitkLookupTableWidget(void);

	void SetDataTreeNode(mitk::DataTreeNode* node);

	/** 
	\brief Set render window to show the legend. 
	\note This functionality should be inside MITK
	*/
	void SetMitkRenderWindow( mitk::wxMitkRenderWindow* renderWindow );

	void ResetWidget(void);

	void EnableUseColor( bool bVal );

	void SetScalarType( blShapeUtils::ShapeUtils::VTKScalarType scalarType );
	blShapeUtils::ShapeUtils::VTKScalarType GetScalarType( );

private:
	void OnUseColor(wxCommandEvent& event);

	void OnColorChanged( wxColourPickerEvent& WXUNUSED(event) );

	void OnBackgroundColorChanged( wxColourPickerEvent& WXUNUSED(event) );

	void OnComboLUTModified(wxCommandEvent& event);

	void OnShowLegend(wxCommandEvent& event);

	void InitSelectedColor( mitk::DataTreeNode* node );

	void UpdateLookupTableProp(  );

	void UpdateWidgets( );

	void UpdateData( );

	//!
	void CleanWidgets( );

private:
	//! LookupTable stuff
	mitk::BoolProperty::Pointer m_useColor;
	mitk::wxMitkColorSelectorControl* m_ColorSelector;
	mitk::wxMitkColorSelectorControl* m_BackgroundColorSelector;
	wxCheckBox* m_chkUseColor;
	wxComboBox* m_ComboLUT;
	wxCheckBox* m_chkShowLegend;

	mitk::wxMitkRenderWindow* m_renderWindow;
	mitk::DataTreeNode* m_node;

	bool m_bEnableUserColor;
	blShapeUtils::ShapeUtils::VTKScalarType m_selectedScalarType;

	//!
	unsigned long m_NodeTagModified;


	DECLARE_EVENT_TABLE();
};
 
} // namespace mitk


#endif // wxMitkLookupTableWidget_H
