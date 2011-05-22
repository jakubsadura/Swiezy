/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreptRadiographVisualizationWidget_H
#define _coreptRadiographVisualizationWidget_H

#include "gmWidgetsWin32Header.h"
#include "ptRadiographVisualizationWidgetUI.h"
#include "coreInputControl.h"
#include "coreDRRVisualizationProcessor.h"
#include "coreProcessingWidget.h"



/** 
\brief Widget for Showing RadiographVisualizations

\ingroup MeshEditorPlugin
\author Chiara Riccobene
\date 26 August 2009
*/
class PLUGIN_EXPORT ptRadiographVisualizationWidget 
	: public ptRadiographVisualizationWidgetUI
{
public:
	coreDefineBaseWindowFactory( ptRadiographVisualizationWidget )

	//!
	ptRadiographVisualizationWidget(
		wxWindow* parent, 
		int id = wxID_ANY, 
		const wxPoint& pos=wxDefaultPosition, 
		const wxSize& size=wxDefaultSize, 
		long style=wxDEFAULT_DIALOG_STYLE);

	//!
	void Append( wxString file, wxString title = wxEmptyString );

	//!
	void OnModifiedOutputDataEntity( );

	//!
	Core::BaseProcessor::Pointer GetProcessor( );

private:
	//!
	void OnBtnShowProjection(wxCommandEvent &event);

	//!
	~ptRadiographVisualizationWidget(void);

private:
	//!
	Core::Widgets::InputControl* m_AcquireInput;

	//!
	Core::DRRVisualizationProcessor::Pointer m_DRRProcessor;
};

#endif // _coreptRadiographVisualizationWidget_H
