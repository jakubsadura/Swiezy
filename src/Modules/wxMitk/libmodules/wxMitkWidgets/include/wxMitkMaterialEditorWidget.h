/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef wxMitkMaterialEditorWidget_H
#define wxMitkMaterialEditorWidget_H

#include "wxMitkWidgetsWin32Header.h"
#include <mitkMaterialProperty.h>
#include <wx/panel.h>
#include <mitkDataTreeNode.h>

class wxCheckBox;
class wxCommandEvent;
class wxButton;
class wxBoxSizer;

namespace mitk
{
class wxMitkWidgetUpdateEvent;
class wxMitkMaterialSettingsPanel;
class wxMitkScalarsArrayControl;
class wxMitkRenderWindow;

/*
The wxMitkMaterialEditorWidget is a widget that allows the user to
easily change the material properties of a mitk
surface.

\ingroup wxMitkWidgets
\author Juan Antonio Moya
\date 17 Dec 2007
*/
class WXMITKWIDGETS_EXPORT wxMitkMaterialEditorWidget : public wxPanel
{
public:
	wxMitkMaterialEditorWidget(
		wxWindow* parent, 
		wxWindowID id, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxBORDER_NONE, 
		const wxString& name = wxT("wxMitkWidgetStackControl"));
	virtual ~wxMitkMaterialEditorWidget(void);

	void SetDataTreeNode(mitk::DataTreeNode::Pointer node);
	void SetDefaultScalarArray();
	void SetMitkRenderWindow( mitk::wxMitkRenderWindow* renderWindow );

protected:
	void SetMaterial(MaterialProperty::Pointer material);
	MaterialProperty::Pointer GetMaterial(void) const;

private:
	void ApplyMaterialProperty(void);
	void OnLightingChange(wxMitkWidgetUpdateEvent& event);
	void OnRepresentationChange( wxCommandEvent& event );
	void OnScalarArrayChange(wxMitkWidgetUpdateEvent& event);
	void UpdateLayout( );

private:
	wxNotebook* m_MeshNotebook;
	wxMitkMaterialSettingsPanel* m_pMaterialSettingsPane;
	wxPanel* m_ScalarsNotebookPane;

	wxMitkScalarsArrayControl* m_scalarArray;

	MaterialProperty::Pointer m_materialProperty;

	wxBoxSizer* m_vLayoutScalar;
	
	mitk::DataTreeNode::Pointer m_node;
	mitk::wxMitkRenderWindow* m_renderWindow;
	DECLARE_EVENT_TABLE();
};
 

} // namespace mitk


#endif // wxMitkMaterialEditorWidget_H


