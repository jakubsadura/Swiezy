#include "TestWxMitkDataTreeListView.h"
#include "wx/button.h"
#include "wx/sizer.h"
#include "wx/frame.h"
#include <iostream>
#include "mitkDataTreeHelper.h"
#include "mitkProperties.h"
#include "wxMitkDataTreeListView.h"
#include "mitkDataTree.h"

using namespace mitk;
namespace mitk
{
class wxMitkDataTreeListViewDebug : public wxButton
{
public:
	wxMitkDataTreeListViewDebug(wxWindow* parent, wxMitkDataTreeListView* treeView) : wxButton(parent, 1, "Debug"), m_treeView(treeView)
	{
	}

	void Debug(wxCommandEvent& event)
	{
		this->m_treeView->m_DataTreeFilter->DeleteSelectedItemsAndSubItems();
	}

private:
	wxMitkDataTreeListView* m_treeView;
	DECLARE_EVENT_TABLE();
};
}

BEGIN_EVENT_TABLE(wxMitkDataTreeListViewDebug, wxButton)
EVT_BUTTON(1, wxMitkDataTreeListViewDebug::Debug)
END_EVENT_TABLE()

mitk::DataTreeNode::Pointer CreateNode(mitk::Image::Pointer image, const char* name)
{
	mitk::DataTreeNode::Pointer node = mitk::DataTreeNode::New();
	node->SetData(image);
	mitk::StringProperty::Pointer nodeName = new mitk::StringProperty(name);
	mitk::BoolProperty::Pointer nodeVis = new mitk::BoolProperty(false);
	node->SetProperty("visible", nodeVis);
	node->SetProperty("name", nodeName);
	return node;
}

wxWindow* CreateWidget()
{
	mitk::Image::Pointer image = mitk::Image::New();
	mitk::DataTree::Pointer tree = mitk::DataTree::New();

	mitk::DataTreeNode::Pointer node1 = CreateNode(image, "node1");
	mitk::DataTreeNode::Pointer node2 = CreateNode(image, "node2");
	mitk::DataTreeNode::Pointer node3 = CreateNode(image, "node3");
	mitk::DataTreeNode::Pointer node4 = CreateNode(image, "node4");

	mitk::DataTreePreOrderIterator it(tree);
	it.Add(node1);

	bool debugTheCaseOfOnlyOneNode = false;
	if( !debugTheCaseOfOnlyOneNode )
	{
		it.Add(node2);

		mitk::DataTreeIteratorClone it2 = mitk::DataTreeHelper::FindIteratorToNode(tree, node2);
		it2->Add(node3);

		mitk::DataTreeIteratorClone it3 = mitk::DataTreeHelper::FindIteratorToNode(tree, node2);
		++it3;
		it3->Add(node4);
	}

	wxFrame* holderFrame = new wxFrame(NULL, -1, "Hallo");
	holderFrame->SetAutoLayout(true);
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	holderFrame->SetSizer(sizer);
	holderFrame->SetMaxSize(wxSize(200, 300));

	typedef wxMitkDataTreeListView TreeViewType;
	TreeViewType* treeView = new TreeViewType(holderFrame, -1);
	treeView->SetAutoLayout(true);

	treeView->SetDataTree(tree);
	sizer->Add(treeView, 1, wxEXPAND, 0);
	treeView->SetMinSize(wxSize(200, 500));
	treeView->Show();

	wxMitkDataTreeListViewDebug* debugTree = new wxMitkDataTreeListViewDebug(holderFrame, treeView);
	sizer->Add(debugTree, wxSizerFlags() .Proportion(1));
	sizer->Fit(holderFrame);
	holderFrame->Show();
	return holderFrame;
}

IMPLEMENT_APP_CONSOLE(MyApp)

bool MyApp::OnInit()
{
	this->wxMitkApp::OnInit();
	this->mainWindow = CreateWidget();
	this->mainWindow->Show(true);
	return true;
}
