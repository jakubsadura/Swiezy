// For compilers that don't support precompilation, include "wx/wx.h"
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
       #include "wx/wx.h"
#endif

#include "wxMitkSurfaceVisor.h"
#include "wxMitkMultiRenderWindow.h"
#include "wxMitkRenderWindow.h"
#include "wxMitkMaterialEditorWidget.h"

#include <mitkDataTreeNodeFactory.h>
#include <mitkRenderingManager.h>
#include <mitkProperties.h>
#include <mitkGlobalInteraction.h>

#include <mitkDataStorage.h>
#include <mitkSurface.h>
#include <string>
#include <mitkDataTree.h>

const std::string INPUT_FILENAME = "F:/ToolKit/Source/Data/Angio/Aneurisms/aneu_1.vtk";

IMPLEMENT_APP_CONSOLE(wxMitkSurfaceVisor)

#define ID_wxMitkMultiRenderWindow 2001
#define ID_wxMitkMaterialEditorWidget 2002

//! this is executed upon startup, like 'main()' in non-wxWidgets programs
int wxMitkSurfaceVisor::OnRun(void)
{

	// Basic initialization: create a tree
	mitk::DataTree::Pointer tree = mitk::DataTree::New();
	mitk::DataStorage::CreateInstance(tree);

	// create an iterator on the tree
	mitk::DataTreePreOrderIterator it(tree);

	// Create some data by reading a file
	std::string filename = INPUT_FILENAME;
	mitk::DataTreeNodeFactory::Pointer nodeReader = mitk::DataTreeNodeFactory::New();

	nodeReader->SetFileName(filename.c_str());
	nodeReader->Update();

	mitk::DataTreeNode::Pointer node = nodeReader->GetOutput();

	mitk::Surface::Pointer surface = dynamic_cast<mitk::Surface*>(node->GetData());

	it.Add(node);

	wxFrame* mainWindow = new wxFrame(NULL, wxID_ANY, wxT("wxMitk test"));

	// create a renderwindow and pass the tree to it
	mitk::wxMitkMultiRenderWindow* renderWindow = new mitk::wxMitkMultiRenderWindow(mainWindow, ID_wxMitkMultiRenderWindow, wxDefaultPosition, wxSize(400, 400));
	mitk::wxMitkMaterialEditorWidget* materialEditor = new mitk::wxMitkMaterialEditorWidget(mainWindow, ID_wxMitkMaterialEditorWidget, wxDefaultPosition, wxSize(300, 400));
	materialEditor->SetDataTreeNode(node);
	materialEditor->SetMinSize(wxSize(300, 32000));

	mainWindow->SetAutoLayout(true);
	wxBoxSizer* layout = new wxBoxSizer(wxHORIZONTAL);
	layout->Add(renderWindow, 1, wxEXPAND);
	layout->Add(materialEditor);
	mainWindow->SetSizer(layout);
	
	mitk::DataTreePreOrderIterator itRoot(tree);

	renderWindow->EnableNavigationControllerEventListening(true);
	renderWindow->SetData(&itRoot);
	renderWindow->InitializeStandardViews(&itRoot);

	renderWindow->EnableStandardLevelWindow(false);
	renderWindow->EnableDepartmentLogo(false);

	mainWindow->Show(true);

	renderWindow->ChangeLayoutToDefault();

	this->wxMitkApp::OnRun();

	return 0;
}



