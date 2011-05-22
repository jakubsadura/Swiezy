// Copyright 2006 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "blwxRangeValidator.h"

BEGIN_EVENT_TABLE(blwxRangeValidator, wxValidator)
  EVT_KEY_UP(blwxRangeValidator::OnKeyUp)
END_EVENT_TABLE()


blwxRangeValidator::blwxRangeValidator( 
	double *val /*= 0*/, 
	double min /*= DBL_MIN*/, 
	double max /*= DBL_MAX */ )
{
	m_Value = val;
	m_RangeMin = min;
	m_RangeMax = max;
}

blwxRangeValidator::blwxRangeValidator( const blwxRangeValidator& val )
{
	wxValidator::Copy(val);

	m_Value = val.m_Value;
	m_RangeMin = val.m_RangeMin;
	m_RangeMax = val.m_RangeMax;
}

wxValidator* blwxRangeValidator::Clone() const
{
	return new blwxRangeValidator(*this);
}

bool blwxRangeValidator::Validate( wxWindow *parent )
{
	if( !CheckValidator() )
		return false;

	wxTextCtrl *control = (wxTextCtrl *) m_validatorWindow;

	// If window is disabled, simply return
	if ( !control->IsEnabled() )
		return true;

	bool valid = CheckRange( control );

	return valid;
}

bool blwxRangeValidator::TransferFromWindow()
{
	if( !CheckValidator() )
		return false;

	wxTextCtrl *control = (wxTextCtrl *) m_validatorWindow;

	// If window is disabled, simply return
	if ( !control->IsEnabled() )
		return true;

	if ( Validate( control ) )
	{
		control->GetValue( ).ToDouble( m_Value );
	}

	return true;
}

bool blwxRangeValidator::TransferToWindow()
{
	if( !CheckValidator() )
		return false;

	wxTextCtrl *control = (wxTextCtrl *) m_validatorWindow;

	// If window is disabled, simply return
	if ( !control->IsEnabled() )
		return true;

	control->SetValue( wxString::Format("%.1f", *m_Value ) );

	CheckRange( control );

	return true;
}

void blwxRangeValidator::OnKeyUp( wxKeyEvent& event )
{
	Validate( NULL );

	event.Skip();
}

void blwxRangeValidator::SetRange( double min, double max )
{
	m_RangeMin = min;
	m_RangeMax = max;
}

bool blwxRangeValidator::CheckRange( wxTextCtrl *control )
{
	// Check range
	bool valid = false;
	double val;
	control->GetValue( ).ToDouble( &val );
	valid = val > m_RangeMin && val < m_RangeMax;

	// Change color
	if ( valid )
	{
		control->SetBackgroundColour( control->GetDefaultAttributes().colBg );
	}
	else
	{
		control->SetBackgroundColour( wxColour(255, 0, 0) );
	}

	control->Refresh();

	return valid;
}

bool blwxRangeValidator::CheckValidator() const
{
	wxCHECK_MSG( m_validatorWindow, false,
		_T("No window associated with validator") );
	wxCHECK_MSG( m_validatorWindow->IsKindOf(CLASSINFO(wxTextCtrl)), false,
		_T("wxTextValidator is only for wxTextCtrl's") );

	return true;
}