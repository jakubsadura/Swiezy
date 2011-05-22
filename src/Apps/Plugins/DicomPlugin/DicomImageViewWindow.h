/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef DICOMSLICEVIEWWINDOW_H
#define DICOMSLICEVIEWWINDOW_H

#include "CILabNamespaceMacros.h"
#include "wxMitkSelectableGLWidget.h"
#include "mitkDataTree.h"
#include "mitkDataStorage.h"
#include "mitkLevelWindowManager.h"
#include "coreDataEntity.h"
#include "coreDataHolder.h"
#include "coreRenderingTreeMITK.h"
#include "corePluginMacros.h"

namespace DicomPlugin{
/**
This class is a render window that shows a DICOM slice.
\author Jakub Lyko
26 may 2008
*/

class PLUGIN_EXPORT DcmImageViewWindow : public mitk::wxMitkSelectableGLWidget, public boost::signals::trackable
{
public:
	typedef wxMitkSelectableGLWidget Parent;

	//!
	DcmImageViewWindow(wxWindow* parent, wxWindowID id);
	//! \param _dataEntityHolder - The mesh that is being displayed.
	void Init(Core::DataEntityHolder::Pointer _dataEntityHolder, const std::string& name = "");
	//! Centralize the mesh in the window.
	void Fit();
	//! 
	mitk::LevelWindowManager::Pointer GetLevelWindowManager() {return levelWindowManager;}
	//!
	void SetLevelWindowManager(mitk::LevelWindowManager::Pointer LevelWindowManager)
		{levelWindowManager = LevelWindowManager;}
	//!
	const mitk::LevelWindow& GetLevelWindow() {return levelWindow;}
	//!
	void SetLevelWindow(const mitk::LevelWindow& LevelWindow)
		{levelWindow = LevelWindow;}
	//!
	void SaveCurrentLevelWindow();

	//!
	Core::RenderingTreeMITK::Pointer GetDataTree() {return dataTree;}

	//!
	bool GetUseFixedLevelWindow(){return useFixedLevelWindow;};

	//!
	void SetUseFixedLevelWindow(bool use) {useFixedLevelWindow = use;};

private:
	//! Start displaying a new mesh
	void OnNewDataEntity();

private:
	//! The input mesh holder
	Core::DataEntityHolder::Pointer dataEntityHolder;
	//! The rendering tree
	Core::RenderingTreeMITK::Pointer dataTree;
	//! Level window manager
	mitk::LevelWindowManager::Pointer levelWindowManager;
	//! Level window property
	mitk::LevelWindow levelWindow;
	//! 
	bool useFixedLevelWindow;
	
};

} // DicomPlugin

#endif //DICOMIMAGEVIEWWINDOW_H
