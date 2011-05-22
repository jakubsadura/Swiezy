/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreClippingWidget.h"
#include "coreKernel.h"
#include "wx/xrc/xmlres.h"
#include "boost/filesystem.hpp"
#include <mitkRenderingManager.h>
#include <blMitkUnicode.h>
#include "coreReportExceptionMacros.h"

using namespace mitk;

Core::ClippingWidget::ClippingWidget( wxWindow* parent )
{
	using namespace boost::filesystem;
	wxXmlResource::Get()->InitAllHandlers();
	path filename = 
		Core::Runtime::Kernel::GetApplicationSettings()->GetCoreResourceForFile("CoreClippingWidget.xrc");
	bool okay = wxXmlResource::Get()->Load(_U(filename.string()));
	okay = okay && wxXmlResource::Get()->LoadPanel(this, parent, wxT("CoreClippingWidget"));
	this->Connect(
		this->GetButtonShowPlane()->GetId(), 
		wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,
		wxCommandEventHandler(Core::ClippingWidget::OnButtonShowPlane)
	);
	this->Connect(
		this->GetButtonResetPlane()->GetId(), 
		wxEVT_COMMAND_BUTTON_CLICKED, 
		wxCommandEventHandler(Core::ClippingWidget::OnButtonResetPlane)
	);
	this->Connect(
		this->GetButtonClip()->GetId(), 
		wxEVT_COMMAND_BUTTON_CLICKED, 
		wxCommandEventHandler(Core::ClippingWidget::OnButtonClip)
	);
}

void Core::ClippingWidget::Init(
	Core::ClippingProcessor::Pointer processor,
	Core::ClippingSceneManager::Pointer sceneManager
)
{
	this->m_Processor = processor;
	this->m_SceneManager = sceneManager;
	this->m_Processor->GetProcessorDataHolder()->AddObserver(this, &Core::ClippingWidget::UpdateWidget);

	this->m_Processor->GetOutputDataEntityHolder()->AddObserver(this, 
													&Core::ClippingWidget::OnModifiedOutputDataEntity);

	this->UpdateWidget();
}

void Core::ClippingWidget::OnButtonClip( wxCommandEvent& event )
{
	this->m_Processor->Run();
	this->UpdateWidget();
}

void Core::ClippingWidget::OnButtonShowPlane( wxCommandEvent& event )
{
	this->SetIsInClippingMode( this->GetButtonShowPlane()->GetValue() );
	//this->UpdateWidget();
}

void Core::ClippingWidget::OnButtonResetPlane( wxCommandEvent& event )
{
	this->m_Processor->ResetPlane();
	this->m_SceneManager->SetIsInClippingMode(true);
	this->UpdateWidget();
}

void Core::ClippingWidget::UpdateWidget()
{
	Core::ClippingProcessorData::Pointer parameters = this->m_Processor->GetProcessorDataHolder()->GetSubject();
	if( parameters.IsNull() )
		return;

	this->GetButtonShowPlane()->SetValue( this->m_SceneManager->GetIsInClippingMode() );
	std::string label = this->m_SceneManager->GetIsInClippingMode() ? "Hide " : "Show ";
	this->GetButtonShowPlane()->SetLabel(_U(label + std::string("Plane")));
	this->Validate();
	mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void Core::ClippingWidget::UpdateDataHolders()
{
	if( !this->Validate() )
		return;

	this->m_Processor->GetProcessorDataHolder()->NotifyObservers();
}

bool Core::ClippingWidget::Validate()
{
	using boost::get;
	bool okay = true;
	//gbl::wx::Validator validator;
	Core::ClippingProcessorData::Pointer parameters = this->m_Processor->GetProcessorDataHolder()->GetSubject();
	if( parameters.IsNull() )
		return false;

	return okay;

}

wxToggleButton* Core::ClippingWidget::GetButtonShowPlane() const
{
	return XRCCTRL(*this, "m_toggleBtnShowPlane", wxToggleButton);
}

wxButton* Core::ClippingWidget::GetButtonResetPlane() const
{
	return XRCCTRL(*this, "m_btnResetPlane", wxButton);
}

wxButton* Core::ClippingWidget::GetButtonClip()
{
	return XRCCTRL(*this, "m_btnClip", wxButton);
}

bool Core::ClippingWidget::GetIsInClippingMode() const
{
	return this->GetButtonShowPlane()->GetValue();
}

void Core::ClippingWidget::SetIsInClippingMode( bool isInClippingMode )
{
	this->m_SceneManager->SetIsInClippingMode(isInClippingMode);
	this->UpdateWidget();
}

void Core::ClippingWidget::OnModifiedOutputDataEntity( )
{
	try
	{
			this->m_SceneManager->UpdateModifiedOutput(this->m_Processor);
	}
	coreCatchExceptionsLogAndNoThrowMacro( 
		"MeshOptimization::PanelWidget::OnModifiedOutputDataEntity" );
}

bool Core::ClippingWidget::Enable( bool enable /*= true */ )
{
	bool bReturn =wxWindow::Enable( enable );

	try
	{
		if ( enable )
		{
			//connect interactor and show plane 
			if ( !this->GetButtonShowPlane()->GetValue() )
				this->m_Processor->ResetPlane();
			this->m_SceneManager->SetIsInClippingMode(true);
			this->UpdateWidget();
		}
		else
		{
			//disconnect interactor and hide plane
			this->m_SceneManager->SetIsInClippingMode(false);
		}

		mitk::RenderingManager::GetInstance()->RequestUpdateAll();
	}
	coreCatchExceptionsLogAndNoThrowMacro(
		"Enable" );

	return bReturn;
}
