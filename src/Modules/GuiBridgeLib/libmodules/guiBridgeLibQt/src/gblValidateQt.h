#ifndef GBLVALIDATEQT_H
#define GBLVALIDATEQT_H

// Copyright 2007 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "CILabNamespaceMacros.h"
#include "CILabExceptionMacros.h"
#include "qcolor.h"
#include "gblValidate.h"
#include "gblBridgeQt.h"
#include "boost/format.hpp"

CILAB_BEGIN_NAMESPACE(gbl)

cilabDeclareExceptionMacro(EValidatorQt, std::exception)

//! Temporarily using this function to set the appearance of _widget.
inline bool SetAppearance(QWidget* _widget, bool _valid)
{
	_widget->setPaletteBackgroundColor(_valid ? QColor(200, 200, 200) : QColor(255, 0, 0) );
	return _valid;
}

/**
This class extends gbl::Validator with functions that check the content of Qt widgets.
Moreover, the appearance of the widget is set to (in)valid, based on the widget contents.
For example, calling ValidatorQt::GetNumber(myQLineEdit, VC_POSITIVE_REAL) will return a tuple with a bool (true iff
the line edits contains a positive real number) and the real number inside the QLineEdit.

as a double
\author Maarten Nieber
\date 13 Jan 2008
*/

class ValidatorQt : public gbl::Validator
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
				std::string message = (boost::format("ValidatorQt::GetNumber invalid value for _condition: %1%") % _condition).str();
				throw EValidatorQt(message.c_str());
			}
		}

		// Set appearance of the widget.
		gbl::SetAppearance(_control, boost::get<0>(result));

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

CILAB_END_NAMESPACE(gbl)

#endif //GBLVALIDATEQT_H
