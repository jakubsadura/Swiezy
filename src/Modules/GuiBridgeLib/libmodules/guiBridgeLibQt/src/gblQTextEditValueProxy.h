#ifndef GBLQTEXTEDITVALUEPROXY_H
#define GBLQTEXTEDITVALUEPROXY_H

#include "gblValueProxyQt.h"
#include <qobject.h>

class QTextEdit;

namespace gbl
{
	//! Set the text field in a QTextEdit
	template <> void SetText(QTextEdit* textEdit, const std::string& text);

	//! Set the text field in a QTextEdit
	template <> std::string GetText(QTextEdit* textEdit);

} // namespace gbl

#endif //GBLQTEXTEDITVALUEPROXY_H
