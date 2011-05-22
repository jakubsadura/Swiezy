/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "CmguiPluginImageVisualProps.h"
#include "CmguiImageImpl.h"
#include "CmguiPluginRenderWidget.h"
#include "CmguiPluginRenderingTree.h"

#include "blwxTreeCtrl.h"

#include "wx/wupdlock.h"

using namespace CmguiPlugin;

class wxTreeItemRegion : public wxTreeItemData
{
public:
	wxTreeItemRegion( Cmiss_region* region )
	{
		m_Region = region;
	}
	Cmiss_region* m_Region;
};

// Event the widget
BEGIN_EVENT_TABLE(ImageVisualProps, CmguiPluginImageVisualPropsUI)
    EVT_TREE_STATE_IMAGE_CLICK( wxID_TREE, ImageVisualProps::OnItemStateClick)
END_EVENT_TABLE()

ImageVisualProps::ImageVisualProps(Cmgui* cmgui, wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style)
: CmguiPluginImageVisualPropsUI(parent, id, pos, size, style)
{

	wxSizer* sizer = m_Tree->GetContainingSizer();
	sizer->Detach( m_Tree );
	m_Tree->Destroy();

	m_Tree = new blwxTreeCtrl( this, wxID_TREE, wxDefaultPosition, wxDefaultSize, 
		wxTR_HAS_BUTTONS|wxTR_LINES_AT_ROOT|wxTR_DEFAULT_STYLE|wxSUNKEN_BORDER);
	sizer->Insert(0,m_Tree, 1, wxEXPAND);
}

ImageVisualProps::~ImageVisualProps()
{
}


void CmguiPlugin::ImageVisualProps::SetInputHolder( Core::DataEntityHolder::Pointer inputHolder )
{
	m_InputHolder = inputHolder;
	this->m_InputHolder->AddObserver(
		this, 
		&ImageVisualProps::OnInputHolderChangeSubject, 
		Core::DH_SUBJECT_MODIFIED_OR_NEW_SUBJECT);
}

bool CmguiPlugin::ImageVisualProps::IsValidData( Core::DataEntity::Pointer dataEntity )
{
	if ( dataEntity.IsNull() ||
		!GetMultiRenderWindow() ||
		GetMultiRenderWindow()->GetPrivateRenderingTree().IsNull() )
	{
		return false;
	}

	RenderWidget* cmguiRenderWidget = dynamic_cast<RenderWidget*> ( GetMultiRenderWindow() );
	if ( !cmguiRenderWidget )
	{
		return false;
	}

	RenderingTree* tree = dynamic_cast<RenderingTree*> ( GetMultiRenderWindow()->GetRenderingTree().GetPointer() );
	if ( !tree )
	{
		return false;
	}


	CmguiPlugin::CmguiImage3D::Pointer image;
	Core::DataEntity::Pointer renDataEntity;
	boost::any anyData = dataEntity->GetRenderingData( typeid( image ) );
	Core::CastAnyProcessingData( anyData, renDataEntity );
	return ( renDataEntity.IsNotNull( ) );

}

void CmguiPlugin::ImageVisualProps::OnInputHolderChangeSubject()
{
	UpdateWidget( );

	UpdateVisibility();
}

void CmguiPlugin::ImageVisualProps::UpdateWidget()
{
	Core::DataEntity::Pointer dataEntity = this->m_InputHolder->GetSubject();

	wxWindowUpdateLocker locker( this );

	m_Tree->DeleteAllItems();
	if ( !IsValidData( dataEntity ) )
	{
		m_btnShowAll->Enable( false );
		m_btnHideAll->Enable( false );
		m_btnShowSelected->Enable( false );
		return ;
	}
	m_btnShowAll->Enable( true );
	m_btnHideAll->Enable( true );
	m_btnShowSelected->Enable( true );

	m_Tree->AddRoot(wxT("Root"), -1, -1);

	CmguiPlugin::CmguiImage3D::Pointer image;
	Core::DataEntity::Pointer renDataEntity;
	boost::any anyData = dataEntity->GetRenderingData( typeid( image ) );
	Core::CastAnyProcessingData( anyData, renDataEntity );
	renDataEntity->GetProcessingData( image );

	RenderingTree* tree = dynamic_cast<RenderingTree*> ( GetMultiRenderWindow()->GetRenderingTree().GetPointer() );
	struct Scene *scene = Scene_viewer_get_scene( tree->GetSceneViewer() );

	std::vector<CmguiImage2D::Pointer>::iterator it;
	for ( it = image->m_Slices.begin( ) ; it != image->m_Slices.end() ; it++ )
	{
		char *name = Cmiss_region_get_name( (*it)->m_Region );
		std::string regionName = name;
		delete[] name;

		m_Tree->AppendItem( m_Tree->GetRootItem( ), regionName, -1, -1, 
			new wxTreeItemRegion( (*it)->m_Region ) );
	}

	m_Tree->ExpandAll();
	wxSize size = m_Tree->GetSize( );
}

void CmguiPlugin::ImageVisualProps::OnShowAll( wxCommandEvent &event )
{
	GetMultiRenderWindow()->GetRenderingTree()->Show( m_InputHolder->GetSubject( ), true );
	UpdateVisibility( );
}

void CmguiPlugin::ImageVisualProps::OnTreeSelChanged( wxTreeEvent &event )
{
	if ( m_btnShowSelected->GetValue() )
	{
		ShowSlice( m_LastSelectedItem, false );
		m_LastSelectedItem = event.GetItem();
		ShowSlice( m_LastSelectedItem, true );
		UpdateVisibility( );
	}
}

void CmguiPlugin::ImageVisualProps::OnHideAll( wxCommandEvent &event )
{
	GetMultiRenderWindow()->GetRenderingTree()->Show( m_InputHolder->GetSubject( ), false );
	UpdateVisibility( );
}

void CmguiPlugin::ImageVisualProps::OnShowSelected( wxCommandEvent &event )
{
	ShowSlice( m_LastSelectedItem, false );
	m_LastSelectedItem = m_Tree->GetSelection();
	ShowSlice( m_LastSelectedItem, true );
	UpdateVisibility( );
}

void CmguiPlugin::ImageVisualProps::ShowSlice( wxTreeItemId item, bool show )
{
	if ( !item.IsOk() || item == m_Tree->GetRootItem() )
	{
		return;
	}

	m_Tree->EnsureVisible( item );
	std::string selectedRegionName = m_Tree->GetItemText( item ).c_str();

	RenderingTree* tree = dynamic_cast<RenderingTree*> ( GetMultiRenderWindow()->GetRenderingTree().GetPointer() );
	struct Scene *scene = Scene_viewer_get_scene( tree->GetSceneViewer() );

	wxTreeItemIdValue cookie;
	wxTreeItemId itemId = m_Tree->GetFirstChild( m_Tree->GetRootItem(), cookie );

	while( itemId.IsOk() )
	{
		wxTreeItemRegion* itemRegion = dynamic_cast<wxTreeItemRegion*> (m_Tree->GetItemData( itemId ) );

		if ( item == itemId )
		{
			GT_visibility_type visible;
			visible = show? g_VISIBLE : g_INVISIBLE;
			Scene_set_element_group_visibility( scene, itemRegion->m_Region, visible );
		}

		itemId = m_Tree->GetNextChild( m_Tree->GetRootItem(), cookie);
	}
}

void CmguiPlugin::ImageVisualProps::OnItemStateClick( wxTreeEvent& event )
{
	wxTreeItemId itemId = event.GetItem();
	int state = m_Tree->GetItemImage( itemId );
	ShowSlice( itemId, state - 1 );
	UpdateVisibility( );
}

void CmguiPlugin::ImageVisualProps::UpdateVisibility()
{
	Core::DataEntity::Pointer dataEntity = this->m_InputHolder->GetSubject();

	wxWindowUpdateLocker locker( this );

	if ( !IsValidData( dataEntity ) )
	{
		return ;
	}

	RenderingTree* tree = dynamic_cast<RenderingTree*> ( GetMultiRenderWindow()->GetRenderingTree().GetPointer() );
	struct Scene *scene = Scene_viewer_get_scene( tree->GetSceneViewer() );

	wxTreeItemIdValue cookie;
	wxTreeItemId itemId = m_Tree->GetFirstChild( m_Tree->GetRootItem(), cookie );

	while( itemId.IsOk() )
	{

		wxTreeItemRegion* itemRegion = dynamic_cast<wxTreeItemRegion*> (m_Tree->GetItemData( itemId ) );

		GT_visibility_type visible;
		visible = Scene_get_element_group_visibility( scene, itemRegion->m_Region );
		if ( visible == g_VISIBLE )
		{
			m_Tree->SetItemImage( itemId, 2 );
		}
		else
		{
			m_Tree->SetItemImage( itemId, 1 );
		}

		itemId = m_Tree->GetNextChild( m_Tree->GetRootItem(), cookie);
	}

}
