#ifndef GBLCONTROLQT_H
#define GBLCONTROLQT_H

// Copyright 2007 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "CILabNamespaceMacros.h"
#include "qwidget.h"

CILAB_BEGIN_NAMESPACE(gbl)

/**
Use this function as a generic interface to change the visibility of a control.
*/
inline void SetVisibility(QWidget* _widget, bool _visible)
{
	if( _visible )
		_widget->show();
	else
		_widget->hide();
}

/**
Use this function as a generic interface to change the visibility of a control.
*/
inline void SetEnabled(QWidget* _widget, bool _enabled)
{
	_widget->setEnabled(_enabled);
}

CILAB_END_NAMESPACE(gbl)

#endif //GBLCONTROLQT_H
