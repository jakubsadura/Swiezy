/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreProcessorInputWidget_H
#define _coreProcessorInputWidget_H

#include "gmWidgetsWin32Header.h"
#include "coreBaseInputControl.h"
#include "coreDataEntity.h"
#include "wxCheckableControl.h"

namespace Core{ namespace Widgets{ class DataEntityListBrowser; } }

namespace Core
{
namespace Widgets 
{ 
/** 
Widget to show and set the input of a processor.

If you set bAutomaticSelection to true, the selected data entity form the
DataList will be selected automatically, else, a button will appear
to allow the user to set it as selected.

\sa AcquireInputControl, DataEntity
\ingroup gmWidgets
\author Xavi Planes
\date 06 Nov 2009
*/
class GMWIDGETS_EXPORT ProcessorInputWidget 
	: public BaseInputControl
{
public:
	coreClassNameMacro(ProcessorInputWidget);

	ProcessorInputWidget(
		wxWindow* parent, 
		wxWindowID id, 
		bool bAutomaticSelection = false,
		bool bSetDefaultDataEntity = true,
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxBORDER_NONE, 
		const wxString& name = "ProcessorInputWidget");
	virtual ~ProcessorInputWidget(void);

	//!
	void SetInputPort( Core::BaseFilterInputPort::Pointer inputPort );

	//!
	wxCheckableControl* GetCheckableControl() const;

protected:
	//! 
	bool SetAcquiredInputData( Core::DataEntity::Pointer &subject );

	//! Check if subject is ok to be selected or not
	bool CheckSubjectIsOk( DataEntity::Pointer &subject );

	//!
	void UpdateOptional( );

	//!
	void OnCheckBox(wxCommandEvent& event);

	DECLARE_EVENT_TABLE( );

protected:

	//!
	Core::BaseFilterInputPort::Pointer m_InputPort;

	//!
	wxCheckableControl* m_CheckableControl;
};

}
}


#endif // _coreProcessorInputWidget_H
