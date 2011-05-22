/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _blwxTreeCtrl_H
#define _blwxTreeCtrl_H

#include "TpExtLibWxWidgetsWin32Header.h"

#include "wx/treectrl.h"

/** 
\brief wxTreeCtrl with states

\ingroup TpExtLibWxWidgets
\author Xavi Planes
\date 18 Dec 2009
*/
class TPEXTLIBWXWIDGETS_EXPORT blwxTreeCtrl : public wxTreeCtrl
{
public:

    blwxTreeCtrl();
    blwxTreeCtrl(wxWindow *parent, const wxWindowID id,
               const wxPoint& pos, const wxSize& size,
               long style);
    virtual ~blwxTreeCtrl();;

	wxTreeItemId GetDraggedItem() const;

protected:

	void OnBeginDrag(wxTreeEvent& event);
	void OnEndDrag(wxTreeEvent& event);
	void OnItemStateClick(wxTreeEvent& event);

	void CreateStateImageList(bool del = false);

	void DoEnsureVisible();

	void DoToggleState(const wxTreeItemId& item);

	void ResetBrokenStateImages();

	void CreateImageList(int size = 16);

	void OnLeftClick(wxMouseEvent& event);

private:
    void AddItemsRecursively(const wxTreeItemId& idParent,
                             size_t nChildren,
                             size_t depth,
                             size_t folder);

    void DoResetBrokenStateImages(const wxTreeItemId& idParent,
                                  wxTreeItemIdValue cookie, int state);

    int          m_imageSize;               // current size of images
    wxTreeItemId m_lastItem;                // for OnEnsureVisible()
    wxTreeItemId m_draggedItem;             // item being dragged right now
	bool		 m_showStates;

    // NB: due to an ugly wxMSW hack you _must_ use DECLARE_DYNAMIC_CLASS()
    //     if you want your overloaded OnCompareItems() to be called.
    //     OTOH, if you don't want it you may omit the next line - this will
    //     make default (alphabetical) sorting much faster under wxMSW.
    DECLARE_DYNAMIC_CLASS(blwxTreeCtrl)
    DECLARE_EVENT_TABLE()
};

#endif // _blwxTreeCtrl_H
