// Copyright 2006 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "gblQTextEditValueProxy.h"
#include <qtextedit.h>

template <> 
void gbl::SetText( QTextEdit* textEdit, const std::string& text )
{
	if( textEdit->text().ascii() != text )
		textEdit->setText( text );
}

template <> 
std::string gbl::GetText( QTextEdit* textEdit )
{
	return textEdit->text().ascii();
}

