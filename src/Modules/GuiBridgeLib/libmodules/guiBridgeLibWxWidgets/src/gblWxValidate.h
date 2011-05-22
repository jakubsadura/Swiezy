/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef GBLVALIDATEQT_H
#define GBLVALIDATEQT_H

#include "CILabNamespaceMacros.h"
#include "CILabExceptionMacros.h"
#include "gblValidate.h"
#include "boost/format.hpp"

class wxWindow;

CILAB_BEGIN_NAMESPACES(gbl, wx)

cilabDeclareExceptionMacro(EValidator, std::exception)

//! Temporarily using this function to set the appearance of _widget.
bool SetAppearance(wxWindow* widget, bool valid);

/**
This class extends gbl::Validator with functions that check the content of Qt widgets.
Moreover, the appearance of the widget is set to (in)valid, based on the widget contents.
For example, calling Validator::GetNumber(myQLineEdit, VC_POSITIVE_REAL) will return a tuple with a bool (true iff
the line edits contains a positive real number) and the real number inside the QLineEdit.

as a double
\author Maarten Nieber
\date 13 Jan 2008
*/

class Validator : public gbl::Validator
{
public:
	template< class T >
	boost::tuple<bool, double> GetNumber(T* _control, int _condition)
	{
		boost::tuple<bool, double> result;
		switch( _condition )
		{
		case VC_REAL:
			result = this->Validator::GetRealNumber( gbl::GetText(_control) );
			break;
		case VC_NONNEGATIVE_REAL:
			result = this->Validator::GetNonNegativeRealNumber( gbl::GetText(_control) );
			break;
		case VC_POSITIVE_REAL:
			result = this->Validator::GetPositiveRealNumber( gbl::GetText(_control) );
			break;
		case VC_POSITIVE_INTEGER:
			result = this->Validator::GetPositiveInteger( gbl::GetText(_control) );
			break;
		case VC_NONNEGATIVE_INTEGER:
			result = this->Validator::GetNonNegativeInteger( gbl::GetText(_control) );
			break;
		default:
			{
				std::string message = (boost::format("Validator::GetNumber invalid value for _condition: %1%") % _condition).str();
				throw EValidator(message.c_str());
			}
		}

		// Set appearance of the widget.
		gbl::wx::SetAppearance(_control, boost::get<0>(result));
		return result;
	}

	//! Convenience function. It does the same as GetNumber, but only returns a bool.
	template< class T >
	bool IsNumber(T* _control, int _condition)
	{
		boost::tuple<bool, double> result = this->GetNumber(_control, _condition);
		return boost::get<0>(result);
	}
};

CILAB_END_NAMESPACES(gbl, wx)

#endif //GBLVALIDATEQT_H
