// Copyright 2006 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef _blwxRangeValidator_h
#define _blwxRangeValidator_h

#include "TpExtLibWxWidgetsWin32Header.h"

#include <wx/valgen.h>

#include <float.h>

/**
Check the range of a text control using double values

Example:
  m_txtVariability->SetValidator( blwxRangeValidator( 
	&m_Processor->GetNumericalParam( )->variability, 50,  97) );
  m_txtVariability->GetValidator()->TransferToWindow();
  m_txtVariability->GetValidator()->TransferFromWindow();

\ingroup TpExtLibWxWidgets
\author Xavi Planes
\date 09 Oct 2009
*/
class TPEXTLIBWXWIDGETS_EXPORT blwxRangeValidator : public wxValidator{

public:

	/**
	\param [in] val Input value address
	\param [in] min minimum range value
	\param [in] max maximum range value
	*/
	blwxRangeValidator( double *val = 0, double min = DBL_MIN, double max = DBL_MAX );

	//!
	blwxRangeValidator(const blwxRangeValidator& val);;

	//!
	virtual wxValidator* Clone() const;;

	/** Called when the value in the window must be validated.
	This function can change the color of the text control
	Is called automatically by TransferFromWindow( )
	*/
	virtual bool Validate(wxWindow *parent);;

	//! If the Validate( ) is true -> set the value to m_Value
	virtual bool TransferFromWindow();;

	//! Put the contents of m_Value to the text control
	virtual bool TransferToWindow();;

	// Filter keystrokes
	void OnKeyUp(wxKeyEvent& event);

	//! Set range of the min and max
	void SetRange( double min, double max );

	DECLARE_EVENT_TABLE()

protected:

	//!
	bool CheckRange( wxTextCtrl *control );

	//!
	bool CheckValidator() const;

private:
	//!
	double m_RangeMin;

	//!
	double m_RangeMax;

	//!
	double *m_Value;
};


#endif // _blwxRangeValidator_h
