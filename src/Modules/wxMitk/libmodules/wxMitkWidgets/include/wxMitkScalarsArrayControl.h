/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef wxMitkScalarsArrayControl_H
#define wxMitkScalarsArrayControl_H

#include "wxMitkWidgetsWin32Header.h"
#include "wxMitkWidgetUpdateEvent.h"
#include <wx/panel.h>

#include "blShapeUtils.h"

#include "mitkDataTreeNode.h"
#include "wxMitkRenderWindow.h"

class wxChoice;
class wxSpinCtrl;
class wxSpinEvent;

namespace mitk
{
class wxWidgetStackControl;
class wxMitkLookupTableWidget;

/*
The wxMitkScalarsArrayControl is a widget component used for selecting the
scalars arrays form a vtkDataSet

\ingroup wxMitkWidgets
\author Chiara Riccobene	
\date Dec 2008
*/

class WXMITKWIDGETS_EXPORT wxMitkScalarsArrayControl : public wxPanel
{
public:
	wxMitkScalarsArrayControl(
		wxWindow* parent, 
		wxWindowID id, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxBORDER_NONE, 
		const wxString& name = wxT("wxMitkScalarsArrayControl"));
	virtual ~wxMitkScalarsArrayControl(void);

	//!
	void ShowAdvancedControls( bool show );

	//! Set array of selectable scalar arrays
	void SetScalarArrayTypes(
		blShapeUtils::ShapeUtils::ScalarArrayType& scalar,
		bool bAddSolidColorArrayName );

	//! Set the range of the lut to the automatic one
	void SetAutomaticRange();
	
	//! Get selected scalar array
	blShapeUtils::ShapeUtils::VTKScalarType GetCurrentSelectedScalarType();

	//! Get selected scalar array index
	int GetScalarArrayIndex(void) const;	
	
	//! 
	bool SetSelectedArray( const char *arrayName );

	//!
	void SetMitkRenderWindow( mitk::wxMitkRenderWindow* renderWindow );
	void SetDataTreeNode( mitk::DataTreeNode::Pointer node );

private:
	void OnScalarArraySelected(wxCommandEvent& event);
	void OnSetRangeSelected(wxCommandEvent& event);
	void OnSetAutomaticRangeSelected(wxCommandEvent& WXUNUSED(event));
	
	wxChoice* m_cmbScalarArray;
	wxButton* m_rangeBtn;
	wxButton* m_automaticRangeBtn;
	wxStaticText* m_lblMinRange;
	wxStaticText* m_lblMaxRange;
	wxTextCtrl* m_txtMinRange;
	wxTextCtrl* m_txtMaxRange;
	blShapeUtils::ShapeUtils::ScalarArrayType m_currentScalarArray;
	blShapeUtils::ShapeUtils::VTKScalarType m_selectedScalarType;

	//! To manage lookup table
	mitk::wxMitkLookupTableWidget *m_LookupTableWidget;

	mitk::DataTreeNode::Pointer m_node;
	
	DECLARE_EVENT_TABLE();
};


} // namespace mitk


#endif // wxMitkScalarsArrayControl_H

