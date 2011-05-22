/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreBullEyePlotWidget_H
#define coreBullEyePlotWidget_H

#include <wx/panel.h>

#include <string>
#include <map>

#include "coreBullEyePlotWidgetUI.h"
#include "corePluginMacros.h"
#include "coreDataEntityHolder.h"
#include "coreCommonDataTypes.h"
#include "coreDataEntityListBrowser.h"
#include "coreInputControl.h"

#include "vtkSmartPointer.h"
#include "vtkDataObject.h"
#include "vtkFieldData.h"
#include "vtkScalarsToColors.h"

#include "wxMitkSelectableGLWidget.h"

#include "blSignal.h"

#include "vtkScalarsToColors.h"
#include "vtkSmartPointer.h"

namespace mitk{ 
	class wxMitkRenderWindow; 
	class wxMitkSelectableGLWidget;
}

class blVtkBullsEyeActor;

namespace Core
{
namespace Widgets
{

#define wxID_BullEyePlotWidget wxID("wxID_BullEyePlotWidget")

/** 
\brief Widget with a bell's eye plot

\ingroup CardioToolsPlugin
\author Chiara Riccobene
\date 19 Feb 2009
*/
class PLUGIN_EXPORT BullEyePlotWidget : 
	public coreBullEyePlotWidgetUI,
	public Core::BaseWindow
{

public:
	coreDefineBaseWindowFactory( BullEyePlotWidget )
	//!
	BullEyePlotWidget(
		wxWindow* parent, 
		int id = wxID_BullEyePlotWidget, 
		const wxPoint& pos=wxDefaultPosition, 
		const wxSize& size=wxDefaultSize, 
		long style=wxDEFAULT_DIALOG_STYLE);
	~BullEyePlotWidget(void);

	//!
	void SetListBrowser(	
		Core::Widgets::DataEntityListBrowser* dataEntityListBrowser );

	//!
	Core::BaseProcessor::Pointer GetProcessor( );

private:

	//!
	bool CheckInputs();
	//!
	void OnModifiedInput();

	//!
	void OnComboSelected( wxCommandEvent& WXUNUSED(event));

	//!
	void BuildRenderWindow( );

	//! Build specfici LUT for specific signal names
	void BuildLUTMap();

	//! Clean all arrays of m_dobj and the combo box
	void CleanDataObject( );

	//! Add all blSignal with size 17 from m_InputDataHolder to m_dobj
	void AddInputSignalsToDataObject( );

	//! Add a signal to m_dobj
	void AddSignalToDataObject( blSignal::Pointer signal );

	//! Refresh the VTK and MITK pipeline
	void RefreshPlot( );

	//!
	void SetSelectedSignal( int iSignal );

	//! Use the specific LUT for current signal name
	void UpdateLUT( );

	//! Update combo box
	void UpdateWidgets( );

	//!
	void SetTimeStep( int time );

	DECLARE_EVENT_TABLE()

private:

	//! Widget 
 	Core::Widgets::InputControl* m_AcquireInputWidget;

	//! Contains all vtkDataArrays
	vtkSmartPointer<vtkDataObject> m_DataObject;

	//! Input data
	Core::DataEntityHolder::Pointer m_InputDataHolder;

	//! Rendering window to plot the VTK actor
	mitk::wxMitkSelectableGLWidget*	m_wxMitkSelectableGLWidget;
	
	//! Bulls eye actor
	vtkSmartPointer<blVtkBullsEyeActor> m_bullsEyeActor;

	//!
	std::map< std::string, vtkSmartPointer<vtkScalarsToColors> > m_LUTMap;
};

} // namespace Widgets
} // namespace Core

#endif // coreBullEyePlotWidget_H
