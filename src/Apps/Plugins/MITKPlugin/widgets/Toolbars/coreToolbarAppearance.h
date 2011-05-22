/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreToolbarAppearance_H
#define _coreToolbarAppearance_H

#include <wx/wx.h>
#include <wx/image.h>
#include "wx/toolbar.h"

#include "gmWidgetsWin32Header.h"

#include "coreBaseWindow.h"

#include "mitkDataTreeNode.h"

namespace Core
{
namespace Widgets
{
	class MultiRenderWindowMITK;

#define wxID_toolSelectLUT			wxID("wxID_toolSelectLUT")
#define wxID_toolShowLUT			wxID("wxID_toolShowLUT")
#define wxID_toolOpacity			wxID("wxID_toolOpacity")

/**
\brief Manage appearance properties (rendering node)
\ingroup gmWidgets
\author Xavi Planes
\date 6 April 2010
*/
class PLUGIN_EXPORT ToolbarAppearance: public wxToolBar, public BaseWindow {
public:
	//!
	coreDefineBaseWindowFactory( ToolbarAppearance );

	//!
	ToolbarAppearance(wxWindow* parent, wxWindowID id = wxID_ANY, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxTB_HORIZONTAL | wxTB_FLAT, 
		const wxString& name = wxPanelNameStr);

	//!
	~ToolbarAppearance( );

	//!
	Core::BaseProcessor::Pointer GetProcessor( );

	//!
	void SetMultiRenderWindow( Core::Widgets::RenderWindowBase* val );

protected:
	//!
	void UpdateState();

	//! 
	void OnSelectLUT(wxCommandEvent &event);

	//! 
	void OnShowLUT(wxCommandEvent &event);

	//! 
	void OnOpacity(wxScrollEvent &event);

	//!
	void SetListBrowser(Core::Widgets::DataEntityListBrowser* val);

	//!
	void SetRenderingNode( mitk::DataTreeNode* renderingNode );

	//!
	void OnSelectedModified( );

	//!
	void OnNodeModified();

	//!
	MultiRenderWindowMITK* GetMultiRenderWindowMITK();

	DECLARE_EVENT_TABLE();
private:

	//!
	wxComboBox* m_ComboLUT;

	//!
	wxSlider* m_OpacitySlider;

	//!
	mitk::DataTreeNode* m_RenderingNode;

	//!
	unsigned long m_NodeTagModified;

};

} // namespace Widget
} // namespace Core

#endif // _coreToolbarAppearance_H
