/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "DicomImageViewWindow.h"
#include "mitkSliceNavigationController.h"
#include "vtkRenderer.h"
#include "coreKernel.h"
#include "coreReportExceptionMacros.h"

using namespace DicomPlugin;

DcmImageViewWindow::DcmImageViewWindow( wxWindow* parent, wxWindowID id )
: DcmImageViewWindow::Parent(parent, id, "DcmImageViewWindow")
, dataTree(Core::RenderingTreeMITK::New())
, levelWindowManager(mitk::LevelWindowManager::New())
, useFixedLevelWindow(false)
{
	try
	{
		this->GetRenderer()->SetMapperID(mitk::BaseRenderer::Standard2D);
		levelWindowManager->SetDataStorage(dataTree->GetDataStorage());
	}
	coreCatchExceptionsReportAndNoThrowMacro(DcmImageViewWindow::DcmImageViewWindow)
}

void DcmImageViewWindow::Init( Core::DataEntityHolder::Pointer _dataEntityHolder, const std::string& name )
{
	try
	{
		this->dataEntityHolder = _dataEntityHolder;
		this->dataEntityHolder->AddObserver(this, &DcmImageViewWindow::OnNewDataEntity, Core::DH_NEW_SUBJECT);
		this->OnNewDataEntity();
	}
	coreCatchExceptionsReportAndNoThrowMacro(DcmImageViewWindow::Init)
}

void DcmImageViewWindow::OnNewDataEntity()
{
	try
	{
		Core::DataEntity::Pointer dataEntity = this->dataEntityHolder->GetSubject();
		if( dataEntity.IsNull() )
			return;

		// add slice to data tree
		dataTree->Add( dataEntity );

		//set desired level window
		if(GetUseFixedLevelWindow())
			levelWindowManager->SetLevelWindow(levelWindow);
		
		// Setup the defaults of the tree
		this->GetRenderer()->SetDataStorage( this->dataTree->GetDataStorage( ) );
		
		// Set proper rendering (see also wxMitkMultiRenderWindow::InitRenderWindows)
		mitk::Geometry3D::Pointer geometry(this->dataTree->GetDataStorage()->ComputeVisibleBoundingGeometry3D());
      GetSliceNavigationController()->SetInputWorldGeometry( geometry );
      GetSliceNavigationController()->SetTop( true ); // true
      GetSliceNavigationController()->SetFrontSide( false ); // true
      GetSliceNavigationController()->SetRotated( true ); // false
	  GetSliceNavigationController()->SetKeepPlanePosition( false );
		GetSliceNavigationController()->Update();
		
		this->Fit();
		
	}
	coreCatchExceptionsReportAndNoThrowMacro(DcmImageViewWindow::OnNewDataEntity)
}

void DcmImageViewWindow::Fit()
{
	try
	{
		this->GetRenderer()->GetDisplayGeometry()->Fit();
		this->GetRenderer()->ForceImmediateUpdate();
		this->GetRenderer()->GetVtkRenderer()->ResetCamera();
	}
	coreCatchExceptionsReportAndNoThrowMacro(DcmImageViewWindow::Fit)
}

void DcmImageViewWindow::SaveCurrentLevelWindow()
{
	try
	{
		if(levelWindowManager->GetLevelWindowProperty().IsNotNull())
			levelWindow = levelWindowManager->GetLevelWindow();
	}
	coreCatchExceptionsReportAndNoThrowMacro(DcmImageViewWindow::SaveCurrentLevelWindow)
}
