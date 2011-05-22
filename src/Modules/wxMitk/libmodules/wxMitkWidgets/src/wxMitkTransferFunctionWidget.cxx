/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

// For compilers that don't support precompilation, include "wx/wx.h"

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
       #include <wx/wx.h>
#endif

#include <wx/notebook.h>

#include "wxMitkTransferFunctionWidget.h"
#include "wxMitkScalarOpacityWidget.h"
#include "wxMitkScalarColorWidget.h"
#include "wxMitkGradientOpacityWidget.h"
#include "wxMitkHistogramHelper.h"
#include "wxMitkRenderWindow.h"
#include "wxMitkColorSelectorControl.h"
#include "wxMitkLookupTableWidget.h"
#include "wxMitkRangeSliderControl.h"
#include "wxMitkWidgetUpdateEvent.h"
#include "wxMitkSurfaceLightingControl.h"

#include <mitkDataTreeNode.h>
#include <mitkTransferFunction.h>
#include <mitkTransferFunctionProperty.h>
#include <mitkRenderingManager.h>
#include "wxMitkMaterialSettingsPanel.h"

#include <vtkPiecewiseFunction.h>
#include "vtkSmartPointer.h"
#include <vtkCommand.h>
#include <mitkBaseVtkMapper3D.h>
#include <vtkVolumeProperty.h>
#include <vtkMitkVolumeTextureMapper3D.h>

#include "wxID.h"

#include "blMITKUtils.h"

using namespace mitk;

#define ID_wxMitkTransferFunctionWidget_Opacity wxID("ID_wxMitkTransferFunctionWidget_Opacity")
#define ID_wxMitkTransferFunctionWidget_Color wxID("ID_wxMitkTransferFunctionWidget_Color")
#define ID_wxMitkTransferFunctionWidget_Gradient wxID("ID_wxMitkTransferFunctionWidget_Gradient")
#define ID_chkVolumeRendering wxID("ID_chkVolumeRendering")
#define ID_chkXRayVolumeRendering wxID("ID_chkXRayVolumeRendering")
#define wxID_chkGPUAcceleration wxID("wxID_chkGPUAcceleration")
#define wxID_chkLOD wxID("wxID_chkLOD")
#define ID_sldXRayVolumeRenderingThr wxID("ID_sliderXRayVolumeRenderingThreshold")
#define ID_sldXRayVolumeRenderingAtt wxID("ID_sliderXRayVolumeRenderingAttenuation")
#define wxID_wxMitkTransferFunctionWidget_LookupTable wxID("wxID_wxMitkTransferFunctionWidget_LookupTable")
#define wxID_sliderOpacity wxID("ID_sliderOpacity")
#define wxID_ImageNotebook wxID("ID_ImageNotebook")
#define ID_wxMitkTransferFunctionWidget_Presets wxID("ID_wxMitkTransferFunctionWidget_Presets")
#define wxID_CROPPINGPLANE wxID("CroppingPlane")


//#define wxEVT_CLOSEINFO_EVENT wxID("wxCloseInfoEvent")

BEGIN_DECLARE_EVENT_TYPES()
	DECLARE_EVENT_TYPE(wxEVT_OPENCLOSEBUSYINFO, -1)
END_DECLARE_EVENT_TYPES()

DEFINE_EVENT_TYPE(wxEVT_OPENCLOSEBUSYINFO)


// Declare events to process
BEGIN_EVENT_TABLE(wxMitkTransferFunctionWidget, wxPanel)
	EVT_CHECKBOX            (ID_chkVolumeRendering, wxMitkTransferFunctionWidget::OnVolumeRenderingChanged)
	EVT_CHECKBOX            (ID_chkXRayVolumeRendering, wxMitkTransferFunctionWidget::OnXRayVolumeRenderingChanged)
	EVT_CHECKBOX            (wxID_chkGPUAcceleration, wxMitkTransferFunctionWidget::OnGPUAccelerationChanged)
	EVT_CHECKBOX            (wxID_chkLOD, wxMitkTransferFunctionWidget::OnLODChanged)
	//TODO: EVT_COMMAND_SCROLL_ENDSCROLL works only on MSW not on linux!!
	EVT_COMMAND_SCROLL_ENDSCROLL	(ID_sldXRayVolumeRenderingThr, wxMitkTransferFunctionWidget::OnTF_XR_ThresholdChanged)
	EVT_COMMAND_SCROLL_ENDSCROLL	(ID_sldXRayVolumeRenderingAtt, wxMitkTransferFunctionWidget::OnXRayVolRendAttenuationChanged)
	EVT_COMMAND_SCROLL      (wxID_sliderOpacity, wxMitkTransferFunctionWidget::OnIntensitySliderChanged)
	EVT_COMBOBOX            (ID_wxMitkTransferFunctionWidget_Presets,wxMitkTransferFunctionWidget::OnComboPresets )
	EVT_COMMAND             (wxID_ANY,wxEVT_OPENCLOSEBUSYINFO,wxMitkTransferFunctionWidget::OnOpenCloseBusyInfo)
	EVT_COMMAND             (wxID_CROPPINGPLANE, wxEVT_RANGE_CHANGED,wxMitkTransferFunctionWidget::OnCroppingPlanesChanged)
	//EVT_WIDGET_UPDATE       ( wxMitkMaterialSettingsPanel::wxID_lighting, wxMitkTransferFunctionWidget::OnLightingChange )
END_EVENT_TABLE()


#define HIGHRESAMPLING 0.5
#define FASTRESAMPLING 1

//!
wxMitkTransferFunctionWidget::wxMitkTransferFunctionWidget(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name) 
: wxPanel(parent, id, pos, size, style, name)
{
	m_renderWindow = NULL;
	m_node = NULL;
	m_PreviousNode = NULL;
	m_NodeTagModified = 0;
	m_DeletePreviousNodeTag = 0;

	SetId(wxID_ANY);
	m_ImageNotebook = new wxNotebook(this, wxID_ImageNotebook, wxDefaultPosition, wxDefaultSize, wxNB_TOP);
	m_ImageSettingsNotebook_Pane = new wxPanel(m_ImageNotebook, wxID_ANY);
	m_VolumeRenderingNotebook_Pane = new wxPanel(m_ImageNotebook, wxID_ANY);
	m_pMaterialSettingsPane = new wxMitkMaterialSettingsPanel( m_ImageNotebook, wxID_ANY );
	m_pMaterialSettingsPane->EnableControl( wxMitkMaterialSettingsPanel::SURFACE_REPRESENTATION_CONTROL, false );
	m_pMaterialSettingsPane->GetLightingControlPointer()->EnableControl( wxMitkSurfaceLightingControl::AMBIENT_COLOR_CONTROL, false );
	m_pMaterialSettingsPane->GetLightingControlPointer()->EnableControl( wxMitkSurfaceLightingControl::DIFFUSE_COLOR_CONTROL, false );
	m_pMaterialSettingsPane->GetLightingControlPointer()->EnableControl( wxMitkSurfaceLightingControl::SPECULAR_COLOR_CONTROL, false );
	m_pMaterialSettingsPane->GetLightingControlPointer()->EnableControl( wxMitkSurfaceLightingControl::OPACITY_CONTROL, false );
	m_pMaterialSettingsPane->GetLightingControlPointer()->EnableControl( wxMitkSurfaceLightingControl::LIGHTING_MODEL_CONTROL, false );

	m_pMaterialSettingsPane->Connect( wxMitkMaterialSettingsPanel::wxID_lighting, wxEVT_WIDGET_UPDATE,
	    wxMitkWidgetUpdateEventHandler( wxMitkTransferFunctionWidget::OnLightingChange ), NULL, this );
	
	m_LookupTableWidget = new wxMitkLookupTableWidget(m_ImageSettingsNotebook_Pane, wxID_wxMitkTransferFunctionWidget_LookupTable);
	sizer_1_staticbox = new wxStaticBox(m_ImageSettingsNotebook_Pane, -1, wxT("Opacity"));
	m_OpacitySlider = new wxSlider(m_ImageSettingsNotebook_Pane, wxID_sliderOpacity, 100, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_LABELS);

	m_chkVolumeRendering = new wxCheckBox(m_VolumeRenderingNotebook_Pane, ID_chkVolumeRendering, wxT("Enable VR"));
	m_chkXRayVolumeRendering = new wxCheckBox(m_VolumeRenderingNotebook_Pane, ID_chkXRayVolumeRendering, wxT("Enable XRay VR"));
	
	m_XRayVolRendThrSlider = new wxSlider(m_VolumeRenderingNotebook_Pane, ID_sldXRayVolumeRenderingThr, 50, 10, 90, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_LABELS);
	wxStaticText *xRayRendThr_statictext = new wxStaticText(m_VolumeRenderingNotebook_Pane, -1, wxT("Transfer Function threshold(%) (only for T.F. of types XR_*)"));

	m_XRayVolRendAttSlider = new wxSlider(m_VolumeRenderingNotebook_Pane, ID_sldXRayVolumeRenderingAtt, 30, 20, 500, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_LABELS);
	wxStaticText *xRayRendAtt_statictext = new wxStaticText(m_VolumeRenderingNotebook_Pane, -1, wxT("XRay Attenuation (/10) (Xray only)"));

	m_chkGPUAcceleration = new wxCheckBox(m_VolumeRenderingNotebook_Pane, wxID_chkGPUAcceleration, wxT("Use GPU Acceleration"));
	m_chkLOD = new wxCheckBox(m_VolumeRenderingNotebook_Pane, wxID_chkLOD, wxT("Use LOD"));

	for(int i=0;i<3;i++)
		m_sliderCroppingRegionPlanes[i] = new mitk::wxMitkRangeSliderControl(m_VolumeRenderingNotebook_Pane, 
			wxID_CROPPINGPLANE,0,20,-10,10,wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL); 

	m_opacityWidget = new wxMitkScalarOpacityWidget(m_VolumeRenderingNotebook_Pane, ID_wxMitkTransferFunctionWidget_Opacity);
	m_colorWidget = new wxMitkScalarColorWidget(m_VolumeRenderingNotebook_Pane, ID_wxMitkTransferFunctionWidget_Color);
	m_gradientWidget = new wxMitkGradientOpacityWidget(m_VolumeRenderingNotebook_Pane, ID_wxMitkTransferFunctionWidget_Gradient);

	wxString volumeRenderingPreset[ mitk::TransferFunction::TF_CT_DEFAULT + 1 ];
	volumeRenderingPreset[ mitk::TransferFunction::TF_CT_BLACK_WHITE ] = wxT( "CT_BLACK_WHITE" );
	volumeRenderingPreset[ mitk::TransferFunction::TF_CT_CARDIAC ] = wxT( "CT_CARDIAC" );
	volumeRenderingPreset[ mitk::TransferFunction::TF_CT_BONE ] = wxT( "CT_BONE" );
	volumeRenderingPreset[ mitk::TransferFunction::TF_CT_BONE_GRADIENT ] = wxT( "CT_BONE_GRADIENT" );
	volumeRenderingPreset[ mitk::TransferFunction::TF_MR_GENERIC ] = wxT( "MR_GENERIC" );
	volumeRenderingPreset[ mitk::TransferFunction::TF_MR_MIP ] = wxT( "MR_MIP" );
	volumeRenderingPreset[ mitk::TransferFunction::TF_XR_VR ] = wxT( "XR_VR" );
	volumeRenderingPreset[ mitk::TransferFunction::TF_3DRA ] = wxT( "3DRA" );
	volumeRenderingPreset[ mitk::TransferFunction::TF_XR_LINEAR_60 ] = wxT( "XR_LINEAR_60" );
	volumeRenderingPreset[ mitk::TransferFunction::TF_CT_DEFAULT ] = wxT( "CT_DEFAULT" );

	wxStaticText *preset_statictext = new wxStaticText(m_VolumeRenderingNotebook_Pane, -1, wxT("Transfer function"));
	m_ComboPresets = new wxComboBox(
		m_VolumeRenderingNotebook_Pane, 
		ID_wxMitkTransferFunctionWidget_Presets, 
		wxT(""), 
		wxDefaultPosition, 
		wxDefaultSize, 
		mitk::TransferFunction::TF_CT_DEFAULT + 1, 
		volumeRenderingPreset, 
		wxCB_DROPDOWN|wxCB_SIMPLE|wxCB_DROPDOWN|wxCB_READONLY);


	//m_ComboPresets->SetSelection( mitk::TransferFunction::TF_XR_LINEAR_60  );
	m_ComboPresets->SetSelection( mitk::TransferFunction::TF_3DRA  );

	m_updatingVolumeProperty = false;

	// Set default state
	CleanWidgets();

	// A sizer containing all the image setting related stuff
	wxBoxSizer* imageSettingSizer = new wxBoxSizer(wxVERTICAL);
	imageSettingSizer->Add( m_LookupTableWidget, 0 );
	imageSettingSizer->AddSpacer(16);
	wxStaticBoxSizer* sizer_1 = new wxStaticBoxSizer(sizer_1_staticbox, wxVERTICAL);
	sizer_1->Add(m_OpacitySlider, 0, wxEXPAND, 0);
	imageSettingSizer->Add(sizer_1,0, wxEXPAND, 0);
	imageSettingSizer->AddStretchSpacer(1);
	m_ImageSettingsNotebook_Pane->SetSizer(imageSettingSizer);

	// A sizer containing all the volume renderization related widgets
	wxBoxSizer* volumeRenderingSizer = new wxBoxSizer(wxVERTICAL);
	volumeRenderingSizer->AddSpacer(10);
	volumeRenderingSizer->Add(m_chkVolumeRendering, 0, wxEXPAND);
	volumeRenderingSizer->AddSpacer(10);
	volumeRenderingSizer->Add(m_chkXRayVolumeRendering, 0, wxEXPAND);
	
	volumeRenderingSizer->AddSpacer(8);
	volumeRenderingSizer->Add(xRayRendThr_statictext,0,wxEXPAND);
	volumeRenderingSizer->AddSpacer(3);
	volumeRenderingSizer->Add(m_XRayVolRendThrSlider,0,wxEXPAND);
	volumeRenderingSizer->AddSpacer(5);
	volumeRenderingSizer->Add(xRayRendAtt_statictext,0,wxEXPAND);
	volumeRenderingSizer->AddSpacer(3);
	volumeRenderingSizer->Add(m_XRayVolRendAttSlider,0,wxEXPAND);

	volumeRenderingSizer->AddSpacer(5);
	volumeRenderingSizer->Add(m_chkGPUAcceleration, 0, wxEXPAND);
	volumeRenderingSizer->Add(m_chkLOD, 0, wxEXPAND);


	volumeRenderingSizer->AddSpacer(2);
	staticSliderCroppingsPlanes = new wxStaticBox(m_VolumeRenderingNotebook_Pane, -1, wxT("Cropping Planes"));
	wxStaticBoxSizer* sizer_cropping = new wxStaticBoxSizer(staticSliderCroppingsPlanes, wxVERTICAL);
	for(int i=0;i<3;i++)
		sizer_cropping->Add(m_sliderCroppingRegionPlanes[i], 0, wxEXPAND );
	volumeRenderingSizer->Add(sizer_cropping, 0, wxEXPAND, 0);

	volumeRenderingSizer->AddSpacer(16);
	volumeRenderingSizer->Add(preset_statictext, 0, wxEXPAND);
	volumeRenderingSizer->AddSpacer(3);
	volumeRenderingSizer->Add(m_ComboPresets, 0, wxEXPAND);
	volumeRenderingSizer->AddSpacer(16);
	volumeRenderingSizer->Add(m_opacityWidget, 0, wxEXPAND);
	volumeRenderingSizer->Add(m_colorWidget, 0, wxEXPAND);
	volumeRenderingSizer->Add(m_gradientWidget, 0, wxEXPAND);
	
	
	m_VolumeRenderingNotebook_Pane->SetSizer(volumeRenderingSizer);


	// Layout the canvases
	wxBoxSizer* layout = new wxBoxSizer(wxVERTICAL);
	m_ImageNotebook->AddPage(m_ImageSettingsNotebook_Pane,wxT("Image Settings"));
	m_ImageNotebook->AddPage(m_VolumeRenderingNotebook_Pane,wxT("Volume Rendering"));

	m_ImageNotebook->AddPage( m_pMaterialSettingsPane, wxT( "Lighting settings" ) );

	layout->Add(m_ImageNotebook,1, wxEXPAND, 10);
	SetSizer(layout);
	layout->Fit(this);
	Layout();

	m_transferFunction = mitk::TransferFunction::New();
	m_GradientHistogram = NULL;
	m_renderWindow = NULL;
	m_renCallback = NULL;
	m_info = NULL;	
	m_hasRenderObserver = false;
	m_hasNodeChanged = false;


	m_transferfunctionAlreadyInited = false;
	m_gradientHistogramAlreadyInited = false;
}

//!
wxMitkTransferFunctionWidget::~wxMitkTransferFunctionWidget(void)
{
	RemoveRenderObservers();

	if ( m_node )
	{
		m_node->GetPropertyList()->RemoveObserver( m_NodeTagModified );
	}
	if ( m_PreviousNode )
	{
		m_PreviousNode->RemoveObserver( m_DeletePreviousNodeTag );
	}
}

void wxMitkTransferFunctionWidget::RemoveRenderObservers()
{
	if((m_renderWindow==NULL) ||(!m_hasRenderObserver))
		return;

	vtkSmartPointer<vtkRenderer> vtkrenderer;
	vtkrenderer = mitk::BaseRenderer::GetInstance(m_renderWindow->GetRenderWindow())->GetVtkRenderer();
	
	if(m_renCallback!=NULL)
	{
		vtkrenderer->RemoveObserver(m_renCallback);
	}
	m_hasRenderObserver = false;
}

void wxMitkTransferFunctionWidget::AddRenderObservers()
{
	if(m_renderWindow==NULL)
		return;

	this->SendOpenClosBInfoCommand(2); //open a new info window

	vtkSmartPointer<vtkRenderer> vtkrenderer;
	vtkrenderer = mitk::BaseRenderer::GetInstance(m_renderWindow->GetRenderWindow())->GetVtkRenderer();

	if(m_renCallback==NULL)
	{
		m_renCallback = vtkRendererCallback::New();
		m_renCallback->SetCallerWindow(this);
	}


	if(!vtkrenderer->HasObserver(vtkCommand::StartEvent))
		vtkrenderer->AddObserver(vtkCommand::StartEvent, m_renCallback );

	if(!vtkrenderer->HasObserver(vtkCommand::EndEvent))
		vtkrenderer->AddObserver(vtkCommand::EndEvent, m_renCallback );

	m_hasRenderObserver = true;
}

//!
void wxMitkTransferFunctionWidget::ResetWidget(void)
{
	SetDataTreeNode( NULL );
}

/**
This is the call for making the widget work with an image (in fact, a DataTreeNode that contains an image)
Manages a DataTreeNode, getting the image inside and computing its scalar histogram and gradient 
histogram. It also creates the TransferFunctionProperty for the given node and assigns it.
Also transfers all the computed histograms and data to the children canvases.
\note Design decision: all work of analyzing the image is done here, since it implies a lot of computations and we want this widget to be efficient and not to repeat code (like it would happen if we rely this work to the children canvases)
*/

bool HasValidTransferFunction( mitk::DataTreeNode* node )
{
	if( node )
	{
		mitk::TransferFunctionProperty::Pointer transferFunctionProp = NULL;
		transferFunctionProp = static_cast<mitk::TransferFunctionProperty*>( node->GetProperty("TransferFunction") );
		mitk::TransferFunction::Pointer transferFunction = NULL;
		if( transferFunctionProp.IsNotNull() )
		{
			transferFunction = static_cast<mitk::TransferFunction*>( transferFunctionProp->GetValue().GetPointer() );
			if( transferFunction.IsNotNull() )
			{
				return ( transferFunction->GetHistogram() != NULL );
			}
		}
	}
	return false;
}

void wxMitkTransferFunctionWidget::OnDeletePreviousNode()
{
	m_PreviousNode = NULL;
}

void wxMitkTransferFunctionWidget::SetDataTreeNode(mitk::DataTreeNode* node)
{
	if( m_node )
	{
		bool previousNodeHasValidHistogram = HasValidTransferFunction( m_PreviousNode );
		bool currentNodeHasValidHistogram = HasValidTransferFunction( m_node );
		if( ( !previousNodeHasValidHistogram ) ||
		    ( previousNodeHasValidHistogram && currentNodeHasValidHistogram ) )
		{
			if ( m_PreviousNode )
			{
				m_PreviousNode->RemoveObserver( m_DeletePreviousNodeTag );
				m_DeletePreviousNodeTag = 0;
			}

			m_PreviousNode = m_node;
			if( m_OnDeletePreviousNodeMemberCommand.IsNull() )
			{
				m_OnDeletePreviousNodeMemberCommand = itk::SimpleMemberCommand<wxMitkTransferFunctionWidget>::New();
			}
			m_OnDeletePreviousNodeMemberCommand->SetCallbackFunction(
				this,
				&wxMitkTransferFunctionWidget::OnDeletePreviousNode );
			m_DeletePreviousNodeTag = m_PreviousNode->AddObserver( itk::DeleteEvent(), m_OnDeletePreviousNodeMemberCommand );
		}
	}
	
	if ( m_node == node )
	{
		return;
	}

	if ( m_node )
	{
		m_node->GetPropertyList()->RemoveObserver( m_NodeTagModified );
	}
	
	mitk::IntProperty::Pointer dataEntityIdOldProperty, dataEntityIdNewProperty;

	bool argNodeWasNull = true;
	if( node != NULL )
	{
		dataEntityIdNewProperty = static_cast<mitk::IntProperty*>( node->GetProperty( "dataentityid" ) );
		argNodeWasNull = false;
	}
	bool thisNodeWasNull = true;
	if( m_node != NULL )
	{
		dataEntityIdOldProperty = static_cast<mitk::IntProperty*>( m_node->GetProperty( "dataentityid" ) );
		thisNodeWasNull = false;
	}
	
	m_node = node;

	//! do not reset the transfer function!!
	m_hasNodeChanged = true;
	if ( m_node )
	{
		// Add an observer to the node, to remove the actor when the node is deleted
		if( m_MemberCommand.IsNull() )
		{
			m_MemberCommand = itk::SimpleMemberCommand<wxMitkTransferFunctionWidget>::New();
		}
		m_MemberCommand->SetCallbackFunction(
			this,
			&wxMitkTransferFunctionWidget::UpdateWidgets );
		m_NodeTagModified = m_node->GetPropertyList()->AddObserver(
			itk::ModifiedEvent(), m_MemberCommand );
	}

	// If we are not passing a node, then left the values as it was, because it could
	// have been assigned previously by assigning a node (i.e. Changing plug-in tabs)
	bool mustInitTransferFunction = false;
	// We where not managing a previous node, so the transfer function for the passed
	// node must be calculated
	if( !m_PreviousNode )
	{
		mustInitTransferFunction = true;
	}
	else
	{
		if( m_node )
		{
			// Is the previous node is NOT exactly the same as current node
			if( m_PreviousNode != m_node )
			{
				// Try to see if it has been externaly marked as the same node by just comparing a propery
				//dataEntityIdOldProperty = static_cast<mitk::IntProperty*>( m_PreviousNode->GetProperty( "dataentityid" ) );
				//dataEntityIdNewProperty = static_cast<mitk::IntProperty*>( m_node->GetProperty( "dataentityid" ) );
				//m_transferfunctionAlreadyInited = ((dataEntityIdNewProperty.IsNotNull()) && (dataEntityIdOldProperty.IsNotNull()) && 
				//	dataEntityIdNewProperty->GetValue()==dataEntityIdOldProperty->GetValue());
				if( m_PreviousNode->GetData() == m_node->GetData() )
				{
					m_transferfunctionAlreadyInited = true;
					// But the current rendering node may not have the transfer function,
					// so copy if from the previous rendering node

					mitk::TransferFunctionProperty::Pointer transferFunctionProp = NULL;
					mitk::TransferFunctionProperty::Pointer previousNodeTransferFunctionProp = NULL;
					transferFunctionProp = static_cast<mitk::TransferFunctionProperty*>( m_node->GetProperty("TransferFunction") );
					previousNodeTransferFunctionProp = static_cast<mitk::TransferFunctionProperty*>( m_PreviousNode->GetProperty( "TransferFunction" ) );
					mitk::TransferFunction::Pointer transferFunction = NULL;
					mitk::TransferFunction::Pointer previousNodeTransferFunction = NULL;
					if( transferFunctionProp.IsNotNull() )
					{
						transferFunction = static_cast<mitk::TransferFunction*>( transferFunctionProp->GetValue().GetPointer() );
					}
					if( previousNodeTransferFunctionProp.IsNotNull() )
					{
						previousNodeTransferFunction = static_cast<mitk::TransferFunction*>( previousNodeTransferFunctionProp->GetValue().GetPointer() );
					}
					if( previousNodeTransferFunction.IsNotNull() && transferFunction.IsNotNull() )
					{
						if( previousNodeTransferFunction->GetHistogram() && !transferFunction->GetHistogram() )
						{
							transferFunction->InitializeByItkHistogram( previousNodeTransferFunction->GetHistogram() );
						}
						else if( !previousNodeTransferFunction->GetHistogram() && transferFunction->GetHistogram() )
						{
							previousNodeTransferFunction->InitializeByItkHistogram( transferFunction->GetHistogram() );
						}
					}
				}
				else
				{
					m_transferfunctionAlreadyInited = m_PreviousNode->GetData() == m_node->GetData();
				}
				
			}
		}
	}

	m_LookupTableWidget->SetDataTreeNode( node );

	//Try to reset the sliders values according to the info stored in the CroppingRegionPlanes
	//otherwise, reset to the whole image extent
	ResetCroppingSliders(false);


	UpdateWidgets();
	m_hasNodeChanged = false;
	//m_transferfunctionAlreadyInited = false;
	//m_gradientHistogramAlreadyInited = false;
}


void wxMitkTransferFunctionWidget::DisableVolumeRenderingSubProperties(bool disable)
{
	if(disable)
	{
		m_chkXRayVolumeRendering->SetValue(false);
	}

	m_chkXRayVolumeRendering->Enable(!disable);
	m_chkGPUAcceleration->Enable(!disable);
	m_chkLOD->Enable(!disable);
	for(int i=0;i<3;i++)
		m_sliderCroppingRegionPlanes[i]->Enable(!disable);
}

void wxMitkTransferFunctionWidget::DisableVolumeRendering(bool disable)
{
	m_chkVolumeRendering->Enable(!disable);
	if(m_chkVolumeRendering->IsEnabled())
		if(m_chkVolumeRendering->GetValue())
			DisableVolumeRenderingSubProperties(false);
	else
		DisableVolumeRenderingSubProperties(true);
}


//!
void wxMitkTransferFunctionWidget::OnVolumeRenderingChanged(wxCommandEvent& event)
{
	SetVolumeRenderingCheck(m_chkVolumeRendering->GetValue(), false);
}

//!
void wxMitkTransferFunctionWidget::OnTF_XR_ThresholdChanged( wxScrollEvent &event)
{
	if( ( m_ComboPresets->GetSelection( ) == mitk::TransferFunction::TF_XR_VR ) || 
		( m_ComboPresets->GetSelection( ) == mitk::TransferFunction::TF_3DRA ) || 
		( m_ComboPresets->GetSelection( ) == mitk::TransferFunction::TF_XR_LINEAR_60 ) )
	{
//		int currSel = m_ComboPresets->GetSelection( );
//		m_ComboPresets->SetSelection(mitk::TransferFunction::TF_MR_GENERIC);

		//bool oldTransferfunctionAlreadyInited = m_transferfunctionAlreadyInited;
		//m_transferfunctionAlreadyInited = true;
//		SetPresets();
//		m_ComboPresets->SetSelection(currSel);
		m_transferFunction->SetXRVRThreshold(m_XRayVolRendThrSlider->GetValue()/100.0);
		SetPresets();
		//m_transferfunctionAlreadyInited = oldTransferfunctionAlreadyInited;
	}
}

void wxMitkTransferFunctionWidget::OnXRayVolRendAttenuationChanged( wxScrollEvent &event)
{
	if(m_chkXRayVolumeRendering->GetValue())
		SetPresets();
}


//!
void wxMitkTransferFunctionWidget::OnXRayVolumeRenderingChanged(wxCommandEvent& event)
{
	SetVolumeRenderingCheck(true, m_chkXRayVolumeRendering->GetValue());
}

void wxMitkTransferFunctionWidget::SetVolumeRenderingCheck(bool enableVR, bool enableXR,
															int vrThreshold, /*= -1*/
															int presetMode /*= -1*/)
{
	if((enableXR) && (!enableVR))
		enableXR = false;

	m_chkVolumeRendering->SetValue(enableVR);
	DisableVolumeRenderingSubProperties(!enableVR);
	
	m_chkXRayVolumeRendering->SetValue(enableXR);
	vtkRenderWindow *renWin=this-> m_renderWindow->GetRenderWindow();
	vtkSmartPointer<vtkRenderer> vtkrenderer;
	vtkrenderer = mitk::BaseRenderer::GetInstance(renWin)->GetVtkRenderer();
	if(enableXR)
	{
		vtkrenderer->SetBackground(1,1,1);
	}
	else
	{
		vtkrenderer->SetBackground(0,0,0);
		//restore default volume rendering lookup table
//		m_ComboPresets->SetSelection(mitk::TransferFunction::TF_XR_LINEAR_60);
//		SetPresets();
	}
	UpdateAll();
	if((vrThreshold>0) && (vrThreshold<100))
	{
		m_XRayVolRendThrSlider->SetValue(vrThreshold);
		wxScrollEvent dummyEvent;
		OnTF_XR_ThresholdChanged(dummyEvent);
	}
	if((presetMode>=0) && (presetMode<=mitk::TransferFunction::TF_CT_DEFAULT))
		m_ComboPresets->SetSelection( presetMode );
}

//!
void wxMitkTransferFunctionWidget::OnIntensitySliderChanged(wxScrollEvent &event)
{
	float sliderValue = static_cast<float>(m_OpacitySlider->GetValue());
	m_node->SetOpacity(sliderValue/100);
	mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}


void mitk::wxMitkTransferFunctionWidget::SetMitkRenderWindow( mitk::wxMitkRenderWindow* renderWindow )
{
	m_renderWindow = renderWindow;
	m_LookupTableWidget->SetMitkRenderWindow( renderWindow );
}

mitk::wxMitkRenderWindow* mitk::wxMitkTransferFunctionWidget::GetMitkRenderWindow( ) const
{
	return m_renderWindow;
}

void mitk::wxMitkTransferFunctionWidget::UpdateWidgets()
{
	if( m_updatingVolumeProperty )
		return;

	if ( m_node == NULL )
	{
		CleanWidgets( );
		return;
	}

	bool volumeRenderingEnabled = false;
	m_volumeRenderingProp = static_cast<mitk::BoolProperty*>(m_node->GetProperty("volumerendering"));
	if ( m_volumeRenderingProp.IsNotNull() )
	{
		volumeRenderingEnabled = m_volumeRenderingProp->GetValue();
	}

	bool xRayVolumeRenderingEnabled = false;
	m_xRayVolumeRenderingProp = static_cast<mitk::BoolProperty*>(m_node->GetProperty("xrayvolumerendering"));
	if ( m_xRayVolumeRenderingProp.IsNotNull() )
	{
		xRayVolumeRenderingEnabled = m_xRayVolumeRenderingProp->GetValue();
	}

	bool GPUAccelerationEnabled = true;
	mitk::BoolProperty::Pointer prop = static_cast<mitk::BoolProperty*>(m_node->GetProperty("volumerendering.usegpu"));
	if ( prop.IsNotNull() )
	{
		GPUAccelerationEnabled = prop->GetValue();
	}

	bool LODEnabled = true;
	prop = static_cast<mitk::BoolProperty*>(m_node->GetProperty("volumerendering.uselod"));
	if ( prop.IsNotNull() )
	{
		LODEnabled = prop->GetValue();
	}

	DisableVolumeRenderingSubProperties(!volumeRenderingEnabled);
	m_chkVolumeRendering->SetValue( volumeRenderingEnabled );
	m_chkXRayVolumeRendering->SetValue( xRayVolumeRenderingEnabled );  
	m_chkGPUAcceleration->SetValue(GPUAccelerationEnabled);
	m_chkLOD->SetValue(LODEnabled);
	if(xRayVolumeRenderingEnabled)
		m_chkXRayVolumeRendering->Enable( xRayVolumeRenderingEnabled );
	m_opacityWidget->Enable( volumeRenderingEnabled );
	m_colorWidget->Enable( volumeRenderingEnabled );
	m_gradientWidget->Enable( volumeRenderingEnabled );

	if( m_chkVolumeRendering->GetValue() )
	{
		if( m_transferfunctionAlreadyInited )
		{
			if( m_node )
			{
				UpdateTransferFunctionDependentWidgets();
			}
		}
		else
		{
			InitTransferFunctionWidgets(!m_hasNodeChanged);
		}
	}
		

	// Update the value of the slider
	//float sliderValue = static_cast<float>(m_OpacitySlider->GetValue());
	float opacity;
	m_node->GetOpacity(opacity, NULL);
	m_OpacitySlider->SetValue(opacity*100);


	double currThr = this->m_transferFunction->GetXRVRThreshold()*100;
	m_XRayVolRendThrSlider->SetValue(currThr);
}

void mitk::wxMitkTransferFunctionWidget::UpdateData()
{
	UpdateVolumeRenderingProp( m_node );

	// update renderingon 
	mitk::RenderingManager::GetInstance()->SetNextLOD(0);
	mitk::RenderingManager::GetInstance()->RequestUpdateAll( 
		mitk::RenderingManager::REQUEST_UPDATE_3DWINDOWS );
}

bool TreeNodeReallyModified( mitk::DataTreeNode* pNode )
{
	bool reallyModified = false;

	const PropertyList::PropertyMap* pPropertyMap = pNode->GetPropertyList()->GetMap();

	for( PropertyList::PropertyMap::const_iterator it = pPropertyMap->begin();
		it != pPropertyMap->end(); ++it )
	{
		if( it->second.first.IsNull() )
		{
			continue;
		}
		if( pNode->GetMTime() < it->second.first->GetMTime() )
		{
			int a = 1;
			break;
		}
	}

	return reallyModified;
}


void mitk::wxMitkTransferFunctionWidget::InitTransferFunctionWidgets( bool resetTransferFunction /*true*/ )
{
	mitk::TransferFunctionProperty::Pointer transferFunctionProp = NULL;
	mitk::Image::Pointer image;
	
	// Check the transferFunction property
	image = static_cast<mitk::Image*>(m_node->GetData());
	if (image.IsNull()) 
	{
		return;
	}

	// Check if it already has a TransferFunctionProperty
	transferFunctionProp = static_cast<mitk::TransferFunctionProperty*>(m_node->GetProperty("TransferFunction"));
	if (transferFunctionProp.IsNull())
	{
		// If it had not a TransferFunction already, lets create one and assign the property to the node
		mitk::TransferFunction::Pointer transferFunction = mitk::TransferFunction::New();
		m_node->SetProperty("TransferFunction", mitk::TransferFunctionProperty::New(transferFunction));
	}

	// this takes too long 
	// Get TransferFunction
	bool transferfunctionAlreadyInitedNewValue = m_transferfunctionAlreadyInited;
	m_transferFunction = static_cast<mitk::TransferFunction*>(transferFunctionProp->GetValue().GetPointer());
	if (m_transferFunction.IsNotNull())
	{
		bool transferFunctionOutdated = m_transferFunction->GetMTime() < m_node->GetMTime();
		if( (!m_transferfunctionAlreadyInited) && transferFunctionOutdated )
		{
			// Let's see if the node was modified because just one of its properties was modified, and that
			// property does not affect the transfer function created data
			if( transferFunctionOutdated )
			{
				mitk::HistogramGenerator::HistogramType::ConstPointer histogram;
				histogram = wxMitkHistogramHelper::ComputeHistogram( image );
				m_transferFunction->InitializeByItkHistogram( histogram );
				//m_transferFunction->InitializeByMitkImage( image );
				m_transferFunction->Modified();
				m_gradientHistogramAlreadyInited = false;
			}
		}
		transferfunctionAlreadyInitedNewValue = true;
	}
	else
	{
		//transferfunctionproperty has not been correctly created!!
		return;
	}
	if(resetTransferFunction)
		m_transferFunction->SetTransferFunctionMode( m_ComboPresets->GetSelection( ) );
	else
		m_ComboPresets->SetSelection( m_transferFunction->GetTransferFunctionMode() ); 
		
	m_transferFunction->SetXRVRThreshold(m_XRayVolRendThrSlider->GetValue()/100.0);


	// Set the values to the canvases
	m_opacityWidget->SetHistogram(m_transferFunction->GetHistogram());
	m_opacityWidget->SetPiecewiseFunction(m_transferFunction->GetScalarOpacityFunction());

	m_colorWidget->SetHistogram(m_transferFunction->GetHistogram());
	m_colorWidget->SetColorTransferFunction(m_transferFunction->GetColorTransferFunction());

	if( !m_gradientHistogramAlreadyInited )
	{
		m_GradientHistogram = wxMitkHistogramHelper::GenerateGradientHistogram( image );
		m_gradientWidget->SetHistogram( m_GradientHistogram.GetPointer() );
		m_gradientWidget->SetPiecewiseFunction( m_transferFunction->GetGradientOpacityFunction() );
		m_gradientHistogramAlreadyInited = true;
	}
	if(!resetTransferFunction)
	{
		this->m_opacityWidget->Refresh();
		this->m_colorWidget->Refresh();
		this->m_gradientWidget->Refresh();
	}

	m_transferfunctionAlreadyInited = transferfunctionAlreadyInitedNewValue;
}

void mitk::wxMitkTransferFunctionWidget::UpdateVolumeRenderingProp( mitk::DataTreeNode* node )
{
	m_updatingVolumeProperty = true;
	mitk::BaseVtkMapper3D *mapper3D = NULL;
	vtkVolume *mapperVolume = NULL;
	
	// Enable volume rendering property
	m_volumeRenderingProp = static_cast<mitk::BoolProperty*> (node->GetProperty("volumerendering"));
	if(m_volumeRenderingProp.IsNull())
	{
		m_volumeRenderingProp = mitk::BoolProperty::New(false);
		node->SetProperty("volumerendering", m_volumeRenderingProp);
	}

	// Check the volume rendering checkbox
	m_volumeRenderingProp->SetValue( m_chkVolumeRendering->GetValue() );

	if(!m_volumeRenderingProp->GetValue())
	{
		RemoveRenderObservers();
		DisableVolumeRenderingSubProperties(true);
	}
	else
	{
		AddRenderObservers();

		mitk::BoolProperty::Pointer prop = static_cast<mitk::BoolProperty*>(node->GetProperty("volumerendering.uselod")); 
		mitk::BaseRenderer * renderer = mitk::BaseRenderer::GetInstance(m_renderWindow->GetRenderWindow());
		if(prop.IsNull())
		{
			prop = mitk::BoolProperty::New(false);
			node->SetProperty("volumerendering.uselod", prop);
		}
		prop->SetValue(m_chkLOD->GetValue());

		prop = static_cast<mitk::BoolProperty*>(node->GetProperty("volumerendering.usegpu"));
		if(prop.IsNull())
		{
			prop = mitk::BoolProperty::New(false);
			node->SetProperty("volumerendering.usegpu", prop);
		}
		prop->SetValue(m_chkGPUAcceleration->GetValue());

		//node->AddProperty( "volumerendering.uselod", mitk::BoolProperty::New( true ), renderer, true );
		//should we use gpu acceleration? well..volume raycast (used by cpu) is much more accurate; texture3d
		//(used by gpu) are not so good
		//node->AddProperty( "volumerendering.usegpu", mitk::BoolProperty::New( true ), renderer, true );

		mapper3D =static_cast<mitk::BaseVtkMapper3D *> (node->GetMapper(mitk::BaseRenderer::Standard3D));
		if(mapper3D==NULL)
			return;

		mapperVolume = static_cast<vtkVolume *> (mapper3D->GetVtkProp(renderer));
		if(mapperVolume==NULL)
			return;

		vtkVolumeProperty* volumeProperty = mapperVolume->GetProperty();

		//volumeProperty->SetAmbient (0.0f); //0.05f
		//volumeProperty->SetDiffuse (0.45f); //0.45f
		//volumeProperty->SetSpecular(0.20f); //0.50f
		//volumeProperty->SetSpecularPower(0.0f);
		volumeProperty->SetAmbient( m_pMaterialSettingsPane->GetLightingControl().GetAmbientCoefficient() );
		volumeProperty->SetDiffuse ( m_pMaterialSettingsPane->GetLightingControl().GetDiffuseCoefficient() );
		volumeProperty->SetSpecular( m_pMaterialSettingsPane->GetLightingControl().GetSpecularCoefficient() );
		volumeProperty->SetSpecularPower( m_pMaterialSettingsPane->GetLightingControl().GetSpecularPower() );

		mapperVolume->SetProperty(volumeProperty);

	}

	// Enable xray volume rendering property
	m_xRayVolumeRenderingProp = static_cast<mitk::BoolProperty*>(node->GetProperty("xrayvolumerendering"));
	if(m_xRayVolumeRenderingProp.IsNull())
	{
		m_xRayVolumeRenderingProp = mitk::BoolProperty::New(false);
		node->SetProperty("xrayvolumerendering", m_xRayVolumeRenderingProp);
	}

	// Check the xray volume rendering checkbox
	m_xRayVolumeRenderingProp->SetValue( m_chkXRayVolumeRendering->GetValue() );


	if(m_xRayVolumeRenderingProp->GetValue())
	{
		//m_ComboPresets->SetSelection(mitk::TransferFunction::TF_XR_VR);
		m_ComboPresets->SetSelection(mitk::TransferFunction::TF_XR_LINEAR_60);
		SetPresets();

		//to increase the resolution one should also take the texture3d mapper (only in case of use gpu and not xray) and
		 
		if((mapperVolume!=NULL) && (m_chkGPUAcceleration->GetValue()))
		{
			vtkMitkVolumeTextureMapper3D  *vtkTextureMapper = static_cast <vtkMitkVolumeTextureMapper3D *> (mapperVolume->GetMapper());
			vtkTextureMapper->SetUseCompressedTexture(false);  //default from MITK is true!
		}


	}
	m_updatingVolumeProperty = false;

}

void mitk::wxMitkTransferFunctionWidget::UpdateTransferFunctionDependentWidgets()
{
	mitk::TransferFunctionProperty::Pointer transferFunctionProp = NULL;

	// Check if it already has a TransferFunctionProperty
	transferFunctionProp = static_cast<mitk::TransferFunctionProperty*>( m_node->GetProperty( "TransferFunction" ) );
	if( transferFunctionProp.IsNull() )
	{
		return;
	}

	m_transferFunction = static_cast<mitk::TransferFunction*>( transferFunctionProp->GetValue().GetPointer() );
	if( m_transferFunction.IsNull() )
	{
		return;
	}

	m_transferFunction->SetTransferFunctionMode( m_ComboPresets->GetSelection( ) );
	m_opacityWidget->SetHistogram( m_transferFunction->GetHistogram() );
	m_opacityWidget->SetPiecewiseFunction( m_transferFunction->GetScalarOpacityFunction() );
	m_colorWidget->SetHistogram( m_transferFunction->GetHistogram() );
	m_colorWidget->SetColorTransferFunction( m_transferFunction->GetColorTransferFunction() );
	m_gradientWidget->SetHistogram( m_GradientHistogram.GetPointer() );
	m_gradientWidget->SetPiecewiseFunction( m_transferFunction->GetGradientOpacityFunction() );
}

void mitk::wxMitkTransferFunctionWidget::SetPresets()
{
	mitk::TransferFunctionProperty::Pointer transferFunctionProp = NULL;

	if ( m_node == NULL )
	{
		return;
	}

	// Check if it already has a TransferFunctionProperty
	transferFunctionProp = static_cast<mitk::TransferFunctionProperty*>(m_node->GetProperty("TransferFunction"));
	if (transferFunctionProp.IsNull())
	{
		return;
	}

	m_transferFunction = static_cast<mitk::TransferFunction*>(transferFunctionProp->GetValue().GetPointer());
	if (m_transferFunction.IsNull())
	{
		return;
	}
	
	m_transferFunction->SetTransferFunctionMode( m_ComboPresets->GetSelection( ) );
	m_opacityWidget->SetPiecewiseFunction(m_transferFunction->GetScalarOpacityFunction());
	m_colorWidget->SetColorTransferFunction(m_transferFunction->GetColorTransferFunction());
	m_gradientWidget->SetPiecewiseFunction(m_transferFunction->GetGradientOpacityFunction());	

	// Enable/Disable MIP 
	mitk::BoolProperty::Pointer propertyMIP;
	propertyMIP = static_cast<mitk::BoolProperty*>(m_node->GetProperty("EnableMIP"));
	if(propertyMIP.IsNull())
	{
		propertyMIP = mitk::BoolProperty::New(false);
		m_node->SetProperty("EnableMIP", propertyMIP);
	}
	propertyMIP->SetValue( m_ComboPresets->GetSelection( ) == mitk::TransferFunction::TF_MR_MIP );


	// Enable xray attenuation rendering property
	mitk::DoubleProperty::Pointer xRayAttenuationProp = static_cast<mitk::DoubleProperty*>(m_node->GetProperty("xrayattenuation"));
	if(xRayAttenuationProp.IsNull())
	{
		xRayAttenuationProp = mitk::DoubleProperty::New();
		m_node->SetProperty("xrayattenuation", xRayAttenuationProp);
	}
	xRayAttenuationProp->SetValue( m_XRayVolRendAttSlider->GetValue()/10.0 );

	double thrVal = m_XRayVolRendThrSlider->GetValue()/100.0;
	m_transferFunction->SetXRVRThreshold(thrVal);

	//set also a property of the node, since the transferfunction generated by mitk sucks!
	m_node->GetPropertyList()->RemoveObserver( m_NodeTagModified );

	mitk::DoubleProperty::Pointer xRayTFThresholdProp = static_cast<mitk::DoubleProperty*>(m_node->GetProperty("xraythreshold"));
	if(xRayTFThresholdProp.IsNull())
	{
		xRayTFThresholdProp = mitk::DoubleProperty::New();
		m_node->SetProperty("xraythreshold", xRayTFThresholdProp);
	}
	xRayTFThresholdProp->SetValue( thrVal);

	m_NodeTagModified = m_node->GetPropertyList()->AddObserver(
		itk::ModifiedEvent(), m_MemberCommand );

	Refresh();

	mitk::RenderingManager::GetInstance()->RequestUpdateAll( 
		mitk::RenderingManager::REQUEST_UPDATE_3DWINDOWS );

}

void mitk::wxMitkTransferFunctionWidget::OnComboPresets(wxCommandEvent& event)
{
	//Simply call the SetPresets
	SetPresets();
}

void mitk::wxMitkTransferFunctionWidget::UpdateAll()
{
	Update();
	UpdateData( );
	UpdateWidgets( );
}

void mitk::wxMitkTransferFunctionWidget::OnGPUAccelerationChanged( wxCommandEvent& event )
{
	UpdateData();
}

void mitk::wxMitkTransferFunctionWidget::OnLODChanged( wxCommandEvent& event )
{
	UpdateData();
}


void mitk::wxMitkTransferFunctionWidget::OnOpenCloseBusyInfo(wxCommandEvent& event)
{
	switch (event.GetInt())
	{
	case 1:
		//close an open m_info
		if(m_info!=NULL)
		{
			delete m_info;
			m_info=NULL;
		}
		break;
	
	case 2:
		//open a new m_info
		if(m_info!=NULL)
			delete m_info;

		m_info = new wxBusyInfo(wxT("Volume rendering, please wait...."));
		break;
	}
}

void mitk::wxMitkTransferFunctionWidget::SendOpenClosBInfoCommand(int openClose)
{
	wxCommandEvent evt( wxEVT_OPENCLOSEBUSYINFO, wxID_ANY );
	evt.SetEventObject(this);
	evt.SetInt(openClose);
	if(openClose==2) //open...must be fast processed!!
		GetEventHandler()->ProcessEvent(evt);
	else
		GetEventHandler()->AddPendingEvent(evt);

}



mitk::wxMitkTransferFunctionWidget::vtkRendererCallback::vtkRendererCallback()
{
}

mitk::wxMitkTransferFunctionWidget::vtkRendererCallback *mitk::wxMitkTransferFunctionWidget::vtkRendererCallback::New()
{
	return new vtkRendererCallback; 
}


void mitk::wxMitkTransferFunctionWidget::vtkRendererCallback::Execute(vtkObject *caller, 
															   unsigned long event, 
															   void *callData)
{
	vtkRenderer* ren = vtkRenderer::SafeDownCast(caller);
	if(m_callerWin==NULL)
		return;


	//wxCommandEvent evt( wxEVT_OPENCLOSEBUSYINFO, wxID_ANY );
	//evt.SetEventObject(m_callerWin);


	switch(event)
	{
	case (vtkCommand::StartEvent):
		////Call this directly from the updatevolumerendering properties...so that the info is shown immediately (otherwise it takes some times)
		//((wxMitkTransferFunctionWidget *)m_callerWin)->SendOpenClosBInfoCommand(2); //open a new info window

		break;

	case (vtkCommand::EndEvent):
		((wxMitkTransferFunctionWidget *)m_callerWin)->SendOpenClosBInfoCommand(1); //close an open info window
		caller->RemoveObserver(this);

		break;
	}

}

void mitk::wxMitkTransferFunctionWidget::CleanWidgets()
{
	m_opacityWidget->Enable(false);
	m_colorWidget->Enable(false);
	m_gradientWidget->Enable(false);
	m_opacityWidget->ResetWidget();
	m_colorWidget->ResetWidget();
	m_gradientWidget->ResetWidget();
	m_volumeRenderingProp = NULL;
	m_xRayVolumeRenderingProp = NULL;
	m_chkVolumeRendering->SetValue(false);
	m_chkXRayVolumeRendering->SetValue(false);
	m_chkGPUAcceleration->SetValue(true);
	m_chkLOD->SetValue(true);
	m_LookupTableWidget->ResetWidget();

	if ( m_renderWindow != NULL )
	{
		m_renderWindow->EnableColorLegend( false );
	}
}

void mitk::wxMitkTransferFunctionWidget::OnCroppingPlanesChanged(wxCommandEvent &event)
{
	if(!m_chkVolumeRendering->GetValue())
		return;

	//Instead of updating al volume rendering properties, simply update the cropping
	mitk::BaseVtkMapper3D *mapper3D = NULL;
	vtkVolume *mapperVolume = NULL;

	mitk::BaseRenderer * renderer = mitk::BaseRenderer::GetInstance(m_renderWindow->GetRenderWindow());

	mapper3D =static_cast<mitk::BaseVtkMapper3D *> (m_node->GetMapper(mitk::BaseRenderer::Standard3D));
	if(mapper3D==NULL)
		return;

	mapperVolume = static_cast<vtkVolume *> (mapper3D->GetVtkProp(renderer));
	if(mapperVolume==NULL)
		return;
	
	vtkVolumeMapper *vtkVolM = static_cast<vtkVolumeMapper  *>(mapperVolume->GetMapper());
	if(vtkVolM==NULL)
		return;

	vtkVolM->SetCropping(true);

	//mitk::Image::Pointer mImage = dynamic_cast<mitk::Image *>(m_node->GetData());
	//mitk::Point3D origin = m_node->GetData()->GetGeometry(0)->GetOrigin();
	//mitk::Vector3D spacing = m_node->GetData()->GetGeometry(0)->GetSpacing();
	double cropPlanes[6];
	for(int i=0;i<3;i++)
	{
		unsigned int rng = m_sliderCroppingRegionPlanes[i]->GetRange();
		int val = m_sliderCroppingRegionPlanes[i]->GetValue();
		
		int maxVal = val + rng/2;
		int minVal = val - rng/2;
		//cropPlanes[2*i] = minVal*spacing[i]+origin[i];
		//cropPlanes[2*i+1] = maxVal*spacing[i]+origin[i];
		cropPlanes[2*i]=minVal;
		cropPlanes[2*i+1]=maxVal;
	}
	vtkVolM->SetCroppingRegionPlanes(cropPlanes);

	mitk::RenderingManager::GetInstance()->RequestUpdateAll( mitk::RenderingManager::REQUEST_UPDATE_3DWINDOWS );
}

void wxMitkTransferFunctionWidget::OnLightingChange( wxMitkWidgetUpdateEvent& WXUNUSED(event) )
{
	m_pMaterialSettingsPane->UpdateMaterial();
	UpdateData();
}

/** 
	If the node is an image, get the extent of the image and set the cropping region sliders
	to include the whole extent. If the image already had cropping planes, set these ones!
**/

void mitk::wxMitkTransferFunctionWidget::ResetCroppingSliders(bool forceWholeExtent /*=false*/)
{
	if(m_node==NULL)
		return;
	
	mitk::Image::Pointer mImage = dynamic_cast<mitk::Image *>(m_node->GetData());
	if(mImage.IsNull() || m_renderWindow == NULL )
		return;

	//1. Determine the extent of the volume image
	vtkImageData *vImage = mImage->GetVtkImageData(0);
	int wholeExt[6];
	double currCPlanes[6];
	vImage->GetWholeExtent(wholeExt);

	//2. (if forceWholeExtent is false): determine if the image has a volumeMapper and croppingPlanes
	bool hasCroppingPlanes = false;

	if(!forceWholeExtent)
	{
		//current cropping planes (if any) are set in the mapper
		mitk::BaseVtkMapper3D *mapper3D = NULL;
		vtkVolume *mapperVolume = NULL;
		mitk::BaseRenderer * renderer = mitk::BaseRenderer::GetInstance(m_renderWindow->GetRenderWindow());

		mapper3D =static_cast<mitk::BaseVtkMapper3D *> (m_node->GetMapper(mitk::BaseRenderer::Standard3D));
		if(mapper3D!=NULL)
		{
			mapperVolume = static_cast<vtkVolume *> (mapper3D->GetVtkProp(renderer));
			if(mapperVolume!=NULL)
			{
				vtkVolumeMapper* volMapper = static_cast<vtkVolumeMapper *> (mapperVolume->GetMapper());
				if((volMapper!=NULL) && volMapper->GetCropping())
				{
					hasCroppingPlanes = true;
					volMapper->GetCroppingRegionPlanes(currCPlanes);
				}

			}
		}
	}

	//3. for each m_sliderCroppingRegionPlanes control, set the extent and range to the whole image
	//or to the croppingPlanes already specified in the vtkVolumeMapper
	for(int i=0;i<3;i++)
	{
		int minVal = wholeExt[2*i];
		int maxVal = wholeExt[2*i+1];
		unsigned int range;
		int value;
		if(hasCroppingPlanes)
		{
			range = currCPlanes[2*i+1]-currCPlanes[2*i];
			value = .5*(currCPlanes[2*i]+currCPlanes[2*i+1]);
		}
		else
		{
			range = maxVal-minVal;
			value = .5*(minVal+maxVal);
		}
		m_sliderCroppingRegionPlanes[i]->SetAll(value, range, minVal, maxVal );
		m_sliderCroppingRegionPlanes[i]->Refresh();
	}
}
