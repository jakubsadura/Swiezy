/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#ifndef corePointsTableWidget_H
#define corePointsTableWidget_H

#include <wx/panel.h>

#include <string>

#include "corePointsTableWidgetUI.h"
#include "gmWidgetsWin32Header.h"
#include "coreDataEntityHolder.h"
#include "coreCommonDataTypes.h"

#include "boost/signal.hpp"


namespace Core
{
namespace Widgets
{

	class RenderWindowBase;
/** 
\brief Widget with a table of points

Each time the input data is modified, the table will be refreshed 
automatically

The columns will be "Id" and all the vtkStringArray inside the input

If m_MultiRenderWindow is not NULL, the views are centered to the selected 
point

\ingroup gmWidgets
\author Xavi Planes
\date 31 July 2008
*/
class PLUGIN_EXPORT PointsTableWidget : public corePointsTableWidgetUI
{
public:
	typedef Core::DataHolder<int> SelectedItemHolderType;

public:
	PointsTableWidget(
		wxWindow* parent, 
		int id, 
		const wxPoint& pos=wxDefaultPosition, 
		const wxSize& size=wxDefaultSize, 
		long style=wxDEFAULT_DIALOG_STYLE);
	~PointsTableWidget(void);

	//! Set the polydata to show the points
	void SetInput( Core::DataEntityHolder::Pointer data );

	//! Selected item
	SelectedItemHolderType::Pointer GetSelectedItemHolder() const;

	//!
	void SetMultiRenderWindow(Core::Widgets::RenderWindowBase* val);

	//!
	void SetTimeStepHolder( Core::IntHolderType::Pointer timeStepHolder );

	//!
	bool Enable( bool enable = true );

	//!
	void UpdateListControl( );

private:

	//! Update table of points
	void OnModifiedInput( );

	//!
	void SetUpColumns( );

	//! Fill all items
	void FillItems( );

	//!
	void OnItemSelected(wxListEvent& event);

	//!
	void CenterViewsToLandmark( );

	DECLARE_EVENT_TABLE()

private:

	//! Input data
	Core::DataEntityHolder::Pointer m_InputDataHolder;

	//! Selected item
	SelectedItemHolderType::Pointer m_SelectedItemHolder;

	//! To center the views to the selected point
	Core::Widgets::RenderWindowBase* m_MultiRenderWindow;

	/** When the subject is modified, the time step of the m_InputDataHolder 
	will be shown
	*/
	Core::IntHolderType::Pointer m_timeStepHolder;

	//! Connection to m_InputDataHolder
	boost::signals::connection m_InputDataHolderConnection;
};

} // namespace Widgets
} // namespace Core

#endif // corePointsTableWidget_H
