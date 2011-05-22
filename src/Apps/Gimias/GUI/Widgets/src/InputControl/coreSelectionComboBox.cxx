/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreSelectionComboBox.h"

using namespace Core::Widgets;

#include "BulbOn.xpm"
#include "bulbOff.xpm"
#include <wx/tooltip.h>
#include "wx/imaglist.h"
#include "wx/combo.h"

//!
SelectionComboBox::SelectionComboBox(
	wxWindow* parent, wxWindowID id,
	const wxString& value,
	const wxPoint& pos, const wxSize& size, 
	int n, const wxString choices[],
	long style,
	const wxValidator& validator,
	const wxString& name)
: wxComboCtrl(parent, id, value, pos, size, style, validator, name)
{
	UseAltPopupWindow();
}


//!
SelectionComboBox::~SelectionComboBox(void)
{
}

BEGIN_EVENT_TABLE(wxListViewComboPopup, wxListView)
    EVT_MOTION(wxListViewComboPopup::OnMouseMove)
    EVT_LEFT_DOWN(wxListViewComboPopup::OnMouseClick)
	EVT_SIZE(wxListViewComboPopup::OnSize)
END_EVENT_TABLE()



void Core::Widgets::wxListViewComboPopup::Init()
{
	m_value = -1;
	m_itemHere = -1; // hot item in list
	m_SelectedDataEntity = -1;
	m_EnableOpacitySelection = true;

	wxIcon icons[2];
	icons[0] = wxIcon(bulboff_xpm);
	icons[1] = wxIcon(bulbon_xpm);
	int width  = icons[0].GetWidth(),
		height = icons[0].GetHeight();

	m_ImageList = new wxImageList(width, height, true);
	for ( size_t i = 0; i < WXSIZEOF(icons); i++ )
		m_ImageList->Add(icons[i]);
}

bool Core::Widgets::wxListViewComboPopup::Create( wxWindow* parent )
{
	bool val = wxListView::Create(parent,1,wxDefaultPosition,wxDefaultSize, 
		wxLC_REPORT|wxLC_NO_HEADER|wxLC_SINGLE_SEL|wxBORDER_THEME);

	wxListItem itemCol;
	itemCol.SetText(_T("Column 1"));
	itemCol.SetImage(-1);
	wxListView::InsertColumn(0, itemCol);
	wxListView::SetColumnWidth( 0, GetSize().GetWidth() );

	AssignImageList(m_ImageList, wxIMAGE_LIST_SMALL);

	//SetToolTip( _T("Move mouse pointer over bulb light to highlight") );

	return val;
}

void Core::Widgets::wxListViewComboPopup::SetStringValue( const wxString& s )
{
	int n = wxListView::FindItem(-1,s);
	if ( n >= 0 && n < wxListView::GetItemCount() )
		wxListView::Select(n);
}

wxWindow * Core::Widgets::wxListViewComboPopup::GetControl()
{
	return this;
}

wxString Core::Widgets::wxListViewComboPopup::GetStringValue() const
{
	if ( m_value >= 0 )
		return wxListView::GetItemText(m_value);
	return wxEmptyString;
}

void Core::Widgets::wxListViewComboPopup::OnMouseMove( wxMouseEvent& event )
{
	// Move selection to cursor if it is inside the popup
	int resFlags;
	int itemHere = HitTest(event.GetPosition(),resFlags);
	if ( wxListView::GetFirstSelected( ) != itemHere )
	{
		wxListView::Select( m_itemHere, false );
		m_itemHere = itemHere;
		wxListView::Select( m_itemHere,true);
	}

	// Select data entity if mouse over icon
	if ( ( ( resFlags & wxLIST_HITTEST_ONITEMICON ) &&
		m_SelectedDataEntity != itemHere ) ||
		itemHere == -1 ) 
	{
		SelectDataEntity( itemHere );
	}

	event.Skip();
}

void Core::Widgets::wxListViewComboPopup::OnMouseClick( wxMouseEvent& WXUNUSED(event) )
{
	m_value = m_itemHere;

	wxCommandEvent event( wxEVT_COMMAND_COMBOBOX_SELECTED, m_combo->GetId() );

	int n = m_itemHere;
	event.SetInt( n );
	event.SetString( GetStringValue() );
	event.SetEventObject( this );
	InitCommandEvent(event);
	m_combo->GetEventHandler()->ProcessEvent(event);

	// TODO: Send event as well
	Dismiss();
}

void Core::Widgets::wxListViewComboPopup::Append( const wxString& selstr, unsigned int data )
{
	long tmp = wxListView::InsertItem( wxListView::GetItemCount( ), selstr, 0 );
	wxListView::SetItemData( tmp, data );
}

void Core::Widgets::wxListViewComboPopup::SetSelection( long item )
{
	if ( item != -1 )
	{
		wxListView::SetItemState(item, wxLIST_STATE_FOCUSED, wxLIST_STATE_FOCUSED);
		m_combo->SetValue( GetItemText( item ) );
	}
	else
	{
		m_combo->SetValue( "" );
	}
	m_value = item;
	m_itemHere = item;
}

void Core::Widgets::wxListViewComboPopup::OnSize(wxSizeEvent& event)
{
	if ( wxListView::GetColumnCount() > 0 )
	{
		wxListView::SetColumnWidth( 0, GetSize().GetWidth() - 10 );
	}

}

void Core::Widgets::wxListViewComboPopup::SetRenderingTree( 
	Core::RenderingTree::Pointer renderingTree )
{
	m_RenderingTree = renderingTree;
}

void Core::Widgets::wxListViewComboPopup::SelectDataEntity( int item )
{
	if ( !m_EnableOpacitySelection || m_RenderingTree.IsNull( ) )
	{
		return;
	}

	// Restore off item
	if ( m_SelectedDataEntity != -1 )
	{
		SetItemImage( m_SelectedDataEntity, 0 );
	}

	// Select new item and on image
	m_SelectedDataEntity = item;
	if ( m_SelectedDataEntity != -1 )
	{
		SetItemImage( m_SelectedDataEntity, 1 );
	}

	// Check ID selected item
	long selectedDataEntityId = -1;
	if ( m_SelectedDataEntity != -1 )
	{
		selectedDataEntityId = GetItemData( m_SelectedDataEntity );
	}

	if ( selectedDataEntityId != -1 )
	{
		
		// Change opacity
		std::vector<DataEntity::Pointer> set;
		set = m_RenderingTree->GetAllDataEntities();
		std::vector<DataEntity::Pointer>::iterator it;
		for ( it = set.begin( ) ; it != set.end() ; it++ )
		{
			Core::DataEntity::Pointer dataEntity = *it;

			if ( dataEntity->GetId() == selectedDataEntityId )
			{
				m_RenderingTree->SetProperty( dataEntity, blTag::New( "opacity", 1.0 ) );
			}
			else
			{
				m_RenderingTree->SetProperty( dataEntity, blTag::New( "opacity", 0.1 ) );
			}
		}

		m_MultiRenderWindow->RequestUpdateAll();
	}
	else
	{
		RestoreOpacity();
	}

}

void Core::Widgets::wxListViewComboPopup::OnPopup()
{
	// Adjust height
	wxSize size = wxListView::GetSize();
	if ( GetItemCount() )
	{
		wxRect rect;
		GetItemRect( 0, rect );
		
		int height = GetItemCount() * rect.GetHeight() + 50;
		height = std::min( height, 400 );
		height = std::max( height, 50 );
		size.SetHeight( height );
		wxListView::SetSize( size );
	}
	else
	{
		size.SetHeight( 30 );
		wxListView::SetSize( size );
	}

	m_SelectedDataEntity = -1;
	BackupOpacity();
}

void Core::Widgets::wxListViewComboPopup::OnDismiss()
{
	if ( m_SelectedDataEntity != -1 )
	{
		RestoreOpacity();
		// Restore off item
		SetItemImage( m_SelectedDataEntity, 0 );
	}
}

void Core::Widgets::wxListViewComboPopup::RestoreOpacity()
{
	if ( !m_EnableOpacitySelection || m_RenderingTree.IsNull() )
	{
		return;
	}

	std::vector<DataEntity::Pointer> set;
	set = m_RenderingTree->GetAllDataEntities();
	std::vector<DataEntity::Pointer>::iterator it;
	for ( it = set.begin( ) ; it != set.end() ; it++ )
	{
		Core::DataEntity::Pointer dataEntity = *it;
		if ( dataEntity.IsNotNull()  )
		{
			float opacity = m_BackupOpacityMap[ dataEntity->GetId() ];
			m_RenderingTree->SetProperty( dataEntity, blTag::New( "opacity", opacity ) );
		}
	}

	m_MultiRenderWindow->RequestUpdateAll();
}

void Core::Widgets::wxListViewComboPopup::BackupOpacity()
{
	if ( !m_EnableOpacitySelection || m_RenderingTree.IsNull() )
	{
		return;
	}

	std::vector<DataEntity::Pointer> set;
	set = m_RenderingTree->GetAllDataEntities();
	std::vector<DataEntity::Pointer>::iterator it;
	for ( it = set.begin( ) ; it != set.end() ; it++ )
	{
		Core::DataEntity::Pointer dataEntity = *it;
		if ( dataEntity.IsNotNull()  )
		{
			float opacity = 0;
			blTag::Pointer tag = m_RenderingTree->GetProperty( dataEntity, "opacity" );
			if ( tag.IsNotNull() )
			{
				tag->GetValue( opacity );
			}
			m_BackupOpacityMap[ dataEntity->GetId() ] = opacity;
		}
	}
}

void Core::Widgets::wxListViewComboPopup::SetEnableOpacitySelection( bool enable )
{
	m_EnableOpacitySelection = enable;
}

bool Core::Widgets::wxListViewComboPopup::GetEnableOpacitySelection()
{
	return m_EnableOpacitySelection;
}

void Core::Widgets::wxListViewComboPopup::SetMultiRenderWindow( 
	Core::Widgets::RenderWindowBase* val )
{
	m_MultiRenderWindow = val;
}
