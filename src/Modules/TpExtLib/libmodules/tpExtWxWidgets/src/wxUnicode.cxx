/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "wxUnicode.h"

std::string _U(const wxString& s)
{
	return std::string( s.mb_str( wxConvUTF8 ) ); 
}

wxString _U(const std::string& s)
{
	return wxString::FromUTF8( s.c_str() ); 
}

wxString _U(const char* c)
{
	return wxString::FromUTF8( c );
}

