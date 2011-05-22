/*
* Copyright (c) 2010,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "CmguiPluginInfoWidget.h"

// STD
#include <cstdio>
#include <limits>
#include <list>

// Cmgui
extern "C"
{
#include "api/cmiss_command_data.h"
#include "api/cmiss_field_image.h"
#include "api/cmiss_field.h"
#include "api/cmiss_field_composite.h"
#include "api/cmiss_field.h"
#include "api/cmiss_scene_viewer.h"
#include "region/cmiss_region.h"
}

#include "boost/algorithm/string.hpp"
#include "boost/foreach.hpp"


using namespace CmguiPlugin;

const long wxID_RegionTree = wxNewId();

// Event the widget
BEGIN_EVENT_TABLE(InfoWidget, wxScrolledWindow)
  EVT_TREE_SEL_CHANGED(wxID_RegionTree, InfoWidget::OnSelectionChanged)
END_EVENT_TABLE()


InfoWidget::InfoWidget(Cmgui* cmgui, wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style)
: wxScrolledWindow(parent, id, pos, size, style)
{
    m_cmgui = cmgui;
    m_notebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
    m_notebookPaneRegions = new wxPanel(m_notebook, wxID_ANY);
    m_treeRegions = new wxTreeCtrl(m_notebookPaneRegions,  wxID_RegionTree, wxDefaultPosition, wxDefaultSize, wxTR_HAS_BUTTONS|wxTR_NO_LINES|wxTR_DEFAULT_STYLE|wxSUNKEN_BORDER);
    m_treeRegions->AddRoot(wxT("[Root Region]"));
    
    do_layout();
    
    wxString name = wxT("CMGUI Data Information");
    SetLabel(name);
    SetName(name);
}


InfoWidget::~InfoWidget()
{
    // We don't need to destroy anything because all the child windows
    // of this wxWindow are destroyed automatically
}


void InfoWidget::do_layout()
{
    wxBoxSizer* sizer_1 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_2 = new wxBoxSizer(wxHORIZONTAL);
    //sizer_2->Add(m_listBoxRegions, 1, wxEXPAND, 0);
    sizer_2->Add(m_treeRegions, 1, wxEXPAND, 0);
    m_notebookPaneRegions->SetSizer(sizer_2);
    m_notebook->AddPage(m_notebookPaneRegions, wxT("Regions"));
    sizer_1->Add(m_notebook, 1, wxEXPAND, 0);
    SetSizer(sizer_1);
    sizer_1->Fit(this);
    Layout();
}


void InfoWidget::UpdateWidget() {
    
    updateRegions(m_cmgui->GetRootRegion( ), m_treeRegions->GetRootItem());
}

void InfoWidget::printRegions(struct Cmiss_region *region, int indent, int indentIncrement) {
    int numChildRegions = 0;
    Cmiss_region_get_number_of_child_regions(region, &numChildRegions);
    for (int childRegionNr = 0; childRegionNr < numChildRegions; childRegionNr++) {
        struct Cmiss_region *childRegion = Cmiss_region_get_child_region(region, childRegionNr);
        char * childRegionName = Cmiss_region_get_name(childRegion);
        for (int i = 0; i < indent; i++) {
            std::cout << " ";
        }
        std::cout << childRegionName << std::endl;
        delete[] childRegionName;
        printRegions(childRegion, indent + indentIncrement, indentIncrement);
    }
}

void InfoWidget::updateRegions(struct Cmiss_region *region, wxTreeItemId treeItem) {
    // get child regions
    std::map<std::string, struct Cmiss_region *> childRegions;
    int numChildRegions = 0;
    Cmiss_region_get_number_of_child_regions(region, &numChildRegions);
    for (int childRegionNr = 0; childRegionNr < numChildRegions; childRegionNr++) {
        struct Cmiss_region *childRegionID = Cmiss_region_get_child_region(region, childRegionNr);
        char * childRegionName = Cmiss_region_get_name(childRegionID);
        childRegions[std::string(childRegionName)] = childRegionID;
        delete childRegionName;
    }
    
    // check the items that are currently in the tree
    wxTreeItemIdValue cookie;
    wxTreeItemId childTreeItem = m_treeRegions->GetFirstChild(treeItem, cookie);
    std::list<wxTreeItemId> treeItemsToDelete;
    while (childTreeItem.IsOk()) {
        // check, if that region still exists
        std::map<std::string, struct Cmiss_region *>::iterator found = childRegions.find(std::string(m_treeRegions->GetItemText(childTreeItem)));
        if (found == childRegions.end()) {
            // not found any longer => delete it (but not right now because this would screw up our iteration)
            treeItemsToDelete.push_back(childTreeItem);
        } else {
            // found => check children
            updateRegions(found->second, childTreeItem);
            
            // remove it from map, so we can check afterwards if there was a region that hasn't been found in the tree
            childRegions.erase(found);
        }
        
        // next child
        childTreeItem = m_treeRegions->GetNextChild(treeItem, cookie);
    }
    
    // Really delete the tree items that have been marked for deletion
    BOOST_FOREACH(const wxTreeItemId& treeItemToDelete, treeItemsToDelete) {
        m_treeRegions->Delete(treeItemToDelete);
    }
    
    // check, if there are any new regions that haven't been found in the tree
    std::pair<std::string, struct Cmiss_region *> childRegion;
    BOOST_FOREACH(childRegion, childRegions) {
        // add it to the tree
        wxTreeItemId childTreeItem = m_treeRegions->InsertItem(treeItem, -1, childRegion.first);
        m_treeRegions->EnsureVisible(childTreeItem);
        updateRegions(childRegion.second, childTreeItem);
    }
}


bool InfoWidget::Enable(bool enable /*= true */)
{
    bool bReturn = wxScrolledWindow::Enable(enable);

    // If this panel widget is selected -> Update the widget
    if (enable) {
        UpdateWidget();
    }

    return bReturn;
}

void InfoWidget::OnSelectionChanged(wxTreeEvent& event) {
    std::list<std::string> regionPath;
    wxTreeItemId item = event.GetItem();
    while (item.IsOk()) {
        regionPath.push_front(m_treeRegions->GetItemText(item).mb_str());
        item = m_treeRegions->GetItemParent(item);
    }
    
    std::stringstream regionPathString;
    int i = 0;
    BOOST_FOREACH(std::string region, regionPath) {
        if (i > 1) {
            regionPathString << "/";
        }
        if (i > 0) {
            regionPathString << region;
        }
        i++;
    }
    //m_cmgui->setSelectedRegionPath(regionPathString.str());
    std::cout << regionPathString.str() << std::endl;
}


