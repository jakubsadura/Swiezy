/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef CORECLIPPINGWIDGET_H
#define CORECLIPPINGWIDGET_H

#include "gmWidgetsWin32Header.h"
#include "CILabNamespaceMacros.h"
#include "coreClippingSceneManager.h"
#include "coreClippingProcessor.h"
#include <wx/wx.h>
#include "wx/tglbtn.h"

namespace Core{

/** 
\brief This Widget allows mesh clipping for a better visualization of the mesh 
\ingroup gmWidgets
\author Chiara Riccobene
\date 
*/
class GMWIDGETS_EXPORT ClippingWidget : public wxPanel
{
public:
	//!
	ClippingWidget(wxWindow* parent);
	//! Widget Initialization
	void Init(
		Core::ClippingProcessor::Pointer processor,
		Core::ClippingSceneManager::Pointer sceneManager
	);

	//! Enables or Disables the Clipping Widget
	bool Enable(bool enable = true); 

private:
	//!
	void OnButtonClip(wxCommandEvent& event);
	//!
	void OnButtonShowPlane(wxCommandEvent& event);
	//!
	void OnButtonResetPlane(wxCommandEvent& event);
	//!
	void UpdateWidget();
	//!
	void UpdateDataHolders();
	/** Returns true iff all data holders are connected and all controls 
	have valid contents.
	*/
	bool Validate();

	void OnModifiedOutputDataEntity();

private:
	wxButton* GetButtonClip();
	wxToggleButton* GetButtonShowPlane() const;
	wxButton* GetButtonResetPlane() const;
	//! Returns true if the widget is in the clipping mode
	bool GetIsInClippingMode() const;
	//! Sets whether the widget is in the cropping mode
	void SetIsInClippingMode(bool flag);

private:
	Core::ClippingProcessor::Pointer m_Processor;
	Core::ClippingSceneManager::Pointer m_SceneManager;
};

} // Core)

#endif //CORECLIPPINGWIDGET_H
