/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef wxMitkTransformVisualPropWidget_H
#define wxMitkTransformVisualPropWidget_H

#include "wxMitkWidgetsWin32Header.h"
#include "wxMitkTransformVisualPropWidgetUI.h"
#include "mitkDataTreeNode.h"

namespace mitk
{

/*
The wxMitkTransformVisualPropWidget is a widget that allows the user to
easily change the visual properties of a mitk::Transform.

\ingroup wxMitkWidgets
\author Xavi Planes
\date 2 August 2010
*/
class WXMITKWIDGETS_EXPORT wxMitkTransformVisualPropWidget : public wxMitkTransformVisualPropWidgetUI
{
public:
	wxMitkTransformVisualPropWidget(
		wxWindow* parent, 
		wxWindowID id, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxBORDER_NONE, 
		const wxString& name = wxT("wxMitkWidgetStackControl"));
	virtual ~wxMitkTransformVisualPropWidget(void);

	//!
	void SetDataTreeNode(mitk::DataTreeNode::Pointer node);

protected:

	virtual void OnSpacingX(wxCommandEvent &event);
	virtual void OnSpacingY(wxCommandEvent &event);
	virtual void OnSpacingZ(wxCommandEvent &event);
	virtual void OnOffsetX(wxCommandEvent &event);
	virtual void OnOffsetY(wxCommandEvent &event);
	virtual void OnOffsetZ(wxCommandEvent &event);

	//!
	void UpdateWidget( );

	DECLARE_EVENT_TABLE();

private:
	//! Current node
	mitk::DataTreeNode::Pointer m_node;
};
 

} // namespace mitk


#endif // wxMitkTransformVisualPropWidget_H


