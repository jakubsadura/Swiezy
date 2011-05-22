// For compilers that don't support precompilation, include "wx/wx.h"
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
       #include "wx/wx.h"
#endif

#include "wxMitkVisor.h"
#include "wxMitkMultiRenderWindow.h"
#include "wxMitkRenderWindow.h"
#include "wxMitkDisplayPlaneSubtreeHelper.h"
#include "wxMitkTransferFunctionWidget.h"

#include <mitkDataTreeNodeFactory.h>
#include <mitkRenderingManager.h>
#include <mitkProperties.h>
#include <mitkGlobalInteraction.h>
#include <mitkSliceNavigationController.h>
#include <mitkCameraRotationController.h>
#include <mitkGeometry2DDataMapper2D.h>
#include <mitkDataStorage.h>
#include <string>
#include <mitkDataTree.h>
#include <wx/combobox.h>
#include <wx/notebook.h>
IMPLEMENT_APP_CONSOLE(wxMitkVisor)

#define ID_wxMitkMultiRenderWindow (01 + wxID_HIGHEST)
#define ID_wxMitkTransferFunctionWidget (02 + wxID_HIGHEST)
#define ID_ComboBox (03 + wxID_HIGHEST)

using namespace mitk;

BEGIN_EVENT_TABLE(wxMitkVisor, wxMitkApp)
	EVT_COMBOBOX(ID_ComboBox, wxMitkVisor::OnLayoutChanges)
END_EVENT_TABLE()

//! this is executed upon startup, like 'main()' in non-wxWidgets programs
int wxMitkVisor::OnRun(void)
{

	// Basic initialization: create a tree
	mitk::DataTree::Pointer tree = mitk::DataTree::New();
	mitk::DataStorage::CreateInstance(tree);

	// create an iterator on the tree
	mitk::DataTreePreOrderIterator it(tree);

	// Create some data by reading a file
	std::string filename = "./crop.vtk";
	mitk::DataTreeNodeFactory::Pointer nodeReader = mitk::DataTreeNodeFactory::New();

	nodeReader->SetFileName(filename.c_str());
	nodeReader->Update();

	mitk::DataTreeNode::Pointer node = nodeReader->GetOutput();
	
	mitk::Image::Pointer image = dynamic_cast<mitk::Image*>(node->GetData());
	it.Add(node);

	wxFrame* mainWindow = new wxFrame(NULL, wxID_ANY, wxT("wxMitk test"));
	wxNotebook* note = new wxNotebook(mainWindow, wxID_ANY);
	wxPanel* panel = new wxPanel(note, wxID_ANY);

	// create a renderwindow and pass the tree to it
	this->renderWindow = new mitk::wxMitkMultiRenderWindow(panel, ID_wxMitkMultiRenderWindow, wxDefaultPosition, wxSize(400, 400));
	
	mitk::wxMitkTransferFunctionWidget* tfw = new mitk::wxMitkTransferFunctionWidget(panel, ID_wxMitkTransferFunctionWidget, wxDefaultPosition, wxSize(300, 400));
	tfw->SetDataTreeNode(static_cast<mitk::DataTreeNode*>(node.GetPointer()));
	tfw->SetMinSize(wxSize(300, 32000));
	this->cmbbox = new wxComboBox(panel, ID_ComboBox);
	cmbbox->Append("Default 2x2");
	cmbbox->Append("2D images left");
	cmbbox->Append("Single 3D");
	cmbbox->Append("Single X view");
	cmbbox->Append("Single Y view");
	cmbbox->Append("Single Z view");
	cmbbox->Append("Only 2D");

	wxBoxSizer* layout = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* vlayout = new wxBoxSizer(wxVERTICAL);
	vlayout->Add(cmbbox, 1, wxEXPAND);
	vlayout->Add(tfw);
	layout->Add(renderWindow, 1, wxEXPAND);
	layout->Add(vlayout);
	
	panel->SetSizer(layout);
	note->AddPage(panel, "A panel");
	
	++it;
	mitk::DataTreeNode::Pointer nodeX = renderWindow->GetCurrentXViewWorldGeometry2DNode();
	mitk::DataTreeNode::Pointer nodeY = renderWindow->GetCurrentYViewWorldGeometry2DNode();
	mitk::DataTreeNode::Pointer nodeZ = renderWindow->GetCurrentZViewWorldGeometry2DNode();
	mitk::wxMitkDisplayPlaneSubtreeHelper::AddDisplayPlaneSubTree(&it, nodeX, nodeY, nodeZ);

	const float red[3] = {1.0, 0.0, 0.0};
	const float green[3] = {0.0, 1.0, 0.0};
	const float blue[3] = {0.0, 0.0, 1.0};
	mitk::wxMitkDisplayPlaneSubtreeHelper::SetColorToDisplayPlanes(&it, red, green, blue);

	mitk::DataTreePreOrderIterator itRoot(tree);

	renderWindow->EnableNavigationControllerEventListening(true);
	renderWindow->SetData(&itRoot);
	renderWindow->InitializeStandardViews(&itRoot);

	renderWindow->EnableStandardLevelWindow(true);
	renderWindow->EnableDepartmentLogo(false);
	mainWindow->Show(true);

	renderWindow->ChangeLayoutToDefault();

	this->wxMitkApp::OnRun();

	return 0;
}

//!
void wxMitkVisor::OnLayoutChanges(wxCommandEvent& event)
{
	switch(this->cmbbox->GetCurrentSelection())
	{
		case 1: this->renderWindow->ChangeLayoutTo2DImagesLeft(); break;
		case 2: this->renderWindow->ChangeLayoutToSingle3D(); break;
		case 3: this->renderWindow->ChangeLayoutToSingle2D_X(); break;
		case 4: this->renderWindow->ChangeLayoutToSingle2D_Y(); break;
		case 5: this->renderWindow->ChangeLayoutToSingle2D_Z(); break;
		case 6: this->renderWindow->ChangeLayoutToOnly2D(); break;
		default: this->renderWindow->ChangeLayoutToDefault(); break;
	}
}


