/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _wxUnicode_H
#define _wxUnicode_H

#include "TpExtLibWxWidgetsWin32Header.h"
#include <string>
#include <wx/string.h>

/**
* Convert from wxString to std::string.
*/ 
extern TPEXTLIBWXWIDGETS_EXPORT std::string _U(const wxString& s);

/**
* Convert from std::string to wxString.
*/ 
extern TPEXTLIBWXWIDGETS_EXPORT wxString _U(const std::string& s);

/**
* Convert from char to wxString.
*/ 
extern TPEXTLIBWXWIDGETS_EXPORT wxString _U(const char* c);

#endif // _wxUnicode_H

