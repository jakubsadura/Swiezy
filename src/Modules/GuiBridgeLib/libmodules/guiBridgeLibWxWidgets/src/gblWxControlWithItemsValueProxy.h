/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef GBLWXCONTROLWITHITEMSVALUEPROXY_H
#define GBLWXCONTROLWITHITEMSVALUEPROXY_H

#include "wx/wx.h"
#include "gblValueProxy.h"
#include "CILabNamespaceMacros.h"

CILAB_BEGIN_NAMESPACES(gbl, wx)

//! Overload to set listBox contents as text. See gblValueProxy.h
void SetTextOfControlWithItems(wxControlWithItems* listBox, const std::string& text);

//! Overload to get listbox contents as text. See gblValueProxy.h
std::string GetTextOfControlWithItems(wxControlWithItems* listBox);

//! Overload to set listBox contents as a number. See gblValueProxy.h
void SetNumberOfControlWithItems(wxControlWithItems* listBox, int number);

//! Overload to get listbox contents as a number. See gblValueProxy.h
double GetNumberOfControlWithItems(wxControlWithItems* listBox);

CILAB_END_NAMESPACES(glb, wx)

#endif //GBLWXCONTROLWITHITEMSVALUEPROXY_H
