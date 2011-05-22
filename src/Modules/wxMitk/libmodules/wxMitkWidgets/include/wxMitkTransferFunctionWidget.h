/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#ifndef wxMitkTransferFunctionWidget_H
#define wxMitkTransferFunctionWidget_H

#include "wxMitkWidgetsWin32Header.h"
#include <mitkProperties.h>
#include <wx/panel.h>

#include "mitkLookupTableProperty.h"
#include "mitkTransferFunction.h"
#include "wxMitkColorSelectorControl.h"
#include <wx/busyinfo.h>

class wxCheckBox;
class wxCommandEvent;
class wxComboBox;
class wxSlider;
class wxNotebook;

namespace mitk
{
class DataTreeNode;
class wxMitkScalarOpacityWidget;
class wxMitkScalarColorWidget;
class wxMitkGradientOpacityWidget;
class wxMitkRenderWindow;
class wxMitkColorSelectorControl;
class wxMitkLookupTableWidget;
class wxMitkRangeSliderControl;
class wxMitkWidgetUpdateEvent;
class wxMitkMaterialSettingsPanel;

/*
The wxMitkTransferFunctionWidget is a custom widget for editing transfer 
functions for volume rendering

\ingroup wxMitkWidgets
\author Juan Antonio Moya
\date 26 Nov 2007
*/
class WXMITKWIDGETS_EXPORT wxMitkTransferFunctionWidget : public wxPanel
{
public:

	wxMitkTransferFunctionWidget(
		wxWindow* parent, 
		wxWindowID id, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = 0, 
		const wxString& name = wxT("wxMitkTransferFunctionWidget"));
	virtual ~wxMitkTransferFunctionWidget(void);

	void SetDataTreeNode(mitk::DataTreeNode* node);

	/** 
	\brief Set/Get render window to show the legend. 
	\note This functionality should be inside MITK
	*/
	void SetMitkRenderWindow( mitk::wxMitkRenderWindow* renderWindow );

	mitk::wxMitkRenderWindow* GetMitkRenderWindow( ) const;


	/**
	\brief Disable the volume rendering option. This is used for images than do not have volume rendering properties, like a ROI.
	\param disable The value to set the rendering to.
	*/
	void DisableVolumeRendering(bool disable);

	//!
	void UpdateAll();

	//!
	void SetVolumeRenderingCheck(bool enableVR, bool enableXR, int vrThreshold = -1, int presetMode = -1);

private:
	//! disable/enable checkboxes and sliders related to volume rendering only
	void DisableVolumeRenderingSubProperties(bool disable);
	void UpdateTransferFunctionDependentWidgets();
	void SetPresets();
	void OnVolumeRenderingChanged(wxCommandEvent& event);
	void OnXRayVolumeRenderingChanged(wxCommandEvent& event);
	void OnTF_XR_ThresholdChanged( wxScrollEvent &event);
	void OnXRayVolRendAttenuationChanged( wxScrollEvent &event);
	void OnInteractiveApplyChanged(wxCommandEvent& event);
	void OnIntensitySliderChanged( wxScrollEvent &event);
	void OnComboPresets(wxCommandEvent& event);
	void OnGPUAccelerationChanged(wxCommandEvent& event);
	void OnLODChanged(wxCommandEvent& event);
	void OnOpenCloseBusyInfo(wxCommandEvent& event);
	void OnCroppingPlanesChanged(wxCommandEvent& event);
	void OnLightingChange( wxMitkWidgetUpdateEvent& event );

	void OnDeletePreviousNode();

	//!
	void ResetWidget(void);

	//!
	void InitTransferFunctionWidgets( bool resetTransferFunction = true );

	//!
	void UpdateVolumeRenderingProp( mitk::DataTreeNode* node );

	//!
	void UpdateWidgets( );

	//!
	void UpdateData( );

	//!
	void CleanWidgets( );

	//!
	void AddRenderObservers();

	//!
	void RemoveRenderObservers();

	//! Open/Close a busy info window 
	void SendOpenClosBInfoCommand(int openClose);

	/** Reset the min, max and actual range (and central value) for each 
	\  m_sliderCroppingRegionPlanes according to the actual values specified 
	\  in the CroppingRegionPlanes property of the volume mapper or to the 
	\   whole volume extent (if the property is not set). 
	\  If forceWholeExtent --> the extent will be reset to the volume extent in any case 
	**/
	void ResetCroppingSliders(bool forceWholeExtent = false);

private:

	bool m_updatingVolumeProperty;
	wxMitkScalarOpacityWidget* m_opacityWidget;
	wxMitkScalarColorWidget* m_colorWidget;
	wxMitkGradientOpacityWidget* m_gradientWidget;
	wxMitkMaterialSettingsPanel* m_pMaterialSettingsPane;
	wxCheckBox* m_chkVolumeRendering;
	wxCheckBox* m_chkXRayVolumeRendering;
	wxSlider* m_XRayVolRendThrSlider;
	wxSlider* m_XRayVolRendAttSlider;
	wxCheckBox* m_chkGPUAcceleration;
	wxCheckBox* m_chkLOD;
	wxSlider* m_OpacitySlider;
	wxStaticBox* sizer_1_staticbox;
	mitk::BoolProperty::Pointer m_volumeRenderingProp;
	mitk::BoolProperty::Pointer m_xRayVolumeRenderingProp;

	mitk::wxMitkRenderWindow* m_renderWindow;
	mitk::DataTreeNode* m_node;
	mitk::DataTreeNode* m_PreviousNode;
	itk::SimpleMemberCommand<wxMitkTransferFunctionWidget>::Pointer m_OnDeletePreviousNodeMemberCommand;
	itk::SimpleMemberCommand<wxMitkTransferFunctionWidget>::Pointer m_MemberCommand;

	wxNotebook* m_ImageNotebook;
	wxPanel* m_ImageSettingsNotebook_Pane;
	wxPanel* m_VolumeRenderingNotebook_Pane;

	//! To manage lookup table
	mitk::wxMitkLookupTableWidget *m_LookupTableWidget;

	mitk::TransferFunction::Pointer m_transferFunction;
	mitk::HistogramGenerator::HistogramType::ConstPointer m_GradientHistogram;
	//!
	wxComboBox* m_ComboPresets;

	wxStaticBox* staticSliderCroppingsPlanes;
	//!
	wxMitkRangeSliderControl* m_sliderCroppingRegionPlanes[3];

	//!
	unsigned long m_NodeTagModified;

	//!
	unsigned long m_DeletePreviousNodeTag;

	//! a busy info box when volume rendering is not finished
	wxBusyInfo* m_info;

	//! set true only when AddRenderObserver has been called
	bool m_hasRenderObserver;

	//! true if node has just changed (to avoid resetting the transferfunction)
	bool m_hasNodeChanged;

	//! true if new node has the same dataentityid of the previous one (no need to reinit tfunction
	bool m_transferfunctionAlreadyInited;
	bool m_gradientHistogramAlreadyInited;

	DECLARE_EVENT_TABLE();


	class vtkRendererCallback : public vtkCommand
	{
	public:
		vtkRendererCallback();
		static vtkRendererCallback *New();
		void Execute(vtkObject *caller, unsigned long event, void *callData);
		void SetCallerWindow(wxWindow *callerWin){ m_callerWin = callerWin;};
	
	private:
		wxWindow * m_callerWin;

	};

	vtkRendererCallback *m_renCallback;

};
 
} // namespace mitk


#endif // wxMitkTransferFunctionWidget_H
