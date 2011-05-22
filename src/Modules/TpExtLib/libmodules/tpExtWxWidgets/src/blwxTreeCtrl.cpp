/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
    #include "wx/log.h"
#endif

#include "wx/colordlg.h"
#include "wx/numdlg.h"

#include "wx/artprov.h"
#include "wx/image.h"
#include "wx/imaglist.h"
#include "wx/treectrl.h"
#include "wx/math.h"
#include "wx/renderer.h"

#include "blwxTreeCtrl.h"

#include "unchecked.xpm"
#include "checked.xpm"

BEGIN_EVENT_TABLE(blwxTreeCtrl, wxTreeCtrl)
    EVT_TREE_BEGIN_DRAG(wxID_ANY, blwxTreeCtrl::OnBeginDrag)
    EVT_TREE_END_DRAG( wxID_ANY, blwxTreeCtrl::OnEndDrag)
	EVT_LEFT_DOWN( blwxTreeCtrl::OnLeftClick )
END_EVENT_TABLE()


// blwxTreeCtrl implementation
IMPLEMENT_DYNAMIC_CLASS(blwxTreeCtrl, wxTreeCtrl)

blwxTreeCtrl::blwxTreeCtrl(wxWindow *parent, const wxWindowID id,
                       const wxPoint& pos, const wxSize& size,
                       long style)
          : wxTreeCtrl(parent, id, pos, size, style)
{
	
	m_showStates = true;

	//CreateImageList( );
    CreateStateImageList();
}

blwxTreeCtrl::blwxTreeCtrl()
{
	m_showStates = true;
}

blwxTreeCtrl::~blwxTreeCtrl()
{

}


void blwxTreeCtrl::CreateStateImageList(bool del)
{
    if ( del )
    {
        SetStateImageList(NULL);
        return;
    }

    wxImageList *states;
    wxBusyCursor wait;


	// The first state is only to initialize
    wxIcon icons[3];
    icons[0] = wxIcon(checked_xpm);
    icons[1] = wxIcon(unchecked_xpm);
	icons[2] = wxIcon(checked_xpm);
    
    int width  = icons[0].GetWidth(),
        height = icons[0].GetHeight();
        
    // Make an state image list containing small icons
    states = new wxImageList(width, height, true);

    for ( size_t i = 0; i < WXSIZEOF(icons); i++ )
        states->Add(icons[i]);

    AssignImageList(states);
}

void blwxTreeCtrl::DoToggleState(const wxTreeItemId& item)
{
    // we have only 2 checkbox states, so next state will be reversed
    //SetState(item, -1);
	int image = GetItemImage( item );
	SetItemImage( item, (image) % 2 + 1 );
}

void blwxTreeCtrl::DoResetBrokenStateImages(const wxTreeItemId& idParent,
                                          wxTreeItemIdValue cookie, int state)
{
    wxTreeItemId id;

    if ( !cookie )
        id = GetFirstChild(idParent, cookie);
    else
        id = GetNextChild(idParent, cookie);

    if ( !id.IsOk() )
        return;

    //int curState = GetState(id);
    //if ( curState != -1 && curState > state )
        ;//SetState(id, state);
	SetItemImage( id, state );

    if (ItemHasChildren(id))
        DoResetBrokenStateImages(id, 0, state);

    DoResetBrokenStateImages(idParent, cookie, state);
}

void blwxTreeCtrl::OnBeginDrag(wxTreeEvent& event)
{
    // need to explicitly allow drag
    if ( event.GetItem() != GetRootItem() )
    {
        m_draggedItem = event.GetItem();

        wxPoint clientpt = event.GetPoint();
        wxPoint screenpt = ClientToScreen(clientpt);

        event.Allow();
    }
}

void blwxTreeCtrl::OnEndDrag(wxTreeEvent& event)
{
	event.Skip();
}

void blwxTreeCtrl::DoEnsureVisible()
{
	if (m_lastItem.IsOk()) EnsureVisible(m_lastItem);
}

void blwxTreeCtrl::ResetBrokenStateImages()
{
	const size_t count = GetStateImageList()->GetImageCount();
	int state = count > 0 ? count - 1 : -1;
	DoResetBrokenStateImages(GetRootItem(), 0, state);
}

void blwxTreeCtrl::CreateImageList(int size)
{
	if ( size == -1 )
	{
		SetImageList(NULL);
		return;
	}
	if ( size == 0 )
		size = m_imageSize;
	else
		m_imageSize = size;

	// Make an image list containing small icons
	wxImageList *images = new wxImageList(size, size, true);

	// should correspond to TreeCtrlIcon_xxx enum
	wxBusyCursor wait;
	wxIcon icons[1];

	wxSize iconSize(size, size);
	icons[0] = wxArtProvider::GetIcon(wxART_NORMAL_FILE, wxART_CMN_DIALOG, iconSize);

	for ( size_t i = 0; i < WXSIZEOF(icons); i++ )
	{
		int sizeOrig = icons[0].GetWidth();
		if ( size == sizeOrig )
		{
			images->Add(icons[i]);
		}
		else
		{
			images->Add(wxBitmap(wxBitmap(icons[i]).ConvertToImage().Rescale(size, size)));
		}
	}

	AssignImageList(images);
}

void blwxTreeCtrl::OnLeftClick( wxMouseEvent& event )
{
	int flag;
	wxTreeItemId i = HitTest(wxPoint(event.GetX(),event.GetY()),flag);

	bool itemIconHit = ( flag & wxTREE_HITTEST_ONITEMICON ) != 0;
	bool itemIconStateHit = ( flag & wxTREE_HITTEST_ONITEMSTATEICON ) != 0;
	if(i.IsOk() && ( itemIconHit || itemIconStateHit ) )
	{
		wxTreeEvent event( wxEVT_COMMAND_TREE_STATE_IMAGE_CLICK, this, i );
		GetEventHandler()->AddPendingEvent( event );
		DoToggleState(i);
		return;//eat message
	} 
	
	event.Skip();
}

wxTreeItemId blwxTreeCtrl::GetDraggedItem() const
{
	return m_draggedItem;
}
